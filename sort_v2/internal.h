#pragma once

typedef struct sort_event_t sort_event_t;

void generate_random_array(int seed, int* array, int size, int value_max);
void swap_values(int* a, int* b, int count);
void apply_sort_event(int* array, sort_event_t* event);

