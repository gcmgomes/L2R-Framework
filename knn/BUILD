package(default_visibility = ["//visibility:public"])

cc_library(
  name = "knn",
  hdrs = ["k_nearest_neighbours.h"],
  srcs = ["k_nearest_neighbours.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query",
          "//euclid:vector_operations"],
)

cc_library(
  name = "smote",
  hdrs = ["smote.h"],
  srcs = ["smote.cc"],
  deps = [":knn",
          "//base:dataset",
          "//base:document",
          "//base:query",
          "//euclid:vector_operations"],
)

cc_binary(
  name = "knn_test",
  srcs = ["test.cc"],
  deps = [":knn",
          "//base:dataset",
          "//base:document",
          "//base:query",
          "//euclid:vector_operations"],
)
