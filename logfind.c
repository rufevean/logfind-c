
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

char *rtrim(char *s) {
    char *back = s + strlen(s);
    while (isspace((unsigned char)*(--back)));
    *(back + 1) = '\0';
    return s;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s [-o|-a] pattern [pattern ...]\n", argv[0]);
        return 1;
    }

    int or_flag = 0, and_flag = 0;
    if (strcmp(argv[1], "-o") == 0) {
        or_flag = 1;
    } else if (strcmp(argv[1], "-a") == 0) {
        and_flag = 1;
    } else {
        printf("Error: You must specify either -o (OR) or -a (AND) as the first argument.\n");
        return 1;
    }

    FILE *file = fopen("./.flogs", "r");
    if (file == NULL) {
        perror("Error opening .flogs file");
        return 1;
    }

    char lines[MAX_LINE_LENGTH];
    char files[MAX_LINES][MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(lines, sizeof(lines), file) != NULL && i < MAX_LINES) {
        rtrim(lines);
        strcpy(files[i], lines);
        i++;
    }
    fclose(file);

    for (int j = 0; j < i; j++) {
        FILE *newfile = fopen(files[j], "r");
        if (newfile == NULL) {
            perror("Error opening log file");
            continue;  // Skip this file and continue with the next one
        }

        char content[MAX_LINE_LENGTH];
        int found = 0;
        int line_number = 0;
        while (fgets(content, sizeof(content), newfile) != NULL) {
            line_number++;
            int match = and_flag ? 1 : 0;
            for (int a = 2; a < argc; a++) {  // Start from argv[2] to skip the flag
                if (strstr(content, argv[a]) != NULL) {
                    if (or_flag) {
                        match = 1;
                        break;
                    }
                } else {
                    if (and_flag) {
                        match = 0;
                        break;
                    }
                }
            }
            if (match) {
                found = 1;
                printf("Found in %s on line %d: %s \n", files[j], line_number, content);
                break;
            }
        }
        fclose(newfile);

        if (!found) {
            printf("Not found in %s\n", files[j]);
        }
    }

    return 0;
}
