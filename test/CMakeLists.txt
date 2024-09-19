add_executable(Transform_4OPH
  ${XtalTransforms_SOURCE_DIR}/test/Transform_4OPH.cpp
)
target_link_libraries(Transform_4OPH
  XtalTransformer
)

add_executable(Transform_2Y2A
  ${XtalTransforms_SOURCE_DIR}/test/Transform_2Y2A.cpp
)
target_link_libraries(Transform_2Y2A
  XtalTransformer
)
