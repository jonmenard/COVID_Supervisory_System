/**
* Jon Menard 
* ARSLab - Carleton University
*
* Temperature Sensor:
* Cadmium implementation of Water Treatment 
*/

#ifndef _OCCUPENCY_CONTROLLER_HPP__
#define _OCCUPENCY_CONTROLLER_HPP__

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
struct occupencyController_defs{
    struct personIn : public web::in_port<int> {};
    struct personOut : public web::in_port<int> {};
    struct personSafe : public web::out_port<int> {};
};

template<typename TIME> class OccupencyController{
    public:
    // ports definition
    using input_ports=tuple<typename occupencyController_defs::personIn,typename occupencyController_defs::personOut>;
    using output_ports=tuple<typename occupencyController_defs::personSafe>;
    // state definition
    const int MAX_OCCUPANCY = 3;
    
    struct state_type{
        int personCount;
        bool output;    
    }; 


    state_type state;    
    // default constructor
    OccupencyController() {
        state.personCount = 0;
        state.output = false;
    }     
    // internal transition
    void internal_transition() {
        state.output = false;  
    }
    // external transition -- this is where water will be inputed into the model
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        for(const auto &x : get_messages<typename occupencyController_defs::personOut>(mbs)){
            if(state.personCount > 0){
                state.personCount -= x;
            }
        }
       
        for(const auto &x : get_messages<typename occupencyController_defs::personIn>(mbs)){
            state.personCount += x;
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
        
        if(state.personCount < MAX_OCCUPANCY){
            safe = 1;
        }

        get_messages<typename occupencyController_defs::personSafe>(bags).push_back(safe);
        
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        if(state.output)
            return TIME("00:00:00");
        return std::numeric_limits<TIME>::infinity();
    }

     friend std::ostringstream& operator<<(std::ostringstream& os, const typename OccupencyController<TIME>::state_type& i) {
        //os << i.personCount;
        os << i.personCount;
        return os;
    }

    message_type get_state_message_type() {
        vector<string> fields({"OccupencyOut"});
        string description = "State of the model";
        return message_type("OccupencyController_",fields,description);
    }

};    
#endif