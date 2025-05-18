#ifndef LRUCACHE_H
#define LRUCACHE_H

#pragma once
#include <unordered_map>
#include <list>

template <typename Key, typename Value>
class LRUCache {
public:
    explicit LRUCache(size_t capacity) : capacity(capacity) {}

    bool get(const Key& key, Value& value) {
        auto it = map.find(key);
        if (it == map.end()) return false;

        list.splice(list.begin(), list, it->second);
        value = it->second->second;
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = map.find(key);
        if (it != map.end()) {
            list.erase(it->second);
            map.erase(it);
        }

        list.emplace_front(key, value);
        map[key] = list.begin();

        if (map.size() > capacity) {
            auto last = list.end();
            --last;
            map.erase(last->first);
            list.pop_back();
        }
    }

private:
    using List = std::list<std::pair<Key, Value>>;
    List list;
    std::unordered_map<Key, typename List::iterator> map;
    size_t capacity;
};


#endif // LRUCACHE_H
