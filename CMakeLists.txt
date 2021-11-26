cmake_minimum_required(VERSION 3.20)
project(cursword)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cursword)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

target_include_directories(cursword
	PRIVATE
		/usr/include/sword
)

target_link_libraries(cursword
	PRIVATE
		${CURSES_LIBRARIES}
		sword
)

add_subdirectory(src)
