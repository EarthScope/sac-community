
#ifndef __FID_H__

int fid(char *file);

enum {
    FID_NULL  = 0,
    FID_SAC   = 1<<1,
    FID_AH00  = 1<<2,
    FID_AH10  = 1<<3,
    FID_AH20  = 1<<4,
    FID_CSS28 = 1<<5,
    FID_CSS30 = 1<<6,
    FID_SEGY  = 1<<7,
    FID_GSE20 = 1<<8,
    FID_MSEED = 1<<9,
    FID_SACA = (1<<10),
};


#endif /* __FID_H__ */
