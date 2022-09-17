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
    page_temp(T page) {
        this->page = page;
        counter = 1;
    }

    int increase_counter() {
        counter++;
        return counter;
    }

    T get_page() const {
        return page;
    }

    int get_counter() const {
        return counter;
    }
};


template <typename T, typename KeyF = int>
class cache_t {
    size_t sz;
    std::list<page_temp<T>> cache;
    using ListIt = typename std::list<page_temp<T>>::iterator;//Well, i need to check this
    std::unordered_map<KeyF, ListIt> hash;

  public:
    cache_t(size_t size) {
        sz = size;
    }

  private:
    ListIt pos_for_new_element() {
        ListIt it = cache.begin();
        ListIt back = cache.end();
        back--;
        while(true) {
            if(it == back) {
                return it;
            }
            if(it->get_counter() == 1) {
                return it;
            }
            it++;
        }
    }

    template <typename F = page_t>
    ListIt insert_new_element(page_temp<F> &page_) {
        if (cache.size() == 0) {
            cache.push_front(page_);
            return cache.begin();
        }
        ListIt it = pos_for_new_element();
        cache.insert(it, page_);
        it--;
        return it;
    }

    ListIt search_pos_for_update_element(ListIt &iter) {
        int ctr = iter->get_counter() + 1;
        ListIt it = iter;
        while(true) {
            if(it == cache.begin()) {
                break;
            }
            if(it->get_counter() == ctr) {
                it++;
                break;
            }
            it--;
        }
        return it;
    }

    bool update_element(ListIt &iter) {
        ListIt it = search_pos_for_update_element(iter);
        iter->increase_counter();
        if(iter == it) {
            return false;
        }
        else {
            cache.splice(it, cache, iter);
            return true;
        }
    }

    bool full() const {
        if(cache.size() == sz) {
            return true;
        }
        return false;
    }

  public:
    template <typename F = page_t>
    bool lookup_update(KeyF key, F (*slow_get_page)(KeyF)) {
        auto hit = hash.find(key);

        if (hit == hash.end()) {
            if(full()) {
                hash.erase(cache.back().get_page().id);
                cache.pop_back();
            }
            page_temp<F> page_(slow_get_page(key));
            hash[key] = insert_new_element(page_);
            return false;
        }
        
        ListIt eltit = hit->second;
        update_element(eltit);
        return true;
    }

    bool lookup_update(page_t &element) {
        KeyF key = element.id;
        auto hit = hash.find(key);

        if (hit == hash.end()) {
            if(full()) {
                hash.erase(cache.back().get_page().id);
                cache.pop_back();
            }
            page_temp<page_t> page_(element);
            hash[key] = insert_new_element(page_);
            return false;
        }
        
        ListIt eltit = hit->second;
        update_element(eltit);
        return true;
    }
};

int result_of_lfu_cache(std::vector<page_t> &array_of_pages, size_t num_of_pages, int size) {
    cache_t<page_t> cache(size);
    int counter = 0;
    for(int i = 0; i < num_of_pages; i++) {
        if(cache.lookup_update(array_of_pages[i])) {
            counter++;
        }
    }
    return counter;
}