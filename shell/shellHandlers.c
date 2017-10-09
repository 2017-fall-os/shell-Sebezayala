#include <unistd.h>/*For read, write, execve and fork*/
#include <stdio.h>/*For printf*/
#include <stdlib.h>/*For free*/
#include <sys/wait.h>/*For wait*/
#include <sys/types.h>/*For open and close*/
#include <sys/stat.h>/*For open and close*/
#include <fcntl.h>/*For open and close*/
#include "mytoc.h"
#include "utils.h"
#include "shellFunctions.h"

void pipesHandler(int numOfPipes,char **pipeVec,char **envp,char **pathVec, int isBground){
    int retVal, wstatus;
    int pids[numOfPipes];
    int *pipeFds;
    char **commandVec;
    
    //Create an array of size 2*numOfPipes so that we hav input and output for each pipe
    pipeFds= (int *) calloc(2*numOfPipes, sizeof(int));
    for(int i=0; i<numOfPipes; i++){
        pipe(pipeFds+i*2);//initialize the pipes
    }
    int pipeNum=0;
    while(pipeVec[pipeNum]){
        pids[pipeNum] =fork();
        if (pids[pipeNum] < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (pids[pipeNum] == 0) {
            //close and dup input for all commands except the first one
            if(pipeNum!=0){
                dup2(pipeFds[(pipeNum-1)*2],0);
            } 
            //close and dup output for all commands except the last one
            if(pipeVec[pipeNum+1]){
                dup2(pipeFds[(pipeNum*2)+1],1);
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
            retVal=execute(commandVec,envp,pathVec);
            exit(retVal);
        }
        pipeNum++;
    }
    //Close all pipes from parent
    for(int i=0; i<numOfPipes*2;i++){
                close(pipeFds[i]);
    }
    if(!isBground){//Do not wait for background processess
        //Wait for all the children
        for(int i=0; i<numOfPipes+1;i++){
                    waitpid(pids[i],&wstatus,0);
                    if(!WIFEXITED(wstatus)){
                        fprintf(stderr,"Program exited abnormally\n");
                    }
        }
    }
    free(commandVec);
}

void normalHandler(char *buf, char**envp,char**pathVec,int isBground){
    char **commandVec;
    int retVal,wstatus,pid;
    commandVec= mytoc(buf,' ');//tokenize the user input
    if(commandVec[0]){
        /***Change Directory Handler***/
        //Check if the command is cd
        if(strLength(commandVec[0])==2 && startsWith(commandVec[0],"cd")){
            if(!commandVec[1]){//If no dir specified change dir to home
                char *home=getVar(envp,"HOME");
                int ret=chdir(home);
                    if(ret==-1){
                        fprintf(stderr,"No such file or directory\n");
                    }
                    return;
            }else{//Else try to change dir to the specified directory
                    int ret=chdir(commandVec [1]);
                    if(ret==-1){
                        fprintf(stderr,"No such file or directory\n");
                    }
                    return;
            }
        }
        pid =fork();
        if (pid < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (pid == 0) {
            retVal=execute(commandVec,envp,pathVec);//executes the command
            exit(retVal);
        }else {
            if(!isBground){//Do not wiat for background processess
                waitpid(pid,&wstatus,0);
                if(!WIFEXITED(wstatus)){
                    fprintf(stderr,"Program exited abnormally\n");
                }
            }
        }
        free(commandVec);
    }
}

void redirHandler(int numOfOut,int numOfIn,char*line, char**envp,char**pathVec,int numOfbgs){
    char **redirVec,**redirVec2,**pipeVec,**inOutVec;
    char *command;
    int wstatus,in,out;
    //This shell does not handle multiple redirects
    if(numOfOut>1 || numOfIn>1){
    printf("Sorry this shell does not accept multiple redirects\n");
        return;
    }
    int pid =fork();
    if (pid < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (pid == 0) {
        if(numOfIn && !numOfOut){//If we only have redirected input
            redirVec=mytoc(line,'<');//redirVec[0] should be the command and redirVec[0] the filename
            inOutVec=mytoc(redirVec[1],' ');//Get rid of any spaces in the file name
            if((in=open(inOutVec[0],O_RDONLY,0))<0){
                fprintf(stderr,"Couldn't open input file\n");
                exit(0);
            }
            dup2(in,0);
            close(in);
            command=redirVec[0];
        }else if(!numOfIn && numOfOut){//If we only have redirected output
            redirVec=mytoc(line,'>');//redirVec[0] should be the command and redirVec[0] the filename
            inOutVec=mytoc(redirVec[1],' ');//Get rid of any spaces in the file name
            if((out=open(inOutVec[0],O_RDWR|O_CREAT,S_IRWXU))<0){
                fprintf(stderr,"Couldn't open input file\n");
                exit(0);
            }
            dup2(out,1);
            close(out);
            command=redirVec[0];
        }else{//If we have both input and output redirection
            redirVec=mytoc(line,'<');
            if(containChar(redirVec[0],'>')){//If we have output redirection first
                inOutVec=mytoc(redirVec[1],' ');//Get rid of any spaces in the input filename
                if((in=open(inOutVec[0],O_RDONLY,0))<0){
                    fprintf(stderr,"Couldn't open input file\n");
                    exit(0);
                }
                dup2(in,0);
                close(in);
                redirVec2=mytoc(redirVec[0],'>');//The first token should countain the command and input filename
                free(inOutVec);
                inOutVec=mytoc(redirVec2[1],' ');//Get rid of any spaces in the output filename
                if((out=open(inOutVec[0],O_RDWR|O_CREAT,S_IRWXU))<0){
                    fprintf(stderr,"Couldn't open input file\n");
                    exit(0);
                }
                dup2(out,1);
                close(out);
                command=redirVec2[0];
            }else{//If we hav input redirection first
                command=redirVec[0];
                redirVec2=mytoc(redirVec[1],'>');//The second token should contain the output and input filenames
                inOutVec=mytoc(redirVec2[0],' ');//Get rid of the spaces in the input filename
                if((in=open(inOutVec[0],O_RDONLY,0))<0){
                    fprintf(stderr,"Couldn't open input file\n");
                    exit(0);
                }
                dup2(in,0);
                close(in);
                free(inOutVec);
                inOutVec=mytoc(redirVec2[1],' ');//Get rid of the spaces in the output filename
                if((out=open(inOutVec[0],O_RDWR|O_CREAT,S_IRWXU))<0){
                    fprintf(stderr,"Couldn't open input file\n");
                    exit(0);
                }
                dup2(out,1);
                close(out);
            }
        }
        //Once we have the command and have changed the input and output acordingly
        //we proceed normaly
        pipeVec=mytoc(command,'|');//Tokenize for pipes
        //Count the number of pipes
        int numOfPipes=0;
        for(int pipeNum=1;pipeVec[pipeNum];pipeNum++){
                numOfPipes++;
        }
        if (numOfPipes){//If there are pipes
            pipesHandler(numOfPipes,pipeVec,envp,pathVec,numOfbgs);
        }else{//If no pipes just try to run the command
            normalHandler(command,envp,pathVec,numOfbgs);
        }
        free(pipeVec);
        free(redirVec);
        free(inOutVec);
        exit(0);
    }else {
        if(!numOfbgs){//Do not wait for background processess
            waitpid(pid,&wstatus,0);
            if(!WIFEXITED(wstatus)){
                fprintf(stderr,"Program exited abnormally\n");
            }
        }
    }
}
