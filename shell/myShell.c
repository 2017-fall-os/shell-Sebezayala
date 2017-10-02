#include <unistd.h>/*For read, write, execve and fork*/
#include <stdio.h>/*For printf*/
#include <stdlib.h>/*For free*/
#include <sys/wait.h>/*For wait*/
#include "mytoc.h"
#include "utils.h"
#include "shellFunctions.h"
#define BUFLEN 1024

int main(int argc, char**argv, char**envp){
    char buf[BUFLEN];
    char **newLineVec,**pipeVec, **pathVec,**bgrndVec;
    int retVal, readRet, stat, numOfPipes,numOfbgs;
    
    //Find the $PATH environmental variable and tokenize it
    for (int i=0; envp[i] != (char*)0; i++){
        if(startsWith(envp[i],"PATH")){
            pathVec=mytoc(envp[i],':');
            remvChrsfromLeft(5,pathVec[0]);//Used to remove "PATH=" fomr the first token
            break;
        }
    }
    
    for(;;){
        write(1,"$ ",2);
        readRet=read(0,buf,BUFLEN);
        if(timeToExit(buf)||!readRet){//exit the program if the users enters the word exit or reached EOF.
            return 0;
        }
        newLineVec=mytoc(buf,'\n');
        for(int i=0;newLineVec[i];i++){
            bgrndVec=mytoc(newLineVec[i],'&');//tokenize for background first
            if(bgrndVec[0]){
                numOfbgs=0;
                for(int i=1;bgrndVec[i];i++){
                    numOfbgs++;
                }
                if (numOfbgs){//if there are background processes
                    bgroundHandler(bgrndVec,envp,pathVec);
                }else{//if no back ground processes
                    pipeVec=mytoc(newLineVec[i],'|');
                    numOfPipes=0;
                    for(int i=1;pipeVec[i];i++){
                        numOfPipes++;
                    }
                    if (numOfPipes){//check for pipes
                        pipesHandler(numOfPipes,pipeVec,envp,pathVec);
                    }else{//if no pipes just try to run the command
                        normalHandler(pipeVec[0],envp,pathVec);
                    }
                    free(pipeVec);
                }
            
            }
            free(bgrndVec);
            clean(buf);//Clean buf so that there are no residual chars.
        }
        free(newLineVec);
    }
}
