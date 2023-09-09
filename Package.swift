// swift-tools-version: 5.7
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "AppBootstrapKit",
    products: [
        .library(name: "AppBootstrapKit", targets: [
            "AppBootstrapKit"
        ])
    ],
    targets: [
        .target(name: "CAppBootstrap",
                cSettings: [
                    .define("SDL_VIDEO_DRIVER_WAYLAND"),
                    .define("SDL_VIDEO_DRIVER_VIVANTE"),
                    .define("IMGUI_IMPL_OPENGL_ES2"),
                ]
                ,linkerSettings: [
                    .linkedLibrary("EGL"),
                    .linkedLibrary("SDL2"),
                    .linkedLibrary("GLESv2"),
                ]),
        .target(name: "AppBootstrapKit",
                dependencies: [
                    .target(name: "CAppBootstrap")
                ])
    ]
)
