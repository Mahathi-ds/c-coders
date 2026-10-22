
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include "../include/project.h"

/* Classic Porter Stemmer with a few requested word-form rules. */
static char *b;
static int k;
static int j;

static int is_consonant(int i)
{
    switch (b[i]) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            return 0;
        case 'y':
            return (i == 0) ? 1 : !is_consonant(i - 1);
        default:
            return 1;
    }
}

static int measure(void)
{
    int n = 0;
    int i = 0;

    while (i <= j && is_consonant(i))
        i++;

    while (i <= j) {
        while (i <= j && !is_consonant(i))
            i++;

        if (i > j)
            break;

        n++;

        while (i <= j && is_consonant(i))
            i++;
    }

    return n;
}

static int contains_vowel(void)
{
    for (int i = 0; i <= j; i++) {
        if (!is_consonant(i))
            return 1;
    }
    return 0;
}

static int ends_with(const char *suffix)
{
    size_t len = strlen(suffix);

    if ((size_t)(k + 1) < len)
        return 0;

    if (memcmp(b + k + 1 - (int)len, suffix, len) != 0)
        return 0;

    j = k - (int)len;
    return 1;
}

static void set_to(const char *replacement)
{
    size_t len = strlen(replacement);

    memcpy(b + j + 1, replacement, len);
    k = j + (int)len;
    b[k + 1] = '\0';
}

static void replace_if_measure_positive(const char *replacement)
{
    if (measure() > 0)
        set_to(replacement);
}

static int is_double_consonant(int i)
{
    return i >= 1 &&
           b[i] == b[i - 1] &&
           is_consonant(i);
}

static int is_cvc(int i)
{
    if (i < 2 ||
        !is_consonant(i) ||
        is_consonant(i - 1) ||
        !is_consonant(i - 2))
        return 0;

    return b[i] != 'w' &&
           b[i] != 'x' &&
           b[i] != 'y';
}

static void step1(void)
{
    /* Step 1a: plurals */
    if (ends_with("sses")) {
        k -= 2;
        b[k + 1] = '\0';
    } else if (ends_with("ies")) {
        /* Requested behavior: studies -> study */
        set_to("y");
    } else if (ends_with("ss")) {
        /* Keep ss unchanged. */
    } else if (ends_with("s")) {
        k--;
        b[k + 1] = '\0';
    }

    /* Step 1b: eed, ed, ing */
    if (ends_with("eed")) {
        if (measure() > 0) {
            
            b[k] = '\0';
        }
    } else if (ends_with("ed") && contains_vowel()) {
        k = j;
        b[k + 1] = '\0';

        if (ends_with("at"))
            set_to("ate");
        else if (ends_with("bl"))
            set_to("ble");
        else if (ends_with("iz"))
            set_to("ize");
        else if (is_double_consonant(k)) {
            if (b[k] != 'l' &&
                b[k] != 's' &&
                b[k] != 'z') {
                k--;
                b[k + 1] = '\0';
            }
        } else if (measure() == 1 && is_cvc(k)) {
            set_to("e");
        }
    } else if (ends_with("ing") && contains_vowel()) {
        k = j;
        b[k + 1] = '\0';

        if (ends_with("at"))
            set_to("ate");
        else if (ends_with("bl"))
            set_to("ble");
        else if (ends_with("iz"))
            set_to("ize");
        else if (is_double_consonant(k)) {
            if (b[k] != 'l' &&
                b[k] != 's' &&
                b[k] != 'z') {
                k--;
                b[k + 1] = '\0';
            }
        } else if (measure() == 1 && is_cvc(k)) {
            set_to("e");
        }
    }

    /* Step 1c: y -> i */
    // if (ends_with("y") && contains_vowel())
    //     b[k] = 'i';
}

