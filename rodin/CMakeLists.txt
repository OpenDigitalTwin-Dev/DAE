cmake_minimum_required (VERSION 3.16)

cmake_policy(SET CMP0054 NEW)
cmake_policy(SET CMP0076 NEW)

# Prohibit in-source builds
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are prohibited.")
endif ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

project(Rodin LANGUAGES CXX C)

# ---- Options ---------------------------------------------------------------
option(RODIN_BUILD_SRC              "Build the Rodin source code"               ON)
option(RODIN_BUILD_EXAMPLES         "Build the Rodin examples"                  ON)
option(RODIN_BUILD_UNIT_TESTS       "Build Rodin unit tests"                    OFF)
option(RODIN_BUILD_BENCHMARKS       "Build Rodin benchmarks"                    OFF)
option(RODIN_BUILD_DOC              "Build the Rodin documentation"             OFF)
option(RODIN_WITH_PLOT              "Build the Rodin::Plot module"              OFF)
option(RODIN_WITH_PY                "Build Python bindings"                     OFF)
option(RODIN_USE_MCSS               "Build documentation with m.css"            OFF)
option(RODIN_USE_MPI                "Use Rodin with MPI support"                OFF)
option(RODIN_USE_OPENMP             "Use Rodin with OpenMP support"             OFF)
option(RODIN_USE_UMFPACK            "Use Rodin with UMFPACK support"            OFF)
option(RODIN_USE_PASTIX             "Use Rodin with PaStiX support"             OFF)
option(RODIN_USE_KLU                "Use Rodin with KLU support"                OFF)
option(RODIN_USE_SUPERLU            "Use Rodin with SuperLU support"            OFF)
option(RODIN_USE_SPQR               "Use Rodin with SPQR support"               OFF)
option(RODIN_USE_PARDISO            "Use Rodin with Pardiso support"            OFF)
option(RODIN_USE_APPLE_ACCELERATE   "Use Rodin with Apple Accelerate support"   OFF)
option(RODIN_SILENCE_WARNINGS       "Silence warnings outputted by Rodin"       OFF)
option(RODIN_SILENCE_EXCEPTIONS     "Silence exceptions thrown by Rodin"        ON)
option(RODIN_CODE_COVERAGE          "Compile with code coverage flags"          OFF)
option(RODIN_LTO                    "Compile with link time optimization"       OFF)
option(RODIN_FEATURE_SUMMARY        "Print Rodin feature summary"               ON)
option(RODIN_THREAD_SAFE            "Enforces thread-safety."                   OFF)
option(RODIN_MULTITHREADED          "Utilizes multithreading capabalities."     OFF)
option(BUILD_SHARED_LIBS            "Build using shared libraries"              OFF)

if (RODIN_MULTITHREADED)
  set(RODIN_THREAD_SAFE ON CACHE BOOL "" FORCE)
  set(Boost_USE_MULTITHREADED ON)
endif()

