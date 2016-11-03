#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

//so apparently you can repeat a thingy, %0*s repeatz de string

char * humanReadableSize(int size);
void printDir(char dirName[]);
void printDirHelper(char dirName[], int nest);

char * humanReadableSize(int size) {
    int ctr = 0;
    int sizeTemp = size;
    char * retStr = malloc(size + 2);
    while (sizeTemp) {
        sizeTemp /= 10;
        ctr++;
    }
    ctr--;
    if (ctr < 3) {
        sprintf(retStr, "%dB", size);
    }
    else if (ctr < 6) {
        sprintf(retStr, "%dKB", size / 1000);
    }
    else if (ctr < 9) {
        sprintf(retStr, "%dMB", size / 1000000);
    }
    else {
        sprintf(retStr, "%dGB", size / 1000000000);
    }
    //i don't know why this works but it does
    free(retStr);
    return retStr;
}

void printDir(char dirName[]) {
    struct stat buffer;
    stat(dirName, &buffer);
    int dirSize = buffer.st_size;
    printf("Directory: %s \n Size: %s \n", dirName, humanReadableSize(dirSize));
    printDirHelper(dirName, 0);
}

void printDirHelper(char dirName[], int nest) {
    DIR * dirStream = opendir(dirName);
    char * tempDirName;
    struct dirent * current;
    //get rid of the first two directories, ./ and ../
    current = readdir(dirStream);
    current = readdir(dirStream);
    while (dirStream) {
        current = readdir(dirStream);
        if ((current -> d_type == DT_DIR) && ((strcmp(current -> d_name, ".")) || strcmp(current -> d_name, ".."))) {
            for (int i = 0 ; i < nest ; i++) {
                printf("\t");
            }
            tempDirName = malloc(strlen(current -> d_name) + 1);
            tempDirName = current -> d_name;
            strcat(tempDirName, "/");
            printf("%s \n", tempDirName);
            free(tempDirName);
        }
        else if (current -> d_type == DT_DIR) {
            for (int i = 0 ; i < nest ; i++) {
                printf("\t");
            }
            // name and null terminator
            tempDirName = malloc(strlen(current -> d_name) + 1);
            tempDirName = current -> d_name;
            strcat(tempDirName, "/");
            printf("%s \n", tempDirName);
            free(tempDirName);
            printDirHelper(current -> d_name, nest++);
        }
        else {
            for (int i = 0 ; i < nest ; i++) {
                printf("\t");
            }
            printf("%s \n", current -> d_name);
        }
    }
    closedir(dirStream);
}

int main() {
    printDir("/Users/eccentricayman/Github/Systems/durStat");
}

