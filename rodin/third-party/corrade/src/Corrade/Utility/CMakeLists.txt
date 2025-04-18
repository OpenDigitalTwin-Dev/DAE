#
#   This file is part of Corrade.
#
#   Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
#               2017, 2018, 2019, 2020, 2021, 2022
#             Vladimír Vondruš <mosra@centrum.cz>
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
#

# IDE folder in VS, Xcode etc. CMake 3.12+, older versions have only the FOLDER
# property that would have to be set on each target separately.
set(CMAKE_FOLDER "Corrade/Utility")

if(CORRADE_WITH_UTILITY)
    set(CorradeUtility_SRCS
        Debug.cpp
        Configuration.cpp
        ConfigurationValue.cpp
        MurmurHash2.cpp
        Sha1.cpp
        System.cpp

        ../Cpu.cpp

        Implementation/ErrorString.cpp)

    set(CorradeUtility_GracefulAssert_SRCS
        Algorithms.cpp
        Arguments.cpp
        ConfigurationGroup.cpp
        Format.cpp
        Json.cpp
        JsonWriter.cpp
        Resource.cpp
        String.cpp
        Unicode.cpp

        ../Containers/ArrayTuple.cpp
        ../Containers/BitArray.cpp
        ../Containers/BitArrayView.cpp
        ../Containers/String.cpp
        ../Containers/StringIterable.cpp
        ../Containers/StringView.cpp)

    # Files that directly or indirectly use CPU dispatch, such as calling
    # Containers::String::contains(). The list gets added to either
    # CorradeUtility_GracefulAssert_SRCS or CorradeUtility_SRCS below based on
    # whether tests use a different CPU dispatch than the main library.
    set(CorradeUtility_CpuDispatch_SRCS
        Path.cpp)

    set(CorradeUtility_HEADERS
        Algorithms.h
        Arguments.h
        AbstractHash.h
        Assert.h
        Configuration.h
        ConfigurationGroup.h
        ConfigurationValue.h
        Debug.h
        DebugAssert.h
        DebugStl.h
        DebugStlStringView.h
        Endianness.h
        EndiannessBatch.h
        Format.h
        FormatStl.h
        FormatStlStringView.h

        # Because various platforms can have multi-architecture binaries
        # (macOS, iOS), the CORRADE_TARGET_X86 variable isn't exposed to CMake.
        # And because all desktop and mobile platfoms have an x86 variant
        # including iOS and Android, and Emscripten has x86 intrinsics
        # compatibility (!!), we install the x86-specific intrinsics wrappers
        # always.
        IntrinsicsSse2.h
        IntrinsicsSse3.h
        IntrinsicsSsse3.h
        IntrinsicsSse4.h
        IntrinsicsAvx.h

        Json.h
        JsonWriter.h
        Macros.h
        Math.h
        Memory.h
        Move.h
        MurmurHash2.h
        Path.h
        Resource.h
        Sha1.h
        String.h
        StlForwardArray.h
        StlForwardString.h
        StlForwardTuple.h
        StlForwardVector.h
        StlMath.h
        System.h
        TypeTraits.h
        Unicode.h
        utilities.h
        Utility.h
        VisibilityMacros.h
        visibility.h)

    set(CorradeUtility_PRIVATE_HEADERS
        Implementation/ErrorString.h
        Implementation/Resource.h)

    if(CORRADE_BUILD_DEPRECATED)
        list(APPEND CorradeUtility_SRCS
            Directory.cpp)
        list(APPEND CorradeUtility_HEADERS
            Directory.h)
    endif()

    # Unix-specific / non-RT-Windows-specific functionality. Also Emscripten.
    if(CORRADE_TARGET_UNIX OR (CORRADE_TARGET_WINDOWS AND NOT CORRADE_TARGET_WINDOWS_RT) OR CORRADE_TARGET_EMSCRIPTEN)
        list(APPEND CorradeUtility_SRCS
            FileWatcher.cpp
            Tweakable.cpp)
        list(APPEND CorradeUtility_CpuDispatch_SRCS
            TweakableParser.cpp)
        list(APPEND CorradeUtility_HEADERS
            FileWatcher.h
            Tweakable.h
            TweakableParser.h)
        list(APPEND CorradeUtility_PRIVATE_HEADERS
            Implementation/tweakable.h)
    endif()

    # Android-specific functionality
    if(CORRADE_TARGET_ANDROID)
        list(APPEND CorradeUtility_SRCS AndroidLogStreamBuffer.cpp)
        list(APPEND CorradeUtility_HEADERS AndroidLogStreamBuffer.h)
    endif()

    # Functionality specific to static Windows builds
    if(CORRADE_TARGET_WINDOWS AND NOT CORRADE_TARGET_WINDOWS_RT AND CORRADE_BUILD_STATIC)
        list(APPEND CorradeUtility_SRCS Implementation/WindowsWeakSymbol.cpp)
        list(APPEND CorradeUtility_PRIVATE_HEADERS Implementation/WindowsWeakSymbol.h)
    endif()

    # If the tests are built with pointer dispatch but the main libraries
    # aren't, we have to compile select files separately for each. Otherwise
    # some code would expect a function symbol to be exist and some expect a
    # function pointer symbol.
    if(CORRADE_BUILD_TESTS_FORCE_CPU_POINTER_DISPATCH AND (NOT CORRADE_BUILD_CPU_RUNTIME_DISPATCH OR CORRADE_CPU_USE_IFUNC))
        list(APPEND CorradeUtility_GracefulAssert_SRCS ${CorradeUtility_CpuDispatch_SRCS})
    else()
        list(APPEND CorradeUtility_SRCS ${CorradeUtility_CpuDispatch_SRCS})
    endif()

    # Objects shared between main and test library
    add_library(CorradeUtilityObjects OBJECT
        ${CorradeUtility_SRCS}
        ${CorradeUtility_HEADERS}
        ${CorradeUtility_PRIVATE_HEADERS})
    target_include_directories(CorradeUtilityObjects PUBLIC $<TARGET_PROPERTY:CorradeUtility,INTERFACE_INCLUDE_DIRECTORIES>)
    if(NOT CORRADE_BUILD_STATIC)
        target_compile_definitions(CorradeUtilityObjects PRIVATE "-DCorradeUtilityObjects_EXPORTS")
    endif()
    if(NOT CORRADE_BUILD_STATIC OR CORRADE_BUILD_STATIC_PIC)
        set_target_properties(CorradeUtilityObjects PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    # Main Utility library
    add_library(CorradeUtility ${SHARED_OR_STATIC}
        $<TARGET_OBJECTS:CorradeUtilityObjects>
        ${CorradeUtility_GracefulAssert_SRCS})
    target_include_directories(CorradeUtility PUBLIC
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}/src)
    # Require (at least) C++11 for users
    set_property(TARGET CorradeUtility PROPERTY
        INTERFACE_CORRADE_CXX_STANDARD 11)
    set_property(TARGET CorradeUtility APPEND PROPERTY
        COMPATIBLE_INTERFACE_NUMBER_MAX CORRADE_CXX_STANDARD)
    set_target_properties(CorradeUtility PROPERTIES DEBUG_POSTFIX "-d")
    if(NOT CORRADE_BUILD_STATIC)
        set_target_properties(CorradeUtility PROPERTIES VERSION ${CORRADE_LIBRARY_VERSION} SOVERSION ${CORRADE_LIBRARY_SOVERSION})
    elseif(CORRADE_BUILD_STATIC_PIC)
        set_target_properties(CorradeUtility PROPERTIES POSITION_INDEPENDENT_CODE ON)
    endif()

    # Path::libraryLocation() needs this
    if(CORRADE_TARGET_UNIX)
        target_link_libraries(CorradeUtility PUBLIC ${CMAKE_DL_LIBS})
    endif()
    # AndroidLogStreamBuffer class needs to be linked to log library
    if(CORRADE_TARGET_ANDROID)
        target_link_libraries(CorradeUtility PUBLIC log)
    endif()

    install(TARGETS CorradeUtility
            RUNTIME DESTINATION ${CORRADE_BINARY_INSTALL_DIR}
            LIBRARY DESTINATION ${CORRADE_LIBRARY_INSTALL_DIR}
            ARCHIVE DESTINATION ${CORRADE_LIBRARY_INSTALL_DIR})
    install(FILES ${CorradeUtility_HEADERS} DESTINATION ${CORRADE_INCLUDE_INSTALL_DIR}/Utility)

    if(CORRADE_BUILD_TESTS)
        # Library with graceful assert, function-pointer-based CPU dispatch
        # and WASM SIMD128 for testing. If tests need such behavior, they
        # should link to CorradeTestSuiteTestLib instead. Otherwise the
        # implicitly linked CorradeTestSuite would drag in CorradeUtility in
        # addition to CorradeUtilityTestLib, leading to ODR violations and
        # making ASan builds fail.
        add_library(CorradeUtilityTestLib ${SHARED_OR_STATIC} ${EXCLUDE_FROM_ALL_IF_TEST_TARGET}
            $<TARGET_OBJECTS:CorradeUtilityObjects>
            ${CorradeUtility_GracefulAssert_SRCS})
        target_include_directories(CorradeUtilityTestLib PUBLIC
            $<TARGET_PROPERTY:CorradeUtility,INTERFACE_INCLUDE_DIRECTORIES>)
        target_link_libraries(CorradeUtilityTestLib PUBLIC
            $<TARGET_PROPERTY:CorradeUtility,INTERFACE_LINK_LIBRARIES>)
        target_compile_definitions(CorradeUtilityTestLib PRIVATE "CORRADE_GRACEFUL_ASSERT")
        if(CORRADE_BUILD_TESTS_FORCE_CPU_POINTER_DISPATCH)
            target_compile_definitions(CorradeUtilityTestLib PUBLIC "CORRADE_UTILITY_FORCE_CPU_POINTER_DISPATCH")
            if(CORRADE_BUILD_TESTS_FORCE_WASM_SIMD128)
                target_compile_options(CorradeUtilityTestLib PUBLIC "-msimd128")
            endif()
        endif()
        set_target_properties(CorradeUtilityTestLib PROPERTIES DEBUG_POSTFIX "-d")
        if(CORRADE_BUILD_STATIC_PIC)
            set_target_properties(CorradeUtilityTestLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
        endif()

        add_subdirectory(Test ${EXCLUDE_FROM_ALL_IF_TEST_TARGET})
    endif()

    # Corrade::Utility target alias for superprojects
    add_library(Corrade::Utility ALIAS CorradeUtility)
endif()

# TODO: https://cmake.org/pipermail/cmake-developers/2015-January/024242.html
# mentions that in the future it might be possible to not require external
# corrade-rc when generating WinRT targets by setting
#  set_target_properties(corrade-rc PROPERTIES VS_WINRT_COMPONENT OFF)
# Then this would be if(NOT CMAKE_CROSSCOMPILING OR CORRADE_TARGET_WINDOWS_RT).
# However, it seems like this feature never materialized, as doing this will
# result in corrade-rc that's looking for vcruntime140_app.dll in order to be
# run. Last checked: Nov 2019.
if(NOT CMAKE_CROSSCOMPILING)
    # Sources for standalone corrade-rc
    set(CorradeUtilityRc_SRCS
        Arguments.cpp
        Debug.cpp
        Configuration.cpp
        ConfigurationGroup.cpp
        ConfigurationValue.cpp
        Format.cpp
        Path.cpp
        String.cpp

        Implementation/ErrorString.cpp

        ../Cpu.cpp
        ../Containers/String.cpp
        ../Containers/StringIterable.cpp
        ../Containers/StringView.cpp)
    if(CORRADE_TARGET_WINDOWS)
        list(APPEND CorradeUtilityRc_SRCS
            # Needed for dealing with the design failure that's called "Unicode WINAPI"
            Unicode.cpp)

        # Functionality specific to static Windows builds. Not really needed
        # for corrade-rc as it doesn't load any plugins yet but easier than
        # having a special handling.
        if(NOT CORRADE_TARGET_WINDOWS_RT)
            list(APPEND CorradeUtilityRc_SRCS Implementation/WindowsWeakSymbol.cpp)
        endif()
    endif()
    if(CORRADE_TARGET_APPLE)
        list(APPEND CorradeUtilityRc_SRCS
            # Needed for isSandboxed() used by some Path APIs
            System.cpp)
    endif()

    # Having a tool used during a build depend on a shared library brings a lot
    # of annoyances especially for CMake and RPATH beginners (and Windows users
    # as there is no RPATH), so I'm making it dependency-less here by compiling
    # everything the executable needs directly inside. In case the library is
    # already built as static, there's no need to do any extra work -- just
    # link it.
    if(NOT CORRADE_BUILD_STATIC OR NOT CORRADE_WITH_UTILITY)
        add_executable(corrade-rc rc.cpp ${CorradeUtilityRc_SRCS})
        # Same as Utility, but since we can't depend on it, we can't use its
        # INTERFACE_INCLUDE_DIRECTORIES
        target_include_directories(corrade-rc PRIVATE
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_BINARY_DIR}/src)
        target_compile_definitions(corrade-rc PRIVATE "CORRADE_BUILD_STATIC")
    else()
        add_executable(corrade-rc rc.cpp)
        target_link_libraries(corrade-rc PRIVATE CorradeUtility)
    endif()
    target_link_libraries(corrade-rc PRIVATE CorradeMain)
    # Path::libraryLocation() needs this
    if(CORRADE_TARGET_UNIX)
        target_link_libraries(corrade-rc PRIVATE ${CMAKE_DL_LIBS})
    endif()
    install(TARGETS corrade-rc DESTINATION ${CORRADE_BINARY_INSTALL_DIR})

    # Corrade::rc target alias for superprojects
    add_executable(Corrade::rc ALIAS corrade-rc)
endif()
