
char * lower ( char * instring ) ;

int gcCheckWftag ( struct wftag * wt , struct wfdiscList * wfL ) ;

int gcCheckSensor ( struct sensor * se , struct wfdiscList * wfL ) ;

int gcCheckSitechan ( struct sitechan * sc , struct wfdiscList * wfL ) ;

int gcCheckSite ( struct site * si , struct wfdiscList * wfL ) ;

int gcCheckInstrument ( struct instrument * in , struct sensorList * sL ) ;

int gcCheckOrigin ( struct origin * orig , struct eventList *evL );

int gcCheckEvent ( struct event * ev , struct wftagList * wtL ) ;

struct wftagList * gcCollectWftag ( struct wftagList *wtL , DBlist dblList ) ;

struct sensorList * gcCollectSensor ( struct sensorList *seL , DBlist dblList ) ;

struct sitechanList * gcCollectSitechan ( struct sitechanList *scL , DBlist dblList ) ;

struct siteList * gcCollectSite ( struct siteList *siL , DBlist dblList ) ;

struct instrumentList * gcCollectInstrument ( struct instrumentList *inL , DBlist dblList ) ;

struct originList * gcCollectOrigin ( struct originList *orL , DBlist dblList ) ;

struct eventList * gcCollectEvent ( struct eventList *evL , DBlist dblList ) ;

void gcCollect ( DBlist dblList ) ;


