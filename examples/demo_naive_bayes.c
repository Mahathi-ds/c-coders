#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project.h"

#define NUM_TRAIN 32
#define NUM_TEST 8

/* Compile with -DRUN_STRESS_TESTS to also run risky robustness tests
 * (huge tokens, UTF-8, ~1 MB document). */

/* =========================================
 * Test bookkeeping
 * ========================================= */

static const char *KNOWN_LABELS[] = {
    "Sports", "Technology", "Health", "Politics"
};

#define NUM_KNOWN (sizeof(KNOWN_LABELS) / sizeof(KNOWN_LABELS[0]))

static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

typedef enum
{
    EXPECT_EXACT,
    EXPECT_VALID,
    EXPECT_NO_CRASH
} Mode;


/* =========================================
 * Helper functions
 * ========================================= */

static int is_known_label(const char *label)
{
    if (label == NULL)
        return 0;

    for (size_t i = 0; i < NUM_KNOWN; i++)
    {
        if (strcmp(label, KNOWN_LABELS[i]) == 0)
            return 1;
    }

    return 0;
}


static void record(const char *category,
                   const char *name,
                   int ok,
                   const char *expected,
                   const char *got)
{
    g_total++;

    if (ok)
        g_passed++;
    else
        g_failed++;

    printf("[%s] [%s] %s\n",
           ok ? "PASS" : "FAIL",
           category,
           name);

    printf("       expected: %s\n", expected ? expected : "NULL");
    printf("       got     : %s\n\n", got ? got : "NULL");
}


/* =========================================
 * File reading
 * ========================================= */

/* Read the complete contents of a text file */

char *read_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        perror(filename);
        return NULL;
    }

    if (fseek(fp, 0, SEEK_END) != 0)
    {
        fclose(fp);
        return NULL;
    }

    long size = ftell(fp);

    if (size < 0)
    {
        fclose(fp);
        return NULL;
    }

    rewind(fp);

    char *text = malloc((size_t)size + 1);

    if (text == NULL)
    {
        fclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(text, 1, (size_t)size, fp);

    if (bytes_read != (size_t)size || ferror(fp))
    {
        free(text);
        fclose(fp);
        return NULL;
    }

    text[bytes_read] = '\0';

    fclose(fp);

    return text;
}


/* =========================================
 * Memory management helpers
 * ========================================= */

void free_documents(char **docs, int count)
{
    if (docs == NULL)
        return;

    for (int i = 0; i < count; i++)
        free(docs[i]);

    free(docs);
}


static char *dup_str(const char *s)
{
    if (s == NULL)
        return NULL;

    size_t len = strlen(s);

    char *copy = malloc(len + 1);

    if (copy != NULL)
        memcpy(copy, s, len + 1);

    return copy;
}


static char *repeat_text(const char *s, int times)
{
    size_t len = strlen(s);

    char *out = malloc(len * (size_t)times + 1);

    if (out == NULL)
        return NULL;

    for (int i = 0; i < times; i++)
        memcpy(out + (size_t)i * len, s, len);

    out[len * (size_t)times] = '\0';

    return out;
}


/* =========================================
 * Prediction helper
 * ========================================= */

/* Predict a single document.
 * The text is copied because normalization may modify input.
 * Returns a heap-allocated label. Caller must free it.
 */

static char *predict_one(NaiveBayesModel model, const char *text)
{
    char *copy = dup_str(text != NULL ? text : "");

    if (copy == NULL)
        return NULL;

    char *docs[1] = { copy };

    char **res = Naivebayes_predict(model, docs, 1);

    char *label = NULL;

    if (res != NULL)
    {
        if (res[0] != NULL)
            label = dup_str(res[0]);

        free(res[0]);
        free(res);
    }

    free(copy);

    return label;
}



