rm ./bin/homeio_main_dev_offline
g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread src/mains/main_dev_offline.cpp -o bin/homeio_main_dev_offline
#gdb ./homeio
