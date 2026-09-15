cmake_minimum_required(VERSION 3.10)
project(VesaBall CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# SDL2/SDL2_image: try pkg-config first (the most consistently available
# route on Linux/macOS), fall back to CMake's own find_package() modules
# (e.g. for a vcpkg-based Windows setup) if that's not available.
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
	pkg_check_modules(SDL2 QUIET sdl2)
	pkg_check_modules(SDL2_IMAGE QUIET SDL2_image)
endif()

if(NOT SDL2_FOUND)
	find_package(SDL2 REQUIRED)
endif()
if(NOT SDL2_IMAGE_FOUND)
	find_package(SDL2_image REQUIRED)
endif()

add_executable(VesaBall
	VesaBall.cpp
	StdAfx.cpp
	App.cpp
	Scene.cpp
	Timer.cpp
	Sprite.cpp
	SpriteMoving.cpp
	SpriteEffect.cpp
	SpriteAnimated.cpp
	Ball.cpp
	Brick.cpp
	BrickArray.cpp
	Bonus.cpp
	Paddle.cpp
	Counter.cpp
	Cursor.cpp
	MenuItem.cpp
	GameBoard.cpp
	GameEditor.cpp
	GameEngine.cpp
	GameMenu.cpp
)

target_include_directories(VesaBall PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_directories(VesaBall PRIVATE ${SDL2_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS})
target_link_libraries(VesaBall PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

# Copy the runtime assets next to the built executable, so an out-of-source
# build (e.g. `build/VesaBall`) can be run directly without needing to be
# launched from the repository root.
add_custom_command(TARGET VesaBall POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/gfx $<TARGET_FILE_DIR:VesaBall>/gfx
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/lev $<TARGET_FILE_DIR:VesaBall>/lev
)
