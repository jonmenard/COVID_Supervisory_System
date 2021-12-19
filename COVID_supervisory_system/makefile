CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include
INCLUDEJSON=-I C:/Cadmium-Simulation-Environment/cadmium/json/include
INCLUDEWEB=-I C:/Cadmium-Simulation-Environment/cadmium-web-extension/include
#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) top_model/main.cpp -o build/main_top.o
main_occ_test.o: test/main_occ_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_occ_test.cpp -o build/main_occ_test.o
main_led_test.o: test/main_led_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_led_test.cpp -o build/main_led_test.o
main_c02_test.o: test/main_c02_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_c02_test.cpp -o build/main_c02_test.o

#TARGET TO COMPILE ONLY ABP SIMULATOR main_top.o
simulator: main_top.o main_occ_test.o main_led_test.o main_c02_test.o
	$(CC) -g -o bin/TOP build/main_top.o 
	$(CC) -g -o bin/OCC_TEST build/main_occ_test.o
	$(CC) -g -o bin/LED_TEST build/main_led_test.o
	$(CC) -g -o bin/C02_TEST build/main_c02_test.o


all: simulator
#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*