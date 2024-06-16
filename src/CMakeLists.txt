add_executable (graph_decomposition main.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET graph_decomposition PROPERTY CXX_STANDARD 20)
endif()