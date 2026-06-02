cmake_minimum_required(VERSION 3.10)

project(AirportSimulation C)

set(CMAKE_C_STANDARD 11)

add_executable(AirportSimulation
    main.c
    queue.c
    simulation.c
    fileio.c
)
configure_file(config.txt config.txt COPYONLY)