
struct _station_data {
    char *net;
    char *sta;
    double stla;
    double stlo;
    double stel;
    char *sitename;
    datetime *start;
    datetime *end;
};

struct _channel_meta {
    char net[16];
    char sta[16];
    char loc[16];
    char cha[16];
    float stla;
    float stlo;
    float stel;
    float stdp;
    float cmpaz;
    float cmpinc;
    char desc[256];
    float scale;
    float scalefreq;
    char scaleunits[16];
    float samprate;
    datetime starttime;
    datetime endtime;
};

