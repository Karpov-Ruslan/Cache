#include <iostream>
#include "lfu_cache.hpp"
#include "ideal_cache.hpp"

int main() {
    
    size_t num_of_pages;
    int size;
    std::cin >> size >> num_of_pages;
    std::vector<krvlib::page_t> array;
    for (int i = 0; i < num_of_pages; i++) {
        int id;
        std::cin >> id;
        array.push_back(krvlib::page_t(id, i));
    }

#ifdef LFU
    std::cout << "Numbers of LFU cache hits: " << krvlib::result_of_lfu_cache(array, num_of_pages, size) << std::endl;
#endif
#ifdef IDEAL
    std::cout << "Numbers of ideal cache hits: " << krvlib::result_of_ideal_cache(array, num_of_pages, size) << std::endl;
#endif

    return 0;
}