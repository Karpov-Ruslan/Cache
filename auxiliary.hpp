#pragma once

struct page_t {
    int id;
    page_t() {}
    page_t(int number) {
        id = number;
    }
};

page_t slow_get_page(page_t &page) {
    return page;
}