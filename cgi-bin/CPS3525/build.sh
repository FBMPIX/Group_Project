g++ -std=c++11 -Wall -c get_validate_input.cpp   
g++ -std=c++11 -Wall -c style.cpp
g++ -std=c++11 -Wall -c game_function.cpp
g++ -std=c++11 -Wall -c main.cpp      
g++ -o project3 main.o get_validate_input.o style.o game_function.o
