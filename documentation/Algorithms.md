
## Naive Bayes Classifier

### Description

The Naive Bayes classifier is a supervised machine learning algorithm used for text classification.

This implementation uses a Multinomial Naive Bayes approach with Laplace smoothing. During training, it calculates class priors and word likelihoods and stores the learned information in a `NaiveBayesModel`.

### Function Prototypes

```c
NaiveBayesModel Naivebayes_fit(char **docs, char **labels, int num_docs);

char **Naivebayes_predict(NaiveBayesModel model, char **test_docs, int num_docs);

void NaiveBayes_free(NaiveBayesModel *model);
```

### Input Types

**Naivebayes_fit()**

- `docs`: Array of strings containing the training documents (`char **`).
- `labels`: Array of strings containing the corresponding class labels (`char **`).
- `num_docs`: Number of training documents (`int`).

Each document must have a corresponding label at the same index.

**Naivebayes_predict()**

- `model`: Trained Naive Bayes model (`NaiveBayesModel`).
- `test_docs`: Array of strings containing the documents to classify (`char **`).
- `num_docs`: Number of test documents (`int`).

**NaiveBayes_free()**

- `model`: Pointer to the trained model (`NaiveBayesModel *`) whose allocated memory must be released.

### Output Types

**Naivebayes_fit()**

- Returns a `NaiveBayesModel` containing the vocabulary, class priors, and learned word-frequency information.

**Naivebayes_predict()**

- Returns a `char **` containing the predicted class label for each test document.
- Each predicted label is stored as a separately allocated string.
- Returns `NULL` if prediction cannot be performed successfully.

**NaiveBayes_free()**

- Returns `void`.
- Releases the dynamically allocated memory owned by the model and resets its fields.

### How to Use

1. Prepare an array of training documents and their corresponding labels.
2. Pass them to `Naivebayes_fit()` to train the classifier.
3. Pass the trained model and test documents to `Naivebayes_predict()`.
4. Access the predicted label for each test document.
5. Free the prediction strings and prediction array.
6. Once the model is no longer required, call `NaiveBayes_free()`.

### Example

```c
char *docs[] = {
    "I love this movie",
    "This movie is terrible",
    "Amazing acting and story",
    "Very boring movie"
};

char *labels[] = {
    "Positive",
    "Negative",
    "Positive",
    "Negative"
};

int num_docs = 4;

NaiveBayesModel model = Naivebayes_fit(
    docs, labels, num_docs
);

char *test_docs[] = {
    "Amazing movie",
    "Very boring story"
};

int num_test = 2;

char **predictions = Naivebayes_predict(
    model, test_docs, num_test
);

if (predictions != NULL) {
    for (int i = 0; i < num_test; i++) {
        printf("%s\n", predictions[i]);
    }

    // Free individual prediction strings
    for (int i = 0; i < num_test; i++) {
        free(predictions[i]);
    }

    // Free prediction array
    free(predictions);
}

// Free trained model
NaiveBayes_free(&model);
```

### Memory Management and Important Notes

- **Training model:** `Naivebayes_fit()` dynamically allocates memory for the vocabulary, class priors, and word-frequency information. Always call `NaiveBayes_free(&model)` when the model is no longer needed.

- **Prediction output:** `Naivebayes_predict()` allocates memory for the prediction array and each predicted label separately. Free every `predictions[i]` first, followed by `predictions`.

- **Model ownership:** The model owns its vocabulary, class labels, and other dynamically allocated structures. Some internal pointers refer to memory owned by other parts of the model. These borrowed pointers must not be freed separately.

- **Do not free the model directly:** The model is returned as a structure by value, not as a dynamically allocated pointer. Use `NaiveBayes_free(&model)` rather than `free(model)`.

- **Model reuse:** The same trained model can be used to classify multiple sets of test documents. You do not need to train the model again for every prediction.

- **Multiple models:** If you train another model, remember to free the previous model when it is no longer needed.

- **Input arrays:** The training and test document arrays and their strings are supplied by the caller. They are not released by `NaiveBayes_free()`.
