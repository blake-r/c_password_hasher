project(passhash)
file(GLOB SOURCES *.c)
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:libpasshash> ${SOURCES})
add_dependencies(${PROJECT_NAME} libpasshash)
target_link_libraries(${PROJECT_NAME}  ${OPENSSL_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	INCLUDE_DIRECTORIES ${libpasshash_SOURCE_DIR})
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin)

