#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char *ltrim(char *s);
#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

int main(int argc, char** argv[]){
    FILE *file;
    file  = fopen("./.flogs", "r");
    
    if (file == NULL){
        printf("Error: Could not open file\n");
        return 1;
    }
    char lines[MAX_LINES];
    char files[MAX_LINES][MAX_LINE_LENGTH];
    int i = 0 ;
    while(fgets(lines, MAX_LINES, file) != NULL){
        strcpy(files[i], lines);
        i++;
    }
    
    for (int j = 0; j < i; j++){
        printf("File %d: %s", j, files[j]);
        FILE *newfile = fopen( files[j], "r");
        if (newfile == NULL){
            printf("Error: Could not open file\n");
            return 1;
        }
        char content[MAX_LINES];
        while(fgets(content, MAX_LINES, newfile) != NULL){
            printf("%s", content);
        }
        fclose(newfile);
    }
    fclose(file);
}


char *ltrim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}
