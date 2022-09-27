#pragma once

namespace krvlib {

struct page_t {
    int id;
    size_t num = 0;
    page_t() {}
    page_t(int number) {
        id = number;
    }
    page_t(int number, size_t identificator): id(number), num(identificator) {}
};

page_t slow_get_page(page_t &page) {
    return page;
}

}