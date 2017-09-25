#include <stdlib.h>/* For size_t*/

//Function that clears a string.
void clean(char *str) {
    int i = 0;
    while(str[i] != '\0') {
        str[i] = '\0';
        i++;
    }
}

//Function that calculates the length of a string.
int strLength(char *inStr){
  char *pStr;
  size_t len;
  for (pStr = inStr; *pStr; pStr++);
  len = pStr - inStr;
  return len;
}

//Function that returns true if the given word is the word exit and only the word exit.
int timeToExit(char *var){
    int result=1;
    char *word="exit";
    if (strLength(var)!=5){
        result=0;
    }else{
        for(int i=0;i<4;i++){
            if(var[i]!=word[i]){
                result=0;
            }
        }
    }
    return result;
}

//Function that returns true if the first parameter string starts with the second parameter word.
int startsWith(char *origStr, char *word){
    int result=1;
    int length=strLength(word);
    
    for(int i=0;i<length;i++){
        if(origStr[i] != word[i]){
            result=0;
        }
    }
    return result;
}

//Function that removes the first given number of characters from the string.
void remvChrsfromLeft(int num, char *str){
    int leng = strLength(str);
    if(num>leng){
        return 0;
    }
    int i=0;
    for (i=0;i<leng-num;i++){
        str[i]=str[i+num];
    }
    while(str[i] != '\0') {
        str[i] = '\0';
        i++;
    }
}

//Function that concatenates two strings placing a '/' character betwen them.
char *strConcat(char *first, char *second){
    int lenFir = strLength(first);
    int lenSec = strLength(second);
    char *pStr, *final, *pFinal;

    final=pFinal=(char *)malloc(lenFir+lenSec);
    for(pStr = first;*pStr;pStr++)
        *(pFinal++) = *pStr;
    *(pFinal++) = '/';
    for(pStr = second;*pStr;pStr++)
        *(pFinal++) = *pStr;
    *pFinal=0;
    return final;
}
