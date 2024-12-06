#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int recursive_word_search(char **lines, int num_lines, int line_size, char letter, int i, int j, int di, int dj) {
    if (i < 0 || i >= num_lines || j < 0 || j >= line_size || lines[i][j] != letter)
        return 0;

    if (letter == 'S')
        return 1;

    char next_letter = (letter == 'X') ? 'M' : (letter == 'M') ? 'A' : 'S';

    return recursive_word_search(lines, num_lines, line_size, next_letter, i + di, j + dj, di, dj);
}

int initiate_search(char **lines, int num_lines, int line_size, int i, int j) {
    int directions[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};
    int count = 0;
    int k = 0;

    while (k < 8){
        count += recursive_word_search(lines, num_lines, line_size, 'X', i, j, directions[k][0], directions[k][1]);
        k++;
    }
    return count;
}


int count_words(char **lines, int num_lines) {
    int count = 0;
    int line_length;
    int i = 0;
    int j;

    while (i < num_lines) {
        line_length = strlen(lines[i]);
        j = 0;
        while (j < line_length) {
            if (lines[i][j] == 'X') 
                count += initiate_search(lines, num_lines, line_length, i, j);
            j++;
        }
        i++;
    }

    return count;
}


int main() {
    FILE *fd;
    char **lines = NULL;
    int num_lines = 0;
    char *line;

    fd = fopen("input.txt", "r");
    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }

    while ((line = get_next_line(fd)) != NULL) {
        char **temp = realloc(lines, (num_lines + 1) * sizeof(char *));
        if (temp == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(line);
            for (int i = 0; i < num_lines; i++) free(lines[i]);
            free(lines);
            fclose(fd);
            return 1;
        }
        lines = temp;
        lines[num_lines++] = line;
    }
    fclose(fd);

    printf("Number of words: %d\n", count_words(lines, num_lines));

    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}
