# CopyPolicy: This software may be modified and distributed under the terms of the BSD-3-Clause license.

## Optional Dependencies
find_package(Catch2 QUIET)
checkandset_optional_dependency(Catch2)

find_package(VALGRIND QUIET)
checkandset_optional_dependency(VALGRIND)

named_tuple_dependent_option(NAMED_TUPLE_COMPILE_tests
  "Compile tests?" ON
  "NAMED_TUPLE_HAS_Catch2;BUILD_TESTING" OFF)

named_tuple_dependent_option(NAMED_TUPLE_RUN_Valgrind_tests
  "Run Valgrind tests?" OFF
  "NAMED_TUPLE_COMPILE_tests;VALGRIND_FOUND" OFF)

if (NAMED_TUPLE_RUN_Valgrind_tests)
    set(CTEST_MEMORYCHECK_COMMAND ${VALGRIND_PROGRAM})
    set(MEMORYCHECK_COMMAND ${VALGRIND_PROGRAM})
    set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1 ${MEMORYCHECK_SUPPRESSIONS}"  CACHE STRING "Options to pass to the memory checker")
    mark_as_advanced(MEMORYCHECK_COMMAND_OPTIONS)
    set(MEMCHECK_COMMAND_COMPLETE "${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}")
    separate_arguments(MEMCHECK_COMMAND_COMPLETE)
endif()

if (NAMED_TUPLE_COMPILE_tests)
    configure_file(cmake/Catch2Main.cpp.in ${CMAKE_BINARY_DIR}/Testing/Catch2Main.cpp)
    add_library(CatchTestMain ${CMAKE_BINARY_DIR}/Testing/Catch2Main.cpp)
    target_link_libraries(CatchTestMain PUBLIC Catch2::Catch2)
endif()


function(add_named_tuple_test)

    if(NAMED_TUPLE_COMPILE_tests)

      set(options)
      set(oneValueArgs NAME)
      set(multiValueArgs SOURCES LINKS COMPILE_DEFINITIONS)

      set(prefix "named_tuple")

      cmake_parse_arguments(${prefix}
          "${options}"
          "${oneValueArgs}"
          "${multiValueArgs}"
          ${ARGN})

      set(name ${${prefix}_NAME})
      set(unit_test_files ${${prefix}_SOURCES})

      set(targetname ${name}_unit_tests)
      add_executable(${targetname}
          "${unit_test_files}")

      target_link_libraries(${targetname} PRIVATE CatchTestMain ${${prefix}_LINKS})
      target_compile_definitions(${targetname} PRIVATE CATCH_CONFIG_FAST_COMPILE CATCH_CONFIG_DISABLE_MATCHERS)
      target_compile_features(${targetname} PUBLIC cxx_std_17)

      add_test(NAME ${targetname} COMMAND ${targetname})
      target_compile_definitions(${targetname} PRIVATE ${${prefix}_COMPILE_DEFINITIONS})

      if(NAMED_TUPLE_RUN_Valgrind_tests)
        add_test(NAME memcheck_${targetname} COMMAND ${MEMCHECK_COMMAND_COMPLETE} $<TARGET_FILE:${targetname}>)
      endif()

    endif()

endfunction()
