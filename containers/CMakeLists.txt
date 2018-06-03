add_library(containers_lib INTERFACE)
target_include_directories(containers_lib INTERFACE "${PROJECT_SOURCE_DIR}/containers/include")

add_executable(test_dense_ordered_map src/test_dense_ordered_map.cpp)

message(${PROJECT_SOURCE_DIR})

message(${THIRD_PARTY_DIR})

target_include_directories(test_dense_ordered_map PRIVATE
  ${THIRD_PARTY_DIR} ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(test_dense_ordered_map containers_lib)
