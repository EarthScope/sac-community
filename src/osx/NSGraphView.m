
#import <math.h>
#import "NSGraphView.h"

@implementation NSGraphAxis 

- (void) setDirection: (axisDirection) where {
    direction = where;
}

- (id) init {
    self = [super init];
    if(self) {
        direction  = X;
        show       = YES;
        autoLimits = YES;
        buffered   = YES;
        scale      = LINEAR;
        start      = 0.0;
        stop       = 1.0;
        majorDiv   = 0.2;
        minorDiv   = 0.05;
        majorTickSize   = 7;
        minorTickSize   = 4;
        tickFormat = nil;
        labelAllLogTicks = NO;
        font       = [NSFont fontWithName:@"Helvetica" size:12.0];
        fontAttributes = [NSDictionary dictionaryWithObject: font
                                                     forKey: NSFontAttributeName];
        [font retain];
        [fontAttributes retain];
    }
    return self;
}

- (void) saveLimits {
    gstart = start;
    gstop  = stop;
}
- (void) restoreLimits {
    start = gstart;
    stop  = gstop;
}

- (BOOL) isAuto     { return autoLimits; }
- (BOOL) isBuffered { return buffered; }
- (BOOL) isLinear { return scale == LINEAR; }
- (BOOL) isLog { return scale == LOG; }

- (float) min { return start; }
- (float) max { return stop;  }
- (void) limitsMin: (float) min max: (float) max {
    start      = min;
    stop       = max;
}
- (axisScale) scale { return scale; }
- (void) linear { scale = LINEAR; }
- (void) log    { scale = LOG;    }
- (void) ticksize: (int) size { majorTickSize = size; }
- (void) autoLimits: (BOOL) flag { autoLimits = flag; }
- (void) buffered:   (BOOL) flag { buffered = flag;   }

- (float) labelSize {
    NSString *str;
    NSSize size;
    str = [NSString stringWithFormat: tickFormat, start];
    size = [str sizeWithAttributes: fontAttributes];
    if(direction == TOP) {
        return size.height;
    } else {
        return size.width;
    }
}

- (void) drawString: (NSString *) s point: (NSPoint) p dx: (float) dx dy: (float) dy {
    NSAttributedString *str = [[NSAttributedString alloc] 
                                     initWithString: s
                                         attributes: fontAttributes];
    NSSize size = [s sizeWithAttributes: fontAttributes];
    [str drawInRect: NSMakeRect(p.x + dx * size.width, 
                                p.y + dy * size.height,
                                size.width,
                                size.height) ];
}

- (float) tickValue: (int) i major: (BOOL) major {
    int n;
    float v;
    float div = (major) ? majorDiv : minorDiv;

    if(scale == LINEAR) {
        return div * floor(start/div) + (i * div);
    }
    n = floor(log10(start));
    if(major) {
        return pow(10, n + i);
    }
    v = pow(10, n + floor(i/10));
    if(i%10 == 0) {
        return v;
    }
    return v * (i%10);
}


