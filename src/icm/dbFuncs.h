#ifndef DB_FUNCS_H
#define DB_FUNCS_H

char *GetRESPfileNameFromDB(char *station, char *component, char *locid,
                            double EpochTime , char * rtypeOut );
			    
void DisconnectFromOracleTransfer(void);			    

#endif
