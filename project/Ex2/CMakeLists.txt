# Add as a subdirectory SimpleRenderEngineProject using add_subdirectory in the bottom of the file
cmake_minimum_required(VERSION 3.23)
if (NOT DEFINED all_libs)
    message( FATAL_ERROR "This CMakeLists.txt must be included using the command add_subdirectory(name)" )
endif (NOT DEFINED all_libs)
# Ensure SimpleRenderEngineProject is up-to-date
IF (WIN32)
    IF (NOT DEFINED DLLFileList)
        message( FATAL_ERROR "The SimpleRenderEngineProject has been updated. Clone the project (https://github.com/mortennobel/SimpleRenderEngineProject) again the project again and start over." )
    ENDIF(NOT DEFINED DLLFileList)
ENDIF(WIN32)

add_executable(Exercise2-1 Exercise1-HelloPreprocessor/main.cpp)

add_executable(Exercise2-2 Exercise2-SquaredSecrets/main.cpp)

add_executable(Exercise2-3 Exercise3-MineSweeper/main.cpp Exercise3-MineSweeper/MineSweeper.cpp)

add_executable(Exercise2-3-GUI Exercise3-MineSweeper/main_gui.cpp Exercise3-MineSweeper/MineSweeper.cpp)
target_link_libraries(Exercise2-3-GUI ${all_libs})

# copy assets to binary dir
file(COPY Exercise3-MineSweeper/minesweeper.json Exercise3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Exercise3-MineSweeper/minesweeper.json Exercise3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY Exercise3-MineSweeper/minesweeper.json Exercise3-MineSweeper/minesweeper.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(Exercise2-3-GUI PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)