- (void) drawTicks: (axisSide) side 
             major: (BOOL) majorTicks
              view: (NSGraphView *) view {
    int j, xtick, ytick;
    float x, y, value, dx, dy;
    int tickSize        = (majorTicks) ? majorTickSize : minorTickSize;
    NSLimits limits     = [view limits];
    NSPoint margin      = [view margin];
    NSPoint marginExtra = [view marginExtra];
    NSBezierPath *path  = [NSBezierPath bezierPath];

    margin.x = (margin.x * (1-1/marginExtra.x))/2;
    margin.y = (margin.y * (1-1/marginExtra.y))/2;

    if(direction == X) {
        margin.x = 0.0;
        margin.y = margin.y * ((side == TOP) ? +1 : -1) ;
        dx = -0.5;
        dy = (side == TOP) ? 0.0 : -1.0;
    } else {
        margin.y = 0.0;
        margin.x = margin.x * ((side == RIGHT) ? +1 : -1) ;
        dy = -0.5;
        dx = (side == RIGHT) ? 0.0 : -1.0;
    }

    j = 0;
    value = [self tickValue: j major:majorTicks];
    while(value <= stop) {
        value = [self tickValue: j++ major:majorTicks];
        xtick = ytick = 0;
        if(direction == X) {
            x = value;
            if(x < limits.xmin || x > limits.xmax) {  continue; }
            y = (side == BOTTOM) ? limits.ymin : limits.ymax;
            ytick = (side == BOTTOM) ? tickSize : -tickSize;
        } 
        if(direction == Y) {
            y = value;
            if(y < limits.ymin || y > limits.ymax) { continue; }
            x = (side == LEFT) ? limits.xmin : limits.xmax;
            xtick = (side == LEFT) ? tickSize : -tickSize;
        }
        NSPoint p = [ view location: NSMakePoint(x, y) ];
        [path moveToPoint: p];
        [path lineToPoint: NSMakePoint(p.x+xtick, p.y+ytick )];

        if(majorTicks) {
            NSString *s;
            s = [NSString stringWithFormat: tickFormat, value];
            p = NSMakePoint(p.x+margin.x, p.y+margin.y);
            [self drawString: s point: p dx: dx dy: dy];
        }
    }
    [path stroke];
}

- (void) autoTicks: (NSGraphView *) view {
    float delta, magn, norm, size, nticks;
    int dec;
    NSRect bounds = [view bounds];
    if(scale == LOG) {
        majorDiv     = 10;
        minorDiv     = 1;
        tickFormat   = @"%.0e";
        tickDecimals = 0;
        return;
    }
    nticks = 0.3 * sqrt((direction == X) ? bounds.size.width : bounds.size.height);
    delta = (stop - start) / nticks;
    dec = -floor(log10(delta));
    magn = pow(10, -dec);
    norm = delta / magn;
    if(norm < 1.5) {
        size = 1;
        minorDiv = 0.25;
    } else if(norm < 3) {
        size = 2;
        minorDiv = 0.5;
        if(norm > 2.25) {
            size = 2.5;
            minorDiv = 0.5;
            dec++;
        }
    } else if(norm < 7.5) {
        size = 5;
        minorDiv = 1;
    } else {
        size = 10;
        minorDiv = 2;
    }
    majorDiv = size * magn;
    minorDiv = minorDiv * magn;
    tickDecimals = MAX(0, dec);
    tickFormat = [NSString stringWithFormat: @"%%.%df", tickDecimals];
}

- (void) draw: (NSGraphView *) view {
    if(!show) {
        return;
    }
    [self autoTicks: view];
    if(direction == X) {
        [self drawTicks: BOTTOM major: YES view:view];
        [self drawTicks: TOP    major: YES view:view];
        [self drawTicks: BOTTOM major: NO  view:view];
        [self drawTicks: TOP    major: NO  view:view];
    }
    if(direction == Y) {
        [self drawTicks: LEFT   major: YES view:view];
        [self drawTicks: RIGHT  major: YES view:view];
        [self drawTicks: LEFT   major: NO  view:view];
        [self drawTicks: RIGHT  major: NO  view:view];
    }
}

@end

@implementation NSGraphData 

- (void) updateLimitsWithData: (NSPoint) p {
    limits.xmin = fmin(limits.xmin, p.x);
    limits.xmax = fmax(limits.xmax, p.x);
    limits.ymin = fmin(limits.ymin, p.y);
    limits.ymax = fmax(limits.ymax, p.y);

    if(p.x > 0.0) {
        llimits.xmin = fmin(llimits.xmin, p.x);
        llimits.xmax = fmax(llimits.xmax, p.x);
    }
    if(p.y > 0.0) {
        llimits.ymin = fmin(llimits.ymin, p.y);
        llimits.ymax = fmax(llimits.ymax, p.y);
    }
}

