#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    FILE * result;
    char cmdstr[1024] = {0};
    char outresult[40960] = {0};
    char *p;

    if(argc < 2){
        printf("parme error\n");
        return 1;
    }

    snprintf(cmdstr, 1023, "dpkg -L %s", argv[1]);
    result = popen(cmdstr, "r");

    fread(outresult, sizeof(char), sizeof(outresult), result);

    p = strtok(outresult, "\n");

    while(p){
        if(access(p, F_OK) == -1){
            printf("%s\n", p);
        }
        p = strtok(NULL, "\n");
    }

    pclose(result);
    return 0;
}
