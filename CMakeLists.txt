cmake_minimum_required(VERSION 3.16)
project(evol)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${evol_SOURCE_DIR}/cmake")

set(SDL2_dir /Library/Frameworks/SDL2.framework/Headers)
set(SDL2_image_dir /Library/Frameworks/SDL2_image.framework/Headers)
set(SDL2_ttf_dir /Library/Frameworks/SDL2_ttf.framework/Headers)
set(SDL2_mixer_dir /Library/Frameworks/SDL2_mixer.framework/Headers)
set(OpenGL_dir /Library/Frameworks/OpenGL.framework/Headers/)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
        ${SDL2_dir}
        ${SDL2_image_dir}
        ${SDL2_ttf_dir}
        ${SDL2_mixer_dir}
        ${OPENGL_INCLUDE_DIRS}
)

add_executable(evol
        main.cpp
        src/Specie.cpp
        include/Specie.h
        src/Behavior.cpp
        include/Behavior.h
        src/Morphology.cpp
        include/Morphology.h
        src/Genome.cpp
        include/Genome.h
        species/eduito.h
        src/Specimen.cpp
        include/Specimen.h
        include/Timer.h
        src/Timer.cpp
        species/andreita.h src/Loader.cpp include/Loader.h include/Colors.h include/Drawable.h species/alpiste.h include/Text.h species/gordito.h)

target_link_libraries(evol
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDLTTF_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${OPENGL_LIBRARIES}
)