static void check_case(NaiveBayesModel model,
                       const char *category,
                       const char *name,
                       const char *text,
                       const char *expected,
                       Mode mode)
{
    char *label = predict_one(model, text);

    const char *exp_str;
    const char *got_str;
    int ok;

    switch (mode)
    {
        case EXPECT_EXACT:

            exp_str = expected;
            got_str = label;

            ok = (label != NULL &&
                  expected != NULL &&
                  strcmp(label, expected) == 0);

            break;


        case EXPECT_VALID:

            exp_str = "Any one of the four known classes";
            got_str = label;

            ok = (label != NULL && is_known_label(label));

            break;


        case EXPECT_NO_CRASH:

            exp_str = "No crash";
            got_str = label != NULL
                      ? "Program executed successfully"
                      : "Prediction failed";

            ok = (label != NULL);

            break;


        default:

            exp_str = "Invalid test mode";
            got_str = "Unknown mode";

            ok = 0;

            break;
    }

    record(category, name, ok, exp_str, got_str);

    free(label);
}

/* =========================================
 * Edge-case tests (file-based)
 * ========================================= */

typedef struct
{
    const char *name;
    const char *path;
    const char *expected;
    Mode mode;

} EdgeCase;


static void run_edge_tests(NaiveBayesModel model)
{
    printf("=================================\n");
    printf(" EDGE CASE TESTS\n");
    printf("=================================\n\n");


    EdgeCase cases[] = {

        /* ---- empty / no-signal input ---- */

        {
            "Empty string",
            "data/corpus/test/edge_cases/edge01_empty.txt",
            NULL,
            EXPECT_NO_CRASH
        },

        {
            "Whitespace only",
            "data/corpus/test/edge_cases/edge02_whitespace.txt",
            NULL,
            EXPECT_NO_CRASH
        },

        {
            "Stopwords only",
            "data/corpus/test/edge_cases/edge03_stopwords.txt",
            NULL,
            EXPECT_NO_CRASH
        },

        {
            "Unseen words only",
            "data/corpus/test/edge_cases/edge04_unseen_words.txt",
            NULL,
            EXPECT_NO_CRASH
        },

        {
            "Special characters only",
            "data/corpus/test/edge_cases/edge05_special_only.txt",
            NULL,
            EXPECT_NO_CRASH
        },

        {
            "Digits only",
            "data/corpus/test/edge_cases/edge06_digits_only.txt",
            NULL,
            EXPECT_NO_CRASH
        },


        /* ---- normalisation behaviour ---- */

        {
            "UPPERCASE text",
            "data/corpus/test/edge_cases/edge07_uppercase.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "MiXeD cAsE text",
            "data/corpus/test/edge_cases/edge08_mixed_case.txt",
            "Technology",
            EXPECT_EXACT
        },

        {
            "Heavy punctuation",
            "data/corpus/test/edge_cases/edge09_punctuation.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Digits mixed into text",
            "data/corpus/test/edge_cases/edge10_digits_mixed.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Special characters mixed into text",
            "data/corpus/test/edge_cases/edge11_special_mixed.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Newlines and tabs",
            "data/corpus/test/edge_cases/edge12_newlines_tabs.txt",
            "Health",
            EXPECT_EXACT
        },

        {
            "Leading and trailing spaces",
            "data/corpus/test/edge_cases/edge13_extra_spaces.txt",
            "Politics",
            EXPECT_EXACT
        },


        /* ---- very short input ---- */

        {
            "Single word: football",
            "data/corpus/test/edge_cases/edge14_football.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Single word: software",
            "data/corpus/test/edge_cases/edge15_software.txt",
            "Technology",
            EXPECT_EXACT
        },

        {
            "Single word: vaccine",
            "data/corpus/test/edge_cases/edge16_vaccine.txt",
            "Health",
            EXPECT_EXACT
        },

        {
            "Single word: parliament",
            "data/corpus/test/edge_cases/edge17_parliament.txt",
            "Politics",
            EXPECT_EXACT
        },


        /* ---- repetition / length ---- */

        {
            "Same word repeated 200 times",
            "data/corpus/test/edge_cases/edge18_repeated_word.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Long document (500 sentences)",
            "data/corpus/test/edge_cases/edge19_long_health.txt",
            "Health",
            EXPECT_EXACT
        },


        /* ---- mixed / ambiguous / out-of-domain ---- */

        {
            "One known word among unseen words",
            "data/corpus/test/edge_cases/edge20_known_unseen.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Negation words present",
            "data/corpus/test/edge_cases/edge21_negation.txt",
            "Sports",
            EXPECT_EXACT
        },

        {
            "Mixed topics",
            "data/corpus/test/edge_cases/edge22_mixed_topics.txt",
            NULL,
            EXPECT_VALID
        },

        {
            "Out-of-domain topic",
            "data/corpus/test/edge_cases/edge23_out_of_domain.txt",
            NULL,
            EXPECT_VALID
        }

    };


    int n = (int)(sizeof(cases) / sizeof(cases[0]));


    /* Read each edge-case text file and predict */

    for (int i = 0; i < n; i++)
    {
        printf("Reading: %s\n", cases[i].path);

        char *text = read_file(cases[i].path);

        if (text == NULL)
        {
            record("Edge",
                   cases[i].name,
                   0,
                   "file readable",
                   "file loading failed");

            continue;
        }

        check_case(model,
                   "Edge",
                   cases[i].name,
                   text,
                   cases[i].expected,
                   cases[i].mode);

        free(text);
    }

    printf("\nCompleted %d edge-case file tests.\n\n", n);


#ifdef RUN_STRESS_TESTS

    printf("--- Stress tests ---\n\n");

    /* A single token longer than struct word.data[1000] */

    char *huge_token = repeat_text("a", 1500);

    if (huge_token != NULL)
    {
        check_case(model,
                   "Stress",
                   "Single token longer than 1000 chars",
                   huge_token,
                   NULL,
                   EXPECT_NO_CRASH);

        free(huge_token);
    }


    /* Non-ASCII characters */

    check_case(model,
               "Stress",
               "Non-ASCII (UTF-8) text",
               "caf\xC3\xA9 r\xC3\xA9sum\xC3\xA9 na\xC3\xAFve football match",
               NULL,
               EXPECT_NO_CRASH);


    /* Roughly 1 MB document */

    char *big_doc = repeat_text(
        "The player scored a goal and the team won the football match tonight. ",
        15000
    );

    if (big_doc != NULL)
    {
        check_case(model,
                   "Stress",
                   "Very large document (~1 MB)",
                   big_doc,
                   "Sports",
                   EXPECT_EXACT);

        free(big_doc);
    }

#endif

}


