//
// Created by Eric Wu on 2023/9/9.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <libappbootstrap/app_bootstrap.h>
#include <SDL2/SDL.h>
#ifdef __cplusplus
}
#endif

typedef struct {
    const app_bootstrap_context_t *app_bootstrap_context;
    SDL_Window *window;
    SDL_GLContext gl_context;
} sdl2_context_t;