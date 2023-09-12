add_executable(parse-fuzzer fuzz.cpp)
target_link_libraries(parse-fuzzer ${SEXPR_PROJECT_NAME})
target_compile_options(parse-fuzzer PRIVATE -fsanitize=address,fuzzer,undefined -fno-sanitize-recover=all)
target_link_options(parse-fuzzer PRIVATE -fsanitize=address,fuzzer,undefined -fno-sanitize-recover=all)
