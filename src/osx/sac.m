
#import "sac.h"
#import "NSSacView.h"
#import <Carbon/Carbon.h>

#include "../inc/select.h"

const char *history_navigate(int direction);

void osx_sac(void *id);
void osx_main_gui_command(char *kmsg, int n, int plot, int echo);
void osx_main();
void osx_init();
void osx_begindevice();

NSArray *command_list;

@implementation SAC

void
sac_table_refresh(void *id) {
    SAC *sac = (SAC *) id;
    [sac tableRefresh];
}

void
sac_focus_window(void *id, int n) {
    SAC *sac = (SAC *) id;
    [sac focusWindow: n];
}

int
sac_find_window(void *id, int n) {
    SAC *sac = (SAC *) id;
    return ([sac findWindow: n] != nil);
}

void sac_create_window(void *id, int n) {
    SAC *sac = (SAC *) id;
    [sac createWindow: nil number: n];
}

- (NSUserDefaults *) userDefaults {
  return [NSUserDefaults standardUserDefaults];
}

- (void) prefsViewUpdate {
  if([prefsView respondsToSelector: @selector(update:)]) {
    NSLog(@"UPDATE PREFS VIEW");
    [prefsView performSelector: @selector(update:) withObject: self];
  }
}

- (void) setStartupFile: (NSString *) file {
  startupFile = [file copy];
  [[self userDefaults] setObject: file forKey: @"startupfile"];
  [[self userDefaults] synchronize];
  NSLog(@"STARTUP: %@", file);
  [self prefsViewUpdate];
}
- (void) setStartup: (BOOL) value {
  NSLog(@"STARTUP: %d", startup);
  startup = value;
  [[self userDefaults] setBool: value forKey: @"startup"];
  [[self userDefaults] synchronize];
  [self prefsViewUpdate];
}
- (NSString *) startupFile {
  startupFile = [[self userDefaults] stringForKey: @"startupfile"];
  return startupFile;
}
- (BOOL) startup {
  startup = [[self userDefaults] boolForKey: @"startup"];
  return startup;
}

- (NSFont *) font {
  NSString *name = [[self userDefaults] stringForKey: @"fontname"];
  float size = [[self userDefaults] floatForKey: @"fontsize"];
  if(!name || !size) {
    return nil;
  }
  font = [NSFont fontWithName: name size: size];
  return font;
}

- (void) setFont: (NSFont *) newFont {
  font = newFont;
  [[self userDefaults] setObject: [font fontName] forKey:@"fontname"];
  [[self userDefaults] setObject: [NSNumber numberWithFloat: [font pointSize]] forKey:@"fontsize"];
  [[self userDefaults] synchronize];
  [self prefsViewUpdate];
}

- (id) init {
    self = [super init];
    if(self) {
        plot_type    = SAC_PLOT_SINGLE;
        plotWindows  = [[NSMutableArray alloc] initWithCapacity: 5];
        command_list = [NSArray arrayWithObjects: @"zero",
                                @"qdp off", // 1
                                @"qdp on",  // 2
                                @"xlin",    // 3
                                @"xlog",    // 4
                                @"ylin",    // 5
                                @"ylog",    // 6
                                @"plot1",   // 7
                                @"plot2",   // 8
                                @"rmean",   // 9
                                @"taper",  // 10
                                @"rtrend verbose", // 11
                                @"write over",     // 12
                                @"transfer from evalresp to none", // 13
                                @"transfer from polezero to none", // 14
                                @"int", // 15
                                @"dif", // 16
                                @"fft", // 17
                                @"ifft", // 18
                                @"correlate", // 19
                                @"convolve", // 20
                                @"envelope", // 21
                                @"sqr", // 22
                                @"sqrt", // 23
                                @"abs", // 24
                                @"log", // 25
                                @"log10", // 26
                                @"exp", // 27
                                @"exp10", // 28
                                @"plotsp am", // 29
                                @"plotsp ph", // 30
                                @"plotsp rl", // 31
                                @"plotsp im", // 32
                                nil];
        [command_list retain];
        initFiles = nil;
        osx_sac( self );
        initialized = NO;
        {
        }
    }
    return self;
}

- (void) applicationWillFinishLaunching: (NSNotification *) note {
}

