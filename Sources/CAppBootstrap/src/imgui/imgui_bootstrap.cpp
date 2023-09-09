//
// Created by Eric Wu on 2023/9/9.
//

extern "C" {
#include <stdlib.h>
#include "imgui_bootstrap.h"
}
#include "imgui_backend_type.h"
#include "imgui.h"

static int imgui_backend_setup(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    const app_bootstrap_configuration_t * const configuration = &context->configuration;

    imgui_context_t *backend_context = (imgui_context_t *)malloc(sizeof(imgui_context_t));
    memset(backend_context, 0, sizeof(imgui_context_t));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    backend_context->io = (void *)&io;
    mutable_context->backend_context[IMGUI] = backend_context;
    backend_context->app_bootstrap_context = context;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static int imgui_backend_dispose(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }
    app_bootstrap_context_t *mutable_context = (app_bootstrap_context_t *)context;
    imgui_context_t *backend_context = (imgui_context_t *)mutable_context->backend_context[IMGUI];

    if (!backend_context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    ImGui::DestroyContext();
    free(backend_context);
    mutable_context->backend_context[IMGUI] = nullptr;

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static app_bootstrap_backend_t imgui_backend = {
        .type = IMGUI,
        .setup = imgui_backend_setup,
        .dispose = imgui_backend_dispose,
};

void app_bootstrap_imgui_backend_register(const app_bootstrap_context_t *context) {
    app_bootstrap_register_backend(
            context,
            &imgui_backend);
}