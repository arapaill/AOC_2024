#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int before;
    int after;
} Rule;

// Function to parse both rules and updates from the file
void parse_rules_and_updates(FILE *file, Rule **rules, int *num_rules, int **update_sizes, int **flat_updates, int *num_updates, int *max_page) {
    char *line = NULL;
    size_t len = 0;
    int flat_index = 0;
    int index = 0;
    char *token = NULL;
    int page;

    while (getline(&line, &len, file) != -1) {
        line[strcspn(line, "\n")] = '\0';

        if (strchr(line, '|')) {
            int before, after;
            sscanf(line, "%d|%d", &before, &after);

            if (before > *max_page) *max_page = before;
            if (after > *max_page) *max_page = after;

            if (before != 0 && after != 0) {
                *rules = realloc(*rules, (*num_rules + 1) * sizeof(Rule));
                (*rules)[*num_rules].before = before;
                (*rules)[*num_rules].after = after;
                (*num_rules)++;
            }
        }
        else if (strchr(line, ',')) {
            *flat_updates = realloc(*flat_updates, (flat_index + 100) * sizeof(int));
            *update_sizes = realloc(*update_sizes, (*num_updates + 1) * sizeof(int));

            index = 0;
            token = strtok(line, ",");
            while (token) {
                (*flat_updates)[flat_index++] = atoi(token);
                page = atoi(token);
                if (page > *max_page) *max_page = page;
                index++;
                token = strtok(NULL, ",");
            }

            (*update_sizes)[*num_updates] = index;
            (*num_updates)++;
        }
        free(line);
        line = NULL;
    }
}

// Function to check if an update is correctly ordered
int is_correctly_ordered(int *flat_updates, int update_size, Rule *rules, int num_rules, int update_start_index, int max_page) {
    int *positions = (int *)calloc(max_page + 1, sizeof(int));
    int index = 0;
    int i = 0;
    int before, after;

    if (positions == NULL) {
        perror("Memory allocation failed");
        return 0;
    }

    while (index < update_size) {
        positions[flat_updates[update_start_index + index]] = index + 1;
        index++;
    }

    while (i < num_rules) {
        before = rules[i].before;
        after = rules[i].after;

        if (positions[before] > 0 && positions[after] > 0) {
            if (positions[before] >= positions[after]) {
                free(positions);
                return 0; // The order is incorrect
            }
        }
        i++;
    }

    free(positions);
    return 1; // The order is correct
}

// Function to reorder the update according to the rules
void reorder_update(int *flat_updates, int update_size, Rule *rules, int num_rules, int update_start_index) {
    int i = 0;
    int before, after;
    int j, k;

    while (i < num_rules) {
        before = rules[i].before;
        after = rules[i].after;

        j = 0;
        while (j < update_size) {
            if (flat_updates[update_start_index + j] == after) {
                k = j + 1;
                while (k < update_size) {
                    if (flat_updates[update_start_index + k] == before) {
                        int temp = flat_updates[update_start_index + j];
                        flat_updates[update_start_index + j] = flat_updates[update_start_index + k];
                        flat_updates[update_start_index + k] = temp;
                        break;
                    }
                    k++;
                }
            }
            j++;
        }
        i++;
    }
}

// Function to calculate the middle sum of page numbers after reordering
int calculate_middle_sum(int *flat_updates, int *update_sizes, int num_updates, Rule *rules, int num_rules, int max_page) {
    int middle_sum = 0;
    int update_start_index = 0;
    int i = 0;
    int update_size;
    int middle_index;

    while (i < num_updates) {
        update_size = update_sizes[i];

        if (!is_correctly_ordered(flat_updates, update_size, rules, num_rules, update_start_index, max_page)) {
        
            // Reorder the update if it's not in the correct order
            while (!is_correctly_ordered(flat_updates, update_size, rules, num_rules, update_start_index, max_page))
                reorder_update(flat_updates, update_size, rules, num_rules, update_start_index);
            
            middle_index = update_size / 2;
            middle_sum += flat_updates[update_start_index + middle_index];

        }

        update_start_index += update_size;
        i++;
    }

    return middle_sum;
}

int main() {
    Rule *rules = NULL;
    int num_rules = 0;
    int *flat_updates = NULL;
    int *update_sizes = NULL;
    int num_updates = 0;
    int max_page = 0;
    int middle_sum;

    FILE *file = fopen("input.txt", "r");
    if (!file) {
        printf("Error: Could not open file.\n");
        return 1;
    }

    parse_rules_and_updates(file, &rules, &num_rules, &update_sizes, &flat_updates, &num_updates, &max_page);

    fclose(file);

    middle_sum = calculate_middle_sum(flat_updates, update_sizes, num_updates, rules, num_rules, max_page);
    printf("\nFinal sum of middle page numbers after reordering: %d\n", middle_sum);

    free(rules);
    free(flat_updates);
    free(update_sizes);

    return 0;
}
