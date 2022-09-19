#include "auxiliary.hpp"

krvlib::page_t::page_t(int number) {
        id = number;
}

krvlib::page_t krvlib::slow_get_page(page_t &page) {
    return page;
}