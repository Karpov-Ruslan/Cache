#include <iostream>
#include "lfu_cache.hpp"
#include "ideal_cache.hpp"

int main() {
    
    int num_of_pages;
    int size, counter = 0;
    std::cin >> num_of_pages >> size;
    std::vector<page_t> array;
    for (int i = 0; i < num_of_pages; i++) {
        int id;
        std::cin >> id;
        array.push_back(page_t(id));
    }

    std::cout << result_of_lfu_cache(array, num_of_pages, size) << " " << result_of_ideal_cache(array, num_of_pages, size) << std::endl;

    return 0;
}