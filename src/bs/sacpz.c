
#include "octopus.h"
#include "string_utils.h"

char *rstrip(char *s);

void
sacpz_init(request *pz) {
    request_set_url(pz, "https://service.iris.edu/irisws/sacpz/1/query?");
    request_set_arg(pz, "nodata", arg_int_new(404));
}

request *
sacpz_new() {
    request *pz = request_new();
    sacpz_init(pz);
    return pz;
}

void
sacpz_set_kind(request *pz, ResponseType rt) {
    switch(rt) {
    case ResponseSacPZ:
        request_set_url(pz, "https://service.iris.edu/irisws/sacpz/1/query?");
        break;
    case ResponseResp:
        request_set_url(pz, "https://service.iris.edu/irisws/resp/1/query?");
        break;
    }
}

request *
sacpz_new_from_nslc(char *net, char *sta, char *loc, char *cha) {
    request *r = sacpz_new();
    sacpz_set_network(r, net);
    sacpz_set_station(r, sta);
    sacpz_set_location(r, loc);
    sacpz_set_channel(r, cha);
    return r;
}


static char *
empty_if_wild(char *v) {
    //char out[2] = "";
    if(strchr(v, '*') || strchr(v, '?') || strcmp(v, "--") == 0 ) {
        return "";
    }
    return v;
}

char *
sacpz_filename(request *pz, char *dst, size_t n) {
    char *key[] = {"net", "sta", "loc", "cha"};
    Arg *a1 = NULL, *a2 = NULL;
    char tmp[128] = { 0 };;
    const char *url = request_get_url(pz);
    
    if(strstr(url, "sacpz")) {
        snprintf(dst, n, "SAC_PZs_");
        for(size_t i = 0; i < 4; i++) {
            arg_to_string(request_get_arg(pz, key[i]), tmp, sizeof(tmp));
            snprintf(dst, n, "%s%s_", dst, empty_if_wild(tmp));
        }
        if((a1 = request_get_arg(pz, "time"))) {
            snprintf(dst, n, "%s%s", dst, arg_to_string(a1, tmp, sizeof(tmp)));
        } else if((a1 = request_get_arg(pz, "start")) &&
                  (a2 = request_get_arg(pz, "end"))) {
            snprintf(dst, n, "%s%s", dst,
                     arg_to_string(a1, tmp, sizeof(tmp)));
            snprintf(dst, n, "%s_%s", dst,
                     arg_to_string(a2, tmp, sizeof(tmp)));
        }
    } else if(strstr(url, "resp")) {
        snprintf(dst, n, "RESP");
        for(size_t i = 0; i < 4; i++) {
            arg_to_string(request_get_arg(pz, key[i]), tmp, sizeof(tmp));
            snprintf(dst, n, "%s.%s", dst, empty_if_wild(tmp));
        }
    }
    return dst;
}
void
sacpz_set_time(request *s, timespec64 t) {
    request_set_arg(s, "time", arg_time_new(t));
}
void
sacpz_set_start(request *s, timespec64 t) {
    request_set_arg(s, "start", arg_time_new(t));
}
void
sacpz_set_end(request *s, timespec64 t) {
    request_set_arg(s, "end", arg_time_new(t));
}
void
sacpz_set_network(request *s, char *net) {
    request_set_arg(s, "net", arg_string_new(net));
}
void
sacpz_set_station(request *s, char *sta) {
    request_set_arg(s, "sta", arg_string_new(sta));
}
void
sacpz_set_location(request *s, char *loc) {
    request_set_arg(s, "loc", arg_string_new(loc));
}
void
sacpz_set_channel(request *s, char *cha) {
    request_set_arg(s, "cha", arg_string_new(cha));
}

int
sacpz_is_ok(request *s) {
    char *keys[] = {"net", "sta", "loc", "cha" };
    for(size_t i = 0; i < 4; i++) {
        if(!request_get_arg(s, keys[i])) {
            return 0;
        }
    }
    return 1;
}

