package(default_visibility = ["//visibility:public"])

cc_library(
  name =  "cpt_node",
  hdrs = ["cpt_node.h"],
  srcs = ["cpt_node.cc"],
  deps = ["//bayes/branch_and_bound:inverted_index"],
)

cc_library(
  name =  "cp_table",
  hdrs = ["cp_table.h"],
  srcs = ["cp_table.cc"],
  deps = [":cpt_node",
          "//bayes/branch_and_bound:bitset"],
)

cc_library(
  name =  "ranker",
  hdrs = ["ranker.h"],
  srcs = ["ranker.cc"],
  deps = ["//base:document",
          "//bayes/branch_and_bound:graph"],
)
