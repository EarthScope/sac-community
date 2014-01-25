#ifndef SUDS_WRITING_H
#define SUDS_WRITING_H

int WriteADlist(adList *adHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteDElist(deList *deHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteESlist(esList *esHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteTRlist(trList *trHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteFElist(feList *feHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteSClist(scList *scHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteTSlist(tsList *tsHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteTClist(tcList *tcHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteTElist(teList *teHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteCAlist(caList *caHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteCOlist(coList *coHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteEVlist(evList *evHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteEDlist(edList *edHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteFOlist(foList *foHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteINlist(inList *inHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteLAlist(laList *laHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteLOlist(loList *loHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteMOlist(moList *moHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteORlist(orList *orHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WritePRlist(prList *prHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteRElist(reList *reHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteSHlist(shList *shHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteERlist(erList *erHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteEQlist(eqList *eqHead, SUDS_STRUCTTAG *Tag, FILE *ptr);
int WriteCSlist(csList *csHead, SUDS_STRUCTTAG *Tag, FILE *ptr);





#endif
