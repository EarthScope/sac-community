
#import "SacControllers.h"
#import "sac.h"

void
filter_response(char   *type, 
                char   *proto,
                int     order,
                float   low,
                float   high,
                double  dt,
                int     n,
                float  *freq,
                float  *amp,
                float  *phase) ;
float max_delta();
int   max_npts ();


@implementation SacPlotLimitsController 

- (void) awakeFromNib {
  SacFormatter *fmt = [[[SacFormatter alloc] init] retain];
  [xmin setFormatter: fmt];
  [xmax setFormatter: fmt];
  [ymin setFormatter: fmt];
  [ymax setFormatter: fmt];
}

- (IBAction) applyXLimits : (id) sender {
    NSString *com;
    if([xauto state] == NSOnState) {
        com = [NSString stringWithString: @"xlim off"];
    } else {
        com = [NSString stringWithFormat: @"xlim %g %g", 
                        [xmin floatValue], [xmax floatValue]];
    }
    [parent sendCommand: com plot: YES];
}
- (IBAction) applyYLimits : (id) sender {
    NSString *com;
    if([yauto state] == NSOnState) {
        com = [NSString stringWithString: @"ylim off"];
    } else {
        com = [NSString stringWithFormat: @"ylim %g %g", 
                        [ymin floatValue], [ymax floatValue]];
    }
    [parent sendCommand: com plot: YES];
}

@end


@implementation SacFilterController

- (void) awakeFromNib {
  SacFormatter *pfmt = [[[SacFormatter alloc] init] retain];
  [pfmt unsigned: YES];
  [corner1 setFormatter: pfmt];
  [corner2 setFormatter: pfmt];
}

- (IBAction) showWindow: (id) sender {
    [filterWindow makeKeyAndOrderFront: sender];
    [self changeFilter: sender];
}

- (NSString *) command {
    NSMutableString *com = [NSMutableString stringWithCapacity: 32];
    NSString *ftype = [type titleOfSelectedItem];
    if([ftype isEqualToString:@"Bandpass"] ||
       [ftype isEqualToString:@"Bandreject"]) {
        [com appendFormat: @"%@ corners %g %g ", ftype, 
             [corner1 floatValue], [corner2 floatValue]];
    } else if([ftype isEqualToString: @"Highpass"]) {
        [com appendFormat: @"%@ corner %g ", ftype, [corner1 floatValue]];
    } else if([ftype isEqualToString: @"Lowpass"]) {
        [com appendFormat: @"%@ corner %g ", ftype, [corner2 floatValue]];
    }
    [com appendFormat: @"npoles %@ passes %@",
         [poles titleOfSelectedItem],
         [passes titleOfSelectedItem]];
    return com;
}

- (IBAction) applyFilter: (id) sender {
    NSString *com = [self command];
    [filterWindow orderOut: self];
    [parent sendCommand: com plot: YES];
}

- (IBAction) changeFilter: (id) sender {
    char *ft;
    int n, n2, npoles;
    float *amp, *phase, *freq;
    float co1, co2;
    NSString *ftype = [type titleOfSelectedItem];
    float dt, nyq;

    dt = max_delta();
    n  = max_npts();
    nyq = 1/(2.0 * dt);
    n2 = 2;
    while(n2 < n) {
        n2 *= 2;
    }
    n = n2;

    co1 = [corner1 floatValue];
    co2 = [corner2 floatValue];
    npoles = [[poles titleOfSelectedItem] intValue];

    if([ftype isEqualToString:@"Bandpass"]) {
        ft = strdup("BP");
        if(co1 >= co2) {
          return;
        }
        if(co2 >= nyq) {
          return;
        }
    }
    if([ftype isEqualToString:@"Bandreject"]) {
        ft = strdup("BR");
        if(co1 >= co2) {
          return;
        }
        if(co2 >= nyq) {
          return;
        }
    }
    if([ftype isEqualToString:@"Highpass"]) {
        ft = strdup("HP");
        if(co1 >= nyq) {
          return;
        }
    }
    if([ftype isEqualToString:@"Lowpass"]) {
        ft = strdup("LP");
        if( co2 >= nyq) {
          return;
        }
    }

    freq  = (float *) malloc(sizeof(float) * n);
    amp   = (float *) malloc(sizeof(float) * n);
    phase = (float *) malloc(sizeof(float) * n);
    NSLog(@"%d %g %g %f %d", npoles, co1, co2, dt, n);
    filter_response(ft, "BU", npoles, co1, co2, dt, n, freq, amp, phase);
    
    NSGraphData *data = [ view getData: @"amplitude" ];
    if(!data) {
        [view setXAxis: LOG];
        [view setYAxis: LOG];
        [view xauto];
        [view ylimMin: 1e-3 max: 1.0]; 
        [view buffered: YES];
        data = [[NSGraphData alloc] init];
        [data setName: @"amplitude"];
        [data setLine: 1];
        [data setSymbol: 0];
        [data setColor: [NSColor blackColor]];
        [view addData: data];
    }

    [ data setData: n x: freq y: amp ];
    [ view setNeedsDisplay: YES ];

    free(freq);
    free(amp);
    free(phase);
    return;
}

