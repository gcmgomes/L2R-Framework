package(default_visibility = ["//visibility:public"])

cc_library(
  name = "statistics",
  srcs = ["statistics.cc"],
  hdrs = ["statistics.h"],
  deps = ["//base:document",
          "//base:query"],
  linkopts = ["-lm"],
)

cc_library(
  name = "file",
  srcs = ["file.cc"],
  hdrs = ["file.h"],
)

cc_library(
  name = "normalizer",
  srcs = ["normalizer.cc"],
  hdrs = ["normalizer.h"],
  deps = ["//base:document",
          "//base:query",
          ":statistics"],
)

cc_binary(
  name = "normalizer_test",
  srcs = ["normalizer_test.cc"],
  deps = ["//base:dataset",
          ":normalizer"],
)

cc_library(
  name = "discretizer",
  srcs = ["discretizer.cc"],
  hdrs = ["discretizer.h"],
  deps = ["//base:document",
          "//base:query",
          ":tube_tree"],
)
cc_library(
  name = "tube_input",
  srcs = ["tube_input.cc"],
  hdrs = ["tube_input.h"],
  deps = [],
)

cc_binary(
  name = "tube_input_test",
  srcs = ["tube_input_test.cc"],
  deps = [":tube_input"],
)

cc_library(
  name = "tube_node",
  srcs = ["tube_node.cc"],
  hdrs = ["tube_node.h"],
  deps = [":tube_input"],
  linkopts = ["-lm"],
)

cc_binary(
  name = "tube_node_test",
  srcs = ["tube_node_test.cc"],
  deps = [":tube_node"],
)

cc_library(
  name = "tube_tree",
  srcs = ["tube_tree.cc"],
  hdrs = ["tube_tree.h"],
  deps = [":tube_node"],
)

cc_library(
  name = "error_output",
  srcs = ["error_output.cc"],
  hdrs = ["error_output.h"],
  deps = [],
)

cc_binary(
  name = "tube_tree_test",
  srcs = ["tube_tree_test.cc"],
  deps = [":tube_tree"],
)

cc_binary(
  name = "discretizer_test",
  srcs = ["discretizer_test.cc"],
  deps = ["//base:dataset",
          ":discretizer"],
)

cc_binary(
  name = "discretizer_query_test",
  srcs = ["discretizer_query_test.cc"],
  deps = ["//base:dataset",
          ":discretizer"],
)
