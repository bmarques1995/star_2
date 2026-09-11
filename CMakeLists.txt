cmake_minimum_required(VERSION 3.28)

project(Starlang VERSION 0.1.0)

option(BUILD_CLI "Build the command line interface" ON)
option(BUILD_TESTS "Build tests" ON)

option(ENABLE_LLVM_COVERAGE "Enable LLVM source coverage" OFF)

macro(enable_coverage target)
    if(ENABLE_LLVM_COVERAGE)
        target_compile_options(${target} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${target} PRIVATE -fprofile-instr-generate)
    endif()
endmacro()

if(VCPKG_PREFIX_PATH AND VCPKG_TARGET_TRIPLET)
    set(CMAKE_PREFIX_PATH "${VCPKG_PREFIX_PATH}")
endif()

include(FetchContent)

set(ARTIFACT_PREFIX ${PROJECT_BINARY_DIR})
if(WIN32)
    set(ARTIFACT_PREFIX ${CMAKE_INSTALL_PREFIX})
endif()

if(USES_VCPKG)
    set(VCPKG_CURRENT_PREFIX ${VCPKG_PREFIX_PATH}/${VCPKG_TARGET_TRIPLET})
    list(APPEND CMAKE_PREFIX_PATH ${VCPKG_CURRENT_PREFIX})
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(VCPKG_CURRENT_PREFIX ${VCPKG_CURRENT_PREFIX}/debug)
    endif()
    if(WIN32)
        set(ARTIFACT_PREFIX ${VCPKG_CURRENT_PREFIX})
    endif()
endif()

# 2. Declare the download location and version details
FetchContent_Declare(
    cmake_helpers
    GIT_REPOSITORY https://github.com/bmarques1995/cmake_helpers.git
    #GIT_TAG        v1.14.0 # Best practice: Use a specific tag or commit hash
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/cmake_helpers
)

# 3. Download the library and make its targets available
FetchContent_MakeAvailable(cmake_helpers)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_helpers")

include(target_destination)

include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
configure_file(
    "${PROJECT_SOURCE_DIR}/LICENSE"
    "${PROJECT_BINARY_DIR}/LICENSE.txt"
    COPYONLY
)
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_BINARY_DIR}/LICENSE.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
if(WIN32)
    set(CPACK_GENERATOR "WIX")
    set(CPACK_WIX_VERSION "4")
    set(CPACK_WIX_UPGRADE_GUID
        "C411F940-3D69-482A-A5EC-229F8A734F14"
    )

    # Nossa UI customizada.
    #
    # IMPORTANTE:
    # Não é WixUI_InstallDir.
    # É o ID que vamos criar em wix_ui.wxs.
    set(CPACK_WIX_UI_REF "WixUI_InstallDir")

    # CPack normalmente já inclui WixToolset.UI.wixext,
    # mas deixamos explícito.
    set(CPACK_WIX_EXTENSIONS
        "WixToolset.UI.wixext"
    )

    set(CPACK_WIX_EXTRA_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/wix_path.wxs"
    )

    set(CPACK_PACKAGE_INSTALL_DIRECTORY "Starlang")
elseif(APPLE)
    set(CPACK_GENERATOR "productbuild") 
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    set(CPACK_GENERATOR "FREEBSD")
    
    # Metadados obrigatórios exigidos pelo gerenciador pkg do FreeBSD:
    set(CPACK_FREEBSD_PACKAGE_MAINTAINER "seu-email@provedor.com")
    set(CPACK_FREEBSD_PACKAGE_COMMENT "Uma breve descricao do seu programa")
    set(CPACK_FREEBSD_PACKAGE_DESCRIPTION "Uma descricao mais detalhada do funcionamento do programa.")
    set(CPACK_FREEBSD_PACKAGE_WWW "https://seu-site.com")
    set(CPACK_FREEBSD_PACKAGE_LICENSE "BSD-3-Clause") # Ou a licenca correspondente
else()
    set(CPACK_GENERATOR "TXZ;DEB;RPM")
    
    # Metadados essenciais exigidos para gerar pacotes DEB e RPM sem erros:
    set(CPACK_PACKAGE_CONTACT "seu-email@provedor.com")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Uma breve descricao do programa para Linux")          # .tar.xz para Linux (Ubuntu, Fedora, etc.)
endif()
include(CPack)

add_subdirectory(src)