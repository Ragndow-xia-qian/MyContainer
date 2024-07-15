cmake_minimum_required(VERSION 3.26)
project(MyContainer)

set(CMAKE_CXX_STANDARD 26)

add_executable(MyContainer
        main.cpp
        XQ/Container/Sequence/ArrayList.h
        XQ/Memory/Allocator.h
        XQ/Container/Sequence/LinkedList.h
        XQ/Container/Associate/HashMap.h
        XQ/Stream/InOut/Print.h
        XQ/Container/Associate/Hash.h
        XQ/Container/Sequence/String.h
        XQ/Maths/Planar/Point.h
        XQ/Maths/Planar/Vector2D.h
        XQ/Maths/Planar/Line.h
        XQ/Maths/Planar/LineSegment.h
        XQ/Maths/Planar/Circle.h
        XQ/Maths/Planar/Polygon.h
        XQ/Container/Sequence/Span.h
        XQ/Stream/InOut/Scan.h
        XQ/Tools/Expected.h
        XQ/Tools/Generator.h
        XQ/Container/Sequence/DynamicSpan.h
        XQ/Tools/Variant.h
        XQ/Maths/Calculate/Algorithm.h
        XQ/Maths/Googology/Ordinals.h
        XQ/Tools/Any.h
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
