#ifndef EXAM_H
#define EXAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(char* a,char* b){
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

void perm(char* str, int index, int size, char** res, int N){
    int i, j;
    if(index == size){
      for(j = 0; j < N; j++){
          if(res[j] == NULL){
              res[j] = (char*)malloc(strlen(str));
              strcpy(res[j], str);
              break;
          }
      }
    }else{
        for(int i = index; i <= size; i++){
            swap(str+i, str+index);
            perm(str, index + 1, size, res, N);
            swap(str+i, str+index);
        }
    }   
}
/* 返回一个以空字符串结尾的char*数组 */
char** permute_string(const char* input)
{
    char ** res = 0;
    int len = strlen(input);
    char inputstr[len];
    int a = 0, b = 0;
    char tmp[len];
    strcpy(inputstr, input);
    
    int N = 1; // size of your result
    if(len > 1){
        for(int i = 1; i <= len; i++){
            N = N * i;
        }
    }
    N = N + 1;
    res = (char**)malloc(sizeof(char*) * N);
    for(a = 0; a < N; a++){
        res[a] = 0;
    }

    perm(inputstr, 0, len-1, res, N);
    
    for(a = 0; a < N-2; ++a){
        for(b = a + 1; b < N-1; ++b){
            if ( strcmp(res[a], res[b]) > 0 ){
                strcpy(tmp, res[a]);
                strcpy(res[a], res[b]);
                strcpy(res[b], tmp);
            }
        }
    }
    return res;
}

#endif // EXAM_H
