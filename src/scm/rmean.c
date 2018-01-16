
void
rmean(float *data, int n, float mean) {
    int i;
    for (i = 0; i < n; i++) {
        data[i] -= mean;
    }
}

void
rmean_(float *data, int *n, float *mean) {
    rmean(data, *n, *mean);
}

void
rmean__(float *data, int *n, float *mean) {
    rmean(data, *n, *mean);
}

/**
 *  Compute the mean of a data series
 *
 *  - data - Data series to compute mean of
 *  - n - Length of data
 *
 *  A double precision variable is used internally to
 *    reduce numerical error
 */
float
compute_mean(float *data, int n) {
    int i;
    double mean = 0.0;
    for(i = 0; i < n; i++) {
        mean += data[i];
    }
    return mean / n;
}

/**
 *  Remove mean from a data set
 *
 *  - data - Data set to remove mean from
 *  - n - Length of data
 *
 *  Input data is overwritten on return
 *
 *  Calls compute_mean() and rmean() internally.
 */
void
remove_mean(float *data, int n) {
    float mean = compute_mean(data, n);
    rmean(data, n, mean);
}

void remove_mean_ (float *data, int *n) {  remove_mean(data, *n); }
void remove_mean__(float *data, int *n) {  remove_mean(data, *n); }
