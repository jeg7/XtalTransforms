add_library(XtalTransformer
  ${XtalTransforms_SOURCE_DIR}/src/Atom.cpp
  ${XtalTransforms_SOURCE_DIR}/src/XtalTransformer.cpp
)
target_include_directories(XtalTransformer PUBLIC
  ${XtalTransforms_SOURCE_DIR}/include
)
