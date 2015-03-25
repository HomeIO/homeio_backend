rm ./bin/homeio_main_dev
g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread src/mains/main_dev.cpp -o bin/homeio_main_dev
sudo bin/homeio_main_dev
#sudo gdb bin/homeio_main
