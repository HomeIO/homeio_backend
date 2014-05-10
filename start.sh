rm ./homeio
#g++ -Wall -oterm -lpthread main.cpp -o homeio
g++ -std=gnu++11 -g -oterm -lpthread main.cpp -o homeio
./homeio
#gdb ./homeio
