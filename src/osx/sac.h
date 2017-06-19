
#import <Cocoa/Cocoa.h>

@interface SAC : NSObject <NSApplicationDelegate>
{
  /* Array of plot Windows */
  NSMutableArray       *plotWindows;
  
  /* Array of files - filled on double click */
  NSMutableArray       *initFiles;
  BOOL                  initialized;

}

- (void) sendCommand: (NSString *) command plot: (BOOL) plot ;

- (IBAction) createWindowSac: (id) sender ;
- (void)     createWindowWithNumber: (NSNumber *) n;
- (id)       findWindow: (int) windowNumber;
- (void)     focusWindow: (int) windowNumber;
- (void)     removeWindow: (id) plot;

- (void)     readFiles: (NSArray *) files clearFiles: (BOOL) clearFiles;

@end


