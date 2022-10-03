#include <iostream>
#include <fstream>
#include "lfu_cache.hpp"
#include "ideal_cache.hpp"

int main() {
    
    int number_of_tests;

    std::ifstream fin("../tests.txt");
    fin >> number_of_tests;
    size_t num_of_pages;
    int size, lfu_hits, ideal_hits;
    for (int j = 0; j < number_of_tests; j++) {
        fin >> lfu_hits >> ideal_hits >> size >> num_of_pages;
        std::vector<krvlib::page_t> array;
        for (int i = 0; i < num_of_pages; i++) {
            int id;
            fin >> id;
            array.push_back(krvlib::page_t(id, i));
        }

        std::cout << "Expected: " << lfu_hits << "; " << "Numbers of LFU cache hits: " << krvlib::result_of_lfu_cache(array, num_of_pages, size) << std::endl;
        std::cout << "Expected: " << ideal_hits << "; " << "Numbers of ideal cache hits: " << krvlib::result_of_ideal_cache(array, num_of_pages, size) << std::endl;
    }
    fin.close();

    return 0;
}