
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gdm.h"

#include "cpf.h"

/** 
 * Erase the screen of window of all active devices
 *
 * @date    870217:  Changed name from erasescreen.
 * @date    831026:  Original version.
 *
 */
void 
erase(int *nerr)
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	*nerr = 0;

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->erase) {
            dev[i]->erase();
          }
        }

}

void 
stroke()
{

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        for(i = 0; i < n; i++) {
          if(dev[i]->on && dev[i]->stroke) {
            dev[i]->stroke();
          }
        }

}


void
xsave() {

        int nerr;
        char file[1024];
        int len;
        char *p;

        int i, n;
        display_t **dev;

        memset(file, 0, 1024);
        if(lcmore(&nerr)) {
          lcchar(1023, &file[0], 1024, &len);
        }
        if(strlen(file) == 0 || file[0] == 0) {
          fprintf(stdout, "save requires an output filename\n");
          return;
        }

        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        p = strrchr(file, '.');
        if(!p) {
          fprintf(stderr, "save file: missing a required file extension\n");
          return;
        }
        p++;
        for(i = 0; i < n; i++) {
          if(strcasecmp(p, dev[i]->extension) == 0 && dev[i]->save) {
            fprintf(stderr, "save file %s [%s]\n", file, dev[i]->name);
            dev[i]->save( dev[i], file );
            return;
          }
        }
        fprintf(stderr, "save file: Error saving the file: %s\n", file);
        fprintf(stderr, "           %s\n", p);
}

void
set_window_width(int width) {
  int i, n;
  display_t **dev;  

  n   = gdm_get_ndevices();
  dev = gdm_get_devices();

  for(i = 0; i < n; i++) {
    if(dev[i]->set_window_width) {
      dev[i]->set_window_width( width );
    }
  }
}

void
set_window_height(int height) {
  int i, n;
  display_t **dev;  

  n   = gdm_get_ndevices();
  dev = gdm_get_devices();

  for(i = 0; i < n; i++) {
    if(dev[i]->set_window_height) {
      dev[i]->set_window_height( height );
    }
  }
}

int
get_file_descriptor() {
  int i, n;
  display_t **dev;  

  n   = gdm_get_ndevices();
  dev = gdm_get_devices();
  
  for(i = 0; i < n; i++) {
    if(dev[i]->on && dev[i]->get_file_descriptor) {
      return dev[i]->get_file_descriptor();
    }
  }
  return -1;
}

void
handle_event(int *nerr) {
  int i, n;
  display_t **dev;  

  n   = gdm_get_ndevices();
  dev = gdm_get_devices();
  
  for(i = 0; i < n; i++) {
    if(dev[i]->on && dev[i]->handle_event) {
      dev[i]->handle_event( nerr );
      return;
    }
  }
}
