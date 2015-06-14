package(default_visibility = ["//visibility:public"])

cc_library(
  name = "k2",
  hdrs = ["k2.h"],
  srcs = ["k2.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_binary(
  name = "k2_test",
  srcs = ["test.cc"],
  deps = [":k2",
          "//base:dataset",
          "//base:document",
          "//base:query"],
)
