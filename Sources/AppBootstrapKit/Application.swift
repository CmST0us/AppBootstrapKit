//
// Created by Eric Wu on 2023/9/9.
//

import Foundation
import CAppBootstrap

open class Application {
    public enum OpenGLVersion: Int {
        case gles2_glsl100
        case gles32_glsl150
        case gles3_glsl130

        var c_enum: app_bootstrap_configuration_opengl_version_t {
            return app_bootstrap_configuration_opengl_version_t(rawValue: UInt32(self.rawValue))
        }
    }

    public enum BackendType: Int {
        case sdl2
        case imgui

        var c_enum: app_bootstrap_backend_type_t {
            return app_bootstrap_backend_type_t(rawValue: UInt32(self.rawValue))
        }
    }

    public enum PipelineType: Int {
        case sdl2_imgui

        var c_enum: app_bootstrap_pipeline_type_t {
            return app_bootstrap_pipeline_type_t(rawValue: UInt32(self.rawValue))
        }
    }

    public struct Configuration {
        public let pipeline: PipelineType
        public let openglVersion: OpenGLVersion
        public let windowName: String
        public let clearColor: (Float, Float, Float, Float)

        public init(pipeline: PipelineType,
                    openglVersion: OpenGLVersion,
                    windowName: String,
                    clearColor: (Float, Float, Float, Float)) {
            self.pipeline = pipeline
            self.openglVersion = openglVersion
            self.windowName = windowName
            self.clearColor = clearColor
        }
    }

    public static var instance: Application!

    private var shouldExit: Bool = false

    private var context: app_bootstrap_context_t

    private var configuration: app_bootstrap_configuration_t

    private var handler: app_bootstrap_handler_t

    public init(configuration: Configuration) {
        self.context = app_bootstrap_context_t()
        self.handler = app_bootstrap_handler_t()
        self.configuration = app_bootstrap_configuration_t()

        Application.instance = self
        let windowNamePtr = configuration.windowName.withCString{$0}

        self.configuration.pipeline = configuration.pipeline.c_enum
        self.configuration.opengl_version = configuration.openglVersion.c_enum
        self.configuration.window_name = windowNamePtr
        self.configuration.clear_color = .init(
                r: configuration.clearColor.0, g: configuration.clearColor.1,
                b: configuration.clearColor.2, a: configuration.clearColor.3)

        self.handler.should_exit = { context in
            return Application.instance.shouldExit
        }

        self.handler.did_exit = { context in
            Application.instance.didExit()
            return kAPP_BOOTSTRAP_ERROR_SUCCESS
        }

        self.handler.main_loop_update = { context in
            Application.instance.update()
            return kAPP_BOOTSTRAP_ERROR_SUCCESS
        }
    }

    public func run() {
        app_bootstrap_setup(&self.context,
                &self.configuration,
                &self.handler)

        setup()

        app_bootstrap_run(&self.context)
    }

    public func setExit() {
        shouldExit = true
    }

    open func didExit() {

    }

    open func setup() {

    }

    open func update() {

    }
}
