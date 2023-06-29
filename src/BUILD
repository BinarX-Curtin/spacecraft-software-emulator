

cc_library(
   name = "serial_communication_interface",
  hdrs = ["serial_communication_interface.h"],
  visibility = ["//visibility:public"],
)

cc_library(
   name = "gpio_interface",
  hdrs = ["gpio_interface.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "emulator",
  srcs = ["emulator.cc"],
  hdrs = ["emulator.h"],
  visibility = ["//visibility:public"], 
  deps = [":serial_communication_interface", 
          ":gpio_interface"],
)


