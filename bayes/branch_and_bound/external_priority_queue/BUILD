package(default_visibility = ["//visibility:public"])

cc_library(
  name = "simple_external_queue",
  hdrs = ["simple_external_queue.h"],
  srcs = ["simple_external_queue.cc"],
  deps = [],
)

cc_library(
  name = "simple_min_max_heap",
  hdrs = ["simple_min_max_heap.h"],
  srcs = ["simple_min_max_heap.cc"],
  deps = [],
)

cc_library(
  name = "simple_external_priority_queue",
  hdrs = ["simple_external_priority_queue.h"],
  srcs = ["simple_external_priority_queue.cc"],
  deps = [":simple_min_max_heap",
          ":simple_external_queue"],
)

cc_library(
  name = "graph_external_queue",
  hdrs = ["graph_external_queue.h"],
  srcs = ["graph_external_queue.cc"],
  deps = ["//bayes/branch_and_bound:graph"],
)

cc_library(
  name = "graph_min_max_heap",
  hdrs = ["graph_min_max_heap.h"],
  srcs = ["graph_min_max_heap.cc"],
  deps = ["//bayes/branch_and_bound:graph"],
)

cc_library(
  name =  "graph_external_priority_queue",
  hdrs = ["graph_external_priority_queue.h"],
  srcs = ["graph_external_priority_queue.cc"],
  deps = [":graph_min_max_heap",
          ":graph_external_queue"],
)

cc_library(
  name =  "external_branch_and_bound",
  hdrs = ["external_branch_and_bound.h"],
  srcs = ["external_branch_and_bound.cc"],
  deps = [":graph_external_priority_queue"],
)
