/**
* Jon Menard 
* ARSLab - Carleton University
*
* Filter:
* Cadmium implementation of Water Treatment 
*/



//Cadmium Simulator headers
#include <iostream>
#include <string>
#include <vector>
#include <NDTime.hpp>
#include <chrono>

#include <web/web_ie_stream.hpp>
#include <web/web_logger.hpp>
#include <web/web_ports.hpp>
#include <web/web_model.hpp>
#include <web/web_results.hpp>


//Time class header

#include "../atomics/ledController.hpp"
#include "../atomics/occupencyController.hpp"
#include "../atomics/C02SensorController.hpp"


using TIME = NDTime;






/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Message_t : public web::iestream_input<int,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : web::iestream_input<int,T>(file_path) {}
};

int main(int argc, char ** argv) {

   
    //This will end the main thread and create a new one with more stack.
  


    const char * i_input = "../input_data/input_test_occ.txt";
    shared_ptr<web::model> input;
    input = web::make_atomic_model<InputReader_Message_t, TIME, const char*>("input", "reader", move(i_input));

    const char * i_input2 = "../input_data/input_test_occ2.txt";
    shared_ptr<web::model> input2;
    input2 = web::make_atomic_model<InputReader_Message_t, TIME, const char*>("input2", "reader2", move(i_input2));

    const char * i_input3 = "../input_data/input_test_co2.txt";
    shared_ptr<web::model> input3;
    input3 = web::make_atomic_model<InputReader_Message_t, TIME, const char*>("input3", "reader3", move(i_input3));



    // Atomic Models
    shared_ptr<web::model> led_Controller;
    led_Controller = web::make_atomic_model<LED_Controller, TIME>("LED_Controller", "led_Controller");

    shared_ptr<web::model> occupencyController;
    occupencyController = web::make_atomic_model<OccupencyController, TIME>("OccupencyController", "occupencyController");

    shared_ptr<web::model> c02SensorController;
    c02SensorController = web::make_atomic_model<C02SensorController, TIME>("C02SensorController", "C02sensorcontroller");


    /*******TOP COUPLED MODEL********/
    web::Ports iports_TOP = {};
    web::Ports oports_TOP = {};
    web::Models submodels_TOP = {c02SensorController,occupencyController,led_Controller,input,input2,input3};
    web::EICs eics_TOP = {};
    web::EOCs eocs_TOP = {};
    web::ICs  ics_TOP = {

            //Inputs from microcontroller
            web::make_IC<web::iestream_input_defs<int>::out, C02SensorController_defs::C02In>("input3","C02SensorController"),
            web::make_IC<web::iestream_input_defs<int>::out, occupencyController_defs::personIn>("input","OccupencyController"),
            web::make_IC<web::iestream_input_defs<int>::out, occupencyController_defs::personOut>("input2","OccupencyController"),

            //Connecting atomic models
			      web::make_IC<occupencyController_defs::personSafe, LED_Controller_defs::OccupancyIn>("OccupencyController","LED_Controller"),
			      web::make_IC<C02SensorController_defs::C02Safe, LED_Controller_defs::C02In>("C02SensorController","LED_Controller")
  
    };
    shared_ptr<web::coupled <TIME>> TOP;
    TOP = web::make_top_model<TIME>("TOP", "top", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    web::runner<TIME, web::logger_top> r(TOP, {0});
    
    r.run_until(NDTime("10:00:00:000"));

    web::output_results(TOP, "DEVS", "../simulation_results/");
    
    return 0;
}