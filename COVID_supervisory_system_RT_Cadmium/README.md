### RT_ARM_MBED INSTALL ###

Clone this repo into an empty folder

Run './install.sh' to install dependencies

### SIMULATE MODELS ### 

cd SeeedBot_RT_ARM_MBED/top_model/

make clean; make all

./COVID_SUP_TOP.exe

This will run the standard Cadmium simulator. Cadmium logs will be generated in /top_model/covid_sup_test_output.txt The pin's inputs are stored in /top_model/inputs. The value of the output pins will be in /top_model/outputs. SVEC (Simulation Visualization for Embedded Cadmium) is a python GUI that parses these files and steps through the simulation to help debug the models.

### RUN MODELS ON TARGET PLATFORM ###

If you are using a platform other then the Nucleo-STM32F401, you will need to change the COMPILE_TARGET / FLASH_TARGET in the make file.

cd SeeedBot_RT_ARM_MBED/top_model/

make clean; make embedded; make flash;