- (void) applicationDidFinishLaunching: (NSNotification *) note {
  {
    if([prefs respondsToSelector: @selector(general)]) {
      id g = [prefs performSelector: @selector(general)];
      NSLog(@"GENERAL: %@", g);
      if([g respondsToSelector: @selector(setModel:)]) {
        NSLog(@"SET MODEL");
        [g performSelector:@selector(setModel:) withObject: self];
      }
      prefsView = [g view];
      NSLog(@"PREFS VIEW: %@",  prefsView);
    }

    if(![self startupFile]) {
      [self setStartupFile: nil];;
    }
    if(![self startup]) {
      [self setStartup: NO];
    }
    if(![self font]) {
      [self setFont: [NSFont fontWithName: @"Menlo-Regular" size: 11.5]];
    }
    [commandView changeFont: font];
    [self prefsViewUpdate];
    NSLog(@"STARTUP: %@ %d", startupFile, startup);
 }
    if(NO) {
        [NSThread detachNewThreadSelector: @selector(stdin_loop:)
                                 toTarget: self
                               withObject: nil];    
    } else {
        NSString *aux = [[NSBundle mainBundle] pathForResource: @"aux" ofType: nil];
        setenv("SACAUX", [aux UTF8String], 1);
        osx_init();
        settextwait("OF");
        sac_history_file_set(NULL);
        sac_history_load(sac_history_file());
        /* Read in the Sac Copyright and display the Prompt */
        if(startup && startupFile) {
          char *sfile = strdup([[NSString stringWithFormat: @"%@ ", startupFile] UTF8String]);
          NSLog(@"EXECUTE COMMAND LINE: %s", sfile);
          osx_execute_macro( sfile,  strlen(sfile));
        }
        if(! initFiles ) {
          [ commandView sendCommand: @" " plot: NO echo: NO];
        } else {
          [self readFiles: initFiles clearFiles: YES];
        }
        sacUpdateOSX();
        initialized = YES;
    }
}

- (void) stdin_loop: (id) sender {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    osx_main();
    [pool drain];
}

