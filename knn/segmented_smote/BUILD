package(default_visibility = ["//visibility:public"])

cc_library(
  name = "segmentation_metadata",
  hdrs = ["segmentation_metadata.h"],
  srcs = ["segmentation_metadata.cc"],
  deps = ["//base:dataset",
          "//base:document",
          "//base:query",
          "//util:discretizer"],
)

cc_binary(
  name = "segmentation_metadata_test",
  srcs = ["segmentation_metadata_test.cc"],
  deps = [":segmentation_metadata",
          "//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_library(
  name = "smote_segmenter",
  hdrs = ["smote_segmenter.h"],
  srcs = ["smote_segmenter.cc"],
  deps = [":segmentation_metadata",
          "//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_binary(
  name = "smote_segmenter_test",
  srcs = ["smote_segmenter_test.cc"],
  deps = [":segmentation_metadata",
          ":smote_segmenter",
          "//base:dataset",
          "//base:document",
          "//base:query"],
)

cc_library(
  name = "segmented_smote",
  hdrs = ["segmented_smote.h"],
  srcs = ["segmented_smote.cc"],
  deps = [":segmentation_metadata",
          ":smote_segmenter",
          "//base:dataset",
          "//base:document",
          "//base:query",
          "//euclid:vector_operations",
          "//knn:knn",
          "//util:discretizer"],
)

cc_binary(
  name = "segmented_smote_test",
  srcs = ["segmented_smote_test.cc"],
  deps = [":segmentation_metadata",
          ":smote_segmenter",
          ":segmented_smote",
          "//base:dataset",
          "//base:document",
          "//base:query"],
)
