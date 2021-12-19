/**
* Jon Menard 
* ARSLab - Carleton University
*
* C02 Sensor:
* Cadmium implementation of COVID SUPERVISORY SYSTEM
*/

#ifndef _C02_SENSOR_HPP__
#define _C02_SENSOR_HPP__

// #include <cadmium/modeling/ports.hpp>
// #include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include <web/web_ports.hpp>
#include <web/output/message_type.hpp>


using namespace cadmium;
using namespace std;

//Port definition
struct C02SensorController_defs{
    struct C02In : public web::out_port<int> {};
    struct C02Safe : public web::in_port<int> {};
};

template<typename TIME> class C02SensorController{
    public:
    // ports definition
    using input_ports=tuple<typename C02SensorController_defs::C02In>;
    using output_ports=tuple<typename C02SensorController_defs::C02Safe>;
    // state definition
    struct state_type{
        float C02;
        bool output;
           
    }; 

    const float MAX_CO2_CONCENTRATION = 0.5;

    state_type state;    
    // default constructor
    C02SensorController() {
        state.output = false;
        state.C02 = 0;
    }     
    // internal transition
    void internal_transition() {
        state.output = false;  
    }
    // external transition -- this is where water will be inputed into the model
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        for(const auto &x : get_messages<typename C02SensorController_defs::C02In>(mbs)){
            state.C02 = x;
        }
        state.output = true;
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
    
        int safe = 0;
        
        if(state.C02 > MAX_CO2_CONCENTRATION){
            safe = 1;
        }


        get_messages<typename C02SensorController_defs::C02Safe>(bags).push_back(safe);
        
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
    
        if(state.output)
          return TIME("00:00:00");
        return std::numeric_limits<TIME>::infinity();
    }

     friend std::ostringstream& operator<<(std::ostringstream& os, const typename C02SensorController<TIME>::state_type& i) {
        os << (int) (i.C02);
        return os;
    }

    message_type get_state_message_type() {
        vector<string> fields({"C02Out"});
        string description = "State of the model";
        return message_type("C02SensorController_",fields,description);
    }

};    
#endif