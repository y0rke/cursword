target_sources(cursword
	PRIVATE
		main.cpp
		format_text.cpp
)

target_include_directories(cursword
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}
)

