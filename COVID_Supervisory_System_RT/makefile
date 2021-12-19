CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include
#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)
# message.o: data_structures/message.cpp
# 	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEWEB) data_structures/message.cpp -o build/message.o

main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o



#main_occ_test.o: test/main_occ_test.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_occ_test.cpp -o build/main_occ_test.o
#main_led_test.o: test/main_led_test.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_led_test.cpp -o build/main_led_test.o
#main_hallway_test.o: test/main_hallway_test.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_hallway_test.cpp -o build/main_hallway_test.o
#main_holding_test.o: test/main_holding_test.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_holding_test.cpp -o build/main_holding_test.o
#main_temp_test.o: test/main_temp_test.cpp
#	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) $(INCLUDEJSON) $(INCLUDEWEB) test/main_temp_test.cpp -o build/main_temp_test.o

## TEST FOR EACH MODEL

# tests: main_inspection_test.o message.o
# 	$(CC) -g -o bin/INSPECTION_TEST build/main_inspection_test.o build/message.o
		
# tests: main_sanitizer_test.o message.o	
# 	$(CC) -g -o bin/SANITIZER_TEST build/main_sanitizer_test.o build/message.o
	
# tests: main_filter_test.o message.o	
# 	$(CC) -g -o bin/FILTER_TEST build/main_filter_test.o build/message.o

# tests: main_smallFilter_test.o message.o	
# 	$(CC) -g -o bin/SMALLFILTER_TEST build/main_smallFilter_test.o build/message.o

# tests: main_settlingChamber_test.o message.o	
# 	$(CC) -g -o bin/SETTLINGCHAMBER_TEST build/main_settlingChamber_test.o build/message.o

# tests: main_floc_test.o message.o	
# 	$(CC) -g -o bin/FLOC_TEST build/main_floc_test.o build/message.o

# tests: microfilter.o message.o	
# 	$(CC) -g -o bin/MICROFILTER build/microfilter.o build/message.o 

# tests: filters.o message.o	
# 	$(CC) -g -o bin/FILTERS build/filters.o build/message.o 

# tests: waterTreatment.o message.o	
# 	$(CC) -g -o bin/WATERTREATMENT build/waterTreatment.o build/message.o 

#tests: main_inspection_test.o main_sanitizer_test.o main_filter_test.o message.o main_smallFilter_test.o main_settlingChamber_test.o main_floc_test.o
#	$(CC) -g -o bin/INSPECTION_TEST build/main_inspection_test.o build/message.o
#	$(CC) -g -o bin/SANITIZER_TEST build/main_sanitizer_test.o build/message.o
#	$(CC) -g -o bin/FILTER_TEST build/main_filter_test.o build/message.o
#	$(CC) -g -o bin/SMALLFILTER_TEST build/main_smallFilter_test.o build/message.o
#	$(CC) -g -o bin/SETTLINGCHAMBER_TEST build/main_settlingChamber_test.o build/message.o
#	$(CC) -g -o bin/FLOC_TEST build/main_floc_test.o build/message.o

#TARGET TO COMPILE ONLY ABP SIMULATOR main_top.o
simulator: main_top.o
#simulator: main_holding_test.o main_hallway_test.o main_occ_test.o main_led_test.o main_temp_test.o main_top.o
	$(CC) -g -o bin/TEST build/main_top.o 
#	$(CC) -g -o bin/FILTER_TEST1 build/main_hallway_test.o
#	$(CC) -g -o bin/FILTER_TEST2 build/main_holding_test.o
#	$(CC) -g -o bin/FILTER_TEST3 build/main_occ_test.o
#	$(CC) -g -o bin/FILTER_TEST4 build/main_led_test.o
#	$(CC) -g -o bin/FILTER_TEST build/main_temp_test.o

#TARGET TO COMPILE EVERYTHING (ABP SIMULATOR + TESTS TOGETHER)
# all: simulator tests
# all: tests
all: simulator
#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*