@end

@implementation SacColorController 


- (IBAction) changeColor: (id) sender {
    NSMutableString *com;
    com = [NSMutableString stringWithString: @"color "];
    [com appendString: [ color titleOfSelectedItem]];
    if([increment state] == NSOnState) {
        [com appendString: @" increment on"];
    } else {
        [com appendString: @" increment off"];
    }
    [parent sendCommand: com plot: YES];
}

@end


@implementation SacFuncGenController 

- (void) awakeFromNib {
    [self change: self];
    SacFormatter *fmt = [[[SacFormatter alloc] init] retain];
    [v1 setFormatter: fmt];
    [v2 setFormatter: fmt];
    [v3 setFormatter: fmt];
    [v4 setFormatter: fmt];
    [begin setFormatter: fmt];
    SacFormatter *pfmt = [[[SacFormatter alloc] init] retain];
    [pfmt unsigned: YES];
    [delta setFormatter: pfmt];
}

- (IBAction) apply: (id) sender {
    NSMutableString *str = [[NSMutableString alloc] initWithCapacity: 50];
    NSString *fstr = [type titleOfSelectedItem];
    [str appendFormat: @"funcgen %@", fstr];
    if([fstr isEqualToString: @"Sine"]) {
        [str appendFormat: @" %g", [v1 floatValue]];
        [str appendFormat: @" %g", [v2 floatValue]];
    } else if([fstr isEqualToString: @"Line"]) {
        [str appendFormat: @" %g", [v1 floatValue]];
        [str appendFormat: @" %g", [v2 floatValue]];
    } else if([fstr isEqualToString: @"Quadratic"]) {
        [str appendFormat: @" %g", [v1 floatValue]];
        [str appendFormat: @" %g", [v2 floatValue]];
        [str appendFormat: @" %g", [v3 floatValue]];
    } else if([fstr isEqualToString: @"Cubic"]) {
        [str appendFormat: @" %g", [v1 floatValue]];
        [str appendFormat: @" %g", [v2 floatValue]];
        [str appendFormat: @" %g", [v3 floatValue]];
        [str appendFormat: @" %g", [v4 floatValue]];
    } else if([fstr isEqualToString: @"Impstring"]) {
        [str appendFormat: @" %g", [v1 floatValue]];
        [str appendFormat: @" %g", [v2 floatValue]];
        [str appendFormat: @" %g", [v3 floatValue]];
        [str appendFormat: @" %g", [v4 floatValue]];
    }
    if([useDelta state] == NSOnState) {
        [str appendFormat: @" Delta %g", [delta floatValue]];
    }
    if([useNpts state] == NSOnState) {
        [str appendFormat: @" Npts %g", [npts intValue]];
    }
    if([useBegin state] == NSOnState) {
        [str appendFormat: @" Begin %g", [begin floatValue]];
    }
    [parent sendCommand: str plot: YES];
    [window orderOut: self];
}

