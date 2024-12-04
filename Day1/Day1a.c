#include <stdio.h>
#include <stdlib.h>

// Example of file content
/*
88276   66757
66898   13714
58877   87487
69396   12997
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

void get_lists(FILE *fd, int **list1, int **list2, int *size) {
    char *line;
    int capacity = 10;  // Initial capacity
    *size = 0;

    *list1 = malloc(capacity * sizeof(int));
    *list2 = malloc(capacity * sizeof(int));

    if (*list1 == NULL || *list2 == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    while ((line = get_next_line(fd)) != NULL) {
        if (*size >= capacity) {
            capacity *= 2;
            *list1 = realloc(*list1, capacity * sizeof(int));
            *list2 = realloc(*list2, capacity * sizeof(int));

            if (*list1 == NULL || *list2 == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(1);
            }
        }
        sscanf(line, "%d %d", &(*list1)[*size], &(*list2)[*size]);
        (*size)++;
        free(line);
    }
}

void sort_list(int *list, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (list[j] > list[j + 1]) {
                int tmp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = tmp;
            }
        }
    }
}

int get_distance(int *list1, int *list2, int size) {
    int distance = 0;

    for (int i = 0; i < size; i++) {
        distance += abs(list1[i] - list2[i]);
    }
    return distance;
}

int main() {
    FILE *fd;
    int *list1 = NULL;
    int *list2 = NULL;
    int size = 0;

    fd = fopen("inputDay1a.txt", "r");
    if (fd == NULL) {
        printf("Error: File not found\n");
        return 1;
    }

    get_lists(fd, &list1, &list2, &size);
    fclose(fd);

    sort_list(list1, size);
    sort_list(list2, size);

    printf("Distance: %d\n", get_distance(list1, list2, size));

    free(list1);
    free(list2);
    return 0;
}
