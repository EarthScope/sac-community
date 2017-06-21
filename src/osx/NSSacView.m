
#import "NSSacView.h"
#import "osx_color.h"

#ifndef MAC_OS_X_VERSION_10_12
#define MAC_OS_X_VERSION_10_12 101200
#endif

/* macOS 10.12 deprecates a bunch of constants. */
#if MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12

  #define NSCompositingOperationSourceOver NSCompositeSourceOver

  /* And adds NSWindowStyleMask. */
  #ifdef __OBJC__
    typedef NSUInteger NSWindowStyleMask;
  #endif
#endif

extern OSXColor pixdef6[100000];

void frange(float *z, int n, float *zmin, float *zmax);
void osx_get_bg_color(float *r, float *g, float *b);
void 
scaleimage(float *input_image,
           unsigned int width,
           unsigned int height,
           float *output_image,
           unsigned int width_out,
           unsigned int height_out,
           float xstart,
           float xstop,
           float ystart,
           float ystop,
           int *nerr);

void osx_sac_nsview(void *id);

@implementation NSSacViewObj 

- (id) initType: (int) t  withPoint: (NSPoint) p {
    self = [super init];
    if(self) {
        type  = t;
        point = p;
    }
    return self;
}
- (id) initPoly: (NSPoint *) p count: (int) n {
    self = [super init];
    if(self) {
        type   = 4;
        points = p;
        npts   = n;
    }
    return self;
}
- (id) initColor: (NSColor *) c {
    self = [super init];
    if(self) {
        type   = 5;
        color  = c;
    }
    return self;
}

- (id) initWidth: (int) w {
    self = [super init];
    if(self) {
        type   = 6;
        width  = w;
    }
    return self;    
}

- (NSString *) description {
    if(type == 5) {
        return [NSString stringWithFormat: @"%d %@", type, color];
    } 
    if(type == 4) {
        return [NSString stringWithFormat: @"%d npts: %d", type, npts];
    }
    return [NSString stringWithFormat: @"%d %@", type, 
                     NSStringFromPoint(point)];
    
}

- (NSColor *) color { return color; }
- (int) type { return type; }
- (NSPoint) point { return point; }
- (int) npts { return npts; }
- (NSPoint *) points { return points; }
- (int) width { return width; }

- (void) setImageData: (float *) data width: (int) w height: (int) h { 
    type        = 7;
    imageData   = (float *) malloc(sizeof(float) * w * h);
    memcpy(imageData, data, sizeof(float) * w * h);
    imageWidth  = w;
    imageHeight = h;
}
- (void) setImageLocationX: (float) x y: (float) y w: (float) w h: (float) h {
    imageX = x;
    imageY = y;
    imageW = w;
    imageH = h;
}
- (void) setImageColors: (int) npseudo nsacolors: (int)nsa ndefcolors: (int)ndef {
    npseudocolors = npseudo;
    nsacolors     = nsa;
    ndefcolors    = ndef;
}

- (void) setImageLimitsXmin: (float) xmin xmax: (float) xmax 
                       ymin: (float) ymin ymax: (float) ymax {
    imageXmin = xmin;
    imageXmax = xmax;
    imageYmin = ymin;
    imageYmax = ymax;
}
- (void) setImageBinary: (int) bin {
    imageBinary = bin;
}

- (NSPoint) pointWithBounds: (NSRect) bounds ratio:(float) ratio{
    return NSMakePoint(point.x * bounds.size.width,
                       point.y * bounds.size.width / ratio);
}

- (NSPoint) imagePosition: (NSRect) bounds {
    return NSMakePoint( imageX * bounds.size.width,
                        (imageY-imageH) * bounds.size.height );
}