- (IBAction) change: (id) sender {
    NSString *str = [type titleOfSelectedItem];
    int height = 355;
    int size = 31;
    if([str isEqualToString:@"Seismogram"] ||
       [str isEqualToString:@"Step"] ||
       [str isEqualToString:@"Boxcar"] ||
       [str isEqualToString:@"Triangle"] ||
       [str isEqualToString:@"Random"] ||
       [str isEqualToString:@"Impulse"] 
       ) {
        [v1 setHidden: YES];
        [v2 setHidden: YES];
        [v3 setHidden: YES];
        [v4 setHidden: YES];
        [t1 setHidden: YES];
        [t2 setHidden: YES];
        [t3 setHidden: YES];
        [t4 setHidden: YES];
        NSRect frame = [window frame];
        frame.size.height = height - 4 * size;
        [window setFrame:frame display:YES animate:YES];        
    } else if([str isEqualToString:@"Sine"]) {
        [v1 setHidden: NO];
        [v2 setHidden: NO];
        [v3 setHidden: YES];
        [v4 setHidden: YES];
        [t1 setHidden: NO];
        [t2 setHidden: NO];
        [t3 setHidden: YES];
        [t4 setHidden: YES];
        [t1 setStringValue: [NSString stringWithString: @"Frequency"]];
        [t2 setStringValue: [NSString stringWithString: @"Phase"]];
        NSRect frame = [window frame];
        frame.size.height = height - 2 * size;
        [window setFrame:frame display:YES animate:YES];        
    } else if([str isEqualToString:@"Line"]) {
        [v1 setHidden: NO];
        [v2 setHidden: NO];
        [v3 setHidden: YES];
        [v4 setHidden: YES];
        [t1 setHidden: NO];
        [t2 setHidden: NO];
        [t3 setHidden: YES];
        [t4 setHidden: YES];
        [t1 setStringValue: [NSString stringWithString: @"Slope"]];
        [t2 setStringValue: [NSString stringWithString: @"Intercept"]];
        NSRect frame = [window frame];
        frame.size.height = height - 2 * size;
        [window setFrame:frame display:YES animate:YES];        
    } else if([str isEqualToString:@"Quadratic"]) {
        [v1 setHidden: NO];
        [v2 setHidden: NO];
        [v3 setHidden: NO];
        [v4 setHidden: YES];
        [t1 setHidden: NO];
        [t2 setHidden: NO];
        [t3 setHidden: NO];
        [t4 setHidden: YES];
        [t1 setStringValue: [NSString stringWithString: @"Squared"]];
        [t2 setStringValue: [NSString stringWithString: @"Linear"]];
        [t3 setStringValue: [NSString stringWithString: @"Constant"]];
        NSRect frame = [window frame];
        frame.size.height = height - 1 * size;
        [window setFrame:frame display:YES animate:YES];        
    } else if([str isEqualToString:@"Cubic"]) {
        [v1 setHidden: NO];
        [v2 setHidden: NO];
        [v3 setHidden: NO];
        [v4 setHidden: NO];
        [t1 setHidden: NO];
        [t2 setHidden: NO];
        [t3 setHidden: NO];
        [t4 setHidden: NO];
        [t1 setStringValue: [NSString stringWithString: @"Cubed"]];
        [t2 setStringValue: [NSString stringWithString: @"Squared"]];
        [t3 setStringValue: [NSString stringWithString: @"Linear"]];
        [t4 setStringValue: [NSString stringWithString: @"Constant"]];
        NSRect frame = [window frame];
        frame.size.height = height;
        [window setFrame:frame display:YES animate:YES];        
    } else if([str isEqualToString:@"Impstring"]) {
        [v1 setHidden: NO];
        [v2 setHidden: NO];
        [v3 setHidden: NO];
        [v4 setHidden: NO];
        [t1 setHidden: NO];
        [t2 setHidden: NO];
        [t3 setHidden: NO];
        [t4 setHidden: NO];
        [t1 setStringValue: [NSString stringWithString: @"Point 1"]];
        [t2 setStringValue: [NSString stringWithString: @"Point 2"]];
        [t3 setStringValue: [NSString stringWithString: @"Point 3"]];
        [t4 setStringValue: [NSString stringWithString: @"Point 4"]];
        NSRect frame = [window frame];
        frame.size.height = height;
        [window setFrame:frame display:YES animate:YES];        
    }
}

