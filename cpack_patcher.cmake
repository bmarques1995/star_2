function(add_cpack_config)

    set(oneValueArgs "CONFIG_IN_FILE" "CONFIG_OUT_FILE" "WIX_INTERFACE_FILE" "WIX_UPGRADE_GUID")
    set(options "USE_SHARE" "INSTALL_RUNTIME")
    set(multiValueArgs "WIX_EXTENSIONS" "WIX_EXTRA_SOURCES" "WIX_UI")
    cmake_parse_arguments(PACKAGE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    configure_file(
        "${PROJECT_SOURCE_DIR}/LICENSE"
        "${PROJECT_BINARY_DIR}/LICENSE.txt"
        COPYONLY
    )
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_BINARY_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_PACKAGE_VENDOR ${PROJECT_NAME})
    if(WIN32)
        set(CPACK_GENERATOR "WIX")
        set(CPACK_WIX_VERSION "4")
        set(CPACK_WIX_UPGRADE_GUID
            "C411F940-3D69-482A-A5EC-229F8A734F14"
        )

        #set(CPACK_WIX_PRODUCT_ICON
        #    "@CMake_SOURCE_DIR@/Utilities/Release/CMakeLogo.ico"
        #)

        #set(CPACK_WIX_UI_BANNER
        #    "@CMake_SOURCE_DIR@/Utilities/Release/WiX/ui_banner.jpg"
        #)

        #set(CPACK_WIX_UI_DIALOG
        #    "@CMake_SOURCE_DIR@/Utilities/Release/WiX/ui_dialog.jpg"
        #)

        # Nossa UI customizada.
        #
        # IMPORTANTE:
        # Não é WixUI_InstallDir.
        # É o ID que vamos criar em wix_ui.wxs.
        set(CPACK_WIX_UI_REF "StarlangUI")

        # CPack normalmente já inclui WixToolset.UI.wixext,
        # mas deixamos explícito.
        set(CPACK_WIX_EXTENSIONS
            "WixToolset.UI.wixext"
            "WixToolset.Util.wixext"
        )

        #set(CPACK_WIX_PRODUCT_ICON
        #    "${CMAKE_CURRENT_SOURCE_DIR}/installer/star.ico"
        #)

        set(CPACK_WIX_EXTRA_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/wix/StarlangUI.wxs"
        )
        set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/wix/wix_patch.xml")

        set(CPACK_WIX_INSTALL_SCOPE "perMachine")
        set(CPACK_WIX_CUSTOM_ACTION_PROPERTIES "STARLANG_ADD_TO_PATH")

        set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
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
endfunction()

macro(set_cpack_defaults)
    set(oneValueArgs "LICENSE_IN_FILE" "LICENSE_OUT_FILE")
    set(options)
    set(multiValueArgs)
    cmake_parse_arguments(PACKER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if(NOT DEFINED PACKER_LICENSE_IN_FILE)
        message(FATAL_ERROR "LICENSE_IN_FILE is a mandatory argument")
    endif()
    if(DEFINED PACKER_LICENSE_OUT_FILE)
        configure_file(
            "${PACKER_LICENSE_IN_FILE}"
            "${PACKER_LICENSE_OUT_FILE}"
            COPYONLY
        )
    else()
        set(PACKER_LICENSE_OUT_FILE ${PACKER_LICENSE_IN_FILE})
    endif()
    
    set(CPACK_RESOURCE_FILE_LICENSE "${PACKER_LICENSE_OUT_FILE}")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_PACKAGE_VENDOR ${PROJECT_NAME})
endmacro()

macro(add_cpack_wix_configs)
    if(WIN32)

        set(oneValueArgs "UPGRADE_GUID" "UI" "PATCH")
        set(options)
        set(multiValueArgs "EXTENSIONS" "EXTRA_SOURCES"  "EXTRA_ACTIONS")
        cmake_parse_arguments(WIX_PACKER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
        if((NOT DEFINED WIX_PACKER_UPGRADE_GUID) OR
            (NOT DEFINED WIX_PACKER_UI))
            message(FATAL_ERROR "UI and UPGRADE_GUID are mandatory")
        endif()
        set(CPACK_GENERATOR "WIX")
        set(CPACK_WIX_VERSION "4")
        set(CPACK_WIX_UPGRADE_GUID
            ${WIX_PACKER_UPGRADE_GUID}
        )

        #set(CPACK_WIX_PRODUCT_ICON
        #    "@CMake_SOURCE_DIR@/Utilities/Release/CMakeLogo.ico"
        #)

        #set(CPACK_WIX_UI_BANNER
        #    "@CMake_SOURCE_DIR@/Utilities/Release/WiX/ui_banner.jpg"
        #)

        #set(CPACK_WIX_UI_DIALOG
        #    "@CMake_SOURCE_DIR@/Utilities/Release/WiX/ui_dialog.jpg"
        #)

        set(CPACK_WIX_UI_REF ${WIX_PACKER_UI})

        # CPack normalmente já inclui WixToolset.UI.wixext,
        # mas deixamos explícito.
        if(DEFINED WIX_PACKER_EXTENSIONS)
            set(CPACK_WIX_EXTENSIONS ${WIX_PACKER_EXTENSIONS})
        endif()

        #set(CPACK_WIX_PRODUCT_ICON
        #    "${CMAKE_CURRENT_SOURCE_DIR}/installer/star.ico"
        #)

        if(DEFINED WIX_PACKER_EXTRA_SOURCES)
            set(CPACK_WIX_EXTRA_SOURCES ${WIX_PACKER_EXTRA_SOURCES})
        endif()
        if(DEFINED WIX_PACKER_PATCH)
            set(CPACK_WIX_PATCH_FILE ${WIX_PACKER_PATCH})
        endif()
        set(CPACK_WIX_INSTALL_SCOPE "perMachine")
        if(DEFINED WIX_PACKER_EXTRA_ACTIONS)
            set(CPACK_WIX_CUSTOM_ACTION_PROPERTIES ${WIX_PACKER_EXTRA_ACTIONS})
        endif()
        set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    endif()
endmacro()

macro(add_cpack_unix_packers)
    if(NOT WIN32)
        if(APPLE)
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
    endif()
endmacro()