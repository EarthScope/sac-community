
#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "request.h"

typedef enum ResponseType ResponseType;

/**
 * @brief Response Type
 * @details Response Type
 */
enum ResponseType {
    ResponseSacPZ = 1, /**< sac pole zero response type */
    ResponseResp = 2, /**< evalresp response type */
};

void      response_init(request *pz);
request * response_new();
request * response_new_from_nslc(char *net, char *sta, char *loc, char *cha);
void      response_set_kind(request *pz, ResponseType rt);
char *    response_filename(request *pz, char *dst, size_t n);
void      response_set_time(request *s, timespec64 t);
void      response_set_start(request *s, timespec64 t);
void      response_set_end(request *s, timespec64 t);
void      response_set_network(request *s, char *net);
void      response_set_station(request *s, char *sta);
void      response_set_location(request *s, char *loc);
void      response_set_channel(request *s, char *cha);
int       response_is_ok(request *s);

#endif /*_RESPONSE_H_*/
