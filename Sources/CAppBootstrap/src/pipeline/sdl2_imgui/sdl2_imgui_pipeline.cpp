//
// Created by Eric Wu on 2023/9/9.
//

extern "C" {
#include "sdl2_imgui_pipeline.h"
}

#include "../../sdl2/sdl2_backend_type.h"
#include "../../imgui/imgui_backend_type.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL2/SDL_opengles2.h>

#define kPIPELINE_TYPE SDL2_IMGUI

static int sdl2_imgui_pipeline_setup(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    app_bootstrap_backend_t *sdl2_context = (app_bootstrap_backend_t *)context->backend[SDL2];
    app_bootstrap_backend_t *imgui_context = (app_bootstrap_backend_t *)context->backend[IMGUI];

    sdl2_context->setup(context);
    imgui_context->setup(context);

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static int sdl2_imgui_pipeline_dispose(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    app_bootstrap_backend_t *sdl2_context = (app_bootstrap_backend_t *)context->backend[SDL2];
    app_bootstrap_backend_t *imgui_context = (app_bootstrap_backend_t *)context->backend[IMGUI];

    imgui_context->dispose(context);
    sdl2_context->dispose(context);

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static int sdl2_imgui_pipeline_run(const app_bootstrap_context_t *context) {
    if (!context) {
        return kAPP_BOOTSTRAP_ERROR_PARAM;
    }

    const app_bootstrap_configuration_t *configuration = &(context->configuration);
    const app_bootstrap_handler_t *handler = &(context->handler);
    app_bootstrap_backend_t *sdl2_backend = (app_bootstrap_backend_t *)context->backend[SDL2];
    app_bootstrap_backend_t *imgui_backend = (app_bootstrap_backend_t *)context->backend[IMGUI];

    sdl2_context_t *sdl2_context = (sdl2_context_t *)context->backend_context[SDL2];
    imgui_context_t *imgui_context = (imgui_context_t *)context->backend_context[IMGUI];
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(sdl2_context->window, sdl2_context->gl_context);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO &io = *((ImGuiIO *)(imgui_context->io));
    ImVec4 clear_color = ImVec4(configuration->clear_color.r,
                                configuration->clear_color.g,
                                configuration->clear_color.b,
                                configuration->clear_color.a);
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(sdl2_context->window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (handler->main_loop_update) {
            int ret = handler->main_loop_update(context);
            if (ret != kAPP_BOOTSTRAP_ERROR_SUCCESS) {
                done = true;
            }
        }

        if (handler->should_exit &&
            handler->should_exit(context)) {
            done = true;
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(sdl2_context->window);
    }

    if (handler->did_exit) {
        handler->did_exit(context);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();

    return kAPP_BOOTSTRAP_ERROR_SUCCESS;
}

static app_bootstrap_pipeline_t sdl2_imgui_backend = {
        .type = kPIPELINE_TYPE,
        .setup = sdl2_imgui_pipeline_setup,
        .dipose = sdl2_imgui_pipeline_dispose,
        .run = sdl2_imgui_pipeline_run
};

void app_bootstrap_sdl2_imgui_pipeline_register(const app_bootstrap_context_t *context) {
    app_bootstrap_register_pipeline(
            context,
            &sdl2_imgui_backend);
}