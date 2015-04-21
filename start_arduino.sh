rm ./bin/homeio_main_arduino
g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread src/mains/main_arduino.cpp -o bin/homeio_main_arduino
sudo bin/homeio_main_arduino
#sudo gdb bin/homeio_main_arduino
