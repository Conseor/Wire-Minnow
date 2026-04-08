#pragma once

#include "enums.hpp"
#include <string>


class Header {

    public:

        // Constructors

        Header(std::string nname, Sort norder, int npadding): name(nname), order(norder), padding(npadding){}; 
        // Header(char* nname, Sort norder, int npadding): name(std::string(nname)), order(norder), padding(npadding){}; 


        // Getters

        std::string get_name();
        Sort get_order();
        int get_padding();

        
        // Setters

        void set_order(Sort new_order);

    private:

        std::string name;
        Sort order;
        int padding = 0;

};