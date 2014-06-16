
void NSSacView_update(void *id);
void NSSacView_mds(void *id, int type, float x, float y);
void NSSacView_size(void *id, int *width, int *height);
void NSSacView_poly(void *id, int n, float *x, float *y);
void NSSacView_color(void *id, float r, float g, float b);
void NSSacView_width(void *id, int width);
void NSSacView_show_image(void *id, 
                          float *data,
                          unsigned int iw,
                          unsigned int ih,
                          float xmin,
                          float xmax,
                          float ymin,
                          float ymax,
                          float x,
                          float y,
                          float w,
                          float h,
                          int npseudocolors,
                          int nsacolors,
                          int ndefcolors,
                          int lbinary);
void NSSacView_wait_for_key_press(void *id, float *x, float *y, char *c);
void sac_create_window (void *id, int n);
int  sac_find_window   (void *id, int n);
void sac_focus_window  (void *id, int n);
void sac_table_update (void *id);
void sac_table_refresh (void *id);
