cmake_minimum_required(VERSION 3.10)

project(tests_unity C)

add_subdirectory(../Frameworks/unity unity)

set(CMAKE_C_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(INCLUDE_DIRS
    #../include/config
	#../include/avr
	../include
	#../include/compat
	#../include/util
	
	#../../../../Dokumente/MkClipse/AVR ToolchainT10/avr/include
	#../../../../Dokumente/MkClipse/AVR ToolchainT10
	#../../App/Lotto
)

set(TEST_INCLUDE_DIRS
	..
	.
)

set(SRCS
	../../App/Lotto/lotto.c
)

set(TEST_SRCS
	lotto/mock_lotto.c
	lotto/lotto_test.c
	
	dio/dio_test.c
	
	dio_test_runner.c
	lotto_test_runner.c
	test_main.c
)

set(GLOBAL_DEFINES

)

include_directories(${INCLUDE_DIRS} ${TEST_INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

link_libraries(unity)
add_executable(${PROJECT_NAME} ${SRCS} ${TEST_SRCS})