- (void) awakeFromNib {
}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot {
    [commandView sendCommand: command plot: plot echo: YES ];
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

- (IBAction) fileMath: (id) sender {
    NSMutableString *com;
    NSOpenPanel *op = [NSOpenPanel openPanel];
    [op setCanChooseFiles: YES];
    [op setCanChooseDirectories: NO];
    [op setAllowsMultipleSelection: NO];
    if([op runModal] == NSOKButton) {
        com = [NSMutableString stringWithCapacity: 75];
        if([sender tag] == SacTagAddFile) {
            [ com appendString: @"addf " ];
        }
        else if([sender tag] == SacTagSubtractFile) {
            [ com appendString: @"subf " ];
        }
        else if([sender tag] == SacTagMultiplyFile) {
            [ com appendString: @"mulf " ];
        }
        else if([sender tag] == SacTagDivideFile) {
            [ com appendString: @"divf " ];
        }
        [com appendString: [[[op URLs] objectAtIndex: 0] path]];
        [self sendCommand: com plot: YES];
    }
}


- (IBAction) openDocument: (id) sender {
    NSOpenPanel *op = [NSOpenPanel openPanel];
    [op setCanChooseFiles:YES];
    [op setCanChooseDirectories:NO];
    [op setAllowsMultipleSelection: YES];
    //[op setAllowedFileTypes: [NSArray arrayWithObjects: @"sac", nil]];
    if([op runModal] == NSOKButton) {
        [self readFiles: [op URLs] 
             clearFiles: [sender tag] != SacTagOpenMoreFile];
    }
}
- (IBAction) saveAsDocument: (id) sender {
  //NSLog(@"Save As Document: %@", sender);
}
- (IBAction) printDocument: (id) sender {
  //NSLog(@"Print Document: %@", sender);
}
- (IBAction) simple: (id) sender {
    [self sendCommand: [command_list objectAtIndex: [sender tag]] plot: YES];
}
- (IBAction) simpleNoPlot: (id) sender {
    [self sendCommand: [command_list objectAtIndex: [sender tag]] plot: NO];
    if([sender tag] == SacTagPlotSingle) {
        plot_type = SAC_PLOT_SINGLE;
    } else if([sender tag] == SacTagPlotOverlay) {
        plot_type = SAC_PLOT_OVERLAY;
    }
}

- (IBAction) qdp: (id) sender {
    if([sender state] == NSOnState) {
        [self sendCommand: @"qdp off" plot: YES];
    } else {
        [self sendCommand: @"qdp on" plot: YES];
    }
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

- (void) createWindow: (id) sender number: (int) n {
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

- (void) tableRefresh {

    int i, n;
    NSArray *cols;

    cols = [headerTable tableColumns];
    n = [cols count];
    while([[headerTable tableColumns] count] > 0) {
        [headerTable removeTableColumn:[[headerTable tableColumns] lastObject]];
    }
    n = sac_files_length() + 1;
    for(i = 0; i < n; i++) {
        NSTableColumn *col = [[NSTableColumn alloc] 
                                    initWithIdentifier: 
                                     [NSNumber numberWithInt: i]];
        [col setWidth: 100.0];
        [col setMaxWidth: 120.0];
        [col setMinWidth: 80.0];
        [[col dataCell] setFont: [NSFont fontWithName: @"Menlo" size: 11.0]];
        if(i == 0) {
            [[col headerCell] setStringValue:@"Header"];
        } else {
            [[col headerCell] setStringValue:@"File"];
        }

        [headerTable addTableColumn: col];
    }

    [ headerTable reloadData ];
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

- (void) changeFontSize: (id) sender {
  [commandView changeFontSize: sender];
}

- (void) changeFont: (id) sender {
  NSLog(@"SAC CHANGE FONT");
  NSFont *newFont = [sender convertFont: font];
  [self setFont: newFont];
  [commandView changeFont: font];
}

@end

@implementation CommandView

- (BOOL)textView:(NSTextView *)aTextView shouldChangeTextInRange:(NSRange)affectedCharRange replacementString:(NSString *)replacementString {
    if(affectedCharRange.location < startOfLine.location) {
        return NO;
    }
    return YES;
}

- (int) length {
    return [[self textStorage] length];
}

- (void) scrollToEnd {
  [self scrollRangeToVisible: NSMakeRange([self length],0)];
}

- (void) end {
    [self setSelectedRange: NSMakeRange([self length],0)];
}
- (void) begin {
    [self setSelectedRange: startOfLine];
}

- (void) prompt {
    startOfLine = NSMakeRange([self length], 0);
}

- (void) append: (NSString *) str {
    [self end];
    [self insertText: str];
    startOfLine = NSMakeRange([self length], 0);
}

- (void) readFromOutput {
    NSData *data;
    NSMutableString *str;
    const char *p;
    str = [NSMutableString stringWithCapacity: 100];
    data = [pipeReadHandle readDataOfLength: 1];
    p = [data bytes];
    while(p[0] != 4) {
        [str appendFormat: @"%c", p[0]];
        data = [pipeReadHandle readDataOfLength: 1];
        p = [data bytes];
    }
    [self append: str];
}

- (void) flush {
    [self readFromOutput];
    [self prompt];
}

- (id) init {
    self = [super init];
    if(self) {
        
    }
    return self;
}

- (void) awakeFromNib {
    [ self setDelegate: self];
    
    pipe = [NSPipe pipe] ;
    [pipe retain];
    pipeReadHandle = [pipe fileHandleForReading] ;
    /* Send stdout to pipe */
    dup2([[pipe fileHandleForWriting] fileDescriptor], fileno(stdout)) ;
    /* Send stderr to stdout (and the pipe) */
    //dup2(fileno(stdout), fileno(stderr));

    NSFont* font = [NSFont fontWithName:@"Menlo Regular" size:11.5];
    [ self setFont: font];
}

- (BOOL) acceptsFirstResponder {
    return YES;
}
- (BOOL) becomeFirstResponder {
    return YES;
}

- (NSRange) currentPoint {
    return [self selectedRange];
}

- (void) osx_command_thread: (NSArray *) data {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *str = [data objectAtIndex: 0];
    BOOL plot     = [[data objectAtIndex: 1] boolValue];
    BOOL echo     = [[data objectAtIndex: 2] boolValue];
    /*
    [self performSelectorOnMainThread: @selector(flush)
                           withObject: nil
                        waitUntilDone: NO];
    */
    osx_main_gui_command((char *)[str UTF8String], [str length], plot, echo);
    [pool drain];
}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot echo :(BOOL) echo {
    [NSThread detachNewThreadSelector: @selector(osx_command_thread:)
                             toTarget: self
                           withObject: 
            [NSArray arrayWithObjects: command,
                     [NSNumber numberWithBool: plot], 
                     [NSNumber numberWithBool: echo],
                     nil]];
    [self flush];
}

- (NSRange) editableArea {
  return NSMakeRange(startOfLine.location, [self length] - startOfLine.location);
}

- (void) replaceLine: (const char *) replace {
  if(!replace) {
    replace = strdup("");
  }
  NSRange range = [self editableArea];
  NSString *new = [NSString stringWithUTF8String: replace];
  if([self shouldChangeTextInRange: range  replacementString: new]) {
    [[self textStorage] beginEditing];
    {
      [[self textStorage] replaceCharactersInRange: range withString: new];
    }
    [[self textStorage] endEditing];
  } 
}

- (BOOL) inEditableArea {
  return ([self currentPoint].location >= startOfLine.location);
}
- (BOOL) afterStartOfEditableArea {
  return ([self currentPoint].location > startOfLine.location);
}

- (BOOL) isDirectory: (NSString *) file path: (NSString *) path FM: (NSFileManager *) fm{
  BOOL isDir;
  NSString *fullpath = [NSString stringWithFormat: @"%@%@", path, file];
  [fm fileExistsAtPath: fullpath isDirectory: &isDir];
  return isDir;
}

- (NSRange) findBackwards: (NSString *) str set: (NSString *) set range: (NSRange) range {
  NSCharacterSet *s = [NSCharacterSet characterSetWithCharactersInString: set];
  return [str rangeOfCharacterFromSet: s options: NSBackwardsSearch range: range];
}

- (NSRange) rangeForUserCompletion {
  NSRange r = [self currentPoint];
  NSRange b = startOfLine;
  NSString *txt = [[self textStorage] string];
  NSRange s = [self findBackwards: txt set: @" /" range: 
                      NSMakeRange(b.location, r.location-b.location)];
  NSRange e = [self findBackwards: txt set: @" "  range: 
                      NSMakeRange(r.location, [self length] - r.location)];
  if(e.location == NSNotFound) {/* No space at end of string */
    e = NSMakeRange([self length],0);
  }
  if(s.location == NSNotFound) { /* At beginning of string */
    s = b;
  } else {
    s.location += 1;
  }
  
  return NSMakeRange(s.location, e.location-s.location);
}


- (NSArray *) completionsForPartialWordRange: (NSRange) range indexOfSelectedItem : (NSInteger *) index {

  *index = -1;
  int i;
  NSString *word, *path;
  NSRange s;
  NSMutableArray *glob;
  NSArray *files;
  NSFileManager *fm = [NSFileManager defaultManager];
  NSString *txt = [[self textStorage] string];

  if(range.location == startOfLine.location && range.length == 0) {
    return nil;
  }

  /* Find first occurance of whitespace backwards - 
     Beginning of full string to complete (including path)
  */
  {
    NSRange b = startOfLine;
    NSRange r = [self currentPoint];
    NSRange sr = NSMakeRange(b.location, r.location-b.location);
    s = [self findBackwards: txt set: @" " range: sr];
    s.location = (s.location == NSNotFound) ? b.location : s.location + 1;
    s.location = MIN(s.location, [self length]);
  }

  /* Filename  */
  word = [[self attributedSubstringFromRange: range] string];
  
  /* Path */
  if(range.location - s.location <= 0) { /* No Path */
    path = [NSString stringWithString: @"."];
  } else {
    path = [[self attributedSubstringFromRange: 
                    NSMakeRange(s.location,range.location-s.location)] string];
  }
  
  /* Relative Path - First Character is NOT '/' */
  if([path characterAtIndex: 0] != '/') {
    /* Current Working Directory */
    NSURL *cwd = [[NSURL URLWithString: [fm currentDirectoryPath]] 
                   URLByAppendingPathComponent: @"" ];
    if(! [path isEqualToString: @"."]) { /* Filename, no Directory */
      cwd = [NSURL URLWithString: path relativeToURL: cwd];
    }
    path = (NSString *) CFURLCopyPath((CFURLRef)cwd);;
  }

  /* Get Files in Directory @ path */
  files = [fm contentsOfDirectoryAtPath: path error: nil];

  /* Grab files that begin with 'word' */
  if(word) { 
    NSString *fmt = [NSString stringWithFormat: @"SELF BEGINSWITH '%@'", word];
    NSPredicate *filter = [NSPredicate predicateWithFormat: fmt];
    files = (NSMutableArray *) [files filteredArrayUsingPredicate: filter];
  }

  glob = [NSMutableArray arrayWithCapacity: 40];

  /* Add trailing slash if directory */
  for(i = 0; i < [files count]; i++) {
    NSString *file = [files objectAtIndex: i];
    if([self isDirectory: file path: path FM: fm]) {
      [glob addObject: [NSString stringWithFormat: @"%@/", file]];
    } else {
      [glob addObject: [NSString stringWithFormat: @"%@", file]];
    }
  }

  /* Replace if a single value */
  if([glob count] == 1) { 
    NSString *file = [glob objectAtIndex: 0];
    if(word) {
      [[self textStorage] replaceCharactersInRange: range withString: file];
    } else {
      range = NSMakeRange(range.location + range.length, 0);
      [[self textStorage] replaceCharactersInRange: range withString: file];      
    }
    return nil;
  }
  return glob;
}

- (void) runCommand: (NSString *) command {
  if(!command) {
    command = [NSString stringWithString: @" "];
  }
  [self end];
  [self insertText: @"\n"];
  [self sendCommand: command plot: NO echo: NO];
  history_id = 0;
}

- (void) paste: (id) sender {
  int i;
  NSPasteboard *pb = [NSPasteboard generalPasteboard];
  NSArray *classArray = [NSArray arrayWithObject:[NSString class]];
  NSDictionary *options = [NSDictionary dictionary];
  BOOL ok = [pb canReadObjectForClasses: classArray options: options];
  if(ok) {
    NSArray *objs = [pb readObjectsForClasses: classArray options: options];
    NSString *str = [objs objectAtIndex: 0];
    NSArray *lines = [str componentsSeparatedByString:@"\n"];
    NSString *s = [lines objectAtIndex: 0];
    if(![self inEditableArea] ) {
      [self end];
    }
    [self insertText: s];
    for(i = 1; i < [lines count]; i++) {
      [self runCommand: s];
      s = [lines objectAtIndex: i];
      [self insertText: s];
    }
  }
}

- (void) keyDown: (NSEvent *) event {
    unichar buf;
    [[event characters] getCharacters: &buf];
    if([event modifierFlags] & NSControlKeyMask) {
        [[event charactersIgnoringModifiers] getCharacters: &buf];
        if(buf == 'a') { /* Control-a */
            [self begin];
            [self scrollToEnd];
            return;
        }
        if(buf == 'e') { /* Control-e */
            [self end];
            [self scrollToEnd];
            return;
        }
    } else if(buf == NSUpArrowFunctionKey) {
      [self replaceLine: history_navigate( (history_id) ? 1 : 0 )];
      history_id = max(0, history_id+1);
      [self scrollToEnd];
      return;
    } else if(buf == NSDownArrowFunctionKey) {
      history_id = max(0, history_id-1);
      [self replaceLine: (history_id > 0) ? history_navigate(-1): nil];
      [self scrollToEnd];
      return;
    } else if(buf == NSLeftArrowFunctionKey) {
      if( ! [self afterStartOfEditableArea ] ) { 
        return;
      }
    } else if(buf == '\t') {
      if(![self inEditableArea]) {
        [self end];
      }
      [self complete: self];
      [self scrollToEnd];
      return;
    } else if(buf == '\n') {
        return;
    } else if(buf == '\r') {
        NSRange r = [self editableArea];
        NSString *str = [[self attributedSubstringFromRange: r] string];
        [self runCommand: str];
        return;
    } else if( ! [self inEditableArea] ) {
      [self end];
    }

    [self interpretKeyEvents:[NSArray arrayWithObject: event]];
}
/*
http://stackoverflow.com/questions/2406204/what-is-the-best-way-to-redirect-stdout-to-nstextview-in-cocoa
*/

- (void) changeFont: (NSFont *) newFont {
  NSTextStorage *txt = [self textStorage];
  [txt beginEditing];

  /* Changed existing text font size */
  [txt enumerateAttributesInRange: NSMakeRange(0, [txt length])
                          options: 0 
                       usingBlock: ^(NSDictionary *attrs,
                                     NSRange range,
                                     BOOL *stop) {
      //NSFont *font = [attrs objectForKey: NSFontAttributeName];
      if(newFont) {
        [txt removeAttribute: NSFontAttributeName range: range];
        //font = [[NSFontManager sharedFontManager] convertFont: font toSize: [font pointSize] + dfont];
        //font = [[NSFontManager sharedFontManager] convertFont: font ];
        [txt addAttribute: NSFontAttributeName value: newFont range: range];
      }
    }
   ];

  [txt endEditing];
  /* Set Current Font */
  {
    //NSFont *font = [self font];
    //font = [[NSFontManager sharedFontManager] convertFont: font toSize: [font pointSize] + dfont];
    [self setFont: newFont];
  }
  [self didChangeText];
}

- (void) changeFontSize: (id) sender {
  int dfont = [sender tag];
  /* Change existing text font size */
  NSFont *font = [self font];
  font = [[NSFontManager sharedFontManager] convertFont: font toSize: [font pointSize] + dfont];
  [self changeFont: font];
}

@end
/*

read /Users/savage13/Bits/sac/sac.build/osx/tmp.sac
plot1

*/
