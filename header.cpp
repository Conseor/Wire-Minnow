#include "header.hpp"



void Header::set_order(Sort new_order) {
    order = new_order;
}

std::string Header::get_name() {
    return name;
}

Sort Header::get_order() {
    return order;
}

int Header::get_padding() {
    return padding;
}