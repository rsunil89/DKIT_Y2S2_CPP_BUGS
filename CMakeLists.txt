cmake_minimum_required(VERSION 3.10.0)
project(BugLife VERSION 1.0.0 LANGUAGES C CXX)

add_executable(BugLife 
    main.cpp 
    Bug.cpp 
    Crawler.cpp 
    Hopper.cpp 
    BugBoard.cpp
)
