#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>


/*
Example of file content
40 42 44 47 49 50 48
65 67 70 71 72 75 75
74 76 78 81 83 85 87 91
73 76 79 81 82 85 86 91
64 67 69 70 68 71 72
76 78 77 79 82 80
12 13 12 13 13
*/
char *get_next_line(FILE *fd) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, fd);
    if (read == -1) {
        free(line);
        return NULL;
    }
    return line;
}


int *get_level(char *line) {
    int *level = NULL;  
    int count = 0;       
    char *token = NULL;

    char *temp = strdup(line);
    token = strtok(temp, " ");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }
    free(temp);

    level = malloc(count * sizeof(int));
    if (level == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int i = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        level[i++] = atoi(token); // Convert token to an integer
        token = strtok(NULL, " ");
    }

    return level; // Return the array
}

/*
The levels are either all increasing or all decreasing.
Any two adjacent levels differ by at least one and at most three.
tolerate a single bad level in what would otherwise be a safe report.
*/


bool is_level_safe(int *level) {
    int i = 0;
    bool tolerance = false;


    while (level[i + 1]) {
        int diff = level[i + 1] - level[i];
        
        // Check if the difference violates the safety rule
        if (abs(diff) < 1 || abs(diff) > 3 || 
            (diff > 0 && level[i + 2] && level[i + 1] > level[i + 2]) ||
            (diff < 0 && level[i + 2] && level[i + 1] < level[i + 2])) {
            
            if (tolerance) // If already tolerated one bad level, fail
                return false;

            tolerance = true;

            // Decide whether to skip the current or next level
            if (level[i + 2] && 
                (abs(level[i + 2] - level[i]) >= 1 &&
                abs(level[i + 2] - level[i]) <= 3)) {
                // Remove the current level[i+1]
                for (int j = i + 1; level[j]; j++) 
                    level[j] = level[j + 1];
            } 
            else {
                for (int j = i; level[j]; j++) 
                    level[j] = level[j + 1];
                i = 0;
                continue;
            }
        }
        i++;
    }

    return true;
}


int main(){
    char *line;
    int *level;
    int safe_levels = 0;
    FILE *fd = fopen("input.txt", "r"); 

    if (fd == NULL) {
        fprintf(stderr, "File not found\n");
        return 1;
    }
    while ((line = get_next_line(fd)) != NULL) {
        if ( line[0] != '\n'){
            level = get_level(line);
            if (is_level_safe(level))
                safe_levels++;
            free(level);
        }   
    }
        
    fclose(fd);
    printf("Safe levels: %d\n", safe_levels);
    return 0;
}