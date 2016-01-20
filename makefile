COMPILER ?= g++
#COMPILER ?= clang++
OPTIM_FLAG = -O3
NO_OPTIM_FLAG = -O0
WARNING_FLAG = -Wall
FULL_WARNING_FLAG = -Wall -Wextra -pedantic -ansi -Wabi -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs  -Woverloaded-virtual -Wredundant-decls  -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wconversion
FULL_WARNING_FLAG_GCC = -Wlogical-op -Wnoexcept -Wstrict-null-sentinel
# http://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings
POSSIBLE_TODO_WARNING_FLAG = -Waggregate-return -Wmissing-declarations   -Wshadow -Wsign-promo  -Wsign-conversion
CPP_PATHS = src/backend/*.cpp src/backend/addons/*.cpp src/backend/io/*.cpp src/backend/ncurses/*.cpp src/backend/log/*.cpp src/backend/meas/*.cpp src/backend/action/*.cpp src/backend/overseer/*.cpp src/backend/tcp/*.cpp src/backend/utils/*.cpp
EXTRA_LINK_FLAG = -static-libstdc++
LINK_FLAG = -lpthread -lcurlpp -lcurl -lncurses -lmenu
#STANDARD_FLAG = -std=gnu++11
STANDARD_FLAG = -std=gnu++14
MULTICORE_FLAG = -pipe

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
	$(COMPILER) $(FULL_WARNING_FLAG) $(OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

build-warning:
	$(COMPILER) $(NO_OPTIM_FLAG) $(FULL_WARNING_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

build-debug: ;
	$(COMPILER) $(FULL_WARNING_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) -g -oterm -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

exec: ;
	sudo bin/homeio_main_${SITE}

exec-debug: ;
	gdb bin/homeio_main_${SITE}

debian-deps: ;
	echo "g++ at least 4.9 is needed"
	sudo apt-get install libcurl4-openssl-dev git g++ libcurlpp0 libcurlpp-dev

format: ;
	astyle --style=google --indent=spaces=2 --suffix=none --verbose --recursive *.?pp

clean: ;
	[ -e bin ] || rm -v ./bin/*
