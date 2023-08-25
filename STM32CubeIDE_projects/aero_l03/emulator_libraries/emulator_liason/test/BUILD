
cc_test(
  name = "emulator_liason_test",
  srcs = ["emulator_liason_test.cc"],
  deps = ["//emulator_liason/public:emulator_liason",
          "//external_libraries:arduino_json", 
          "@com_google_googletest//:gtest_main",
          "//emulator_definitions:emulator_definitions",
          "//abstraction_layer:serial_communication_interface", 
          "//abstraction_layer/gpio/public:gpo_interface",
          "//abstraction_layer/gpio/public:gpi_interface",],
)

