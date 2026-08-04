add_definitions(-DTRANSLATION_DOMAIN=\"markdownpart\")

configure_file(markdownpart.json.in ${CMAKE_CURRENT_BINARY_DIR}/markdownpart.json @ONLY)

add_library(markdownpartbh MODULE)

target_sources(markdownpartbh PRIVATE
    searchtoolbar.cpp
    markdownview.cpp
    markdownpartfactory.cpp
    markdownpart.cpp
    markdownbrowserextension.cpp
    markdownview.hpp
    markdownbrowserextension.hpp
    markdownpart.hpp
    searchtoolbar.hpp
    markdownpartfactory.hpp

    markdownpart.qrc
)

ki18n_wrap_ui(markdownpartbh searchtoolbar.ui)

target_link_libraries(markdownpartbh
    KF${KF_MAJOR_VERSION}::I18n
    KF${KF_MAJOR_VERSION}::Parts
    KF${KF_MAJOR_VERSION}::ConfigCore
    Qt${QT_MAJOR_VERSION}::Widgets
)

install(TARGETS markdownpartbh  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/parts)
