
#import <Cocoa/Cocoa.h>
#import "SacControllers.h"
//#import "prefs.h"

typedef enum _SacPlotType SacPlotType;
enum _SacPlotType {
    SAC_PLOT_SINGLE = 1,
    SAC_PLOT_OVERLAY = 2
};

enum {
  CommandViewFontSizeDecrease = -1,
  CommandViewFontSizeIncrease = 1,
};

enum {
    SacTagQDPOff                = 1,
    SacTagQDPOn                 = 2,
    SacTagXLinear               = 3,
    SacTagXLog                  = 4,
    SacTagYlinear               = 5,
    SacTagYLog                  = 6,
    SacTagPlotSingle            = 7,
    SacTagPlotOverlay           = 8,
    SacTagRemoveMean            = 9,
    SacTagRemoveTrend           = 10,
    SacTagRemoveTrendV          = 11,
    SacTagWriteOVer             = 12,
    SacTagTransferEvalresp      = 13,
    SacTagTransferPolezero      = 14,
    SacTagIntegrate             = 15,
    SacTagDifferentiate         = 16,
    SacTagFFT                   = 17,
    SacTagIFFT                  = 18,
    SacTagCorrelate             = 19,
    SacTagConvolve              = 20,
    SacTagEnvelope              = 21,
    SacTagSquare                = 22,
    SacTagSquareRoot            = 23,
    SacTagAbsoluteValue         = 24,
    SacTagLog                   = 25,
    SacTagLog10                 = 26,
    SacTagExp                   = 27,
    SacTagExp10                 = 28,
    SacTagPlotSpectralAmplitude = 29,
    SacTagPlotSpectralPhase     = 30,
    SacTagPlotSpectralReal      = 31,
    SacTagPlotSpectralImaginary = 32,
    SacTagOpenFile              = 41,
    SacTagOpenMoreFile          = 42,
    SacTagAddFile               = 50,
    SacTagSubtractFile          = 51,
    SacTagMultiplyFile          = 52,
    SacTagDivideFile            = 53,
    SacTagMathAdd               = 60,
    SacTagMathSubtract          = 61,
    SacTagMathMultiply          = 62,
    SacTagMathDivide            = 63,
};

@interface CommandView : NSTextView <NSTextViewDelegate> 
{
    NSRange       startOfLine;
    NSPipe       *pipe;
    NSFileHandle *pipeReadHandle;
    int           history_id;
}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot echo :(BOOL) echo;
- (void) flush;
- (void) changeFontSize: (id) sender ;

@end

@interface SAC : NSObject <NSApplicationDelegate>
{
    NSMutableArray *plotWindows;
    IBOutlet SacFilterController *filterController;
    IBOutlet SacPlotLimitsController *limitsController;
    //IBOutlet id delegate;
    SacPlotType           plot_type;
    IBOutlet CommandView *commandView;
    IBOutlet NSTableView *headerTable;
    NSMutableArray       *initFiles;
    BOOL                  initialized;

    NSString             *startupFile;
    BOOL                  startup;

    NSFont               *font;

    IBOutlet NSWindowController *prefs;
    NSView *prefsView;
}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot ;

- (IBAction) openDocument: (id) sender;
- (IBAction) saveAsDocument: (id) sender;
- (IBAction) printDocument: (id) sender;

- (IBAction) simple: (id) sender;
- (IBAction) simpleNoPlot: (id) sender;
- (IBAction) qdp: (id) sender;
- (IBAction) fileMath: (id) sender;

- (IBAction) createWindowSac: (id) sender ;
- (void)     createWindow: (id) sender number: (int) n;
- (id)       findWindow: (int) windowNumber;
- (void)     focusWindow: (int) windowNumber;
- (void)     removeWindow: (id) plot;

- (void)     tableRefresh;

- (void)     readFiles: (NSArray *) files clearFiles: (BOOL) clearFiles;


- (IBAction) changeFontSize: (id) sender ;
- (IBAction) changeFont: (id) sender ;

- (BOOL) startup;
- (NSString *) startupFile;
- (void) setStartup: (BOOL) value;
- (void) setStartupFile: (NSString *) file;
- (NSFont *) font ;
- (void) setFont: (NSFont *) newFont;
@end


