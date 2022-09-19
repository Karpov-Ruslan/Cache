#pragma once
#include "auxiliary.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

template <typename T = page_t>
class page_temp {
  private:
    T page;
    int counter;

  public:
    page_temp(T page);

    int increase_counter();

    T get_page() const;

    int get_counter() const;
};


template <typename T, typename KeyF = int>
class cache_t {
    size_t sz;
    std::list<page_temp<T>> cache;
    using ListIt = typename std::list<page_temp<T>>::iterator;//Well, i need to check this
    std::unordered_map<KeyF, ListIt> hash;

  public:
    cache_t(size_t size);

  private:
    ListIt pos_for_new_element();

    ListIt insert_new_element(page_temp<T> &page_);

    ListIt search_pos_for_update_element(ListIt &iter);

    bool update_element(ListIt &iter);

    bool full() const;

  public:
    bool lookup_update(KeyF key, T (*slow_get_page)(KeyF));

    bool lookup_update(page_t &element);
};

int result_of_lfu_cache(std::vector<page_t> &array_of_pages, size_t num_of_pages, int size);