/**
 * @file
 * @brief Data requests
 */


#ifndef _DATA_REQUEST_
#define _DATA_REQUEST_

#include <sacio/timespec.h>
#include <libmseed/libmseed.h>

#include "request.h"
#include "chash.h"


typedef enum Quality Quality;
/**
 * @brief Data Quality
 * @public
 *
 * @memberof data
 * @ingroup data
 *
 * http://ds.iris.edu/ds/nodes/dmc/manuals/breq_fast/#quality-option
 */
enum Quality {
    QualityD        = 1, /**< Unknown 'D' @ingroup data */
    QualityRaw      = 2, /**< Raw  'R' */
    QualityQual     = 3, /**< Quality 'Q' */
    QualityModified = 4, /**< Modified  'M' */
    QualityBest     = 5, /**< Best  'B' */
    QualityMerged   = 6, /**< Merged 'M' */
    QualityQC       = 7, /**< Quality controlled 'Q' */
    QualityUnknown  = 8, /**< Unknown quality 'D' */
    QualityAll      = 9, /**< All  '*' */
};

typedef struct data_request data_request;


// Data Requests
request *data_avail_new();
void     data_avail_set_time_range(request *r, timespec64 start, timespec64 end);
void     data_avail_set_network(request *r, char *net);
void     data_avail_set_station(request *r, char *sta);
void     data_avail_set_location(request *r, char *loc);
void     data_avail_set_channel(request *r, char *cha);
void     data_avail_set_quality(request *r, Quality quality);
int      data_avail_is_ok(request *r, int need_net_sta);
void     data_avail_use_duration(request *r, duration *d);
char *   data_avail_from_station_file(request *r, char *file);
void     data_avail_set_origin(request *r, double lon, double lat);
void     data_avail_set_radius(request *r, double minr, double maxr);
void     data_avail_set_region(request *r, double minlon, double maxlon,
                             double minlat, double maxlat);

data_request * data_request_parse(char *data);
void           data_request_chunks(data_request *fdr, size_t max);
void           data_request_write(data_request *fdr, FILE *fp);
MS3TraceList * data_request_download(data_request *fdr,
                                               char *filename,
                                               char *prefix,
                                               int to_mseed,
                                               int to_sac);
void           data_request_write_to_file(data_request *fdr,
                                          char *filename);

void           data_request_free(data_request *r);


#endif /* _DATA_REQUEST_ */
