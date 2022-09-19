#include "ideal_cache.hpp"

template <typename T, typename KeyF>
ideal_cache<T, KeyF>::ideal_cache(const size_t size, const std::vector<T> &buffer): array(buffer) {
    for(long i = buffer.size() - 1; i >= 0; i--) {
        multi_hash.insert(std::make_pair(buffer[i].id, i));
    }
    sz = size;
}

template <typename T, typename KeyF>
bool ideal_cache<T, KeyF>::full() const {
    if(cache.size() == sz) {
        return true;
    }
    return false;
}

template <typename T, typename KeyF>
typename ideal_cache<T, KeyF>::ListIt ideal_cache<T, KeyF>::search_pos_for_new_element(long id) {//Переделать
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

template <typename T, typename KeyF>
bool ideal_cache<T, KeyF>::lookup_update(const size_t ArrN) {
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