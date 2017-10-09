#ifndef ShellFunctionsIncluded		/* prevent multiple inclusion */
#define ShellFunctionsIncluded 
int execute(char**commandVec,char**envp,char**pathVec);
char *getVar(char **envp, char*name);
int setVar(char **envp, char *name,char *str);
#endif
