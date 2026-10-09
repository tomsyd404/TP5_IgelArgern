#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX_ANSWER_LEN 300
// TODO : prevent buffer overflow

#define MAX_HEDGEHOGS 302
#define MAX_LINE 6 // MAX_LINE > 1
#define MAX_ROW 9 // MAX_ROW > 1
#define MAX_SLICE 4
#define MAX_PLAYERS 4
#define NB_HEDGEHOGS 4 // nb of hedgehogs per player >= 3
#define COND_VICTOIRE (NB_HEDGEHOGS-1)

int main(int argc,char **argv){
    for (int iter=0; iter<10000; ++iter){
        printf("Non\n");
        for (int i=0; i<MAX_LINE; ++i){
            for (int j=0; j<MAX_ROW; ++j){
                printf("%c%d\n",'a'+j,i+1);
            }
        }
    }
    return 0;
}