/* =========================================
 * Consistency tests
 * ========================================= */

static int same_label(const char *a, const char *b)
{
    return a != NULL &&
           b != NULL &&
           strcmp(a, b) == 0;
}


static void run_consistency_tests(NaiveBayesModel model)
{
    printf("=================================\n");
    printf(" CONSISTENCY TESTS\n");
    printf("=================================\n\n");


    const char *a =
        "The team won the football match and the player scored a goal";

    const char *b =
        "The doctor prescribed medicine to the patient in the hospital";


    /* Same input twice gives the same answer */

    char *r1 = predict_one(model, a);

    char *r2 = predict_one(model, a);

    record("Consistency",
           "Same document predicted twice gives same label",
           same_label(r1, r2),
           r1,
           r2);


    char *rb = predict_one(model, b);


    /* Batch prediction matches single prediction */

    char *docs[3] = {
        dup_str(a),
        dup_str(b),
        dup_str(a)
    };


    char **res = Naivebayes_predict(model, docs, 3);


    if (res == NULL)
    {
        record("Consistency",
               "Batch prediction returns results",
               0,
               "non-NULL",
               NULL);
    }
    else
    {
        record("Consistency",
               "Batch result [0] matches single prediction",
               same_label(res[0], r1),
               r1,
               res[0]);

        record("Consistency",
               "Batch result [1] matches single prediction",
               same_label(res[1], rb),
               rb,
               res[1]);

        record("Consistency",
               "Duplicate documents in one batch get same label",
               same_label(res[0], res[2]),
               res[0],
               res[2]);


        for (int i = 0; i < 3; i++)
            free(res[i]);

        free(res);
    }


    for (int i = 0; i < 3; i++)
        free(docs[i]);

    free(r1);
    free(r2);
    free(rb);


    /* Zero documents must not crash */

    char *none[1] = { NULL };

    char **zero = Naivebayes_predict(model, none, 0);

    record("Consistency",
           "Predict with 0 documents does not crash",
           1,
           "no crash (any result)",
           zero ? "non-NULL array" : "NULL");

    free(zero);
}


/* =========================================
 * Small-model tests
 * ========================================= */

typedef struct
{
    const char *text;
    const char *expected;

} Query;


