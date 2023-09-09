//
// Created by Eric Wu on 2023/9/9.
//

#include "sdl2_bootstrap.h"
#include "sdl2_backend_type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int sdl2_backend_setup(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    const app_bootstrap_configuration_t * const configuration = &context->configuration;

    sdl2_context_t *backend_context = malloc(sizeof(sdl2_context_t));
    memset(backend_context, 0, sizeof(sdl2_context_t));

    mutable_context->backend_context[SDL2] = backend_context;
    backend_context->app_bootstrap_context = context;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("[SDL2Backend] SDL_Init error: %s\n", SDL_GetError());
        return kAPP_BOOTSTRAP_ERROR_BACKEND_SETUP;
    }

    if (configuration->opengl_version == GLES2_GLSL100) {
        // GL ES 2.0 + GLSL 100
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    } else if (configuration->opengl_version == GLES3_GLSL130) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    } else if (configuration->opengl_version == GLES32_GLSL150) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_Window* window = SDL_CreateWindow(configuration->window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    backend_context->window = window;
    backend_context->gl_context = gl_context;

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static int sdl2_backend_dispose(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }
    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    sdl2_context_t *backend_context = mutable_context->backend_context[SDL2];

    if (!backend_context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    SDL_GL_DeleteContext(backend_context->gl_context);
    SDL_DestroyWindow(backend_context->window);
    SDL_Quit();
    free(backend_context);
    mutable_context->backend_context[SDL2] = NULL;

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static app_bootstrap_backend_t sdl2_backend = {
    .type = SDL2,
    .setup = sdl2_backend_setup,
    .dispose = sdl2_backend_dispose,
};

void app_bootstrap_sdl2_backend_register(const app_bootstrap_context_t *context) {
    app_bootstrap_register_backend(
            context,
            &sdl2_backend);
}