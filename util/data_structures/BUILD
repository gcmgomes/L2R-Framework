package(default_visibility = ["//visibility:public"])

cc_library(
  name = "min_max_heap_node",
  srcs = ["min_max_heap_node.h"],
  linkopts = ["-lm"],
)

cc_library(
  name = "min_max_heap",
  srcs = ["min_max_heap.h"],
  deps = [":min_max_heap_node"],
  linkopts = ["-lm"],
)
