cmake_minimum_required(VERSION 3.15)

project(spaceship-generator
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

add_executable(spaceship-generator
    src/main.cpp
)

file(GLOB SPACESHIP_PART_FILES
    "${CMAKE_SOURCE_DIR}/data/*.txt"
)
add_custom_command(TARGET spaceship-generator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${SPACESHIP_PART_FILES} ${CMAKE_BINARY_DIR}/
)

# Options to add compiler warning flags, and turn on warnings as errors.
if(MSVC)
    target_compile_options(spaceship-generator
    PRIVATE
        /MP                     # Enable multi-processor compilation

        /experimental:external  # Enable external compiler options
        /external:anglebrackets # Treat all headers included via angle brackets as external headers
        /external:W0            # Turn off warnings for external headers

        /permissive-            # Enforce standards conformance

        /W4     # Enable warning level 4
        /WX     # Treat warnings as errors
        /w14242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
        /w14254 # 'operator': conversion from 'type1' to 'type2', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14265 # 'class': class has virtual functions, but destructor is not virtual
        /w14287 # 'operator': unsigned/negative constant mismatch
        /w14289 # nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
        /w14296 # 'operator': expression is always false
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555 # expression has no effect; expected expression with side-effect
        /w14640 # 'instance': construction of local static object is not thread-safe
        /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    )
    add_compile_definitions(
        _WIN32_WINNT=0x0602
        WIN32_LEAN_AND_MEAN
    )
else()
    # Common compile options for gcc and clang
    list(APPEND commonCompileOptions
        -Werror                     # Warnings as errors
        -Wall                       # Good subset of warnings enabled
        -Wextra                     # Additional good subset of warnings enabled
        -Wpedantic                  # Warn if non-standard C++ is used
        -Wshadow                    # warn the user if a variable declaration shadows one from a parent context
        -Wold-style-cast            # warn for c-style casts
        -Wcast-align                # warn for potential performance problem casts
        -Wunused                    # warn on anything being unused
        -Woverloaded-virtual        # warn if you overload (not override) a virtual function
        -Wconversion                # warn on type conversions that may lose data
        -Wsign-conversion           # warn on sign conversions
        -Wdouble-promotion          # (GCC >= 4.6, Clang >= 3.8) warn if float is implicit promoted to double
        -Wformat=2                  # warn on security issues around functions that format output (ie printf)
    )

    if(GNU)
        target_compile_options(smartnode-server
        PRIVATE
            ${commonCompileOptions}
            -Wduplicated-cond           # (only in GCC >= 6.0) warn if if / else chain has duplicated conditions
            -Wduplicated-branches       # (only in GCC >= 7.0) warn if if / else branches have duplicated code
            -Wlogical-op                # (only in GCC) warn about logical operations being used where bitwise were probably wanted
            -Wnull-dereference          # (only in GCC >= 6.0) warn if a null dereference is detected
            -Wuseless-cast              # (only in GCC >= 4.8) warn if you perform a cast to the same type
        )
    elseif()
        target_compile_options(smartnode-server
        PRIVATE
            ${commonCompileOptions}
        )
    endif()
endif()
