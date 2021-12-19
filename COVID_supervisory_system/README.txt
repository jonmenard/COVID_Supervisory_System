This folder contains the WATER TREATMENT SIMULATOR model implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/
/***** Template Made*******/
/*********** By ***********/
/********* Tao Zheng*******/
/***** Modified by:********/
/******* Jon Menard *******/
/**************************/

README.txt	
WaterTreatmentSimulatorDescription.doc
makefile

atomics [This folder contains atomic models implemented in Cadmium]
	C02SensorController.hpp
	ledController.hpp
	occupencyController.hpp
bin [This folder will be created automatically the first time you compile the poject.
     It will contain all the executables]
build [This folder will be created automatically the first time you compile the poject.
       It will contain all the build files (.o) generated during compilation]
input_data [This folder contains all the input data to run the model and the tests]
	input_test_c02.txt
	input_test_led.txt
	input_test_led2.txt
	input_test_occ.txt
	input_test_occ2.txt
simulation_results [This folder will be created automatically the first time you compile the poject.
                    It will store the outputs from your simulations and tests]
test [This folder the unit test of the atomic models]
	main_c02_test.cpp
	main_led_test.cpp
	main_occ_test.cpp
	
top_model [This folder contains the Alternate Bit Protocol top model]	
	main.cpp
	