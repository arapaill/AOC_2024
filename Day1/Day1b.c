#include <stdio.h>
#include <stdlib.h>

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

void get_lists(FILE *fd, int **list1, int **list2, size_t *size) {
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

int get_score(int *list1, int *list2, size_t size) {
    int score = 0;
    for (size_t i = 0; i < size; i++) {
        score += list1[i] * list2[i];
    }
    return score;
}


int get_similarity(int *list1, int *list2, size_t size) {
    int similarity_score = 0;
    size_t i = 0, j = 0;
    int *similarity_list = malloc(size * sizeof(int)); // for each number in list1 its occurence in list2

    if (similarity_list == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    while (i < size) {
        j = 0;
        similarity_score = 0;
        while (j < size) {
            if (list1[i] == list2[j])
                similarity_score++;
            j++;
        }
        similarity_list[i] = similarity_score;
        i++;
    }
    return get_score(list1, similarity_list, size);
}

int main() {
    int similarity_score = 0;
    int *list1, *list2;
    size_t size;
    FILE *fd = fopen("inputDay1a.txt", "r");

    if (fd == NULL) {
        fprintf(stderr, "File not found\n");
        return 1;
    }

    get_lists(fd, &list1, &list2, &size);
    fclose(fd);

    similarity_score = get_similarity(list1, list2, size);
    printf("Similarity score: %d\n", similarity_score);
    
    free(list1);
    free(list2);

    return 0;
}