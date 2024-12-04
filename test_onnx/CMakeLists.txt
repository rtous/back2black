set(EXAMPLE_TARGET test_onnx)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

#SETUP ONNXRUNTIME
set(ONNXRUNTIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/build/MacOS/RelWithDebInfo")
#set(ONNXRUNTIME_ROOTDIR ${CMAKE_SOURCE_DIR}/../onnxruntime)
#include_directories("${ONNXRUNTIME_ROOTDIR}/include")
#find_package(ONNXRUNTIME REQUIRED)

#target_link_libraries(${EXAMPLE_TARGET} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${myonnxruntimelib} )

target_include_directories(
    ${EXAMPLE_TARGET} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/eager
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/framework
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/graph
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/optimizer
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/providers
    ${CMAKE_CURRENT_SOURCE_DIR}/../../onnxruntime/include/onnxruntime/core/session
)

target_link_libraries(
    ${EXAMPLE_TARGET} PRIVATE 
    ${CMAKE_THREAD_LIBS_INIT}

    "-framework Foundation"

    ${ONNXRUNTIME_DIR}/libonnxruntime_session.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_optimizer.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_providers.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_util.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_framework.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_graph.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_mlas.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_common.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_flatbuffers.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_lora.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_mocked_allocator.a
    ${ONNXRUNTIME_DIR}/libonnxruntime_test_utils.a
    ${ONNXRUNTIME_DIR}/libonnx_test_runner_common.a
    ${ONNXRUNTIME_DIR}/libonnx_test_data_proto.a

    ${ONNXRUNTIME_DIR}/_deps/onnx-build/libonnx_proto.a
    ${ONNXRUNTIME_DIR}/_deps/onnx-build/libonnx.a
    ${ONNXRUNTIME_DIR}/_deps/protobuf-build/libprotobuf-lite.a
    ${ONNXRUNTIME_DIR}/_deps/flatbuffers-build/libflatbuffers.a
    ${ONNXRUNTIME_DIR}/_deps/re2-build/libre2.a

    #abseil
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_base.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_throw_delegate.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_spinlock_wait.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_raw_logging_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_strerror.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_malloc_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/base/libabsl_log_severity.a
    
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/container/libabsl_raw_hash_set.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/container/libabsl_hashtablez_sampler.a

    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/crc/libabsl_crc_cord_state.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/crc/libabsl_crc_cpu_detect.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/crc/libabsl_crc_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/crc/libabsl_crc32c.a

    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_debugging_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_decode_rust_punycode.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_demangle_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_demangle_rust.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_examine_stack.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_failure_signal_handler.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_stacktrace.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_symbolize.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/debugging/libabsl_utf8_for_code_point.a

    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_commandlineflag_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_commandlineflag.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_config.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_marshalling.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_parse.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_private_handle_accessor.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_program_name.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_reflection.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_usage_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/flags/libabsl_flags_usage.a
    
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/hash/libabsl_hash.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/hash/libabsl_low_level_hash.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/hash/libabsl_city.a

    #${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_entry.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_globals.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_check_op.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_conditions.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_fnmatch.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_format.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_globals.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_log_sink_set.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_message.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_nullguard.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_internal_proto.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_log_sink.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/log/libabsl_vlog_config_internal.a
 
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/numeric/libabsl_int128.a

    #${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/profiling/libabsl_exponential_biased.a
    
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_cord_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_cord.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_cordz_functions.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_cordz_handle.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_cordz_info.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_str_format_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_string_view.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_strings_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/strings/libabsl_strings.a

    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/synchronization/libabsl_graphcycles_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/synchronization/libabsl_kernel_timeout_internal.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/synchronization/libabsl_synchronization.a

    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/types/libabsl_bad_optional_access.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/types/libabsl_bad_any_cast_impl.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/types/libabsl_bad_variant_access.a
    
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/time/libabsl_civil_time.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/time/libabsl_time_zone.a
    ${ONNXRUNTIME_DIR}/_deps/abseil_cpp-build/absl/time/libabsl_time.a

    #pytorch_cpuinfo-build
    ${ONNXRUNTIME_DIR}/_deps/pytorch_cpuinfo-build/libcpuinfo.a
)

target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_17)

#set(CMAKE_CXX_STANDARD 17)

#If Microsoft Visual C++ or similar
#if(MSVC)
#    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
#endif()




#if (WIN32)
# this code copies the onnxruntime library to the same folder as the executable
#add_custom_command(
#    TARGET ${EXAMPLE_TARGET} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    "${PROJECT_SOURCE_DIR}/../onnxruntime-osx-arm64-1.20.1/libonnxruntime.1.20.1.dylib"
#    $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>
#    VERBATIM)
#endif()

#To avoid macos code signing check error
#set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
#set_target_properties(test_onnx PROPERTIES
#        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
#        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO" 
#    )