- (id) image: (NSRect) bounds {
    int i,j,k,kk;
    int ipw, iph, ipx, ipy, nerr;
    float *newData;
    float fmin, fmax, zmin, zmax, wrange;
    float fsave;
    unsigned short **cdata;
    /* Image Size */
    ipw = imageW * bounds.size.width;
    iph = imageH * bounds.size.height;
    /* Image Location */
    ipx = imageX * bounds.size.width;
    ipy = imageY * bounds.size.height;
    //NSLog(@"%f %f", bounds.size.width, bounds.size.height);
    //NSLog(@"%f %f -> %d %d @ %d %d", imageW, imageH, ipw, iph, ipx, ipy);

    /* New Image Size */
    newData = (float *) malloc(sizeof(float)*ipw*iph);
    //NSLog(@"scaleimage: %f %f %f %f", imageXmin, imageXmax, imageYmin, imageYmax);

    scaleimage(imageData, (unsigned int)imageWidth, (unsigned int) imageHeight,
               newData, (unsigned int) ipw, (unsigned int) iph,
               imageXmin, imageXmax, 
               imageYmin, imageYmax,
               &nerr);
    //NSLog(@"scaleimage done");

    frange(newData, ipw*iph, &zmin, &zmax);
    /* Color the Image */
    //NSLog(@"color image");
    fmin = (float) nsacolors + 1.0 + (float) ndefcolors;
    fmax = (float) (nsacolors + npseudocolors + ndefcolors);
    wrange = (zmax - zmin);

    cdata = (unsigned short **) malloc(sizeof(unsigned short *) * 3);
    for(i = 0; i < 3; i++) {
        cdata[i] = (unsigned short *) malloc(sizeof(unsigned short) * ipw * iph);
    }

    if(wrange == 0.0) {
        wrange = 1e-6;
    }

    k = 0;
    for(i = iph-1; i >= 0; i--) {
        for(j = 0; j < ipw; j++) {
            kk = (i*ipw) + j;
            fsave = (newData[kk] - zmin)/wrange;
            fsave = fsave * (float)npseudocolors;
            fsave = fsave + (float) nsacolors + 1.0 + (float)ndefcolors;
            fsave = fsave < fmin ? fmin : fsave;
            fsave = fsave > fmax ? fmax : fsave;
            cdata[0][k] = pixdef6[(unsigned int)fsave].red;
            cdata[1][k] = pixdef6[(unsigned int)fsave].green;
            cdata[2][k] = pixdef6[(unsigned int)fsave].blue;
            //fprintf(stderr, "%f -> %f %hu %hu %hu \n", fsave, newData[kk], cdata[0][k], cdata[1][k],cdata[2][k]);
            k++;
        }
    }
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] 
                                   initWithBitmapDataPlanes: (unsigned char **) cdata
                                                 pixelsWide: ipw
                                                 pixelsHigh: iph
                                              bitsPerSample: 16
                                            samplesPerPixel: 3
                                                   hasAlpha: NO
                                                   isPlanar: YES
                                             colorSpaceName: NSDeviceRGBColorSpace
                                                bytesPerRow: ipw*2
                                               bitsPerPixel: 16];
    NSImage *image = [[NSImage alloc] init];
    [image addRepresentation:bitmap];
    return image;
}

@end

@implementation NSSacView 

void
NSSacView_wait_for_key_press(void *id, float *x, float *y, char *c) {
    NSPoint p;
    NSSacView *s = (NSSacView *) id;
    [s waitForKeyPress: c atPoint: &p];
    *x = p.x;
    *y = p.y;
}

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
                          int lbinary) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;

            [s  addImageData: data
                       width: iw   height: ih
                        xmin: xmin   xmax: xmax
                        ymin: ymin   ymax: ymax
                           x: x        y: y
                           w: w        h: h
               npseudocolors: npseudocolors
                   nsacolors: nsacolors
                  ndefcolors: ndefcolors
                     lbinary: lbinary];
        });
}
            

void NSSacView_mds(void *id, int type, float x, float y) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;
            //NSLog(@"Thread Main: %d (MDS)", [NSThread isMainThread]);
            if(type == 0) {
                [ s clearStack ];
            }
            [ s addMDSType: type withPoint : NSMakePoint(x,y)];
        });
    /*
    if(type == 3) {
        [s update];
    }
    */
}

void NSSacView_update(void *id) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;
            //NSLog(@"Thread Main: %d (UPDATE)", [NSThread isMainThread]); 
            [s update];
        });
}

void NSSacView_poly(void *id, int n, float *x, float *y) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;
            int i;
            NSPoint *parray;
            //NSLog(@"Thread Main: %d (POLY)", [NSThread isMainThread]);
            parray = (NSPoint*) malloc(sizeof(NSPoint) * n);
            for(i = 0; i < n; i++) {
                parray[i].x = x[i];
                parray[i].y = y[i];
            }
            [s addPoly: parray count: n];
        });
}

void NSSacView_size(void *id, int *width, int *height) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;
            NSRect bounds = [s bounds];
            //NSLog(@"Thread Main: %d (SIZE)", [NSThread isMainThread]);
            *width  = bounds.size.width;
            *height = bounds.size.height;
        });
}

void NSSacView_color(void *id, float r, float g, float b) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            //NSLog(@"Thread Main: %d (COLOR)", [NSThread isMainThread]);
            NSSacView *s = (NSSacView *) id;
            NSColor *c = [NSColor colorWithDeviceRed: r
                                               green: g
                                                blue: b
                                               alpha: 1.0];
            [c retain];
            [s addColor: c];
        });
}

void NSSacView_width(void *id, int width) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            NSSacView *s = (NSSacView *) id;
            [s addWidth: width];
        });
}

