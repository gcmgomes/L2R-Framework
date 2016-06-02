package(default_visibility = ["//visibility:public"])

cc_library(
  name = "external_branch_and_bound",
  hdrs = ["external_branch_and_bound.h"],
  srcs = ["external_branch_and_bound.cc"],
  deps = [":reference_external_priority_queue"],
)

cc_library(
  name = "external_graph_reference",
  hdrs = ["external_graph_reference.h"],
  deps = ["//bayes/branch_and_bound:graph"],
)

cc_library(
  name = "reference_external_queue",
  hdrs = ["reference_external_queue.h"],
  srcs = ["reference_external_queue.cc"],
  deps = [":external_graph_reference",
          "//util/data_structures:min_max_heap"],
)

cc_library(
  name =  "reference_external_priority_queue",
  hdrs = ["reference_external_priority_queue.h"],
  srcs = ["reference_external_priority_queue.cc"],
  deps = [":reference_external_queue",
          "//util/data_structures:min_max_heap"],
)
