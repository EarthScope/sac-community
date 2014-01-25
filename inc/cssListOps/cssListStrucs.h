#ifndef FALSE
#       define FALSE 0
#       define TRUE !FALSE
#endif

#ifndef NULL
#       define NULL 0
#endif


#ifndef CSS_TREE_H
#define CSS_TREE_H TRUE

#include "cssStrucs.h"
#include "dblUserData.h"


/*                 +=======================================+                 */
/*=================|           csstree structure           |=================*/
/*                 +=======================================+                 */

struct CSStree
{
	struct affiliationList *afHead;
	struct arrivalList     *arHead;
	struct assocList       *asHead;
	struct eventList       *evHead;
	struct gregionList     *grHead;
	struct instrumentList  *inHead;
	struct origerrList     *oeHead;
	struct originList      *orHead;
	struct remarkList      *reHead;
	struct sensorList      *seHead;
	struct siteList        *slHead;
	struct sitechanList    *scHead;
	struct stassocList     *saHead;
	struct wfdiscList      *wfHead;
	struct wftagList       *wtHead;
	struct sacdataList     *sdHead;

	struct affiliationList *afTail;
	struct arrivalList     *arTail;
	struct assocList       *asTail;
	struct eventList       *evTail;
	struct gregionList     *grTail;
	struct instrumentList  *inTail;
	struct origerrList     *oeTail;
	struct originList      *orTail;
	struct remarkList      *reTail;
	struct sensorList      *seTail;
	struct siteList        *slTail;
	struct sitechanList    *scTail;
	struct stassocList     *saTail;
	struct wfdiscList      *wfTail;
	struct wftagList       *wtTail;
	struct sacdataList     *sdTail;

        userData *UserData;

};

 


#endif
