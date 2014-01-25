
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gd3.x11.h"

#include "config.h"

#include "select.h"
#include "debug.h"

int
error3(Display *display, XErrorEvent *error) {
  if(error->error_code) {
    char msg[128];
    XGetErrorText(display, error->error_code, msg, sizeof(msg));
    fprintf(stderr,
            "Seismic Analysis Code (SAC) received an X11 error:\n"
            "%s\n"
            "serial %lu error_code %u request_code %u minor_code %u\n",
            msg, error->serial, error->error_code, error->request_code, error->minor_code);
  }
  return 1;
}

int
ioerror3(Display *display) {
  UNUSED(display);
  if(errno == EPIPE) {
    fprintf(stderr, 
	    "Seismic Analysis Code (SAC) lost the connection to the display\n"
	    "Most likely the X server was shut down or you "
	    "killed/destroyed the application\n");
  } else {
    fprintf(stderr, "Seismic Analysis Code (SAC): Fatal IO Error "
	    "%d (%s) on the X server\n",
	    errno, strerror(errno)); 
  }
#ifdef READLINE    
  /* This should probably do more, like clean up memory and such
     and should probably be within its own fucntion */
  rl_callback_handler_remove();
#endif /* READLINE */
  exit(1);
}


void
error3_handling() {
  XSetErrorHandler(error3);
  XSetIOErrorHandler(ioerror3);
}
