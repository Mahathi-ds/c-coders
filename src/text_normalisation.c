#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include "project.h"

#define WORD_MAX 64
#define OUT_MAX  128

/* Returns byte length of an apostrophe at p (' or UTF-8 curly '), else 0 */
static int apos_len(const char *p)
{
    if (p[0] == '\'')
    {
        return 1;
    }
    if ((unsigned char)p[0] == 0xE2 &&
        (unsigned char)p[1] == 0x80 &&
        (unsigned char)p[2] == 0x99)
    {
        return 3;
    }
    return 0;
}

/* Expand one word (apostrophes already normalized to ') into out */
static void expand_word(const char *w, char *out, size_t outsz)
{
    static const struct { const char *from; const char *to; } special[] =
    {
        {"won't",  "will not"},
        {"can't",  "can not"},
        {"shan't", "shall not"},
        {"ain't",  "is not"},
        {"let's",  "let us"}
    };
    static const char *is_stems[] =
    {
        "it", "he", "she", "that", "what", "there",
        "here", "who", "where", "how", "when", "why"
    };

    char lw[WORD_MAX];
    size_t n = strlen(w);
    size_t k;
    char *ap;
    size_t stem;
    const char *suffix;

    for (k = 0; k <= n; k++)
    {
        lw[k] = (char)tolower((unsigned char)w[k]);
    }

    ap = strchr(lw, '\'');
    if (ap == NULL)
    {
        snprintf(out, outsz, "%s", w);
        return;
    }

    /* Irregular forms */
    for (k = 0; k < sizeof(special) / sizeof(special[0]); k++)
    {
        if (strcmp(lw, special[k].from) == 0)
        {
            snprintf(out, outsz, "%s", special[k].to);
            if (isupper((unsigned char)w[0]))
            {
                out[0] = (char)toupper((unsigned char)out[0]);
            }
            return;
        }
    }

    stem = (size_t)(ap - lw);
    suffix = ap + 1;

    if (strcmp(suffix, "t") == 0 && stem > 1 && lw[stem - 1] == 'n')
    {
        /* isn't -> is not, don't -> do not, doesn't -> does not */
        snprintf(out, outsz, "%.*s not", (int)(stem - 1), w);
    }
    else if (strcmp(suffix, "re") == 0)
    {
        snprintf(out, outsz, "%.*s are", (int)stem, w);
    }
    else if (strcmp(suffix, "m") == 0)
    {
        snprintf(out, outsz, "%.*s am", (int)stem, w);
    }
    else if (strcmp(suffix, "ll") == 0)
    {
        snprintf(out, outsz, "%.*s will", (int)stem, w);
    }
    else if (strcmp(suffix, "ve") == 0)
    {
        snprintf(out, outsz, "%.*s have", (int)stem, w);
    }
    else if (strcmp(suffix, "d") == 0)
    {
        snprintf(out, outsz, "%.*s would", (int)stem, w);
    }
    else if (strcmp(suffix, "s") == 0)
    {
        int is_form = 0;
        for (k = 0; k < sizeof(is_stems) / sizeof(is_stems[0]); k++)
        {
            if (strlen(is_stems[k]) == stem &&
                strncmp(lw, is_stems[k], stem) == 0)
            {
                is_form = 1;
                break;
            }
        }
        if (is_form)
        {
            snprintf(out, outsz, "%.*s is", (int)stem, w);
        }
        else
        {
            /* possessive: John's -> John */
            snprintf(out, outsz, "%.*s", (int)stem, w);
        }
    }
    else
    {
        snprintf(out, outsz, "%s", w);   /* unknown, leave as is */
    }
}

/* Append n bytes to dst; returns 0 if it would not fit */
static int put(char *dst, size_t size, size_t *o, const char *s, size_t n)
{
    if (*o + n >= size)
    {
        return 0;
    }
    memcpy(dst + *o, s, n);
    *o += n;
    return 1;
}

void expand_contractions(const char *src, char *dst, size_t size)
{
    size_t i = 0;
    size_t o = 0;

    if (dst == NULL || size == 0)
    {
        return;
    }
    if (src == NULL)
    {
        dst[0] = '\0';
        return;
    }

    while (src[i] != '\0')
    {
        if (isalnum((unsigned char)src[i]))
        {
            size_t e = i;
            size_t len;
            char w[WORD_MAX];
            char out[OUT_MAX];

            while (src[e] != '\0')
            {
                int a;
                if (isalnum((unsigned char)src[e]))
                {
                    e++;
                    continue;
                }
                a = apos_len(src + e);
                if (a > 0 && e > i && isalpha((unsigned char)src[e + a]))
                {
                    e += (size_t)a;
                    continue;
                }
                break;
            }

            len = e - i;
            if (len >= WORD_MAX)
            {
                /* too long to be a contraction: copy untouched */
                if (!put(dst, size, &o, src + i, len))
                {
                    break;
                }
            }
            else
            {
                size_t p = i;
                size_t q = 0;
                while (p < e)
                {
                    int a = apos_len(src + p);
                    if (a > 0)
                    {
                        w[q++] = '\'';
                        p += (size_t)a;
                    }
                    else
                    {
                        w[q++] = src[p++];
                    }
                }
                w[q] = '\0';

                expand_word(w, out, sizeof(out));
                if (!put(dst, size, &o, out, strlen(out)))
                {
                    break;
                }
            }
            i = e;
        }
        else
        {
            if (!put(dst, size, &o, src + i, 1))
            {
                break;
            }
            i++;
        }
    }

    dst[o] = '\0';
}

void text_normalization_n(char *text, size_t size, Option opt)
{
    int digit_removal = opt.digit_removal;
    int punctuation_removal = opt.punctuation_removal;
    int special_character_removal = opt.special_character_removal;

    size_t i = 0;
    size_t j = 0;
    int inside_space = 0;
    char ch;
    char *buf;

    if (text == NULL || size == 0)
    {
        return;
    }

    /* Step 1: expand contractions (isn't -> is not) into a temp copy */
    buf = (char *)malloc(size);
    if (buf == NULL)
    {
        return;
    }
    expand_contractions(text, buf, size);

    /* Step 2: the original cleanup, reading buf and writing back to text.
     * Cleanup only ever shrinks, so j never passes the end of buf. */
    while (buf[i] != '\0')
    {
        ch = buf[i];

        /* Always remove extra spaces */
        if (isspace((unsigned char)ch))
        {
            if (j > 0 && inside_space == 0)
            {
                text[j] = ' ';
                j++;
                inside_space = 1;
            }
            i++;
            continue;
        }

        if (digit_removal && isdigit((unsigned char)ch))
        {
            i++;
            continue;
        }
        if (punctuation_removal && ispunct((unsigned char)ch))
        {
            i++;
            continue;
        }
        if (special_character_removal && !isprint((unsigned char)ch))
        {
            i++;
            continue;
        }

        text[j] = ch;
        j++;
        inside_space = 0;
        i++;
    }

    if (j > 0 && text[j - 1] == ' ')
    {
        j--;
    }
    text[j] = '\0';

    free(buf);
}

/* Original signature kept so existing callers still compile.
 * Assumes the caller's buffer has room for ~2x the current text length. */
void text_normalization(char *text, Option opt)
{
    if (text == NULL)
    {
        return;
    }
    text_normalization_n(text, 2 * strlen(text) + 1, opt);
}