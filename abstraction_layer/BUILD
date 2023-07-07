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
  name = "serial_impl",
  srcs = ["serial_impl.cc"],
  hdrs = ["serial_impl.h"],
  visibility = ["//visibility:public"], 
  deps = [":serial_communication_interface"],
)

cc_library(
  name = "gpio_impl",
  srcs = ["gpio_impl.cc"],
  hdrs = ["gpio_impl.h"],
  visibility = ["//visibility:public"], 
  deps = [":gpio_interface"],
)

