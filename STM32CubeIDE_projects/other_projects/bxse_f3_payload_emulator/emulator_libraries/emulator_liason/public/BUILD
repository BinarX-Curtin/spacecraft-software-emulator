
cc_library(
  name = "emulator_liason",
  srcs = ["//emulator_liason/src:emulator_liason.cc"],
  hdrs = ["emulator_liason.h"],
  visibility = ["//visibility:public"], 
  deps = ["//abstraction_layer:serial_communication_interface", 
          "//abstraction_layer/gpio/public:gpo_interface",
          "//abstraction_layer/gpio/public:gpi_interface",
          "//emulator_definitions:emulator_definitions"],
)