@end

@implementation SacConstantMathController

- (void) awakeFromNib {
  SacFormatter *fmt = [[[SacFormatter alloc] init] retain];
  [v setFormatter: fmt];
}

- (IBAction) apply: (id) sender {
    int tag = [[type selectedItem]tag];
    NSMutableString *com = [NSMutableString stringWithCapacity: 75];
    if(tag == SacTagMathAdd) {
        [com appendString: @"add "];
    } else if(tag == SacTagMathSubtract) {
        [com appendString: @"sub "];
    } else if(tag == SacTagMathMultiply) {
        [com appendString: @"mul "];
    } else if(tag == SacTagMathDivide) {
        [com appendString: @"div "];
    }
    [com appendFormat: @"%g", [v floatValue]];
    [parent sendCommand: com plot: YES];
    [window orderOut: self];
}

@end

@implementation SacHeaderTableSource

/* stubs.c */
int sac_files_length();
char *sac_files_names();
float sac_files_float(int fid, int hid);
char * sac_files_header_name(int hid);


void
sac_table_update(void *id) {
    SacHeaderTableSource *table = (SacHeaderTableSource *) id;
    [ table update ];
    return ;
}

- (id) init {
    self = [super init];
    if(self) {
        osx_sac_table( self );
        filenames = [[NSMutableArray arrayWithCapacity: 10] retain];
        fv = [[NSMutableArray arrayWithCapacity: 70] retain];
    }
    return self;
}

- (void) awakeFromNib {
}

- (NSInteger) numberOfRowsInTableView: (NSTableView *) table {
    return [fv count];
}

- (id) tableView: (NSTableView *) table objectValueForTableColumn: (NSTableColumn *) column row: (NSInteger) row {
    int col = [[column identifier] intValue];
    row = [[fv objectAtIndex: row] intValue];
    if(col > [filenames count]) {
        return [NSString stringWithString: @"N/A"];
    }
    if(row == 0) {
        return [filenames objectAtIndex: col];
    }
    if(col == 0) {
        return [NSString stringWithFormat: @"%s", sac_files_header_name(row-1)];
    }
    return [NSString stringWithFormat: @"%g", sac_files_float(col, row-1)];
}

- (void) update {
    int i, j, v, n, len;
    char *files, *p, file[1024];
    n = sac_files_length();
    //files = sac_files_names();
    i = 0;
    //p = files;
    [filenames removeAllObjects];
    [filenames addObject: [NSString stringWithString: @"Header"]];
    while(i < n) { //}&& sscanf(p, "%s%n", &file[0], &len) != 0) {
        NSString *str = [[NSString alloc] initWithUTF8String: strdup("fixmefilename.sac")];
        if(str && filenames) {
            [filenames addObject: [str retain]];
        }
        p += len;
        i++;
    }
    n = sac_files_length();
    [fv removeAllObjects];
    [fv addObject: [NSNumber numberWithInt: 0]];
    for(i = 0; i < 70; i++) {
        v = 0;
        for(j = 0; j < n; j++) {
            if(sac_files_float(j+1, i) != -12345.0000) {
                v = 1;
            }
        }
        if(v == 1) {
            [fv addObject: [NSNumber numberWithInt: i+1]];
        }
    }
}


@end

@implementation SacFormatter 

- (id) init {
  self = [super init];
  if(self) {
    notSigned = NO;
  }
  return self;
}

- (void) unsigned: (BOOL) isUnsigned {
  notSigned = isUnsigned;
}

- (NSString *) stringForObjectValue: (id) object {
  float value = [(NSNumber*) object floatValue];
  if(notSigned) {
    value = fabs(value);
  }
  return [NSString stringWithFormat: @"%g", value];
}

- (BOOL) getObjectValue: (id *) object forString: (NSString *) string errorDescription : (NSString **)error {
  float value = [string floatValue];
  if(notSigned) {
    value = fabs(value);
  }
  *object = [NSNumber numberWithFloat: value];
  return YES;
}

@end
