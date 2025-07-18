cmake_minimum_required(VERSION 3.20)   # 3.20 cobre Ubuntu 22.04+; 4.x se quiser recursos novos
project(REST_API_C LANGUAGES C)

# Configurações de debug
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

add_executable(server src/main.c)

# Inclui headers em "include/"
target_include_directories(server
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Linka bibliotecas do sistema localizadas com pkg‑config
find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON REQUIRED libcjson)
pkg_check_modules(OPENSSL REQUIRED openssl)

target_link_libraries(server
  PRIVATE ${CJSON_LIBRARIES} ${OPENSSL_LIBRARIES}
)
target_compile_options(server PRIVATE ${CJSON_CFLAGS_OTHER} ${OPENSSL_CFLAGS_OTHER})