- (void) checkThread {
    if(! [NSThread isMainThread]) {
        NSLog(@"NOT MAIN THREAD: Terminating");
        [NSApp terminate:self];
    }
}

- (void) addObject: (NSObject *) object {
    [self checkThread];
    //NSLog(@"Add Object: %@", object);
    [drawing lock];  {
        [objs addObject: object];
    }
    [drawing unlock];

}

- (void) clearStack {
    [self checkThread];
    //NSLog(@"Thread Main: %d (CLEAR)", [NSThread isMainThread]);
    [drawing lock];  {
        [objs removeAllObjects];
    }
    [drawing unlock];
}



- (void) addImageData: (float *) data
                width: (int) width
               height: (int) height
                 xmin: (float) xmin
                 xmax: (float) xmax
                 ymin: (float) ymin
                 ymax: (float) ymax
                    x: (float) x 
                    y: (float) y
                    w: (float) w
                    h: (float) h
        npseudocolors: (int) npseudocolors
            nsacolors: (int) nsacolors
           ndefcolors: (int) ndefcolors
              lbinary: (int) lbinary {
    //NSLog(@"Thread Main: %d", [NSThread isMainThread]);
    NSSacViewObj *obj = [[NSSacViewObj alloc] init];
    [obj setImageData: data width:width height:height];
    [obj setImageLocationX:x y:y w:w h:h];
    [obj setImageLimitsXmin:xmin xmax:xmax ymin:ymin ymax:ymax];
    [obj setImageColors: npseudocolors nsacolors: nsacolors ndefcolors:ndefcolors];
    [obj setImageBinary: lbinary];
    [self addObject: obj];

}

- (void) addWidth: (int) width {
    //NSLog(@"Thread Main: %d (ADD WIDTH)", [NSThread isMainThread]);
    NSSacViewObj *obj = [[NSSacViewObj alloc] initWidth: width];
    [self addObject: obj];
}

- (void) addColor: (NSColor *) c {
    //NSLog(@"Thread Main: %d (ADD COLOR)", [NSThread isMainThread]);
    NSSacViewObj *obj = [[NSSacViewObj alloc] initColor: c];
    [self addObject: obj];
}

- (void) addPoly: (NSPoint *) p count: (int) n {
    //NSLog(@"Thread Main: %d (ADD POLY)", [NSThread isMainThread]);
    NSSacViewObj *obj = [[NSSacViewObj alloc] initPoly: p count: n];
    [self addObject: obj];
}


- (void) update {
    //NSLog(@"Thread Main: %d (UPDATE)", [NSThread isMainThread]);
    [self setNeedsDisplay: YES ];
}

- (void) addMDSType: (int) type withPoint : (NSPoint) p {
    NSSacViewObj *obj = [[NSSacViewObj alloc] initType: type withPoint: p ];
    [self addObject: obj ];
    if(type == 0) {
        NSRect bounds = [self bounds];
        original_ratio = bounds.size.width / bounds.size.height;
    }
}

- (void) erase : (NSRect) rect {
  float r, g, b;
  osx_get_bg_color(&r, &g, &b);
  backgroundColor = [NSColor colorWithDeviceRed: r
                                          green: g
                                           blue: b
                                          alpha: 1.0];
   [backgroundColor set];
    NSRectFill( rect );
}

- (void) setCurrent {
    osx_sac_nsview( self );
}

- (void) awakeFromNib {
    //NSLog(@"Awake from Nib: %@ MAIN: %d", self, [NSThread isMainThread]);
    backgroundColor = [NSColor whiteColor];
    objs = [[NSMutableArray alloc] initWithCapacity: 32];
    [self setCurrent];
    condition = [[NSCondition alloc] init];
    drawing = [[NSCondition alloc] init];
}

- (BOOL) acceptsFirstResponder {
    //NSLog(@"Accept Focus: %@", self);
    return YES;
}

- (void) waitForKeyPress: (char *) c atPoint: (NSPoint *) p {
    [condition lock];
    {
        keyPressed = 0;
        while(keyPressed == 0) {
            [condition wait];
        }
        c[0] = keyPressed;
        p->x = mouseLocation.x;
        p->y = mouseLocation.y;
        keyPressed = 0;
    }
    [condition unlock];
}

- (NSPoint) currentLocation {
    NSPoint mouse = [[self window] mouseLocationOutsideOfEventStream];
    mouse = [self convertPoint:mouse fromView:nil];
    return mouse;
}

- (void) keyUp: (NSEvent *) event {
    unichar buf;
    NSRect bounds = [self bounds];
    mouseLocation = [self currentLocation];
    if(NSPointInRect(mouseLocation, bounds)) {
        [condition lock];
        {
            [[event characters] getCharacters: &buf];
            keyPressed = buf;
            [condition signal];
        }
        [condition unlock];
    }
    return;
}