static void run_small_model(const char *name,
                            const char *train_texts[],
                            char *train_labels[],
                            int n,
                            const Query *queries,
                            int nq)
{
    char **docs = calloc((size_t)n, sizeof(char *));

    if (docs == NULL)
    {
        record("Model",
               name,
               0,
               "model trained",
               "allocation failed");

        return;
    }


    for (int i = 0; i < n; i++)
    {
        docs[i] = dup_str(train_texts[i]);

        if (docs[i] == NULL)
        {
            record("Model",
                   name,
                   0,
                   "training documents allocated",
                   "allocation failed");

            free_documents(docs, n);

            return;
        }
    }


    NaiveBayesModel m = Naivebayes_fit(docs, train_labels, n);


    for (int q = 0; q < nq; q++)
    {
        char label_name[160];

        snprintf(label_name,
                 sizeof(label_name),
                 "%s - query %d",
                 name,
                 q + 1);


        char *label = predict_one(m, queries[q].text);

        record("Model",
               label_name,
               same_label(label, queries[q].expected),
               queries[q].expected,
               label);

        free(label);
    }


    NaiveBayes_free(&m);

    free_documents(docs, n);
}


static void run_model_tests(void)
{
    printf("=================================\n");
    printf(" SMALL MODEL TESTS\n");
    printf("=================================\n\n");


    /* Two tiny classes with clearly separated vocabulary */

    const char *two_texts[] = {

        "The player scored a goal and the team won the football match",

        "The coach trained the team before the big match and the player ran fast",

        "The doctor treated the patient in the hospital with medicine",

        "The nurse gave medicine to the patient and the doctor checked the hospital report"
    };


    char *two_labels[] = {
        "Sports",
        "Sports",
        "Health",
        "Health"
    };


    Query two_queries[] = {

        {
            "The player scored a goal in the match",
            "Sports"
        },

        {
            "The doctor treated the patient with medicine",
            "Health"
        }

    };


    run_small_model("Two-class model",
                    two_texts,
                    two_labels,
                    4,
                    two_queries,
                    2);



    /* Only one class: every query can only be that class */

    const char *one_class_texts[] = {

        "The team won the football match",

        "The striker scored a goal in the league"

    };


    char *one_class_labels[] = {
        "Sports",
        "Sports"
    };


    Query one_class_queries[] = {

        {
            "software computer code program",
            "Sports"
        }

    };


    run_small_model("Single-class model",
                    one_class_texts,
                    one_class_labels,
                    2,
                    one_class_queries,
                    1);



    /* Only one training document */

    const char *one_doc_texts[] = {
        "The team won the football match"
    };


    char *one_doc_labels[] = {
        "Sports"
    };


    Query one_doc_queries[] = {

        {
            "football match",
            "Sports"
        }

    };


    run_small_model("Single-document model",
                    one_doc_texts,
                    one_doc_labels,
                    1,
                    one_doc_queries,
                    1);
}


/* =========================================
 * Training-set sanity check
 * ========================================= */

static void run_training_sanity(NaiveBayesModel model,
                                const char *paths[],
                                char *labels[],
                                int n)
{
    printf("=================================\n");
    printf(" TRAINING SET SANITY CHECK\n");
    printf("=================================\n\n");


    int readable = 0;

    int correct = 0;


    for (int i = 0; i < n; i++)
    {
        char *text = read_file(paths[i]);

        if (text == NULL)
            continue;

        readable++;

        char *label = predict_one(model, text);

        if (same_label(label, labels[i]))
            correct++;

        free(label);

        free(text);
    }


    char expected[80];

    char got[80];


    snprintf(expected,
             sizeof(expected),
             ">= 90%% of %d training docs correct",
             n);


    snprintf(got,
             sizeof(got),
             "%d/%d correct (%d readable)",
             correct,
             n,
             readable);


    record("Sanity",
           "Model classifies its own training documents",
           readable == n && correct * 100 >= 90 * n,
           expected,
           got);
}


/* =========================================
 * File handling tests
 * ========================================= */

