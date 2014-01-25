
#import <Cocoa/Cocoa.h>


@interface NSSacView : NSView 
{
    NSColor        *backgroundColor;
    NSMutableArray *objs;
    float           original_ratio;
    char            keyPressed;
    NSCondition    *condition;
    NSPoint         mouseLocation;
    NSLock         *drawing;
}

- (void) erase : (NSRect) rect;
- (void) drawRect : (NSRect) rect;
- (void) addMDSType: (int) type withPoint : (NSPoint) p;
- (void) clearStack;
- (void) update;
- (void) addPoly: (NSPoint *) p count: (int) n;
- (void) addColor: (NSColor *) c;
- (void) addWidth: (int) w;
- (void) waitForKeyPress: (char *) c atPoint: (NSPoint *) p;

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
              lbinary: (int) lbinary ;

@end 

@interface NSSacViewObj : NSObject 
{
    int      type;
    NSPoint  point;
    NSPoint *points;
    int      npts;
    NSColor *color;
    int      width;
    float   *imageData;
    int      imageWidth, imageHeight;
    float    imageXmin, imageXmax, imageYmin, imageYmax;
    float    imageX, imageY, imageW, imageH;
    int      npseudocolors, nsacolors, ndefcolors;
    int      imageBinary;
}

- (id) initType: (int) type  withPoint: (NSPoint) p;
- (NSString *) description;
- (int) type;
- (NSPoint) point;
- (NSPoint) pointWithBounds: (NSRect) bounds ratio: (float) ratio;
@end

@interface NSSacWindowController : NSWindowController 
{
    IBOutlet NSSacView *sacView;
    id parent;
    int windowNumber;
}

- (id) initWithNumber: (int) number;
- (int) windowNumber;
- (void) setParent: (id) who;
- (void) show;

@end
