#include <stdlib.h>/* For malloc/calloc */
#include "utils.h"/* For strLength */

/*Tokenizer function that separates a given string using a given delimiter, stores the 
tokens in a vector and returns it */
 char ** mytoc(char *str, char delim){
    int nTokens,nChars,tokNum;
    nTokens=nChars=tokNum=0;
    char *pStr,*pStrLimit;
    char lastChar=delim, currChar;
    int len=strLength(str)+1;// Find the length of the given string
    
    //Count the number of tokens so we can allocate necesary memory
    pStrLimit = str + len;
    for (pStr = str; pStr != pStrLimit; pStr++) {
        currChar=*pStr;
        if(currChar == delim || currChar == '\n' || currChar == '\0'){
            if(lastChar != delim){//Only count non empty strings as tokens
                nTokens++;
            }
        }
        lastChar=currChar;
    }
    char **tokenVec = (char **)calloc(nTokens+1, sizeof(char *));
    
    //Count the number of characters in each token and allocate the necesary memory
    for (pStr = str; pStr != pStrLimit; pStr++) {
        currChar=*pStr;
        if(currChar != delim && currChar != '\n' && currChar != '\0'){
          nChars++;
        }else{
            if(nChars != 0 ){//Again we only count non empty strings as tokens
                tokenVec[tokNum] = (char *)malloc(nChars+1);
                tokNum++;
                nChars=0;
            }
        }
    }
    
    //Enter the tokens into the token vector
    tokNum=0;
    for (pStr = str; pStr != pStrLimit; pStr++) {
        currChar=*pStr;
        if(currChar != delim && currChar != '\n' && currChar != '\0'){
          tokenVec[tokNum][nChars]=currChar;
          nChars++;
        }else{
            if(nChars != 0 ){//Terminate each token with a 0 character
                tokenVec[tokNum][nChars+1]=0;
                tokNum++;
                nChars=0;
            }
        }
    }
    
    return tokenVec;
}
