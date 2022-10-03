#pragma once
#include "auxiliary.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <map>

namespace krvlib {

template <typename T = page_t>
class page_temp {
  private:
    T page;
    int counter = 1;

  public:
    page_temp(T page): page(page) {}

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
    std::unordered_map<int, ListIt> search_tree;
    using MapIt = typename std::unordered_map<int, ListIt>::iterator;

  public:
    cache_t(size_t size): sz(size) {
        assert(size != 0);
    }

  private:
    ListIt insert_new_element(page_temp<T> &page_) {
        if (cache.size() == 0) {
            cache.push_front(page_);
            search_tree[1] = cache.begin();
            return cache.begin();
        }
        MapIt Map_it = search_tree.find(1);
        if (Map_it == search_tree.end()) {
            cache.push_back(page_);
            ListIt it = std::prev(cache.end());
            search_tree[1] = it;
            return it;
        }
        ListIt it = Map_it->second;
        it = cache.insert(it, page_);
        search_tree[1] = it;
        return it;
    }

    void update_element(ListIt &iter) {
        auto counter = iter->get_counter();
        iter->increase_counter();
        MapIt Map_it = search_tree.find(counter);

        ListIt it = Map_it->second;
        ListIt next_it = std::next(it);
        if (it == iter) {
            if (next_it == cache.end()) {
                search_tree.erase(Map_it);
            }
            else if (next_it->get_counter() < counter) {
                search_tree.erase(Map_it);
            }
            else {
                search_tree[counter] = next_it;
            }
        }

        Map_it = search_tree.find(counter + 1); //redefined Map_it
        if (Map_it == search_tree.end()) {
            if (it != iter) {
                cache.splice(it, cache, iter);
            }
            search_tree[counter + 1] = iter;
        }
        else {
            it = Map_it->second; //redefined it
            if (it != iter) {
                cache.splice(it, cache, iter);
            }
            search_tree[counter + 1] = iter;
        }
    }

    bool full() const {
        return (cache.size() == sz);
    }

  public:
    bool lookup_update(KeyF key, T (*slow_get_page)(KeyF)) {
        auto hit = hash.find(key);

        if (hit == hash.end()) {
            if(full()) {
                MapIt Map_it = search_tree.find(cache.back().get_counter());
                if (Map_it->second == std::prev(cache.end())) {
                    search_tree.erase(Map_it);
                }
                hash.erase(cache.back().get_page().id);
                cache.pop_back();
            }
            page_temp<T> page_(slow_get_page(key));
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
        if(cache.lookup_update(array_of_pages[i].id, krvlib::slow_get_page)) {
            counter++;
        }
    }
    return counter;
}

}