#include <unistd.h>/*For read, write, execve and fork*/
#include <stdio.h>/*For printf*/
#include <stdlib.h>/*For free*/
#include <sys/wait.h>/*For wait*/
#include "mytoc.h"
#include "utils.h"
#include "shellFunctions.h"
#include "shellHandlers.h"
#define BUFLEN 1024

int main(int argc, char**argv, char**envp){
    char buf[BUFLEN];
    char *prompt;
    char **newLineVec,**pipeVec, **pathVec,**bgrndVec;
    int retVal,readRet, stat, numOfPipes,numOfbgs,numOfOut,numOfIn;
    
    //Find the $PATH environmental variable and tokenize it
    pathVec=mytoc(getVar(envp,"PATH"),':');
    //Find the PS1 var and save it in prompt
    prompt=getVar(envp,"PS1");
    if(!prompt){//in case PS1 was not exported we use a default prompt
        fprintf(stderr,"Failed to get PS1: Using default prompt\n");
        prompt="$ ";
    }
    for(;;){
        write(1,prompt,strLength(prompt));
        readRet=read(0,buf,BUFLEN);
        if(timeToExit(buf)||!readRet){//exit the program if the users enters the word exit or reached EOF.
            return 0;
        }
        newLineVec=mytoc(buf,'\n');//Tokenize for new line first
        for(int lineNum=0;newLineVec[lineNum];lineNum++){//For every token in the new line vector
            numOfbgs=containChar(newLineVec[lineNum],'&');//Count the number of background processess
            bgrndVec=mytoc(newLineVec[lineNum],'&');//tokenize for background second
            for(int bgNum=0;bgrndVec[bgNum];bgNum++){
                    //Check if there are any redirections
                    numOfOut=containChar(bgrndVec[bgNum],'>');
                    numOfIn=containChar(bgrndVec[bgNum],'<');
                    if(numOfOut || numOfIn){//If there are call the handler
                        redirHandler(numOfOut,numOfIn,bgrndVec[bgNum],envp,pathVec,numOfbgs);
                        numOfbgs--;
                    }else{//If no redirections
                        pipeVec=mytoc(bgrndVec[bgNum],'|');//Tokenize for pipes
                        //Count the number of pipes
                        numOfPipes=0;
                        for(int pipeNum=1;pipeVec[pipeNum];pipeNum++){
                            numOfPipes++;
                        }
                        if (numOfPipes){//If there are pipes
                            pipesHandler(numOfPipes,pipeVec,envp,pathVec,numOfbgs);
                        }else{//If no pipes just try to run the command
                            normalHandler(bgrndVec[bgNum],envp,pathVec,numOfbgs);
                        }
                        free(pipeVec);
                    numOfbgs--;
                }
            }
            free(bgrndVec);
        }
        free(newLineVec);
        clean(buf);//Clean buf so that there are no residual chars.
    }
}
