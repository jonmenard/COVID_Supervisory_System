/**
* Jon Menard 
* ARSLab - Carleton University
*
* Filter:
* Cadmium implementation of Water Treatment 
*/



//Cadmium Simulator headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include <cadmium/io/iestream.hpp>


#include <NDTime.hpp>

#include <cadmium/real_time/arm_mbed/io/digitalInput.hpp>
#include <cadmium/real_time/arm_mbed/io/analogInput.hpp>
#include <cadmium/real_time/arm_mbed/io/pwmOutput.hpp>
#include <cadmium/real_time/arm_mbed/io/digitalOutput.hpp>



//Time class header

#include "../atomics/ledController.hpp"
#include "../atomics/occupencyController.hpp"
#include "../atomics/C02SensorController.hpp"


#ifdef RT_ARM_MBED
  #include "../mbed.h"
#else
  const char* D4  = "./outputs/D4_ledGreen_Out.txt";
  const char* D8  = "./inputs/D8_sensorIn_In.txt";
  const char* D7  = "./inputs/D7_sensorOut_In.txt";
  const char* A5  = "./inputs/A5_C02in_in.txt";

  const char* D6  = "./outputs/a.txt";
  const char* D9  = "./outputs/b.txt";
  const char* D10  = "./outputs/c.txt";
  const char* D11  = "./outputs/d.txt";
  const char* D12  = "./outputs/e.txt";
  const char* D14  = "./outputs/f.txt";
  const char* D15  = "./outputs/g.txt";
#endif



using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;


int main(int argc, char ** argv) {

   
    //This will end the main thread and create a new one with more stack.
  #ifdef RT_ARM_MBED
    //Logging is done over cout in RT_ARM_MBED
    struct oss_sink_provider{
      static std::ostream& sink(){
        return cout;
      }
    };
  #else
    // all simulation timing and I/O streams are ommited when running real_time/arm_mbed

    auto start = hclock::now(); //to measure simulation execution time

    /*************** Loggers *******************/

    static std::ofstream out_data("covid_sup_test_output.txt");
    struct oss_sink_provider{
      static std::ostream& sink(){
        return out_data;
      }
    };
  #endif


    using info=cadmium::logger::logger<cadmium::logger::logger_info, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using debug=cadmium::logger::logger<cadmium::logger::logger_debug, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using state=cadmium::logger::logger<cadmium::logger::logger_state, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using log_messages=cadmium::logger::logger<cadmium::logger::logger_messages, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using routing=cadmium::logger::logger<cadmium::logger::logger_message_routing, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using global_time=cadmium::logger::logger<cadmium::logger::logger_global_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using local_time=cadmium::logger::logger<cadmium::logger::logger_local_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
    using log_all=cadmium::logger::multilogger<info, debug, state, log_messages, routing, global_time, local_time>;
    using logger_top=cadmium::logger::multilogger<log_messages, global_time>;


    using AtomicModelPtr=std::shared_ptr<cadmium::dynamic::modeling::model>;
    using CoupledModelPtr=std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>>;




    // Atomic Models

    /****** c02SensorController atomic model instantiation *******************/
    AtomicModelPtr c02SensorController = cadmium::dynamic::translate::make_dynamic_atomic_model<C02SensorController, TIME>("C02SensorController");
    /****** LED_Controller atomic model instantiation *******************/
    AtomicModelPtr led_Controller = cadmium::dynamic::translate::make_dynamic_atomic_model<LED_Controller, TIME>("LED_Controller");
    /****** OccupencyController atomic models instantiation *******************/
    AtomicModelPtr occupencyController = cadmium::dynamic::translate::make_dynamic_atomic_model<OccupencyController, TIME>("OccupencyController");

    // Inputs 

    AtomicModelPtr sensorIn = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalInput, TIME>("sensorIn", D8,TIME("00:00:00:100"));
    AtomicModelPtr sensorOut = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalInput, TIME>("sensorOut", D7,TIME("00:00:00:100"));
    AtomicModelPtr C02In = cadmium::dynamic::translate::make_dynamic_atomic_model<AnalogInput, TIME>("C02In", A5,TIME("00:00:00:100"));

    // Outputs

    AtomicModelPtr ledGreen = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("ledGreen", D4);
    AtomicModelPtr ledRed = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("ledRed", D2);


    // 7 segment display for count people in room -- used to debug
    AtomicModelPtr a = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("a", D14);
    AtomicModelPtr b = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("b", D15);
    AtomicModelPtr c = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("c", D10);
    AtomicModelPtr d = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("d", D9);
    AtomicModelPtr e = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("e", D6);
    AtomicModelPtr f = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("f", D12);
    AtomicModelPtr g = cadmium::dynamic::translate::make_dynamic_atomic_model<DigitalOutput, TIME>("g", D11);

   
    /*******TOP COUPLED MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {};
    dynamic::modeling::Models submodels_TOP = {ledGreen, sensorOut, sensorIn, C02In, c02SensorController, led_Controller,occupencyController,a,b,c,d,e,f,g};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs  eocs_TOP = {};
    dynamic::modeling::ICs  ics_TOP = {

          //Inputs from microcontroller
          cadmium::dynamic::translate::make_IC<digitalInput_defs::out, occupencyController_defs::personOut>("sensorOut", "OccupencyController"),
          cadmium::dynamic::translate::make_IC<digitalInput_defs::out, occupencyController_defs::personIn>("sensorIn", "OccupencyController"),
          cadmium::dynamic::translate::make_IC<analogInput_defs::out, C02SensorController_defs::C02In>("C02In", "C02SensorController"),
          //Connecting atomic models 
			    cadmium::dynamic::translate::make_IC<occupencyController_defs::personSafe, LED_Controller_defs::OccupancyIn>("OccupencyController","LED_Controller"),
			    cadmium::dynamic::translate::make_IC<C02SensorController_defs::C02Safe, LED_Controller_defs::C02In>("C02SensorController","LED_Controller"),
          //Output to microcontroller
          cadmium::dynamic::translate::make_IC<LED_Controller_defs::LEDOut, digitalOutput_defs::in>("LED_Controller","ledGreen"),
          
          
          cadmium::dynamic::translate::make_IC<occupencyController_defs::a, digitalOutput_defs::in>("OccupencyController","a"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::b, digitalOutput_defs::in>("OccupencyController","b"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::c, digitalOutput_defs::in>("OccupencyController","c"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::d, digitalOutput_defs::in>("OccupencyController","d"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::e, digitalOutput_defs::in>("OccupencyController","e"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::f, digitalOutput_defs::in>("OccupencyController","f"),
          cadmium::dynamic::translate::make_IC<occupencyController_defs::g, digitalOutput_defs::in>("OccupencyController","g")
           
            
    
    };
    CoupledModelPtr TOP = std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );


  // Logs are currently blocking opperations. It is recommended to turn them off when embedding your application.
  // They can be used for testing; however, keep in mind they will add extra delay to your model.

  //cadmium::dynamic::engine::runner<NDTime, cadmium::logger::not_logger> r(TOP, {0});

  cadmium::dynamic::engine::runner<NDTime, log_all> r(TOP, {0});

  r.run_until(NDTime("00:10:00:000"));

  #ifndef RT_ARM_MBED
    return 0;
  #endif
    

}