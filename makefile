hello: ;
	@echo Hello

run: prepare-dir clean build exec;
debug: prepare-dir clean build-debug exec-debug;
setup: debian-deps prepare-dir;
	
prepare-dir:
	[ -d bin ] || mkdir bin
	[ -d data ] || mkdir data
	
build: ;
	g++ -std=gnu++11 -O3 -I /usr/include -lpthread -lcurlpp -lcurl src/mains/main_${SITE}.cpp -o bin/homeio_main_${SITE}
	
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
	