static void run_file_tests(NaiveBayesModel model)
{
    printf("=================================\n");
    printf(" FILE HANDLING TESTS\n");
    printf("=================================\n\n");


    /* Missing file */

    char *missing = read_file("data/this_file_does_not_exist.txt");

    record("File",
           "read_file on a missing file returns NULL",
           missing == NULL,
           "NULL",
           missing ? "non-NULL" : "NULL");

    free(missing);


    /* Empty file */

    const char *empty_name = "temp_empty_test.txt";

    FILE *fp = fopen(empty_name, "w");


    if (fp == NULL)
    {
        record("File",
               "Create empty temp file",
               0,
               "file created",
               "fopen failed");

        return;
    }


    fclose(fp);


    char *empty = read_file(empty_name);


    record("File",
           "read_file on an empty file returns empty string",
           empty != NULL && empty[0] == '\0',
           "empty string",
           empty ? (empty[0] == '\0' ? "empty string" : "non-empty") : "NULL");


    if (empty != NULL)
    {
        check_case(model,
                   "File",
                   "Predict on an empty file does not crash",
                   empty,
                   NULL,
                   EXPECT_NO_CRASH);
    }


    free(empty);

    remove(empty_name);
}


/* =========================================
 * Main
 * ========================================= */

int main(void)
{
    /* =========================================
     * 1. TRAINING FILE PATHS
     * ========================================= */

    const char *train_paths[NUM_TRAIN] = {

        /* Sports */

        "data/corpus/train/sports/doc1.txt",
        "data/corpus/train/sports/doc2.txt",
        "data/corpus/train/sports/doc3.txt",
        "data/corpus/train/sports/doc4.txt",
        "data/corpus/train/sports/doc5.txt",
        "data/corpus/train/sports/doc6.txt",
        "data/corpus/train/sports/doc7.txt",
        "data/corpus/train/sports/doc8.txt",


        /* Technology */

        "data/corpus/train/technology/doc1.txt",
        "data/corpus/train/technology/doc2.txt",
        "data/corpus/train/technology/doc3.txt",
        "data/corpus/train/technology/doc4.txt",
        "data/corpus/train/technology/doc5.txt",
        "data/corpus/train/technology/doc6.txt",
        "data/corpus/train/technology/doc7.txt",
        "data/corpus/train/technology/doc8.txt",


        /* Health */

        "data/corpus/train/health/doc1.txt",
        "data/corpus/train/health/doc2.txt",
        "data/corpus/train/health/doc3.txt",
        "data/corpus/train/health/doc4.txt",
        "data/corpus/train/health/doc5.txt",
        "data/corpus/train/health/doc6.txt",
        "data/corpus/train/health/doc7.txt",
        "data/corpus/train/health/doc8.txt",


        /* Politics */

        "data/corpus/train/politics/doc1.txt",
        "data/corpus/train/politics/doc2.txt",
        "data/corpus/train/politics/doc3.txt",
        "data/corpus/train/politics/doc4.txt",
        "data/corpus/train/politics/doc5.txt",
        "data/corpus/train/politics/doc6.txt",
        "data/corpus/train/politics/doc7.txt",
        "data/corpus/train/politics/doc8.txt"

    };


    /* =========================================
     * 2. TRAINING LABELS
     * ========================================= */

    char *labels[NUM_TRAIN] = {

        "Sports", "Sports", "Sports", "Sports",
        "Sports", "Sports", "Sports", "Sports",

        "Technology", "Technology", "Technology", "Technology",
        "Technology", "Technology", "Technology", "Technology",

        "Health", "Health", "Health", "Health",
        "Health", "Health", "Health", "Health",

        "Politics", "Politics", "Politics", "Politics",
        "Politics", "Politics", "Politics", "Politics"

    };


    /* =========================================
     * 3. LOAD TRAINING DOCUMENTS
     * ========================================= */

    char **docs = calloc(NUM_TRAIN, sizeof(char *));

    if (docs == NULL)
    {
        printf("Memory allocation failed.\n");

        return 1;
    }


    int loaded = 0;


    for (int i = 0; i < NUM_TRAIN; i++)
    {
        docs[i] = read_file(train_paths[i]);

        if (docs[i] == NULL)
        {
            printf("Could not load training document: %s\n",
                   train_paths[i]);

            free_documents(docs, loaded);

            return 1;
        }

        loaded++;
    }


    printf("Loaded %d training documents.\n\n", loaded);


    /* =========================================
     * 4. TRAIN NAIVE BAYES MODEL
     * ========================================= */

    NaiveBayesModel model =
        Naivebayes_fit(docs, labels, NUM_TRAIN);

    printf("Training completed.\n\n");


    /* =========================================
     * 5. NORMAL TEST FILE PATHS
     * ========================================= */

    const char *test_paths[NUM_TEST] = {

        "data/corpus/test/test1.txt",
        "data/corpus/test/test2.txt",
        "data/corpus/test/test3.txt",
        "data/corpus/test/test4.txt",
        "data/corpus/test/test5.txt",
        "data/corpus/test/test6.txt",
        "data/corpus/test/test7.txt",
        "data/corpus/test/test8.txt"

    };


    /* Expected labels are used ONLY for evaluation.
     * They are NOT passed to Naivebayes_predict().
     */

    char *expected_labels[NUM_TEST] = {

        "Sports",
        "Sports",

        "Technology",
        "Technology",

        "Health",
        "Health",

        "Politics",
        "Politics"

    };


    /* =========================================
     * 6. LOAD NORMAL TEST DOCUMENTS
     * ========================================= */

    char **test_docs = calloc(NUM_TEST, sizeof(char *));

    if (test_docs == NULL)
    {
        printf("Memory allocation failed.\n");

        NaiveBayes_free(&model);

        free_documents(docs, NUM_TRAIN);

        return 1;
    }


    loaded = 0;


    for (int i = 0; i < NUM_TEST; i++)
    {
        test_docs[i] = read_file(test_paths[i]);

        if (test_docs[i] == NULL)
        {
            printf("Could not load test document: %s\n",
                   test_paths[i]);

            free_documents(test_docs, loaded);

            NaiveBayes_free(&model);

            free_documents(docs, NUM_TRAIN);

            return 1;
        }

        loaded++;
    }


    printf("Loaded %d test documents.\n\n", loaded);


    /* =========================================
     * 7. NORMAL CASE PREDICTION
     * ========================================= */

    char **predictions =
        Naivebayes_predict(model, test_docs, NUM_TEST);


    if (predictions == NULL)
    {
        printf("Prediction failed.\n");

        free_documents(test_docs, NUM_TEST);

        NaiveBayes_free(&model);

        free_documents(docs, NUM_TRAIN);

        return 1;
    }


    printf("=================================\n");
    printf(" NORMAL CASE TESTS\n");
    printf("=================================\n\n");


    int correct = 0;


    for (int i = 0; i < NUM_TEST; i++)
    {
        char name[64];

        snprintf(name,
                 sizeof(name),
                 "Test document %d",
                 i + 1);


        int ok = (predictions[i] != NULL &&
                  strcmp(predictions[i], expected_labels[i]) == 0);


        if (ok)
            correct++;


        record("Normal",
               name,
               ok,
               expected_labels[i],
               predictions[i]);
    }


    printf("Normal-case accuracy: %d/%d (%.2f%%)\n\n",
           correct,
           NUM_TEST,
           (correct * 100.0) / NUM_TEST);


    /* Free predictions */

    for (int i = 0; i < NUM_TEST; i++)
        free(predictions[i]);

    free(predictions);


    /* =========================================
     * 8. ADDITIONAL TEST CASES
     * ========================================= */

    run_edge_tests(model);

    run_consistency_tests(model);

    run_training_sanity(model,
                        train_paths,
                        labels,
                        NUM_TRAIN);

    run_file_tests(model);

    run_model_tests();


    /* =========================================
     * 9. SUMMARY
     * ========================================= */

    printf("=================================\n");
    printf(" SUMMARY\n");
    printf("=================================\n");

    printf("Total tests : %d\n", g_total);

    printf("Passed      : %d\n", g_passed);

    printf("Failed      : %d\n", g_failed);

    printf("Pass rate   : %.2f%%\n",
           g_total > 0
           ? (g_passed * 100.0) / g_total
           : 0.0);


    /* =========================================
     * 10. MEMORY CLEANUP
     * ========================================= */

    free_documents(test_docs, NUM_TEST);

    NaiveBayes_free(&model);

    free_documents(docs, NUM_TRAIN);


    printf("\nMemory cleanup completed.\n");


    return g_failed > 0 ? 1 : 0;
}