/* Data Points */
- (void) setData: (int) n x: (float *) x y: (float *) y {
    int i;

    limits.xmin =  1e37;
    limits.xmax = -1e37;
    limits.ymin =  1e37;
    limits.ymax = -1e37;

    llimits.xmin = 1e37;
    llimits.xmax = 0.0;
    llimits.ymin = 1e37;
    llimits.ymax = 0.0;

    if(data) {
        free(data);
        data = NULL;
    }
    data = (NSPointArray) malloc(sizeof(NSPoint) * n);
    for(i = 0; i < n; i++) {
        data[i] = NSMakePoint(x[i],y[i]);
        [self updateLimitsWithData: data[i]];
    }
    count = n;
}

- (void) setName: (NSString *) str {
    name = str;
    [name retain];
}

- (NSString *) name {
    return name;
}

- (int) count { 
    return count; 
}
- (NSPoint) objectAtIndex : (int) index { 
    return data[index];
}
/* Line */
- (int) line { 
    return line; 
}
- (void) setLine: (int) l { 
    line = l;
}
- (NSColor *) lineColor {
    return lineColor; 
}
- (void) setLineColor: (NSColor *) c {
    lineColor = [[c copy] retain];  
}
/* Symbol */
- (NSColor *) symbolColor { 
    return symbolColor; 
}
- (int) symbol { 
    return symbol; 
}
- (void) setSymbol: (int) s { 
    symbol = s; 
}
- (void) setSymbolColor: (NSColor *) c {
    symbolColor = [[c copy] retain];
}
/* All Color */
- (void) setColor: (NSColor *) c { 
    [self setLineColor: c];
    [self setSymbolColor: c];
}

/* Limits */
- (NSLimits) limits {
    return limits;
}
- (NSLimits) llimits {
    return llimits;
}

@end

@implementation NSGraphView 

- (NSPoint) location : (NSPoint) p {
    NSRect bounds = [self bounds];
    float x, y;
    
    NSLimits lims = [self limits];

    /* Convert from Data Space to Normalized Value [0,1] */
    if([xaxis scale] == LINEAR) {
        x = (p.x - lims.xmin)/(lims.xmax - lims.xmin);
    } else {
        x = log10(p.x) - log10(lims.xmin);
        x = x / (log10(lims.xmax) - log10(lims.xmin));
    }
    if([yaxis scale] == LINEAR) {
        y = (p.y - lims.ymin)/(lims.ymax - lims.ymin);
    } else {
        y = log10(p.y) - log10(lims.ymin);
        y = y / (log10(lims.ymax) - log10(lims.ymin));
    }
    
    /* Convert to Plot Space */
    x = x * (bounds.size.width  - 2*margin.x) + margin.x;
    y = y * (bounds.size.height - 2*margin.y) + margin.y;

    return NSMakePoint(x, y);
}
- (void) limitsPath: (NSBezierPath *) path {
    NSPoint ll,ul,lr,ur;
    ll = [self location: NSMakePoint(xaxis.min, yaxis.min)];
    ul = [self location: NSMakePoint(xaxis.min, yaxis.max)];
    lr = [self location: NSMakePoint(xaxis.max, yaxis.min)];
    ur = [self location: NSMakePoint(xaxis.max, yaxis.max)];
    // Bottom X-Axis
    [path moveToPoint: ll];
    [path lineToPoint: lr];
    [path lineToPoint: ur];
    [path lineToPoint: ul];
    [path lineToPoint: ll];
    
}

- (void) clipToLimits {
    NSBezierPath *path = [NSBezierPath bezierPath];
    [self limitsPath: path];
    [NSGraphicsContext saveGraphicsState];
    [path addClip];
}

- (void) clipReset {
    [NSGraphicsContext restoreGraphicsState];
}

- (void) drawAxis {
    NSBezierPath *path = [NSBezierPath bezierPath];
    [self limitsPath: path];
    [path stroke];
}

- (BOOL) pointOk: (NSPoint) p {
    if([xaxis scale]== LOG && p.x <= 0.0) {
        return NO;
    }
    if([yaxis scale] == LOG && p.y <= 0.0) {
        return NO;
    }
    return YES;
}

