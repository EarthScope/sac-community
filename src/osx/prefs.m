
#import "prefs.h"

@implementation PrefsGeneralView

- (void) awakeFromNib {
}

- (void) update: (SAC *) sac {

  NSFont *font = [sac font];
  [fontLabel setStringValue: [NSString stringWithFormat: @"%@ %.1f pt", 
                                       [font displayName], [font pointSize]]];

  NSString *file = [sac startupFile];
  [[[initMatrix cells] objectAtIndex: 1] setTitle:
                            (!file) ?
                              @"Not Set" :
                              [[NSURL URLWithString: file] lastPathComponent]];

  [initMatrix selectCellWithTag: ([sac startup]) ? 1 : 0];
}

- (int) selected {
  return [initMatrix selectedRow];
}

@end

@implementation PrefsGeneralController 

- (void) awakeFromNib {
  //NSLog(@"AWAKE PREFS GENERAL VIEW CONTROLLER");
}

- (void) setModel: (SAC *) value {
  sac = value;
}

- (IBAction) click: (id) sender {
  int selected = [(PrefsGeneralView *)self.view selected];
  [sac setStartup: (BOOL)selected];
}

- (void) modelSetStartup: (BOOL) value file: (NSString *) file {
  [sac setStartupFile: file];
  [sac setStartup: value];
}

- (IBAction) showInitMenu: (id) sender {
  NSString *file;
  NSOpenPanel *op = [NSOpenPanel openPanel];
  [op setCanChooseFiles: YES];
  [op setCanChooseDirectories: NO];
  [op setAllowsMultipleSelection: NO];
  if([op runModal] == NSOKButton) {
    file = [[[op URLs] objectAtIndex: 0] path];
    [self modelSetStartup: YES file: file];
  }
}

- (void) changeFont: (id) sender {
  [sac changeFont: sender];
}

- (IBAction) showFontMenu: (id) sender {
  NSFontManager *manager = [NSFontManager sharedFontManager];
  [manager setDelegate: self];
  [manager setTarget: self];
  NSFontPanel *panel = [manager fontPanel: YES];
  [panel makeKeyAndOrderFront: sender];
}

@end

@implementation Preferences

- (void) dealloc {
  [super dealloc];
}

- (void) awakeFromNib {
  general = [[PrefsGeneralController alloc] initWithNibName: @"PrefsGeneral" bundle: nil];
  /* Set up View */
  [self.window setContentSize: [[general view] frame].size];
  [[self.window contentView] addSubview: [general view]];
  [bar setSelectedItemIdentifier: @"General"];
  currentView = PrefsGeneral;
}


- (NSView *) viewForTag: (int) tag {
  NSView *view = nil;
  switch(tag) {
  case PrefsGeneral: view = [general view]; break;
  }
  return view;
}

- (NSRect) newFrameForNewContentView: (NSView *) view {
  NSRect newFrame = [self.window frameRectForContentRect: [view frame]];
  NSRect oldFrame = [self.window frame];
  NSSize newSize = newFrame.size;
  NSSize oldSize = oldFrame.size;
  NSRect frame = [self.window frame];
  frame.size = newSize;
  frame.origin.y -= (newSize.height - oldSize.height);
  return frame;
}

- (IBAction) switchView: (id) sender {
  [self switchViewByTag: [sender tag]];
}
- (void) switchViewByTag: (int) tag {
  NSView *view = [self viewForTag: tag];
  NSView *pview = [self viewForTag: currentView];
  if(tag == currentView) {
    return;
  }
  //NSLog(@"%d - %d\n", tag, currentView);
  currentView = tag;
  NSRect newFrame = [self newFrameForNewContentView: view];
#ifdef USE_ANIMATION  
  [NSAnimationContext beginGrouping];
  {
    [[NSAnimationContext currentContext] setDuration: 0.1];
    if([[NSApp currentEvent] modifierFlags] & NSShiftKeyMask) {
      [[NSAnimationContext currentContext] setDuration: 1.0];
    }
    [[[self.window contentView] animator] replaceSubview: pview with: view];
    [[self.window animator] setFrame: newFrame display: YES];
  }
  [NSAnimationContext endGrouping];
#else 
  [[self.window contentView] replaceSubview: pview with: view];
  [[self.window animator] setFrame: newFrame display: YES];
#endif
}

- (id) general {
  return general;
}

@end
