load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "gpo_interface",
    hdrs = [
        "gpo_interface.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "gpi_interface",
    hdrs = [
        "gpi_interface.h",
    ],
    visibility = ["//visibility:public"],
)

# cc_library(
#     name = "bcm_cm4_gpo",
#     srcs = [
#         "//libs/cpp/hal/gpio/src/bcm:bcm_cm4_gpo.cc",
#     ],
#     hdrs = [
#         "bcm_cm4_gpo.h",
#     ],
#     tags = [
#         "cm4",  # Exclude this target from cm7 builds.
#         "manual",
#     ],
#     visibility = ["//visibility:public"],
#     deps = [
#         ":gpo_interface",
#         "//libs/c/v5_primary_hal/CM4:core",
#         "//libs/cpp/hal/gpio/public/internal:gpo",
#     ],
# )

# cc_library(
#     name = "bcm_cm4_gpi",
#     srcs = [
#         "//libs/cpp/hal/gpio/src/bcm:bcm_cm4_gpi.cc",
#     ],
#     hdrs = [
#         "bcm_cm4_gpi.h",
#     ],
#     tags = [
#         "cm4",  # Exclude this target from cm7 builds.
#         "manual",
#     ],
#     visibility = ["//visibility:public"],
#     deps = [
#         ":gpi_interface",
#         "//libs/c/v5_primary_hal/CM4:core",
#         "//libs/cpp/hal/gpio/public/internal:gpi",
#     ],
# )
