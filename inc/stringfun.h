#ifdef DEFINE_STRING_FUN
char* upstring(char* string);
char *stringcat(char *str1, ...);

/* concatenate string2 to string1 after allocating necessary space */
char *strAllocCat(char * string1, char *string2);

#else

extern char *Upstring(char *str);
extern char *stringcat(char *str1, ...);
extern char *CpUpstring(const char *str);
/* concatenate string2 to string1 after allocating necessary space */
extern char *strAllocCat(char * string1, char *string2);


#endif
