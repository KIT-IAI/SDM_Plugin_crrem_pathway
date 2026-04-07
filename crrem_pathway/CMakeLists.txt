# CMakeList.txt: CMake-Projekt für "crrem_pathway". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_library(crrem_pathway SHARED
            CRREMPathwayDlg.cpp
            CRREMPathwayDlg.h
            CRREMPathwayDlgImpl.cpp
            CRREMPathwayDlgImpl.h
            CRREMPathwayPlugin.cpp
            CRREMPathwayPlugin.h
            CRREMPathway.cpp
            CRREMPathway.h
            CRREMDiagram.cpp
            CRREMDiagram.h
            CRREMData.h
            PluginObserver.cpp
            PluginObserver.h
            GoogleGeocode.h
            GoogleGeocode.cpp
            NaisConnector.h
            NaisConnector.cpp
            DataAnalyzer.h
            DataAnalyzer.cpp
            CRREMEnergyDlgImpl.h
            CRREMEnergyDlgImpl.cpp)

target_compile_options(crrem_pathway PRIVATE /utf-8)
target_link_libraries(crrem_pathway PRIVATE ifcexplorer_plugin stdutils ifcdb gps geographiclib)
target_link_libraries(crrem_pathway PRIVATE wx::core wx::base)
target_link_libraries(crrem_pathway PRIVATE CURL::libcurl)
target_link_libraries(crrem_pathway PRIVATE unofficial::sqlite3::sqlite3)

set_target_properties(glaser_calc_plugin PROPERTIES VCPKG_MANIFEST_MODE FALSE)
