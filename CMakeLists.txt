cmake_minimum_required(VERSION 3.19)

set(PROJECT_NAME grpc_practice)
set(PROTO_ENTITIES_LIBRARY_NAME Entities)
set(BINARY_NAME ${PROJECT_NAME}-bin)
set(STATIC_LIB_NAME ${PROJECT_NAME}-static)
set(SHARED_LIB_NAME ${PROJECT_NAME})
set(TARGET_NAMES ${BINARY_NAME} ${STATIC_LIB_NAME} ${SHARED_LIB_NAME})

project(${PROJECT_NAME} LANGUAGES CXX VERSION 0.0.0.1)

include(cmake/find_grpc.cmake)

set(INCLUDE_DIRECTORIES
        src/include)

set(SOURCE_FILES
        src/cpp/main.cpp)

# proto compilation start
file(GLOB PROTOBUF_DEFINITION_FILES "protos/**.proto")

foreach (PROTO_FILE ${PROTOBUF_DEFINITION_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME)
    string(REPLACE ".proto" "" PROTO_NAME ${PROTO_NAME})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/proto-gen/${PROTO_NAME})
    set(PROTO_FILES_PATH ${CMAKE_CURRENT_BINARY_DIR}/proto-gen/${PROTO_NAME})

    message("Processing ${PROTO_NAME} proto to ${PROTO_FILES_PATH}")

    get_filename_component(hw_proto ${PROTO_FILE} ABSOLUTE)
    get_filename_component(hw_proto_path "${hw_proto}" PATH)

    # Generated sources
    set(HW_PROTO_SRCS "${PROTO_FILES_PATH}/${PROTO_NAME}.pb.cc")
    set(HW_PROTO_HDRS "${PROTO_FILES_PATH}/${PROTO_NAME}.pb.h")
    set(HW_GRPC_SRCS "${PROTO_FILES_PATH}/${PROTO_NAME}.grpc.pb.cc")
    set(HW_GRPC_HDRS "${PROTO_FILES_PATH}/${PROTO_NAME}.grpc.pb.h")

    add_custom_command(
            OUTPUT "${HW_PROTO_SRCS}" "${HW_PROTO_HDRS}" "${HW_GRPC_SRCS}" "${HW_GRPC_HDRS}"
            COMMAND ${_PROTOBUF_PROTOC}
            ARGS --grpc_out "${PROTO_FILES_PATH}"
            --cpp_out "${PROTO_FILES_PATH}"
            -I "${hw_proto_path}"
            --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
            "${hw_proto}"
            DEPENDS "${hw_proto}")

    LIST(APPEND PROTO_SRCS ${HW_PROTO_SRCS})
    LIST(APPEND PROTO_HDRS ${HW_PROTO_HDRS})
    LIST(APPEND GRPC_SRCS ${HW_GRPC_SRCS})
    LIST(APPEND GRPC_HDRS ${HW_GRPC_HDRS})
    LIST(APPEND PROTO_HEADERS ${PROTO_FILES_PATH})
endforeach ()

# Include generated *.pb.h files
include_directories("${PROTO_HEADERS}")

add_library(${PROTO_ENTITIES_LIBRARY_NAME} SHARED
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${GRPC_SRCS}
        ${GRPC_HDRS})
target_link_libraries(${PROTO_ENTITIES_LIBRARY_NAME}
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
# proto compilation end

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PROTO_ENTITIES_LIBRARY_NAME})
