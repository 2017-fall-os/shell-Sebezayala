#ifndef UtilsIncluded		/* prevent multiple inclusion */
#define UtilsIncluded  

int strLength(char *inStr);
void clean(char *var);
int timeToExit(char *var);
int startsWith(char *origStr, char *word);
char *strcopy(char *inStr);
void remvChrsfromLeft(int num,char *str);
char *strConcat(char *first, char *second);

#endif
