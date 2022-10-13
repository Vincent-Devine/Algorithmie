
#include <stdbool.h>
#include <limits.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "list.h"
#include "replay.h"
#include "internal.h"

void screenshot(SDL_Renderer* renderer, int index)
{
    char filename[256];
    sprintf(filename, "shots/sort_%04d.png", index);

    SDL_Rect viewport;
    SDL_RenderGetViewport(renderer, &viewport);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, viewport.w, viewport.h, 32, 0, 0, 0, 0);

    // Slow operation...
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);

    // Slow operation...
    IMG_SavePNG(surface, filename);

    SDL_FreeSurface(surface);
}

void replay_sort(stats_t* stats, const int* ref_array, int ref_size)
{
    bool make_screenshots = false;

    int reduce = make_screenshots ? 3 : 1;
    int width  = 1280 / reduce;
    int height =  576 / reduce;

    // Init array copy
    int* array = malloc(ref_size * sizeof(int));
    memcpy(array, ref_array, ref_size * sizeof(int));
    int max_value = INT_MIN;
    for (int i = 0; i < ref_size; ++i)
        if (array[i] > max_value)
            max_value = array[i];

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        exit(1);
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("Sort",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
        exit(1);
    }

    // Create SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | (make_screenshots ? 0 : SDL_RENDERER_PRESENTVSYNC));
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
        exit(1);
    }

    // Calculate bar measurement
    float bar_margin = width / 300;
    float bar_width  = (float)width  / ref_size;
    if (bar_width < bar_margin)
        bar_margin = 0.f;
    float bar_unit_y = (float)height / max_value;

    // Main loop
    int event_index = 0;
    int screenshot_index = 0;
    bool running = true;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x2b, 0x36, 0xff);
        SDL_RenderClear(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        running = false;
                    break;

                default:
                    break;
            }
        }

        // Draw bars
        SDL_SetRenderDrawColor(renderer, 0xd7, 0x5f, 0x00, 0xff);
        for (int i = 0; i < ref_size; ++i)
        {
            SDL_Rect rect;
            rect.x = i * bar_width + bar_margin/2.f;
            rect.y = height - bar_unit_y * array[i];
            rect.w = bar_width - bar_margin/2.f;
            rect.h = bar_unit_y * array[i];

            SDL_RenderFillRect(renderer, &rect);
        }

        // Apply event that will update array
        if (event_index < stats->events_count)
        {
            apply_sort_event(array, &stats->events[event_index]);
            event_index++;
        }

        // Skip all non-swap events
        while (event_index < stats->events_count
                && stats->events[event_index].type == SE_READ)
        {
            apply_sort_event(array, &stats->events[event_index]);
            event_index++;
        }

        SDL_RenderPresent(renderer);

        if (make_screenshots)
            screenshot(renderer, screenshot_index++);
        else
            SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    free(array);
}