- (void) drawData {
    int i, j, n;
    NSBezierPath *path;
    n = [data count];
    for(i = 0; i < n; i++) {
        NSGraphData *obj = [data objectAtIndex: i];
        if(obj.symbol) {
            [self clipToLimits];
            path = [NSBezierPath bezierPath];
            [obj.lineColor set];
            for(j = 0; j < [obj count]; j++) {
                NSPoint p = [obj objectAtIndex: j];
                if([self pointOk: p]) {
                    p = [self location: p];
                    [path appendBezierPathWithOvalInRect: 
                              NSMakeRect(p.x-symbolsize,p.y-symbolsize,
                                         2*symbolsize, 2*symbolsize)];
                }
            }
            [path stroke];
            [self clipReset];         
        }
        if(obj.line) {
            [self clipToLimits];
            int moveOrLine = 0;
            path = [NSBezierPath bezierPath];
            [obj.symbolColor set];
            for(j = 0; j < [obj count]; j++) {
                NSPoint p = [obj objectAtIndex: j];
                if([self pointOk: p]) {
                    if(moveOrLine == 1) {
                        [path lineToPoint: [self location: p]];
                    } else {
                        [path moveToPoint: [self location: p]];
                        moveOrLine = 1;
                    }
                } else {
                    moveOrLine = 0;
                }
            }
            [path stroke];
            [self clipReset];
        }
    }
}

- (NSGraphData *) getData: (NSString *) name {
    int i;
    for(i = 0; i < (int)[data count]; i++) {
        NSGraphData *obj = [data objectAtIndex: i];
        if([[obj name] isEqualToString: name]) {
            return obj;
        }
    }
    return nil;
}

- (void) addData: (NSGraphData *) new {
    [data addObject: new];
}

- (void) awakeFromNib {

    padding     = NSMakePoint(5, 5);
    marginBase  = NSMakePoint(10, 10);
    marginExtra = NSMakePoint(1.35, 1.35);

    symbolsize      = 5;
    drawBoundingBox = YES;

    xaxis = [[NSGraphAxis alloc] init ];
    yaxis = [[NSGraphAxis alloc] init ];
    [xaxis setDirection: X];
    [yaxis setDirection: Y];
    data = [[NSMutableArray alloc] initWithCapacity: 5];
    
}

/* Find Limits from available Data */
- (void) assembleLimitsFromData {
    int i, n;
    NSLimits l;
    n = [data count];
    for(i = 0; i < n; i++) {
        NSGraphData *obj = [ data objectAtIndex: i];
        NSLimits ol = [obj limits];
        NSLimits ll = [obj llimits];
        if(i == 0) {
            l.xmin = ([xaxis isLinear]) ? ol.xmin : ll.xmin;
            l.xmax = ([xaxis isLinear]) ? ol.xmax : ll.xmax;
            l.ymin = ([yaxis isLinear]) ? ol.ymin : ll.ymin;
            l.ymax = ([yaxis isLinear]) ? ol.ymax : ll.ymax;
        } else {
            l.xmin = ([xaxis isLinear]) ? fmin(l.xmin, ol.xmin) : fmin(l.xmin, ll.xmin);
            l.xmax = ([xaxis isLinear]) ? fmax(l.xmax, ol.xmax) : fmax(l.xmax, ll.xmax);
            l.ymin = ([yaxis isLinear]) ? fmin(l.ymin, ol.ymin) : fmin(l.ymin, ll.ymin);
            l.ymax = ([yaxis isLinear]) ? fmax(l.ymax, ol.ymax) : fmax(l.ymax, ll.ymax);
        }
    }
    if([xaxis isAuto]) {
        [xaxis limitsMin:l.xmin max:l.xmax];
    }
    if([yaxis isAuto]) {
        [yaxis limitsMin:l.ymin max:l.ymax];
    }
}

/* Apply Padding between data and bounding axes 
 *   Changes Axes min and max value
 */
