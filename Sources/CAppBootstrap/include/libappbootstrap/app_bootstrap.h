//
// Created by Eric Wu on 2023/9/9.
//

#pragma once

#include <stdbool.h>

#define kAPP_BOOTSTRAP_ERROR_SUCCESS (0)
#define kAPP_BOOTSTRAP_ERROR_PARAM (1)
#define kAPP_BOOTSTRAP_ERROR_BACKEND_SETUP (2)

#define kAPP_BOOTSTRAP_ERROR_EXIT_MAIN_LOOP (100)

typedef enum app_bootstrap_backend_type_t {
    SDL2,
    IMGUI,
} app_bootstrap_backend_type_t;

typedef enum app_bootstrap_pipeline_type_t {
    SDL2_IMGUI,
} app_bootstrap_pipeline_type_t;

typedef enum app_bootstrap_configuration_opengl_version_t {
    GLES2_GLSL100,
    GLES32_GLSL150,
    GLES3_GLSL130,
} app_bootstrap_configuration_opengl_version_t;

typedef struct app_bootstrap_configuration_t {
    app_bootstrap_pipeline_type_t pipeline;
    app_bootstrap_configuration_opengl_version_t opengl_version;
    const char *window_name;
    struct {
        float r, g, b, a;
    } clear_color;
} app_bootstrap_configuration_t;

struct app_bootstrap_context_t;

typedef struct app_bootstrap_handler_t {
    int (*main_loop_update)(const struct app_bootstrap_context_t *context);
    bool (*should_exit)(const struct app_bootstrap_context_t *context);
    int (*did_exit)(const struct app_bootstrap_context_t *context);
} app_bootstrap_handler_t;

typedef struct app_bootstrap_context_t {
    app_bootstrap_configuration_t configuration;
    app_bootstrap_handler_t handler;

    void *backend[8];
    void *backend_context[8];

    void *pipeline[8];
    void *pipeline_context[8];
} app_bootstrap_context_t;

typedef struct app_bootstrap_backend_t {
    app_bootstrap_backend_type_t type;
    int (*setup)(const app_bootstrap_context_t *context);
    int (*dispose)(const app_bootstrap_context_t *context);
} app_bootstrap_backend_t;

typedef struct app_bootstrap_pipeline_t {
    app_bootstrap_pipeline_type_t type;
    int (*setup)(const app_bootstrap_context_t *context);
    int (*dipose)(const app_bootstrap_context_t *context);
    int (*run)(const app_bootstrap_context_t *context);
} app_bootstrap_pipeline_t;

void app_bootstrap_register_backend(const app_bootstrap_context_t *context,
                                    const app_bootstrap_backend_t *backend);

void app_bootstrap_register_pipeline(const app_bootstrap_context_t *context,
                                            const app_bootstrap_pipeline_t *pipeline);

int app_bootstrap_setup(
        app_bootstrap_context_t *context,
        const app_bootstrap_configuration_t *configuration,
        const app_bootstrap_handler_t *handler);

int app_bootstrap_run(const app_bootstrap_context_t *context);
