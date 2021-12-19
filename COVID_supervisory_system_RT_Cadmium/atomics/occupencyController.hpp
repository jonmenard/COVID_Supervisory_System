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

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

using namespace cadmium;
using namespace std;

//Port definition
struct occupencyController_defs{
    struct personIn : public in_port<bool> {};
    struct personOut : public in_port<bool> {};
    struct personSafe : public out_port<bool> {};


    struct a : public out_port<bool> {};
    struct b : public out_port<bool> {};
    struct c : public out_port<bool> {};
    struct d : public out_port<bool> {};
    struct e : public out_port<bool> {};
    struct f : public out_port<bool> {};
    struct g : public out_port<bool> {};
};

template<typename TIME> class OccupencyController{
    public:
    // ports definition
    using input_ports=tuple<typename occupencyController_defs::personIn,typename occupencyController_defs::personOut>;
    using output_ports=tuple<typename occupencyController_defs::personSafe,typename occupencyController_defs::a,typename occupencyController_defs::b,typename occupencyController_defs::c,typename occupencyController_defs::d,typename occupencyController_defs::e,typename occupencyController_defs::f,typename occupencyController_defs::g>;
    
    const bool digits[10][7]={{true,true,true,false,true,true,true},{false,false,true,false,false,true,false},{true,false,true,true,true,false,true},{true,false,true,true,false,true,true},{false,true,true,true,false,true,false},{true,true,false,true,false,true,true},{true,true,false,true,true,true,true},{true,false,true,false,false,true,false},{true,true,true,true,true,true,true},{true,true,true,true,false,true,true},};
    const int MAX_OCCUPANCY = 3;   
    // state definition
    struct state_type{
        int personCount;
        bool output;    
    }; 

    


    state_type state;    
    // default constructor
    OccupencyController() {
        state.personCount = 0;
        state.output = true;
    }     
    // internal transition
    void internal_transition() {
        state.output = false;  
    }
    // external transition -- this is where water will be inputed into the model
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        for(const auto &x : get_messages<typename occupencyController_defs::personOut>(mbs)){
            if(x){
                if(state.personCount > 0){
                    state.personCount -= 1;
                }
            }
        }
       
        for(const auto &x : get_messages<typename occupencyController_defs::personIn>(mbs)){
            if(x){
                state.personCount += 1;
            }
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
    
        bool safe = false;
        
        if(state.personCount < MAX_OCCUPANCY){
            safe = true;
        }
        
        get_messages<typename occupencyController_defs::personSafe>(bags).push_back(safe);

        int number = state.personCount;
        if(number > 9){
            number = 9;
        }

        get_messages<typename occupencyController_defs::a>(bags).push_back(digits[number][0]);
        get_messages<typename occupencyController_defs::b>(bags).push_back(digits[number][2]);
        get_messages<typename occupencyController_defs::c>(bags).push_back(digits[number][5]);
        get_messages<typename occupencyController_defs::d>(bags).push_back(digits[number][6]);
        get_messages<typename occupencyController_defs::e>(bags).push_back(digits[number][4]);
        get_messages<typename occupencyController_defs::f>(bags).push_back(digits[number][1]);
        get_messages<typename occupencyController_defs::g>(bags).push_back(digits[number][3]);


        
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

};    
#endif