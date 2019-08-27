#include <string.h>

#include "amf.h"
#include "proto.h"
#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "errors.h"

#include <fstr.h>
/** 
 * Determine the byte order of the machine
 * 
 * @return 
 *    - ENDIAN_BIG 
 *    - ENDIAN_LITTLE
 */
int
CheckByteOrder() {
    static int byte_order = ENDIAN_UNKNOWN;
    short int word = 0x0001;
    char *byte = (char *) &word;
    if (byte_order == ENDIAN_UNKNOWN) {
        byte_order = (!byte[0]) ? ENDIAN_BIG : ENDIAN_LITTLE;
    }
    return byte_order;
}

void sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr);

void
sac_write_r(sac * s, char *filename, int write_data, int lswap, int *nerr) {
    sac_write_internal(s, filename, write_data, lswap, nerr);
}
int
is_kundef(char *kvalue) {
    size_t n = strlen(kvalue);
    return n == strlen(SAC_CHAR_UNDEFINED) &&
        (memcmp(kvalue, SAC_CHAR_UNDEFINED, strlen(SAC_CHAR_UNDEFINED)) == 0);
}
int
is_kundef2(char *kvalue) {
    size_t n = strlen(kvalue);
    return n == strlen(SAC_CHAR_UNDEFINED_2 ) &&
        (memcmp(kvalue, SAC_CHAR_UNDEFINED_2, strlen(SAC_CHAR_UNDEFINED_2)) == 0);
}

int
is_kundefn(char *kvalue, int item) {
    if(item == 2) {
        return is_kundef2(kvalue);
    }
    return is_kundef(kvalue);
}
int
sac_byte_order(int getset) {
    int i, n;
    int byte_order;
    char *env_string;
    char *env_big[] = { "big", "solaris", "powerpc", "ppc" };
    char *env_little[] = { "little", "x86", "intel" };
    static int swap = -1;       /* Default to System Byte Order */

    if (getset < 0 && swap == -1) {     /* Initial Call, Get */
        env_string = getenv("SAC_WRITE_BYTE_ORDER");
        getset = CheckByteOrder();
        if (env_string != NULL) {
            n = strlen(env_string);
            for (i = 0; i < (int) (sizeof(env_big) / sizeof(char *)); i++) {
                if (strncasecmp
                    (env_string, env_big[i],
                     min(n, (int) strlen(env_big[i]))) == 0) {
                    getset = ENDIAN_BIG;
                }
            }
            for (i = 0; i < (int) (sizeof(env_little) / sizeof(char *)); i++) {
                if (strncasecmp
                    (env_string, env_little[i],
                     min(n, (int) strlen(env_little[i]))) == 0) {
                    getset = ENDIAN_LITTLE;
                }
            }
        }
    }
    if (getset >= 0) {          /* Set, Initial or later call */
        byte_order = CheckByteOrder();
        swap = !(byte_order == getset);
    }
    return swap;
}
