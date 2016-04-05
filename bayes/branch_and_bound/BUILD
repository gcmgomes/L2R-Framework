package(default_visibility = ["//visibility:public"])

cc_library(
  name = "bitset",
  hdrs = ["bitset.h"],
  srcs = ["bitset.cc"],
  deps = [],
)

cc_library(
  name = "cache",
  hdrs = ["cache.h"],
  srcs = ["cache.cc"],
  deps = [":bitset"],
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
  name = "variable",
  hdrs = ["variable.h"],
  srcs = ["variable.cc"],
  deps = [":cache",
          ":instance",
          ":external_queue"],
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
  deps = [":graph"],
)

cc_library(
  name = "inverted_index",
  hdrs = ["inverted_index.h"],
  srcs = ["inverted_index.cc"],
  deps = [":instance"],
)
