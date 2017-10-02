#ifndef ShellFunctionsIncluded		/* prevent multiple inclusion */
#define ShellFunctionsIncluded 

void pipesHandler(int numOfPipes,char **pipeVec,char **envp,char **pathVec);
void normalHandler(char *buf, char**envp,char**pathVec);
void bgroundHandler(char**bgrndVec,char**envp,char**pathVec);

#endif
