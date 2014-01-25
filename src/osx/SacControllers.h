
#import <Cocoa/Cocoa.h>
#import "NSGraphView.h"

@interface SacPlotLimitsController : NSObject 
{
    IBOutlet NSTextField *xmin;
    IBOutlet NSTextField *xmax;
    IBOutlet NSTextField *ymin;
    IBOutlet NSTextField *ymax;
    IBOutlet NSButton    *xauto;
    IBOutlet NSButton    *yauto;
    IBOutlet id parent;
}

- (IBAction) applyXLimits : (id) sender;
- (IBAction) applyYLimits : (id) sender;

@end

@interface SacFilterController : NSObject 
{
    IBOutlet NSWindow      *filterWindow;
    IBOutlet NSPopUpButton *type;
    IBOutlet NSPopUpButton *passes;
    IBOutlet NSPopUpButton *poles;
    IBOutlet NSTextField   *corner1;
    IBOutlet NSTextField   *corner2;
    IBOutlet NSGraphView   *view;
    IBOutlet id parent;
}

- (IBAction) applyFilter  : (id) sender;
- (IBAction) changeFilter : (id) sender;
- (IBAction) showWindow : (id) sender;

@end

@interface SacColorController : NSObject 
{
    IBOutlet NSButton      *increment;
    IBOutlet NSPopUpButton *color;
    IBOutlet id parent;
}

- (IBAction) changeColor : (id) sender ;

@end

@interface SacFuncGenController : NSObject 
{
    IBOutlet NSPopUpButton *type;
    IBOutlet NSTextField   *v1;
    IBOutlet NSTextField   *v2;
    IBOutlet NSTextField   *v3;
    IBOutlet NSTextField   *v4;
    IBOutlet NSTextField   *delta;
    IBOutlet NSTextField   *npts;
    IBOutlet NSTextField   *begin;
    IBOutlet NSButton      *useDelta;
    IBOutlet NSButton      *useNpts;
    IBOutlet NSButton      *useBegin;
    IBOutlet NSTextField   *t1;
    IBOutlet NSTextField   *t2;
    IBOutlet NSTextField   *t3;
    IBOutlet NSTextField   *t4;
    IBOutlet NSWindow      *window;
    IBOutlet id parent;
}

- (IBAction) apply:  (id) sender;
- (IBAction) change: (id) sender;

@end

@interface SacConstantMathController : NSObject 
{
    IBOutlet NSPopUpButton *type;
    IBOutlet NSTextField *v;
    IBOutlet NSWindow *window;
    IBOutlet id parent;
}
- (IBAction) apply:  (id) sender;

@end

@interface SacHeaderTableSource : NSObject <NSTableViewDataSource>
{
    NSMutableArray *filenames;
    NSMutableArray *fv;
}
- (void) update;

@end

@interface SacFormatter : NSNumberFormatter {
  BOOL notSigned;
}

- (void) unsigned: (BOOL) isUnsigned;
- (NSString *) stringForObjectValue: (id) object;
- (BOOL) getObjectValue: (id *) object forString: (NSString *) string errorDescription : (NSString **)error;

@end
