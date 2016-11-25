cmake_minimum_required(VERSION 2.6)

project(regina C)

set(REGINA_SOURCE_FILES
  src/stream.c
  src/regina_tcp_protocol.c
  src/security_mbedtls.c
  src/compression.c
  src/regina_config.c
  src/device_status.c
  src/regina_common_schema.c
  src/device.c
  src/safe_ls_file.c
  src/auth_service.c
  src/profile_service.c
  src/notification_service.c
  src/configuration_service.c
  src/sensor_data_service.c
  src/event_service.c
  src/soft_service.c
  src/thirdparty_service.c
  src/io.c
  src/encoding_binary.c
  src/regina_list.c
  src/bootstrap.c
  src/log.c
  src/acceptor.c
  src/channel.c
  )

set(REGINA_SCHEMA_GEN_FILES
  src/sdk_conf_gen.c
  src/device_status_gen.c
  src/acc_dev_gen.c
  src/bs_dev_gen.c
  )

add_definitions(
  -DMBEDTLS_HAVE_TIME
  -DMBEDTLS_HAVE_TME_DATE
  )
set(MBED_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/mbedtls-2.2.1/library)
set(MBEDTLS_CRYPTO_FILES
    ${MBED_SOURCE_DIR}/aes.c
    ${MBED_SOURCE_DIR}/aesni.c
    # ${MBED_SOURCE_DIR}/arc4.c
    ${MBED_SOURCE_DIR}/asn1parse.c
    # ${MBED_SOURCE_DIR}/asn1write.c
    ${MBED_SOURCE_DIR}/base64.c
    ${MBED_SOURCE_DIR}/bignum.c
    # ${MBED_SOURCE_DIR}/blowfish.c
    # ${MBED_SOURCE_DIR}/camellia.c
    # ${MBED_SOURCE_DIR}/ccm.c
    # ${MBED_SOURCE_DIR}/cipher.c
    # ${MBED_SOURCE_DIR}/cipher_wrap.c
    ${MBED_SOURCE_DIR}/ctr_drbg.c
    # ${MBED_SOURCE_DIR}/des.c
    # ${MBED_SOURCE_DIR}/dhm.c
    # ${MBED_SOURCE_DIR}/ecdh.c
    # ${MBED_SOURCE_DIR}/ecdsa.c
    # ${MBED_SOURCE_DIR}/ecjpake.c
    # ${MBED_SOURCE_DIR}/ecp.c
    # ${MBED_SOURCE_DIR}/ecp_curves.c
    ${MBED_SOURCE_DIR}/entropy.c
    ${MBED_SOURCE_DIR}/entropy_poll.c
    # ${MBED_SOURCE_DIR}/error.c
    # ${MBED_SOURCE_DIR}/gcm.c
    # ${MBED_SOURCE_DIR}/havege.c
    ${MBED_SOURCE_DIR}/hmac_drbg.c
    ${MBED_SOURCE_DIR}/md.c
    # ${MBED_SOURCE_DIR}/md2.c
    # ${MBED_SOURCE_DIR}/md4.c
    ${MBED_SOURCE_DIR}/md5.c
    ${MBED_SOURCE_DIR}/md_wrap.c
    ${MBED_SOURCE_DIR}/memory_buffer_alloc.c
    ${MBED_SOURCE_DIR}/oid.c
    # ${MBED_SOURCE_DIR}/padlock.c
    ${MBED_SOURCE_DIR}/pem.c
    ${MBED_SOURCE_DIR}/pk.c
    ${MBED_SOURCE_DIR}/pk_wrap.c
    # ${MBED_SOURCE_DIR}/pkcs12.c
    # ${MBED_SOURCE_DIR}/pkcs5.c
    ${MBED_SOURCE_DIR}/pkparse.c
    # ${MBED_SOURCE_DIR}/pkwrite.c
    ${MBED_SOURCE_DIR}/platform.c
    # ${MBED_SOURCE_DIR}/ripemd160.c
    ${MBED_SOURCE_DIR}/rsa.c
    # ${MBED_SOURCE_DIR}/sha1.c
    ${MBED_SOURCE_DIR}/sha256.c
    # ${MBED_SOURCE_DIR}/sha512.c
    # ${MBED_SOURCE_DIR}/threading.c
    ${MBED_SOURCE_DIR}/timing.c
    # ${MBED_SOURCE_DIR}/version.c
    # ${MBED_SOURCE_DIR}/version_features.c
    # ${MBED_SOURCE_DIR}/xtea.c
    )

