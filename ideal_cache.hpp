#pragma once
#include "auxiliary.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <climits>

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
    ideal_cache(const size_t size, const std::vector<T> &buffer): array(buffer) {
        for(long i = buffer.size() - 1; i >= 0; i--) {
            multi_hash.insert(std::make_pair(buffer[i].id, i));
        }
        sz = size;
    }

  private:
    bool full() const {
        if(cache.size() == sz) {
            return true;
        }
        return false;
    }
    
    ListIt search_pos_for_new_element(long id) {//Переделать
        long number_of_next_element;
        if (multi_hash.bucket_size(multi_hash.bucket(array[id].id)) == 0) {
            number_of_next_element = LONG_MAX;
        }
        else{
            number_of_next_element = (multi_hash.begin(multi_hash.bucket(array[id].id)))->second;
        }
        for (ListIt it = cache.begin(); it != cache.end(); it++) {
            if (multi_hash.bucket_size(multi_hash.bucket(it->id)) == 0) {
                return it;
            }
            if (number_of_next_element <= (multi_hash.find(it->id)->second)) {
                return it;
            }
        }
        return cache.end();
    }

  public:
    bool lookup_update(const size_t ArrN) {
        KeyF key = array[ArrN].id;
        auto hit = hash.find(key);
        multi_hash.erase(multi_hash.find(key));
        if (hit == hash.end()) {
            if (full()) {
                hash.erase(cache.back().id);
                cache.pop_back();
            }
            hash[key] = cache.insert(search_pos_for_new_element(ArrN), array[ArrN]);
            return false;
        }
        hash.erase(key);
        cache.erase(hit->second);
        hash[key] = cache.insert(search_pos_for_new_element(ArrN), array[ArrN]);
        return true;
    }
};

int result_of_ideal_cache(std::vector<page_t> &array_of_pages, size_t num_of_pages, int size) {
    ideal_cache<page_t> cache(size, array_of_pages);
    int counter = 0;
    for(int i = 0; i < num_of_pages; i++) {
        if(cache.lookup_update(i)) {
            counter++;
        }
    }
    return counter;
}