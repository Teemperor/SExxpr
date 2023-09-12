function(make_unittest)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs FILES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unparsed arguments to make_unittest: ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  if(NOT ARG_FILES)
    set(ARG_FILES "${ARG_NAME}.cpp")
  endif()

  set(EXE_NAME "Test${ARG_NAME}")
  add_executable("${EXE_NAME}" ${ARG_FILES} TestUtils.h)
  target_link_libraries("${EXE_NAME}" ${SEXPR_PROJECT_NAME} gtest_main)
  target_compile_options("${EXE_NAME}" PRIVATE -Werror -Wpedantic -Wall)
  # Manual test binary build.
  #add_test(NAME    "unittest:${ARG_NAME}"
  #         COMMAND "$<TARGET_FILE:${EXE_NAME}>"
  #         WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  gtest_discover_tests("${EXE_NAME}")
endfunction()

include(GoogleTest)
make_unittest(NAME ParseStrings)
make_unittest(NAME ParseSymbols)
make_unittest(NAME ParseIntegers)
make_unittest(NAME ParseLists)
make_unittest(NAME ParseFloats)
make_unittest(NAME Comparisons)
make_unittest(NAME Printing)