set(RODIN_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/resources/")
set(RODIN_THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third-party/")

# ---- Set the module path ---------------------
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# ---- CMake Modules ---------------------------
include(RodinConfig)

# ---- Configure compiler ----------------------
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "")

# ---- Define install paths --------------------------------------------------
set(RODIN_INSTALL_BINARY_PATH bin)
set(RODIN_INSTALL_SHARE_PATH share)
set(RODIN_INSTALL_LIBRARY_PATH lib)
set(RODIN_INSTALL_INCLUDE_PATH include)

# ---- Set compile options for all the submodules ----------------------------
rodin_add_compile_options(LANG CXX OPTIONS -fvisibility=hidden)
rodin_add_compile_options(LANG CXX C OPTIONS -fcolor-diagnostics)
rodin_add_compile_options(LANG CXX C OPTIONS -Wno-error=old-style-cast -Wno-error=double-promotion)

add_compile_definitions(_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
add_compile_definitions(__MAC_OS_X_VERSION_MIN_REQUIRED=__MAC_10_8)

if (RODIN_LTO)
  rodin_add_compile_options(LANG CXX C OPTIONS -flto)
endif()

if (RODIN_CODE_COVERAGE)
  include(CodeCoverage)
  append_coverage_compiler_flags()
  rodin_add_compile_options(LANG CXX C -fprofile-abs-path)
endif()

if (RODIN_BUILD_SRC)
  # set(MFEM_THREAD_SAFE ON CACHE BOOL TRUE FORCE)
  # ---- Find and build dependencies -----------------------------------------
  # ---- PThreads ----
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  add_subdirectory(third-party/BS_thread_pool)

  if (RODIN_USE_OPENMP)
    find_package(OpenMP REQUIRED)
  endif()

  # ---- Boost ----
  add_compile_definitions(PRIVATE BOOST_ASIO_HAS_STD_INVOKE_RESULT=1)
  # set(Boost_USE_STATIC_LIBS        OFF)
  # set(Boost_USE_STATIC_RUNTIME     OFF)

  if (RODIN_USE_MPI)
    find_package(Boost 1.74 REQUIRED COMPONENTS mpi serialization)
  endif()

  find_package(Boost 1.74 REQUIRED
    COMPONENTS
    system
    filesystem)
  include_directories(${Boost_INCLUDE_DIRS})

  # ---- Corrade ----
  message(STATUS "Configuring Corrade...")
  add_subdirectory(third-party/corrade EXCLUDE_FROM_ALL)
  find_package(Corrade REQUIRED Utility)
  set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

  # ---- Eigen ----
  message(STATUS "Configuring Eigen3...")
  set(EIGEN_BUILD_DOC OFF CACHE BOOL "" FORCE)
  set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
  add_subdirectory(third-party/eigen EXCLUDE_FROM_ALL)
  unset(BUILD_TESTING)

  find_package (Eigen3 3.3 REQUIRED NO_MODULE
    HINTS
    "${PROJECT_BINARY_DIR}/third-party/eigen"
    "${PROJECT_BINARY_DIR}/third-party/eigen/cmake")

  # ---- Basix ------
  add_subdirectory(third-party/basix/cpp EXCLUDE_FROM_ALL)

  # ---- Spectra ----
  message(STATUS "Configuring Spectra...")
  set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set (CMAKE_DISABLE_FIND_PACKAGE_CLANG_FORMAT TRUE)
  list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third-party/eigen/cmake)
  add_subdirectory(third-party/spectra EXCLUDE_FROM_ALL)
  unset (CMAKE_DISABLE_FIND_PACKAGE_CLANG_FORMAT)

  find_package (Spectra 1.0.1 REQUIRED)

  if (RODIN_WITH_PLOT)
    # ---- Magnum ----
    message(STATUS "Configuring Magnum...")
    set(WITH_SDL2APPLICATION ON CACHE BOOL "" FORCE)
    add_subdirectory(third-party/magnum EXCLUDE_FROM_ALL)
    find_package(Magnum REQUIRED GL Shaders Sdl2Application)
    set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

    # ---- Magnum-Eigen integration ----
    message(STATUS "Configuring Magnum-Eigen integration...")
    set(EIGEN3_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third-party/eigen)
    list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/third-party/eigen/cmake)
    set(MAGNUM_WITH_EIGEN ON CACHE BOOL "" FORCE)
    add_subdirectory(third-party/magnum-integration EXCLUDE_FROM_ALL)
    find_package(MagnumIntegration REQUIRED Eigen)
  endif()

  add_subdirectory(src)

  if (RODIN_BUILD_EXAMPLES)
    add_subdirectory(examples)
  endif()

  if (RODIN_BUILD_UNIT_TESTS OR RODIN_BUILD_BENCHMARKS)
    # ---- GoogleTest ----
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(third-party/googletest EXCLUDE_FROM_ALL)
    if (RODIN_BUILD_BENCHMARKS)
      if (RODIN_LTO)
        set(BENCHMARK_ENABLE_LTO ON CACHE BOOL "" FORCE)
      endif()
      set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
      set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
      add_subdirectory(third-party/benchmark EXCLUDE_FROM_ALL)
    endif()

    add_subdirectory(tests)
  endif()

  if (RODIN_WITH_PY)
    set(Python3_FIND_REGISTRY LAST)
    set(Python3_FIND_VIRTUALENV FIRST)
    find_package(Python 3.6 COMPONENTS Interpreter Development REQUIRED)
    execute_process(
      COMMAND "${PYTHON_EXECUTABLE}" -c
              "import pybind11; print(pybind11.get_cmake_dir())"
      OUTPUT_VARIABLE _tmp_dir
      OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
    list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
    add_subdirectory(third-party/pybind11)
    add_subdirectory(py)
  endif()
endif()

if (RODIN_BUILD_DOC)
  add_subdirectory(doc)
endif()

if (RODIN_FEATURE_SUMMARY)
  message(STATUS "Rodin feature summary:")
  include(FeatureSummary)
  feature_summary(INCLUDE_QUIET_PACKAGES WHAT ALL)
endif()

rodin_get_files_in_dir(RODIN_RESOURCES PATH ${RODIN_RESOURCES_DIR} GLOB_RECURSE EXCLUDE HIDDEN)
set(RODIN_RELATIVE_RESOURCES "")
foreach(RESOURCE_PATH ${RODIN_RESOURCES})
  rodin_get_relpath(RELATIVE_PATH PATH ${RESOURCE_PATH} DIRECTORY ${PROJECT_SOURCE_DIR})
  list(APPEND RODIN_RELATIVE_RESOURCES ${RELATIVE_PATH})
endforeach()
rodin_install_files(FILES ${RODIN_RELATIVE_RESOURCES} DESTINATION ${RODIN_INSTALL_SHARE_PATH})

