#include <unistd.h>
#include <stdio.h>
#include "utils.h"
/*For command executions*/
int execute(char**commandVec,char**envp,char**pathVec){
    int retVal;
    //If the user specified a directory, just run the command
    if(startsWith(commandVec[0],"/")){
            retVal=execve(commandVec[0],commandVec,envp);
            fprintf(stderr,"Command not found \n");
        //If the user did not specify the directory, try the current directory and those in $PATH    
    }else{
        retVal=execve(commandVec[0],commandVec,envp);
        int i=0;
        while(pathVec[i]){
            char *command=strConcat(pathVec[i],"/");
            command=strConcat(command,commandVec[0]);
            retVal=execve(command,commandVec,envp);
            i++;
        }
        fprintf(stderr,"Command not found \n");
    }
    return retVal;
}

/*returns pointer to the specified environment variable*/
char *getVar(char **envp, char*name){
    for (int i=0; envp[i]; i++){
        if(startsWith(envp[i],name)){//If Variable found
            return envp[i]+(strLength(name)+1);//Add the length of the name plus 1 to return only the variable value
        }
    }
        return 0;//return 0 if the variable was not found
}

int setVar(char **envp, char *name,char *str){
    for (int i=0; envp[i]; i++){
        if(startsWith(envp[i],name)){//Find the desired variable
            name=strConcat(name,"=");//Setup the new string
            str=strConcat(name,str);
            envp[i]=str;//Replace var value
            return 1;//Returns 1 if succesfull
        }
    }
    return 0;//Returns 0 on failure
}
