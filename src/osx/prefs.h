
#import <Cocoa/Cocoa.h>
#import "sac.h"

enum {
  PrefsGeneral = 1,
};

@interface PrefsGeneralView : NSView {
  IBOutlet NSButton    *fontButton;
  IBOutlet NSTextField *fontLabel;
  IBOutlet NSButton    *initButton;
  IBOutlet NSMatrix    *initMatrix;
}

- (void) update: (SAC *) sender;
@end

@interface PrefsGeneralController : NSViewController {
  SAC *sac;
}

- (void) setModel: (id) sender ;
- (IBAction) showFontMenu: (id) sender ;
- (IBAction) showInitMenu: (id) sender ;
- (IBAction) click: (id) sender;

@end

@interface Preferences : NSWindowController {
  IBOutlet NSToolbar  *bar;
  PrefsGeneralController *general;
  int                 currentView;
}

- (IBAction) switchView: (id) sender;
- (void) switchViewByTag: (int) tag;
- (NSRect) newFrameForNewContentView: (NSView *) view;
- (id) general;
@end
