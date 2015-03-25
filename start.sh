rm ./bin/homeio_main
g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread src/mains/main.cpp -o bin/homeio_main
sudo bin/homeio_main
#sudo gdb bin/homeio_main
