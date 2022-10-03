#pragma once
#include "auxiliary.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <map>
#include <climits>
#include <cassert>

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
    std::map<size_t, ListIt> search_tree;
    using MapIt = typename std::map<size_t, ListIt>::iterator;

  public:
    ideal_cache(const size_t size, const std::vector<T> &buffer): array(buffer), sz(size) {
        assert(size != 0);
        for(long i = buffer.size() - 1; i >= 0; i--) {
            multi_hash.insert(std::make_pair(buffer[i].id, i));
        }
    }

  private:
    bool empty() const {
        return (sz == 0);
    }

    bool full() const {
        return (cache.size() == sz);
    }

    size_t next_same_id(size_t id) {
        auto bucket_id = multi_hash.bucket(array[id].id);
        if (multi_hash.bucket_size(bucket_id) == 0) {
            return LONG_MAX;
        }
        return (multi_hash.begin(bucket_id))->second;
    }
    
    ListIt search_pos_for_new_element(size_t id) {
        MapIt map_it = search_tree.lower_bound(next_same_id(id));
        if (map_it == search_tree.end()) {
        return cache.end();
        }
        return map_it->second;
    }

  public:
    bool lookup_update(const size_t ArrN) {
        bool ret;
        const page_t &page = array[ArrN];
        multi_hash.erase(multi_hash.find(page.id));
        auto hit = hash.find(page.id);
        if (hit == hash.end()) {
            if (next_same_id(ArrN) == LONG_MAX) {
                return false;
            }
            if (full()) {
                hash.erase(cache.back().id);
                search_tree.erase(next_same_id(cache.back().num));
                cache.pop_back();
            }
            ret = false;
        }
        else {
            hash.erase(page.id);
            search_tree.erase(next_same_id(page.num));
            cache.erase(hit->second);
            ret = true;
        }
        ListIt it = cache.insert(search_pos_for_new_element(ArrN), page);
        hash[page.id] = it;
        search_tree[next_same_id(ArrN)] = it;
        return ret;
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

}