- (void) keyDown: (NSEvent *) event {
    //NSLog(@"keyDown: %@", event);
    return;
}

- (void) drawRect : (NSRect) rect {
    int i, n;
    NSRect bounds = [self bounds];
    float current_ratio;
    [ self erase : rect ];
    //NSLog(@"Thread Main: %d (DRAWRECT)", [NSThread isMainThread]);
    current_ratio = bounds.size.width / bounds.size.height / original_ratio;
    [drawing lock];
    {
        n = [objs count];
        //NSLog(@"Thread Main: %d %d", [NSThread isMainThread], n);
        if(n > 0) {
            NSBezierPath *path;
            path = [[NSBezierPath alloc] init];
            [path setLineWidth: 1.0];
            [[NSColor blackColor] set];
            for(i = 0; i < n; i++) {
                NSSacViewObj *obj = [objs objectAtIndex: i];
                if(!obj) {
                    continue;
                }
                //NSLog(@"%@", obj);
                switch(obj.type) {
                case 3: 
                    [ path stroke ]; 
                    [ path removeAllPoints ];
                    break;
                case 2:
                    [ path lineToPoint: 
                               [obj pointWithBounds: bounds ratio:current_ratio] ]; 
                    break;
                case 1: 
                    [ path moveToPoint: 
                               [obj pointWithBounds: bounds ratio:current_ratio] ]; 
                    break;
                case 5: {
                    [obj.color set ];
                }
                case 6: {
                    [ path setLineWidth: obj.width ];
                }
                    break;
                case 7: {
                    NSImage *im    = [ obj image: bounds ];
                    NSPoint origin = [ obj imagePosition: bounds ];
                    [im drawAtPoint: origin 
                           fromRect: NSZeroRect
                          operation: NSCompositingOperationSourceOver
                           fraction: 1.0];
                }
                case 4: {
                    int j;
                    NSPoint *p, *o;
                    p = (NSPoint *) malloc(sizeof(NSPoint) * obj.npts);
                    o = obj.points;
                    for(j = 0; j < obj.npts; j++) {
                        p[j].x = o[j].x * bounds.size.width;
                        p[j].y = o[j].y * bounds.size.width / current_ratio;
                    }
                    [ path removeAllPoints ];
                    [ path appendBezierPathWithPoints: p count: obj.npts]; 
                    [path stroke];
                    free(p);
                    p = NULL;
                }
                    break;
                }
            }
            [path release];
        }
    }
    [drawing unlock];
}


- (BOOL) becomeFirstResponder {
    //NSLog(@"Take Focus: %@", self);
    return YES;
}

@end

@implementation NSSacWindowController 

- (id) initWithNumber: (int) number {
    //NSLog(@"INIT WITH NUMBER");

    if((self = [super initWithWindowNibName:@"SacPlotWindow"])){
        NSWindow *w = [self window];

        //NSLog(@"Window: %@", w);
        //NSLog(@"Thread Main: %d", [NSThread isMainThread]);
        windowNumber = number;
        parent = nil;
        [w setTitle: [NSString stringWithFormat: @"Plot Window %d", number]];
        [self show];
        /* Bring Application to the front */
        [[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
        /*
          NSSize size = [[w contentView] frame].size;
          NSPoint org = [[w contentView] frame].origin;
          NSLog(@"Size: %f %f", size.width, size.height);
          NSLog(@"Orgi: %f %f", org.x, org.y);
          NSLog(@"Active: %d", (int)[w isOnActiveSpace]);
          NSLog(@"Visible: %d", (int)[w isVisible]);
          NSLog(@"Opaque: %d", (int)[w isOpaque]);
          NSLog(@"Controller: %@", [w windowController]);
          NSLog(@"Loaded: %d", (int)[self isWindowLoaded]);
        */
    }
    return self;
}

- (void)windowDidBecomeKey: (NSNotification *) notification {
    //NSLog(@"WINDOW BECAME KEY");
    [sacView setCurrent];
}

- (int) windowNumber {
    return windowNumber;
}

- (void) show {
    //NSLog(@"WINDOW SHOW");
    [[self window] makeKeyAndOrderFront: self];
}

- (void) setParent: (id) who {
    parent = who;
}

- (void) windowDidLoad {
    //NSLog(@"WINDOW DID LOAD");
}

- (void) windowWillClose: (NSNotification *) notification {
    //NSLog(@"WINDOW WILL CLOSE");
    [[self retain] autorelease];
    if([parent respondsToSelector: @selector(removeWindow:)]) {
      [parent performSelector: @selector(removeWindow:) withObject: self];
    }
}

@end