# select the target platform
if(WIN32)
  set(REGINA_PLATFORM_OS win)
  set(REGINA_PLATFORM_LIBRARIES ws2_32)
  add_definitions(-DREGINA_PLATFORM_WINDOWS)
elseif (UNIX)
  set(REGINA_PLATFORM_OS posix)
  set(REGINA_PLATFORM_LIBRARIES pthread)
  add_definitions(-DREGINA_PLATFORM_UNIX)
endif()

if(NOT DEFINED REGINA_PLATFORM_OS)
  message(FATAL_ERROR "Unknown platform OS.")
endif()

set(REGINA_PLATFORM_SOURCE_DIR platforms/src/${REGINA_PLATFORM_OS})
set(REGINA_PLATFORM_FILES
  ${REGINA_PLATFORM_SOURCE_DIR}/std_wrapper.c
  ${REGINA_PLATFORM_SOURCE_DIR}/allocator.c
  ${REGINA_PLATFORM_SOURCE_DIR}/tcp_util.c
  ${REGINA_PLATFORM_SOURCE_DIR}/thread.c
  ${REGINA_PLATFORM_SOURCE_DIR}/environment.c
  )


if(UNIX)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -Wl,-no-whole-archive")
elseif (MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DWIN32_LEAN_AND_MEAN /D_CRT_SECURE_NO_WARNINGS /D__BUILD_REGINA__")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE STREQUAL "debug")
  message(STATUS "Build REGINA SDK as Debug mode.")
  set(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -Wall")
  add_definitions(-DREGINA_DEBUG)
else()
  message(STATUS "Build REGINA SDK as Release mode.")
  set(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall")
  if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
  endif()
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/platforms/include
  ${CMAKE_SOURCE_DIR}/thirdparty/mbedtls-2.2.1/include
  )

# Define groupings for source files in IDE project generation.
source_group(src FILES ${REGINA_SOURCE_FILES})
source_group(platform FILES ${REGINA_PLATFORM_FILES})
source_group(schema_gen FILES ${REGINA_SCHEMA_GEN_FILES})
source_group(mbedtls FILES ${MBEDTLS_CRYPTO_FILES})

add_library(regina_o OBJECT
  ${REGINA_SOURCE_FILES}
  ${REGINA_PLATFORM_FILES}
  ${REGINA_SCHEMA_GEN_FILES}
  ${MBEDTLS_CRYPTO_FILES}
  )

option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
option(BUILD_STATIC_LIBS "Build static libraries (.lib/.a)" OFF)


include (TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if(${IS_BIG_ENDIAN})
  add_definitions(-DREGINA_PLATFORM_BIG_ENDIAN)
endif()

if(BUILD_SHARED_LIBS)
  add_library(reginac SHARED $<TARGET_OBJECTS:regina_o>)
  set(REGINA_LIBRARIES reginac)
endif()

if(BUILD_STATIC_LIBS)
  add_library(reginac_s STATIC $<TARGET_OBJECTS:regina_o>)
  set(REGINA_LIBRARIES ${REGINA_LIBRARIES} reginac_s)
endif()


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

foreach(_lib ${REGINA_LIBRARIES})
  target_link_libraries(${_lib}
    ${REGINA_PLATFORM_LIBRARIES}
    )
endforeach()

message(STATUS "REGINA SDK will be installed to ${CMAKE_INSTALL_PREFIX}")
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/regina
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/
  FILES_MATCHING PATTERN *.h
  )

install(TARGETS ${REGINA_LIBRARIES} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


option(BUILD_REGINA_SAMPLE "Build the sample." OFF)

if(BUILD_REGINA_SAMPLE)
  add_subdirectory(sample)
endif()
