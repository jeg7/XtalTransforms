add_executable(Transform
  ${XtalTransforms_SOURCE_DIR}/test/Transform.cpp
)
target_link_libraries(Transform
  XtalTransformer
)
