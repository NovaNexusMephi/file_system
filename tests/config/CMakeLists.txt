add_executable(config_test config_test.cpp)
target_link_libraries(config_test PRIVATE config GTest::GTest GTest::Main yaml-cpp)