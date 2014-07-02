rm ./bin/homeio_main_dev_io_server
g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread src/mains/main_dev_io_server.cpp -o bin/homeio_main_dev_io_server
#gdb ./homeio
bin/homeio_main_dev_io_server
