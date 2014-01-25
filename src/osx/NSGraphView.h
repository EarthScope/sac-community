
#import <Cocoa/Cocoa.h>

typedef enum _axisScale axisScale;
enum _axisScale {
    LINEAR,
    LOG
};
typedef enum _axisDirection axisDirection;
enum _axisDirection {
    X, Y
};
typedef enum _axisSide axisSide;
enum _axisSide {
    TOP, BOTTOM, LEFT, RIGHT
};

typedef struct _NSLimits NSLimits;
struct _NSLimits {
    float xmin, xmax, ymin, ymax;
};


@interface NSGraphAxis : NSObject 
{
    BOOL          show;
    BOOL          buffered;
    BOOL          autoLimits;
    axisDirection direction;
    axisScale     scale;
    float         start, stop;
    float         majorDiv, minorDiv;
    float         gstart, gstop;
    int           majorTickSize, minorTickSize;
    NSFont       *font;
    NSDictionary *fontAttributes;
    BOOL          labelAllLogTicks;
    NSString     *tickFormat;
    int           tickDecimals;
}

@end

@interface NSGraphData : NSObject 
{
    NSPointArray data;
    int          count;
    NSColor     *lineColor;
    NSColor     *symbolColor;
    int          line;
    int          symbol;
    NSLimits     limits, llimits;
    NSString    *name;
}

- (void) setData: (int) n x: (float *) x y: (float *) y ;
- (void) setName: (NSString *) str;
- (void) setLine: (int) l;
- (void) setSymbol: (int) s;
- (void) setColor: (NSColor *) c;

@end 

@interface NSGraphView : NSView 
{

    NSPoint margin; //xmargin, ymargin;   /* Margin from View to Axis */
    NSPoint padding;// xpadding, ypadding; /* Padding from Axis to Data */
    NSPoint marginExtra;
    NSPoint marginBase;
    int symbolsize;

    NSMutableArray *data;
    NSGraphAxis *xaxis;
    NSGraphAxis *yaxis;
    BOOL drawBoundingBox;
}

- (NSPoint) margin;
- (NSPoint) marginExtra;
- (NSGraphData *) getData: (NSString *) name ;
- (void) setXAxis: (axisScale) ax;
- (void) setYAxis: (axisScale) ax;
- (void) addData: (NSGraphData *) new;
- (NSPoint) location : (NSPoint) p ;
- (void) xlimMin: (float) xmin max: (float) xmax;
- (void) ylimMin: (float) ymin max: (float) ymax;
- (NSLimits) limits ;
- (void) xauto;
- (void) yauto;
- (void) buffered: (BOOL) flag;

@end

