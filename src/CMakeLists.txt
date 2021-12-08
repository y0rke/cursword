target_sources(cursword
	PRIVATE
		main.cpp
		txtutils.cpp
		bible.cpp
		display.cpp
)

target_include_directories(cursword
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)

