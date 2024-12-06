#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

const char *pattern = "mul\\(([0-9]+),([0-9]+)\\)";


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

void update_enabled(char *line, int *is_enabled) {
    char *do_pos, *dont_pos;

    do_pos = strstr(line, "do()");
    dont_pos = strstr(line, "don't()");

    if (do_pos != NULL && (dont_pos == NULL || do_pos > dont_pos))
        *is_enabled = 1;
    else if (dont_pos != NULL && (do_pos == NULL || dont_pos > do_pos))
        *is_enabled = 0;
}

// Function to extract numbers from a valid mul(...) substring
void extract_numbers(const char *substring, int *n1, int *n2) {
    regex_t regex;
    regmatch_t matches[3];
    char num1[20], num2[20];
    int len1, len2;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        exit(1);
    }

    if (regexec(&regex, substring, 3, matches, 0) == 0) {
        len1 = matches[1].rm_eo - matches[1].rm_so;
        len2 = matches[2].rm_eo - matches[2].rm_so;

        strncpy(num1, substring + matches[1].rm_so, len1);
        strncpy(num2, substring + matches[2].rm_so, len2);
        num1[len1] = '\0';
        num2[len2] = '\0';

        *n1 = atoi(num1);
        *n2 = atoi(num2);
    }

    regfree(&regex);
}

// Function to process the line and calculate the sum of valid multiplications
void process_line(const char *line, int *total_sum, int *is_enabled) {
    regex_t regex;
    regmatch_t matches[1];
    const char *start = line;
    const char *next_start;
    char *substring;
    int len, n1, n2, product;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        return;
    }

    while (*start) {
        if (regexec(&regex, start, 1, matches, 0) != 0)
            break;

        next_start = start + matches[0].rm_eo;
        len = next_start - start;

        substring = (char *)malloc(len + 1);
        if (substring == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        strncpy(substring, start, len);
        substring[len] = '\0';

        update_enabled(substring, is_enabled);

        if (*is_enabled) {
            extract_numbers(substring, &n1, &n2);
            product = n1 * n2;
            *total_sum += product;
        }
        free(substring);
        start = next_start;
    }

    regfree(&regex);
}

int main() {
    FILE *fd;
    char *line;
    int total_sum = 0;
    int is_enabled = 1;

    fd = fopen("input.txt", "r");
    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }

    while ((line = get_next_line(fd)) != NULL) {
        process_line(line, &total_sum, &is_enabled);
        free(line);
    }

    printf("Total sum: %d\n", total_sum);

    fclose(fd);
    return 0;
}
