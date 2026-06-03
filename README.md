How to build and run:
Visual Studio: 
Please, open Visual Studio, Select File -> Open-> Folder and choose this project folder.
And just run the project
I apologize for the inconvenience, but unfortunately I cannot provide a .sln file because Visual Studio cannot be installed on my MacOS. Therefore, I used Visual Studio Code and added CMake support so that the project can be opened and built in Visual Studio without any problems.



Other:
Using CMake:
mkdir build
cd build
cmake ..
cmake --build .
./AirportSimulation

Using gcc:
gcc main.c simulation.c queue.c fileio.c -o airport
./airport
