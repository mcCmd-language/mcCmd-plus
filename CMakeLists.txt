# CMakeList.txt: mcCmd-plus에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

project ("mcCmd-plus")

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (mcCmd-plus  "main.cpp" "api/2913.cpp" "api/2913.h" "src/command.cpp" "src/tokenizing.cpp" "src/command.h" "src/tokenizing.h")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/api/)
set(
        SRC_FILES
        "api/2913.cpp")
set(
    INCLUDE_FILES
    "api/2913.h"
)

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
