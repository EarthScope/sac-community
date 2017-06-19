
#import "sac.h"
#import "crt_externs.h"
#import "NSSacView.h"

void sac_history_file_set(char *file);
void sac_history_load(char *file);
char *sac_history_file();

void osx_sac(void *id);
void osx_init();
void sac_main_loop();
void settextwait(char *mode);
void osx_gui_command(char *cmd);
void sac_initialize(int argc, char **argv);


@implementation SAC

void
sac_focus_window(void *id, int n) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            SAC *sac = (SAC *) id;
            [sac focusWindow: n];
        });
}

int
sac_find_window(void *id, int n) {
    __block int ok = 0;
    dispatch_sync(dispatch_get_main_queue(), ^{
            SAC *sac = (SAC *) id;
            ok = ([sac findWindow: n] != nil);
        });
    return ok;
}

void sac_create_window(void *id, int n) {
    dispatch_sync(dispatch_get_main_queue(), ^{
            SAC *sac = (SAC *) id;
            [sac createWindowWithNumber: [NSNumber numberWithInt: n]];
        });
}

- (id) init {
    self = [super init];
    if(self) {
        plotWindows  = [[NSMutableArray alloc] initWithCapacity: 5];

        initFiles = nil;
        osx_sac( self );
        initialized = NO;
    }
    return self;
}

- (void) applicationWillFinishLaunching: (NSNotification *) note {
}
- (void) sac_main_loop_thread: (NSArray *) data2 {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    sac_main_loop();
    [pool drain];
}

- (void) applicationDidFinishLaunching: (NSNotification *) note {
  NSString *aux;
  NSArray *args;
  int i;
  int argc;
  char **argv;

  /* Create SACAUX from variable name*/
  aux = [[NSBundle mainBundle] pathForResource: @"aux" ofType: nil];
  setenv("SACAUX", [aux UTF8String], 1);
  
  /* Convert arguments into argc,argv */
  args = [[NSProcessInfo processInfo] arguments]; 
  argc = [args count];
  argv = (char **) malloc(sizeof(char *) * argc);
  for(i = 0; i < argc; i++) {
    argv[i] = strdup( [[args objectAtIndex: i] UTF8String ] );
  }


  sac_initialize(argc, argv);

  osx_init();
  settextwait("OF");
  sac_history_file_set(NULL);
  sac_history_load(sac_history_file());
  if(initFiles) {
    [self readFiles: initFiles clearFiles: YES];
  }
  initialized = YES;
  //NSLog(@"Thread Main: %d", [NSThread isMainThread]);
  /* Thread - command line / keyboard loop */
  [NSThread detachNewThreadSelector: @selector(sac_main_loop_thread:)
                           toTarget: self
                         withObject: nil
   ];
}

- (void) awakeFromNib {
}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot {
  osx_gui_command((char *)[command UTF8String]);
}


- (void) readFiles: (NSArray *) files clearFiles :(BOOL) clearFiles {
    int i;
    NSMutableString *com = [NSMutableString stringWithCapacity: 75];
    [com appendString: @"read"];
    if(! clearFiles) {
        [com appendString: @" more"];
    }
    for(i = 0; i < (int)[files count]; i++) {
        NSString *file = [[files objectAtIndex:i] path];
        [com appendFormat: @" %@", file];
        /* Add file to Open Recent ... */
        [[NSDocumentController sharedDocumentController]
                noteNewRecentDocumentURL:  [files objectAtIndex: i]];
    }
    [self sendCommand: com plot: YES];
}

/* To respond to Open Recent or Double Clicks 
   Connected Application delegate to This Class (SAC) in Interface Builder
*/
- (BOOL) application:(NSApplication *)theApplication openFile:(NSString *)filename {
    initFiles = [NSMutableArray arrayWithCapacity: 2];
    [initFiles addObject: [NSURL fileURLWithPath: filename]];
    if(initialized) {
      [self readFiles: initFiles clearFiles: YES];
    }
    return YES;
}
- (void) application:(NSApplication *)theApplication openFiles:(NSArray*) filenames {
    int i;
    initFiles = [NSMutableArray arrayWithCapacity: 10];
    for(i = 0; i < (int)[filenames count]; i++) {
      [initFiles addObject: [NSURL fileURLWithPath: [filenames objectAtIndex: 0]]];
    }
    if(initialized) {
      [self readFiles: initFiles clearFiles: YES];
    }
    return;
}

-(BOOL) canBecomeFirstResponder {
    return YES;
}

- (IBAction) createWindowSac: (id) sender {
    int i, j;
    j = 0;
    for(i = 0; i < (int)[plotWindows count]; i++) {
        NSSacWindowController *plot = [plotWindows objectAtIndex: i];
        j = MAX(plot.windowNumber,j);
    }
    j++;
    [self sendCommand: [NSString stringWithFormat: @"BeginWindow %d", j] 
                 plot: NO];
}

- (void) createWindowWithNumber: (NSNumber *) np {
    int n = [np intValue];
    NSSacWindowController *plot = [[NSSacWindowController alloc] 
                                      initWithNumber: n];
    [plot setParent: self];
    [plotWindows addObject: plot];
}

- (id) findWindow: (int) windowNumber {
    int i;
    for(i = 0; i < (int)[plotWindows count]; i++) {
        NSSacWindowController *plot = [plotWindows objectAtIndex: i];
        if(plot.windowNumber == windowNumber) {
            return plot;
        }
    }
    return nil;
}

- (void) focusWindow: (int) windowNumber {
    NSSacWindowController *plot = [self findWindow: windowNumber];
    if(plot) {
        [plot show];
    }
}

- (void) removeWindow: (id) plot {
    [plotWindows removeObject: plot];
}

@end

/*

read /Users/savage13/Bits/sac/sac.build/osx/tmp.sac
plot1


*/
