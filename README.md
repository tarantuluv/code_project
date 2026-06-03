How to build and run:
Visual Studio: 
Please, open Visual Studio, Select File -> Open-> Folder and choose this project folder.
And just run the project




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
