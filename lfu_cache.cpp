#include "lfu_cache.hpp"

template <typename T>
krvlib::page_temp<T>::page_temp(T page) {
    this->page = page;
    counter = 1;
}

template <typename T>
int krvlib::page_temp<T>::increase_counter() {
    counter++;
    return counter;
}

template <typename T>
T krvlib::page_temp<T>::get_page() const {
    return page;
}

template <typename T>
int krvlib::page_temp<T>::get_counter() const {
    return counter;
}

template <typename T, typename KeyF>
krvlib::cache_t<T, KeyF>::cache_t(size_t size) {
        sz = size;
}

template <typename T, typename KeyF>
typename krvlib::cache_t<T, KeyF>::ListIt krvlib::cache_t<T, KeyF>::pos_for_new_element() {
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

template <typename T, typename KeyF>
typename krvlib::cache_t<T, KeyF>::ListIt krvlib::cache_t<T, KeyF>::insert_new_element(page_temp<T> &page_) {
    if (cache.size() == 0) {
        cache.push_front(page_);
        return cache.begin();
    }
    ListIt it = pos_for_new_element();
    cache.insert(it, page_);
    it--;
    return it;
}

template <typename T, typename KeyF>
typename krvlib::cache_t<T, KeyF>::ListIt krvlib::cache_t<T, KeyF>::search_pos_for_update_element(ListIt &iter) {
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

template <typename T, typename KeyF>
bool krvlib::cache_t<T, KeyF>::update_element(ListIt &iter) {
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

template <typename T, typename KeyF>
bool krvlib::cache_t<T, KeyF>::full() const {
    if(cache.size() == sz) {
        return true;
    }
    return false;
}

template <typename T, typename KeyF>
bool krvlib::cache_t<T, KeyF>::lookup_update(KeyF key, T (*slow_get_page)(KeyF)) {
    auto hit = hash.find(key);

    if (hit == hash.end()) {
        if(full()) {
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

template <typename T, typename KeyF>
bool krvlib::cache_t<T, KeyF>::lookup_update(page_t &element) {
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

int krvlib::result_of_lfu_cache(std::vector<page_t> &array_of_pages, size_t num_of_pages, int size) {
    cache_t<page_t> cache(size);
    int counter = 0;
    for(int i = 0; i < num_of_pages; i++) {
        if(cache.lookup_update(array_of_pages[i])) {
            counter++;
        }
    }
    return counter;
}