package(default_visibility = ["//visibility:public"])

cc_library(
  name = "bitset",
  hdrs = ["bitset.h"],
  srcs = ["bitset.cc"],
  linkopts = ["-lm"],
)

cc_library(
  name = "cache",
  hdrs = ["cache.h"],
  srcs = ["cache.cc"],
  deps = [":bitset",
          ":instance"],
)

cc_library(
  name = "inverted_index",
  hdrs = ["inverted_index.h"],
  srcs = ["inverted_index.cc"],
  deps = [":instance"],
)

cc_library(
  name = "external_queue",
  hdrs = ["external_queue.h"],
  srcs = ["external_queue.cc"],
  deps = [":bitset"],
)


cc_library(
  name = "instance",
  hdrs = ["instance.h"],
  srcs = ["instance.cc"],
  deps = ["//base:dataset",
          "//util:discretizer"],
)

cc_library(
  name =  "variable",
  hdrs = ["variable.h"],
  srcs = ["variable.cc"],
  deps = [":cache",
          ":external_queue",
          ":inverted_index"],
)

cc_library(
  name = "graph",
  hdrs = ["graph.h"],
  srcs = ["graph.cc"],
  deps = [":variable"],
)

cc_library(
  name = "branch_and_bound",
  hdrs = ["branch_and_bound.h"],
  srcs = ["branch_and_bound.cc"],
  deps = [":graph",
          ":min_max_heap"],
)

cc_library(
  name = "heap",
  hdrs = ["heap.h"],
  srcs = ["heap.cc"],
  deps = [":graph"],
)

cc_library(
  name = "min_max_heap",
  hdrs = ["min_max_heap.h"],
  srcs = ["min_max_heap.cc"],
  deps = [":search_tree"],
)

cc_library(
  name = "search_tree",
  hdrs = ["search_tree.h"],
  srcs = ["search_tree.cc"],
  deps = [":graph"],
)
