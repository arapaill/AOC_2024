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

int is_within_bounds(int i, int j, int num_lines, int line_size) {
    return (i >= 0 && i < num_lines && j >= 0 && j < line_size);
}

int word_search(char **lines, int num_lines, int line_size, int i, int j) {
    int has_pair1 = 0;
    int has_pair2 = 0;

    if (lines[i][j] != 'A')
        return 0;

    if (!is_within_bounds(i, j, num_lines, line_size))
        return 0;

    // top-left to bottom-right diagonal
    if (is_within_bounds(i - 1, j - 1, num_lines, line_size) &&
        is_within_bounds(i + 1, j + 1, num_lines, line_size)) {
        if ((lines[i - 1][j - 1] == 'M' && lines[i + 1][j + 1] == 'S') ||
            (lines[i - 1][j - 1] == 'S' && lines[i + 1][j + 1] == 'M')) {
            has_pair1 = 1;
        }
    }

    // top-right to bottom-left diagonal
    if (is_within_bounds(i - 1, j + 1, num_lines, line_size) &&
        is_within_bounds(i + 1, j - 1, num_lines, line_size)) {
        if ((lines[i - 1][j + 1] == 'M' && lines[i + 1][j - 1] == 'S') ||
            (lines[i - 1][j + 1] == 'S' && lines[i + 1][j - 1] == 'M')) {
            has_pair2 = 1;
        }
    }

    return has_pair1 && has_pair2;
}


int count_words(char **lines, int num_lines) {
    int count = 0;

    for (int i = 0; i < num_lines; i++) {
        int line_length = strlen(lines[i]);
        for (int j = 0; j < line_length; j++) {
            if (lines[i][j] == 'A') 
                count += word_search(lines, num_lines, line_length, i, j);
        }
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
