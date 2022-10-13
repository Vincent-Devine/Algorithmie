
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "sort.h"
#include "replay.h"

#include "internal.h"

void replay_sort(stats_t* stats, const int* ref_array, int ref_size)
{
    if (stats->events_count > 1000)
    {
        printf("Too many events to display in console, try ./sdl_sort instead.\n");
        return;
    }

    int* array = malloc(ref_size * sizeof(int));
    memcpy(array, ref_array, ref_size * sizeof(int));
    for (int i = 0; i < stats->events_count; ++i)
    {
        sort_event_t* event = &stats->events[i];
        switch (event->type)
        {
            case SE_SWAP:
                for (int j = 0; j < event->swap.count; ++j)
                {
                    printf("Swap [%d] %d and [%d] %d\n",
                            event->swap.id_a + j, array[event->swap.id_a + j],
                            event->swap.id_b + j, array[event->swap.id_b + j]);
                }
                break;

            case SE_READ:
                printf("Read [%d] %d\n", event->read.id, array[event->read.id]);
                break;

            default:;
        }
        apply_sort_event(array, event);
    }
    free(array);
}
