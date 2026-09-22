/** 
 * @file   history.h
 * 
 * @brief  History stuff
 * 
 */

#ifndef _HISTORY_SAC_H_
#define _HISTORY_SAC_H_

char *AddToHistory(char *line);
int use_history(int getset);

void sac_history_load(char *where);
void sac_history_file_set(char *name);
char *sac_history_file();
char *sac_history_file();
int history_size();
void history_size_set(int value);

void write_history(char *file);
void clear_history(void);

#endif /* _HISTORY_SAC_H_ */
