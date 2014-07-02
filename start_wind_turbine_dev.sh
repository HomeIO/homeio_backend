rm ./bin/homeio_wind_turbine
g++ -std=gnu++11 -g -oterm -lpthread main.cpp -o bin/homeio_wind_turbine
./homeio
#gdb ./homeio
