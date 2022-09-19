#pragma once
#include "auxiliary.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <climits>

namespace krvlib {

template <typename T, typename KeyF = int>
class ideal_cache {
  private:
    const std::vector<T> &array;
    std::unordered_multimap<KeyF, size_t> multi_hash;
    size_t sz;
    std::list<T> cache;
    using ListIt = typename std::list<T>::iterator;//Well, i need to check this
    std::unordered_map<KeyF, ListIt> hash;

  public:
    ideal_cache(const size_t size, const std::vector<T> &buffer);

  private:
    bool full() const;
    
    ListIt search_pos_for_new_element(long id);

  public:
    bool lookup_update(const size_t ArrN);
};

int result_of_ideal_cache(std::vector<page_t> &array_of_pages, size_t num_of_pages, int size);

}