static void step2(void)
{
    if (k < 1)
        return;

    switch (b[k - 1]) {
        case 'a':
            if (ends_with("ational"))
                replace_if_measure_positive("ate");
            else if (ends_with("tional"))
                replace_if_measure_positive("tion");
            break;

        case 'c':
            if (ends_with("enci"))
                replace_if_measure_positive("ence");
            else if (ends_with("anci"))
                replace_if_measure_positive("ance");
            break;

        case 'e':
            if (ends_with("izer"))
                replace_if_measure_positive("ize");
            break;

        case 'l':
            if (ends_with("bli"))
                replace_if_measure_positive("ble");
            else if (ends_with("alli"))
                replace_if_measure_positive("al");
            else if (ends_with("entli"))
                replace_if_measure_positive("ent");
            else if (ends_with("eli"))
                replace_if_measure_positive("e");
            else if (ends_with("ousli"))
                replace_if_measure_positive("ous");
            break;

        case 'o':
            if (ends_with("ization"))
                replace_if_measure_positive("ize");
            else if (ends_with("ation"))
                replace_if_measure_positive("ate");
            else if (ends_with("ator"))
                replace_if_measure_positive("ate");
            break;

        case 's':
            if (ends_with("alism"))
                replace_if_measure_positive("al");
            else if (ends_with("iveness"))
                replace_if_measure_positive("ive");
            else if (ends_with("fulness"))
                replace_if_measure_positive("ful");
            else if (ends_with("ousness"))
                replace_if_measure_positive("ous");
            break;

        case 't':
            if (ends_with("aliti"))
                replace_if_measure_positive("al");
            else if (ends_with("iviti"))
                replace_if_measure_positive("ive");
            else if (ends_with("biliti"))
                replace_if_measure_positive("ble");
            break;

        case 'g':
            if (ends_with("logi"))
                replace_if_measure_positive("log");
            break;
    }
}

static void step3(void)
{
    switch (b[k]) {
        case 'e':
            if (ends_with("icate"))
                replace_if_measure_positive("ic");
            else if (ends_with("ative"))
                replace_if_measure_positive("");
            else if (ends_with("alize"))
                replace_if_measure_positive("al");
            break;

        case 'i':
            if (ends_with("iciti"))
                replace_if_measure_positive("ic");
            break;

        case 'l':
            if (ends_with("ical"))
                replace_if_measure_positive("ic");
            else if (ends_with("ful"))
                replace_if_measure_positive("");
            break;

        case 's':
            if (ends_with("ness"))
                replace_if_measure_positive("");
            break;
    }
}

static void step4(void)
{
    if (k < 1)
        return;

    switch (b[k - 1]) {
        case 'a':
            if (!ends_with("al"))
                return;
            break;

        case 'c':
            if (!ends_with("ance") && !ends_with("ence"))
                return;
            break;

        // case 'e':
        //     if (!ends_with("er"))
        //         return;
        //     break;

        // case 'i':
        //     if (!ends_with("ic"))
        //         return;
        //     break;

        case 'l':
            if (!ends_with("able") && !ends_with("ible"))
                return;
            break;

        case 'n':
            if (!ends_with("ant") &&
                !ends_with("ement") &&
                !ends_with("ment") &&
                !ends_with("ent"))
                return;
            break;

        case 'o':
            if (ends_with("ion")) {
                if (j < 0 || (b[j] != 's' && b[j] != 't'))
                    return;
            } else if (!ends_with("ou")) {
                return;
            }
            break;

        case 's':
            if (!ends_with("ism"))
                return;
            break;

        case 't':
            if (!ends_with("ate") && !ends_with("iti"))
                return;
            break;

        case 'u':
            if (!ends_with("ous"))
                return;
            break;

        case 'v':
            if (!ends_with("ive"))
                return;
            break;

        case 'z':
            if (!ends_with("ize"))
                return;
            break;

        default:
            return;
    }

    if (measure() > 1) {
        k = j;
        b[k + 1] = '\0';
    }
}

static void step5(void)
{
    j = k;

    if (b[k] == 'e') {
        j = k - 1;
        int m = measure();

        if (m > 1 || (m == 1 && !is_cvc(j))) {
            k--;
            b[k + 1] = '\0';
        }
    }

    if (k >= 1 &&
        b[k] == 'l' &&
        is_double_consonant(k)) {
        j = k - 1;

        if (measure() > 1) {
            k--;
            b[k + 1] = '\0';
        }
    }
}

void nlp_stem_word(char *word)
{
    if (word == NULL)
        return;

    size_t len = strlen(word);

    if (len == 0 || len > 2147483647U)
        return;

    /* Convert ASCII uppercase letters to lowercase. */
    for (size_t i = 0; i < len; i++) {
        unsigned char ch = (unsigned char)word[i];

        if (ch >= 'A' && ch <= 'Z')
            word[i] = (char)(ch - 'A' + 'a');
    }

    /* Requested exceptions. */
    if (strcmp(word, "yes") == 0)
        return;

    if (strcmp(word, "disabled") == 0) {
        strcpy(word, "disable");
        return;
    }

    if (len <= 2)
        return;

    b = word;
    k = (int)len - 1;
    b[k + 1] = '\0';

    step1();
    step2();
    step3();
    step4();
    step5();
}