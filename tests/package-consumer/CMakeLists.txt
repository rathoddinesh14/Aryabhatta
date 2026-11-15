cmake_minimum_required(VERSION 3.20)

project(AryabhattaConsumer LANGUAGES CXX)

find_package(Aryabhatta CONFIG REQUIRED)

add_executable(aryabhatta_consumer main.cpp)

target_compile_features(aryabhatta_consumer PRIVATE cxx_std_20)

target_link_libraries(
    aryabhatta_consumer
    PRIVATE
        Aryabhatta::Math
)