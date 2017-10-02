#include <unistd.h>/*For read, write, execve and fork*/
#include <stdio.h>/*For printf*/
#include <stdlib.h>/*For free*/
#include <sys/wait.h>/*For wait*/
#include "mytoc.h"
#include "utils.h"

void pipesHandler(int numOfPipes,char **pipeVec,char **envp,char **pathVec){
    int rc, retVal, stat;
    int *pipeFds;
    char **commandVec;
    
//     for(int i=0;pipeVec[i];i++){
//         commandVec= mytoc(pipeVec[i],' ');
//         if(!commandVec[0]){
//             fprintf(stderr, "Syntax Error\n");
//             free(commandVec);
//             return;
//         }
//         free(commandVec);
//     }
    
    pipeFds= (int *) calloc(2*numOfPipes, sizeof(int));
    for(int i=0; i<numOfPipes; i++){
        pipe(pipeFds+i*2);
    }
    int pipeNum=0;
    while(pipeVec[pipeNum]){
        rc =fork();
        if (rc < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc == 0) {
            //close and dup input for all commands except the first one
            if(pipeNum!=0){
                close(0);
                dup(pipeFds[(pipeNum-1)*2]);
            } 
            //close and dup output for all commands except the last one
            if(pipeVec[pipeNum+1]){
                close(1);
                dup(pipeFds[(pipeNum*2)+1]);
            }
            //close all child pipes
            for(int i=0; i<numOfPipes*2;i++){
                close(pipeFds[i]);
            }
            if(commandVec){
                free(commandVec);
            }
            //tokenize the command and execute
            commandVec= mytoc(pipeVec[pipeNum],' ');
            
            //If the user specified a directory, just run the command
            if(startsWith(commandVec[0],"/")){
                retVal=execve(commandVec[0],commandVec,envp);
                fprintf(stderr,"Command not found \n");
                //If the user did not specify the directory, try the current directory and those in $PATH    
            }else{
                retVal=execve(commandVec[0],commandVec,envp);
                int i=0;
                while(pathVec[i]){
                    char *command=strConcat(pathVec[i],commandVec[0]);
                    retVal=execve(command,commandVec,envp);
                    i++;
                }
                fprintf(stderr,"Command not found \n");
            }
            exit(retVal);
        }
        pipeNum++;
    }
    //Close all pipes from parent
    for(int i=0; i<numOfPipes*2;i++){
                close(pipeFds[i]);
    }
    //Wait for all the children
    for(int i=0; i<numOfPipes+1;i++){
                wait(&stat);
    }
    free(commandVec);
}

void normalHandler(char *buf, char**envp,char**pathVec){
    char **commandVec;
    int retVal,stat,rc;
    commandVec= mytoc(buf,' ');//tokenize the user input
    //cd handler
    if(strLength(commandVec[0])==2 && startsWith(commandVec[0],"cd")){
                int ret=chdir(commandVec [1]);
                if(ret==-1){
                    printf("No such file or directory\n");
                }
                return;
    }
    rc =fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        //If the user specified a directory, just run the command
        if(startsWith(commandVec[0],"/")){
            retVal=execve(commandVec[0],commandVec,envp);
            printf("Command not found \n");
        //If the user did not specify the directory, try the current directory and those in $PATH    
        }else{
            retVal=execve(commandVec[0],commandVec,envp);
            int i=0;
            while(pathVec[i]){
                char *command=strConcat(pathVec[i],commandVec[0]);
                retVal=execve(command,commandVec,envp);
                i++;
            }
            printf("Command not found \n");
        }
        exit(retVal);
    }else {
        wait(&stat);
        if(!WIFEXITED(stat)){
            printf("Program exited abnormally\n");
        }
    }
    free(commandVec);
}
    
void bgroundHandler(char**bgrndVec,char**envp,char**pathVec){
    int rc;
    char **pipeVec;
    int bgNum=0;
    //for every backround create a child and run a command
    while(bgrndVec[bgNum]){
        rc =fork();
        if (rc < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        }else if(rc == 0){
            //check for pipes
            pipeVec=mytoc(bgrndVec[bgNum],'|');
            int numOfPipes=0;
            for(int i=1;pipeVec[i];i++){
                numOfPipes++;
            }
            if (numOfPipes){
                pipesHandler(numOfPipes,pipeVec,envp,pathVec);
            }else{
                normalHandler(pipeVec[0],envp,pathVec);
            }
            free(pipeVec);
            exit(0);
        }
        bgNum++;
    }
}
