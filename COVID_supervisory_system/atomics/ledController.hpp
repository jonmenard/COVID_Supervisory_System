/**
* Jon Menard 
* ARSLab - Carleton University
*
* Temperature Sensor:
* Cadmium implementation of Water Treatment 
*/

#ifndef _LED_CONTROLLER_HPP__
#define _LED_CONTROLLER_HPP__

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
struct LED_Controller_defs{
    struct LEDStatus : public web::out_port<int> {};
    struct C02In : public web::in_port<int> {};
    struct OccupancyIn : public web::in_port<int> {};
};

template<typename TIME> class LED_Controller{
    public:
    // ports definition
    using input_ports=tuple<typename LED_Controller_defs::OccupancyIn,typename LED_Controller_defs::C02In>;
    using output_ports=tuple<typename LED_Controller_defs::LEDStatus>;
    // state definition
    struct state_type{
        bool occupancySafe;
        bool C02Safe;
        bool ledColor;
        bool output;   
    }; 

    
    state_type state;    
    // default constructor
    LED_Controller() {
        state.ledColor = true;
        state.output = true;
        state.C02Safe = true;
        state.occupancySafe = true;
    }     
    // internal transition
    void internal_transition() {
        state.output = false;  
    }
    // external transition -- this is where water will be inputed into the model
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        int ledColor = state.ledColor;
        
        for(const auto &x : get_messages<typename LED_Controller_defs::OccupancyIn>(mbs)){
            if(x != 0){
                state.occupancySafe = true;
            }else{
                state.occupancySafe = false;
            }
        }
       
        for(const auto &x : get_messages<typename LED_Controller_defs::C02In>(mbs)){
            if(x != 0){
                state.C02Safe = true;
            }else{
                state.C02Safe = false;
            }
        }
        
        if(state.occupancySafe && state.C02Safe){
                state.ledColor = true;  
        }else{
                state.ledColor = false;  
        }

        if(ledColor != state.ledColor){
            state.output = true;
        } 
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
    
        
        int output = 0;
        
        if(state.ledColor){
            output = 1;
        }

        
        get_messages<typename LED_Controller_defs::LEDStatus>(bags).push_back(output);
        
        return bags;
    }
    // time_advance function
    TIME time_advance() const {

        if(state.output)
           return TIME("00:00:00");
        return std::numeric_limits<TIME>::infinity();
    }

     friend std::ostringstream& operator<<(std::ostringstream& os, const typename LED_Controller<TIME>::state_type& i) {
        if(i.ledColor){
            os << 1;
        }else{
            os << 0;
        }
        // os << i.ledColor;
    
        return os;
    }

    message_type get_state_message_type() {
        vector<string> fields({"LEDOut"});
        string description = "State of the model";
        return message_type("ledController_",fields,description);
    }

};    
#endif