g++ -std=c++11 -Wall -c get_validate_input.cpp
g++ -std=c++11 -Wall -c generate_html.cpp
g++ -std=c++11 -Wall -c pattern_analyzer.cpp
g++ -std=c++11 -Wall -c main.cpp
g++ -o project3 main.o get_validate_input.o pattern_analyzer.o generate_html.o -lcgicc
chmod 705 project3