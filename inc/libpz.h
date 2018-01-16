/**
 * @file libpz.h
 *
 * @brief Polezero Functions
 */


/**
 * Transfer function, frequency response
 */
struct Transfer {
    // Length of time series
    int n;
    // Time sampling of time series
    double dt;
    // Length of FFT (power of 2 > n)
    int nfft;
    // Frequency Spacing = 1.0 / (n * dt)
    double df;
    // Number of Frequencies nfft/2 + 1
    int nfreqs;
    // Response, length nfft
    double *f;
};

typedef struct Transfer Transfer;

/**
 * Response as Real and Imaginary components
 */
struct Response {
    // Length of response
    int n;
    // Real part of response
    double *re;
    // Imaginary part of response
    double *im;
};
typedef struct Response Response;


void sac_station_id_split(char *id, char *net, char *sta, char *loc, char *cha);
void sac_station_id(char *id);
void sac_reference_time(char *when);

Transfer * transfer_new(int n, double dt);
void transfer_free(Transfer *t);

void response_recip(Response *r);
Response * response_ones(int n);
Response * response_from_2vec(double *r2, int n);
void response_free(Response *r);
