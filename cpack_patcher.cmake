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
    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
endmacro()

macro(add_cpack_wix_configs)
    if(WIN32)

        set(oneValueArgs "UPGRADE_GUID" "UI" "PATCH" "ICON" "BANNER" "DIALOG")
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

        if(DEFINED WIX_PACKER_ICON)
            set(CPACK_WIX_PRODUCT_ICON ${WIX_PACKER_ICON})
        endif()
        if(DEFINED WIX_PACKER_BANNER)
            set(CPACK_WIX_UI_BANNER ${WIX_PACKER_BANNER})
        endif()
        if(DEFINED WIX_PACKER_DIALOG)
            set(CPACK_WIX_UI_DIALOG ${WIX_PACKER_DIALOG})
        endif()
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
        set(oneValueArgs "MAINTAINER" "URL" "LICENSE" "DESC" "COMMENT" "LICENSE_FILE")
        set(options)
        set(multiValueArgs)
        cmake_parse_arguments(UNIX_PACKER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        if((NOT DEFINED UNIX_PACKER_MAINTAINER) OR
           (NOT DEFINED UNIX_PACKER_URL) OR
           (NOT DEFINED UNIX_PACKER_LICENSE) OR
           (NOT DEFINED UNIX_PACKER_DESC) OR
           (NOT DEFINED UNIX_PACKER_COMMENT) OR
           (NOT DEFINED UNIX_PACKER_LICENSE_FILE))
            message(FATAL_ERROR "MAINTAINER, URL, LICENSE, DESC, COMMENT and LICENSE_FILE are mandatory")
        endif()

        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${UNIX_PACKER_COMMENT})
        set(CPACK_PACKAGE_DESCRIPTION ${UNIX_PACKER_DESC})
        set(CPACK_PACKAGE_HOMEPAGE_URL ${UNIX_PACKER_URL})
        if(APPLE)
            set(CPACK_GENERATOR "productbuild") 
        elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
            set(CPACK_GENERATOR "FREEBSD")
            
            # Metadados obrigatórios exigidos pelo gerenciador pkg do FreeBSD:
            set(CPACK_FREEBSD_PACKAGE_MAINTAINER ${UNIX_PACKER_MAINTAINER})
            set(CPACK_FREEBSD_PACKAGE_LICENSE ${UNIX_PACKER_LICENSE}) # Ou a licenca correspondente
        elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
            set(CPACK_GENERATOR "TXZ;DEB;RPM")
            
            set(CPACK_RPM_PACKAGE_MAINTAINER ${UNIX_PACKER_MAINTAINER})
            set(CPACK_RPM_PACKAGE_LICENSE ${UNIX_PACKER_LICENSE})

            # Metadados essenciais exigidos para gerar pacotes DEB e RPM sem erros:
            set(CPACK_PACKAGE_CONTACT ${UNIX_PACKER_MAINTAINER})
        endif()

        install(
            FILES "${UNIX_PACKER_LICENSE_FILE}" 
            DESTINATION "share/doc/${CPACK_PACKAGE_NAME}"
            RENAME "copyright"
        )
    endif()
endmacro()