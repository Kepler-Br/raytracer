#include "render_mask.h"
#include <math.h>
#include <SDL2/SDL.h>

char *create_render_mask(const size_t size, const int pobability_black)
{
    char *mask;
    size_t index;

    SDL_assert((mask = malloc(sizeof(char)*size)) != NULL);
    index = 0;
    while(index < size)
    {
        if(rand()%100 + 1 < pobability_black)
            mask[index] = 0;
        else
            mask[index] = 1;
        index++;
    }
    return (mask);
}

