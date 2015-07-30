hello: ;
	@echo Hello

prepare-dir:
	[ -d bin ] || mkdir bin
	[ -d data ] || mkdir data
	
debian-deps: ; 
	sudo apt-get install libcurl4-openssl-dev git g++-4.9 cpp-4.9
	
clean: ;
	[ -e bin ] || rm -v ./bin/*
	
build: clean ;
	g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread -lcurlpp src/mains/main_dev.cpp -o bin/homeio_main_dev
	g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread -lcurlpp src/mains/main.cpp -o bin/homeio_main
	g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread -lcurlpp src/mains/main_arduino.cpp -o bin/homeio_main_main_arduino
	
build_dev: clean ;
	g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread -lcurlpp -lcurl src/mains/main_dev.cpp -o bin/homeio_main_dev
	
run_dev: build_dev ;	
	sudo bin/homeio_main_dev

debug_dev: build ;	
	sudo gdb bin/homeio_main