package(default_visibility = ["//visibility:public"])

cc_library(
  name = "document",
  srcs = ["document.cc"],
  hdrs = ["document.h"],
)

cc_library(
  name = "query",
  srcs = ["query.cc"],
  hdrs = ["query.h"],
  deps = [":document"],
)

cc_library(
  name = "dataset",
  srcs = ["dataset.cc"],
  hdrs = ["dataset.h"],
  deps = [":document",
          ":query"],
)

cc_binary(
  name = "test",
  srcs = ["test.cc"],
  deps = [":dataset",
          ":document",
          ":query"],
)
