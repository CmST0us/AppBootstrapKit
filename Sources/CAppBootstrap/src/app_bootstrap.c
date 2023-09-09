//
// Created by Eric Wu on 2023/9/9.
//

#include <libappbootstrap/app_bootstrap.h>
#include <string.h>

#include "imgui/imgui_bootstrap.h"
#include "sdl2/sdl2_bootstrap.h"
#include "pipeline/sdl2_imgui/sdl2_imgui_pipeline.h"

static int register_backend(const app_bootstrap_context_t *context) {
    app_bootstrap_sdl2_backend_register(context);
    app_bootstrap_imgui_backend_register(context);
    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static int register_pipeline(const app_bootstrap_context_t *context) {
    app_bootstrap_sdl2_imgui_pipeline_register(context);
    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

void app_bootstrap_register_backend(
        const app_bootstrap_context_t *context,
        const app_bootstrap_backend_t *backend) {
    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    app_bootstrap_backend_type_t type = backend->type;

    mutable_context->backend[type] = (void*)backend;
}

void app_bootstrap_register_pipeline(
        const app_bootstrap_context_t *context,
        const app_bootstrap_pipeline_t *pipeline) {
    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    app_bootstrap_pipeline_type_t type = pipeline->type;

    mutable_context->pipeline[type] = (void*)pipeline;
}

int app_bootstrap_setup(
        app_bootstrap_context_t *context,
        const app_bootstrap_configuration_t *configuration,
        const app_bootstrap_handler_t *handler) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    memcpy(&context->configuration, configuration, sizeof(app_bootstrap_configuration_t));
    context->handler.did_exit = handler->did_exit;
    context->handler.main_loop_update = handler->main_loop_update;
    context->handler.should_exit = handler->should_exit;

    register_backend(context);
    register_pipeline(context);

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

int app_bootstrap_run(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    app_bootstrap_pipeline_type_t pipeline_type = context->configuration.pipeline;
    app_bootstrap_pipeline_t * pipeline = (app_bootstrap_pipeline_t *)context->pipeline[pipeline_type];
    pipeline->setup(context);
    pipeline->run(context);
    pipeline->dipose(context);
    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}
