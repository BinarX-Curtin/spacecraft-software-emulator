
cc_library(
  name = "emulator",
  srcs = ["//emulator/src:emulator.cc"],
  hdrs = ["emulator.h"],
  visibility = ["//visibility:public"], 
  deps = ["//abstraction_layer:serial_communication_interface", 
          "//abstraction_layer/gpio/public:gpo_interface",
          "//abstraction_layer:time_interface",
          "//emulator_definitions:emulator_definitions"],
)
