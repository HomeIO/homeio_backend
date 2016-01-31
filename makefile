COMPILER ?= g++
#COMPILER ?= clang++
ARM_CROSS_COMPILER = arm-linux-gnueabi-g++-5
ARCHITECTURE_FLAG = -march=native
OPTIM_FLAG = -O2
NO_OPTIM_FLAG = -O0
WARNING_FLAG = -Wall
# http://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings
POSSIBLE_TODO_WARNING_FLAG = -Wsign-promo -Waggregate-return
FULL_WARNING_FLAG_GCC = -Wno-unknown-warning-option -Wlogical-op -Wnoexcept -Wstrict-null-sentinel
BASIC_WARNING_FLAG = -Wall -Wextra -pedantic -ansi
FULL_WARNING_FLAG = $(BASIC_WARNING_FLAG) -Wabi -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs  -Woverloaded-virtual -Wredundant-decls  -Wstrict-overflow=2 -Wswitch-default -Wundef -Werror -Wno-unused -Wconversion -Wlogical-op -Wshadow -Wmissing-declarations -Wsign-conversion $(FULL_WARNING_FLAG_GCC)
CPP_PATHS = src/backend/*.cpp src/backend/addons/*.cpp src/backend/io/*.cpp src/backend/ncurses/*.cpp src/backend/log/*.cpp src/backend/meas/*.cpp src/backend/action/*.cpp src/backend/overseer/*.cpp src/backend/tcp/*.cpp src/backend/utils/*.cpp
EXTRA_LINK_FLAG = -static-libstdc++
LINK_FLAG = -lpthread -lcurlpp -lcurl -lncurses -lmenu
#STANDARD_FLAG ?= -std=gnu++11
STANDARD_FLAG ?= -std=gnu++14
MULTICORE_FLAG = -pipe
DEBUG_FLAGS = -d

hello: ;
	@echo Hello

run: prepare-dir clean build exec;
run-dev: prepare-dir clean build-warning exec;
run-strict: prepare-dir clean build-strict exec;
run-no-warning: prepare-dir clean build-no-warning exec;
debug: prepare-dir clean build-debug exec-debug;
setup: debian-deps prepare-dir;

prepare-dir:
	[ -d bin ] || mkdir bin
	[ -d data ] || mkdir data

build: ;
	$(COMPILER) $(ARCHITECTURE_FLAG) $(BASIC_WARNING_FLAG) $(OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) $(LINK_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE}

build-warning:
	$(COMPILER) $(ARCHITECTURE_FLAG) $(FULL_WARNING_FLAG) $(NO_OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) $(LINK_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE}

build-strict:
	$(COMPILER) $(ARCHITECTURE_FLAG) $(FULL_WARNING_FLAG) $(OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) $(LINK_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE}

build-no-warning:
	$(COMPILER) $(ARCHITECTURE_FLAG) $(NO_OPTIM_FLAG) $(STANDARD_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

# no worky
build-arm:
	$(ARM_CROSS_COMPILER) $(OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

build-debug: ;
	$(COMPILER) $(NO_OPTIM_FLAG) $(STANDARD_FLAG) $(MULTICORE_FLAG) $(DEBUG_FLAGS) -oterm -I /usr/include src/mains/main_${SITE}.cpp ${CPP_PATHS} -o bin/homeio_main_${SITE} $(LINK_FLAG)

exec: ;
	sudo bin/homeio_main_${SITE}

exec-debug: ;
	gdb bin/homeio_main_${SITE}

debian-deps: ;
	echo "g++ at least 4.9 is needed"
	sudo apt-get install libcurl4-openssl-dev git g++ libcurlpp0 libcurlpp-dev libncurses5-dev

debian-dev-deps: ;
	sudo apt-get install valgrind astyle

leak-test-full:
	sudo valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrid_full.txt build/bin/homeio_leak_test

leak-test-std:
	sudo valgrind --leak-check=yes --log-file=valgrid_std.txt build/bin/homeio_leak_test

format: ;
	astyle --style=google --indent=spaces=2 --suffix=none --verbose --recursive *.?pp

clean: ;
	[ -e bin ] || rm -v ./bin/*
