#include <unistd.h>/*For read, write, execve and fork*/
#include <stdio.h>/*For printf*/
#include <stdlib.h>/*For free*/
#include <sys/wait.h>/*For wait*/
#include "mytoc.h"
#include "utils.h"

#define BUFLEN 1024

int main(int argc, char**argv, char**envp){
    char buf[BUFLEN];
    char **commandVec;
    char **pathVec;
    int retVal;
    int readRet;
    int stat;
    //Find the $PATH environmental variable and tokenize it
    for (int i=0; envp[i] != (char*)0; i++){
        if(startsWith(envp[i],"PATH")){
            pathVec=mytoc(envp[i],':');
            remvChrsfromLeft(5,pathVec[0]);//Used to remove "PATH=" fomr the first token
            break;
        }
    }
    
    for(;;){
        write(2,"$ ",2);
        readRet=read(0,buf,BUFLEN);
        if(timeToExit(buf)||!readRet){//exit the program if the users enters the word exit or reached EOF.
            return 0;
        }
        commandVec= mytoc(buf,' ');//tokenize the user input
        if(commandVec[0]){
            int rc =fork();
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
         }
        clean(buf);//Clean buf so that there are no residual chars.
        free(commandVec);
    }
}