- (void) applyPadding {
    NSRect bounds = [self bounds];
    NSLimits limits = [self limits];
    float xmin, xmax, ymin, ymax, dx, dy;
    
    if([xaxis isLinear]) {
        dx = limits.xmax - limits.xmin;
        dx *= (padding.x/bounds.size.width);
        xmin = limits.xmin - dx;
        xmax = limits.xmax + dx;
    } else {
        dx = padding.x * (log10(limits.xmax) - log10(limits.xmin));
        dx /= (bounds.size.width-2*margin.x-2*padding.x);
        xmin = pow(10,log10(limits.xmin) - dx);
        xmax = pow(10,log10(limits.xmax) + dx);
    }
    if([yaxis isLinear]) {
        dy = limits.ymax - limits.ymin;
        dy *= (padding.y/bounds.size.height);
        ymin = limits.ymin - dy;
        ymax = limits.ymax + dy;        
    } else {
        dy = padding.y * (log10(limits.ymax) - log10(limits.ymin));
        dy /= (bounds.size.height-2*margin.y-2*padding.y);
        ymin = pow(10,log10(limits.ymin) - dy);
        ymax = pow(10,log10(limits.ymax) + dy);
    }

    if([xaxis isLog] && xmin <= 0.0) {
        xmin = xaxis.min;
    }
    if([xaxis isBuffered]) {
        [xaxis limitsMin: xmin max: xmax];
    }

    if([yaxis isLog] && ymin <= 0.0) {
        ymin = xaxis.min;
    }
    if([yaxis isBuffered]) {
        [yaxis limitsMin: ymin max: ymax];
    }
}

/* Adjust space between axis and Bounding View
 *    to accomodate axis labels
 */
- (void) adjustMargins {
    margin.x = marginBase.x + [yaxis labelSize] * marginExtra.x;
    margin.y = marginBase.y + [xaxis labelSize] * marginExtra.y;
}

- (void) setYAxis: (axisScale) ax {
    if(ax == LOG) {
        [yaxis log];
    } else {
        [yaxis linear];
    }
}
- (void) setXAxis: (axisScale) ax {
    if(ax == LOG) {
        [xaxis log];
    } else {
        [xaxis linear];
    }
}

- (void) xauto {
    [xaxis autoLimits: YES];
}
- (void) yauto {
    [yaxis autoLimits: YES];
}

- (void) xlimMin: (float) xmin max: (float) xmax {
    [xaxis limitsMin: xmin max:xmax];
    [xaxis autoLimits: NO];
    [xaxis buffered: NO];
}
- (void) ylimMin: (float) ymin max: (float) ymax {
    [yaxis limitsMin: ymin max:ymax];
    [yaxis autoLimits: NO];
    [yaxis buffered: NO];
}

- (NSPoint) margin {
    return margin;
}
- (NSPoint) marginExtra {
    return marginExtra;
}

- (NSLimits) limits {
    NSLimits lims;
    lims.xmin = xaxis.min;
    lims.xmax = xaxis.max;
    lims.ymin = yaxis.min;
    lims.ymax = yaxis.max;
    return lims;
}

- (void) buffered: (BOOL) flag {
    [xaxis buffered: flag];
    [yaxis buffered: flag];
}


- (void) drawRect : (NSRect) rect {

    if([data count] <= 0) {
        return;
    }

    [xaxis saveLimits];
    [yaxis saveLimits];
    [[NSColor blackColor] set];

    if(drawBoundingBox) {
        NSBezierPath *path = [NSBezierPath bezierPath];
        [path appendBezierPathWithRect: rect];
        [path stroke];
    }
    
    /* marginExtra >= 1.0 */
    marginExtra.x = fmax(1.0, marginExtra.x);
    marginExtra.y = fmax(1.0, marginExtra.y);

    [self assembleLimitsFromData]; /* Find Maximum Limits */
    [xaxis autoTicks: self];
    [yaxis autoTicks: self];
    [self adjustMargins];          /* Allow for Axes Labels */
    [self applyPadding];           /* Buffer data from Axes */

    [ self drawAxis ];
    [ xaxis draw: self];
    [ yaxis draw: self];
    [ self drawData ];

    [xaxis restoreLimits];
    [yaxis restoreLimits];

}

@end


