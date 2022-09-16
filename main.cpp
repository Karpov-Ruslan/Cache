#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>

struct page_t {
    int id;
    page_t() {}
    page_t(int number) {
        id = number;
    }
};



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
        for(size_t i = buffer.size() - 1; i >=0; i--) {
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
    
    ListIt search_pos_for_new_element(size_t id) {
        for (ListIt it = cache.begin(); it != cache.end(); it++) {
            if (multi_hash.bucket_size(multi_hash.bucket(it->id)) == 0) {
                return it;
            }
            if (id < (multi_hash.begin(multi_hash.bucket(it->id)))->second) {
                return it;
            }
        }
        return cache.end();
    }

  public:
    bool lookup_update(const size_t ArrN) {
        KeyF key = array[ArrN].id;
        if (full()) {
            hash.erase(cache.back().id);
            cache.pop_back();
        }
        hash[key] = cache.insert(search_pos_for_new_element(ArrN), array[ArrN]);
        multi_hash.erase(multi_hash.find(key));
        if (hash.find(key) == hash.end()) {
            return false;
        }
        return true;
    }
 
};

int result_of_ideal_cache() {
    size_t num_of_pages;
    int size, counter = 0;
    std::cin >> num_of_pages >> size;
    std::vector<page_t> array_of_pages;
    

    for(int i = 0; i < num_of_pages; i++) {
        int id;
        std::cin >> id;
        array_of_pages.push_back(page_t(id));
    }

    ideal_cache<page_t> cache(size, array_of_pages);

    for(int i = 0; i < num_of_pages; i++) {
        if(cache.lookup_update(i)) {
            counter++;
        }
    }

    return counter;
}

int main() {
    
    std::cout << result_of_ideal_cache() << std::endl;

    return 0;
}
