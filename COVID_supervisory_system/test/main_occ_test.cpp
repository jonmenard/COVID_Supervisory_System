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
#include <NDTime.hpp>


//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/occupencyController.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
/***** Define output ports for coupled model *****/
struct personCount: public web::out_port<int>{};




/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Message_t : public web::iestream_input<int,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : web::iestream_input<int,T>(file_path) {}
};


int main(){

    /****** Input Reader atomic model instantiation *******************/
    const char * i_input = "../input_data/input_test_occ.txt";
    shared_ptr<web::model> input;
    input = web::make_atomic_model<InputReader_Message_t, TIME, const char*>("input", "reader", move(i_input));

    const char * i_input2 = "../input_data/input_test_occ2.txt";
    shared_ptr<web::model> input2;
    input2 = web::make_atomic_model<InputReader_Message_t, TIME, const char*>("input2", "reader2", move(i_input2));

    

    /****** Subnet atomic model instantiation *******************/
    shared_ptr<web::model> occupencyController;
    occupencyController = web::make_atomic_model<OccupencyController, TIME>("OccupencyController", "occupencyController");

    /*******TOP MODEL********/
    web::Ports iports_TOP = {};
    web::Ports oports_TOP = {typeid(personCount)};
    web::Models submodels_TOP = {input,input2, occupencyController};
    web::EICs eics_TOP = {};
    web::EOCs eocs_TOP = {
        web::make_EOC <occupencyController_defs::personSafe,personCount>("OccupencyController"),
    };
    web::ICs  ics_TOP = {
        web::make_IC<web::iestream_input_defs<int>::out, occupencyController_defs::personIn>("input","OccupencyController"),
        web::make_IC<web::iestream_input_defs<int>::out, occupencyController_defs::personOut>("input2","OccupencyController")
    };
    shared_ptr<web::coupled <TIME>> TOP;
    TOP = web::make_top_model<TIME>("TOP", "top", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    web::runner<TIME, web::logger_top> r(TOP, {0});
    
    r.run_until(NDTime("10:00:00:000"));

    web::output_results(TOP, "DEVS", "../simulation_results/");
    
    return 0;
}
