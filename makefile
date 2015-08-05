OPTIM_FLAG = -O3
WARNING_FLAG = -Wall
FULL_WARNING_FLAG = -Wall -Wextra

hello: ;
	@echo Hello

run: prepare-dir clean build exec;
run-dev: prepare-dir clean build-warning exec;
debug: prepare-dir clean build-debug exec-debug;
setup: debian-deps prepare-dir;
	
prepare-dir:
	[ -d bin ] || mkdir bin
	[ -d data ] || mkdir data
	
build: ;
	g++ $(WARNING_FLAG) $(OPTIM_FLAG) -std=gnu++11 -I /usr/include -lpthread -lcurlpp -lcurl src/mains/main_${SITE}.cpp -o bin/homeio_main_${SITE}

build-warning:
	g++ $(FULL_WARNING_FLAG) -std=gnu++11 -I /usr/include -lpthread -lcurlpp -lcurl src/mains/main_${SITE}.cpp -o bin/homeio_main_${SITE}
	
build-debug: ;
	g++ -std=gnu++11 -g -oterm -I /usr/include -lpthread -lcurlpp -lcurl src/mains/main_${SITE}.cpp -o bin/homeio_main_${SITE}
	
exec: ;
	sudo bin/homeio_main_${SITE}

exec-debug: ;
	gdb bin/homeio_main_${SITE}
	
debian-deps: ; 
	sudo apt-get install libcurl4-openssl-dev git g++-4.9 cpp-4.9 libcurlpp0 libcurlpp-dev
	
clean: ;
	[ -e bin ] || rm -v ./bin/*
	
