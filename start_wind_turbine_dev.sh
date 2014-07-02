rm ./bin/homeio_wind_turbine
g++ -std=gnu++11 -g -oterm -lpthread src/mains/main_wind_turbine_dev.cpp -o bin/homeio_wind_turbine
bin/homeio_wind_turbine
#gdb bin/homeio_wind_turbine
