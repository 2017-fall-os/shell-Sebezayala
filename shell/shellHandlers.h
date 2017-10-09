#ifndef ShellHandlersIncluded		/* prevent multiple inclusion */
#define ShellHandlersIncluded 

void pipesHandler(int numOfPipes,char **pipeVec,char **envp,char **pathVec,int isBground);
void normalHandler(char *buf, char**envp,char**pathVec,int isBground);
void redirHandler(int numOfOut,int numOfIn,char*line, char**envp,char**pathVec,int numOfbgs);
#endif
