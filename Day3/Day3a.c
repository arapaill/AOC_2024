#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>

/*
    Example data: xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))

    The program must search for mul(nb,nb) in the lines, and multiply the two numbers
    then return the sum of all the results
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

int extract_number(const char *line, regmatch_t *match) {
    int len = match->rm_eo - match->rm_so;
    char num_str[20];

    strncpy(num_str, line + match->rm_so, len);
    num_str[len] = '\0';
    return atoi(num_str);
}

int extract_mul_numbers(const char *line, const regex_t *regex, regmatch_t *matches, int *n1, int *n2) {
    if (regexec(regex, line, 3, matches, 0) == 0) {
        *n1 = extract_number(line, &matches[1]);
        *n2 = extract_number(line, &matches[2]);
        return 1; 
    }
    return 0;
}

int parse_line(const char *line) {
    regex_t regex;
    regmatch_t matches[4];
    const char *pattern = "mul\\(([0-9]+),([0-9]+)\\)";
    int total = 0;
    const char *search_start = line;
    int n1, n2;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        return -1;
    }

    while (extract_mul_numbers(search_start, &regex, matches, &n1, &n2)) {
        total += n1 * n2;
        search_start += matches[0].rm_eo;
    }

    regfree(&regex);
    return total;
}

int main() {
    FILE *fd = fopen("input.txt", "r");
    char *line;
    int sum = 0;

    if (fd == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 1;
    }

    while ((line = get_next_line(fd)) != NULL) {
        if (line[0] != '\n') {
            int line_sum = parse_line(line); // Get the sum of this line
            if (line_sum == -1) {
                fprintf(stderr, "Failed to parse line\n");
                fclose(fd);
                return 1;
            }
            sum += line_sum; // Accumulate the line sum into the total sum
        }
    }

    printf("Sum: %d\n", sum);

    fclose(fd);
    return 0;
}
