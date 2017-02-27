#include <iostream>
#include <unordered_map>

class LRUItem {
public:
    int used;
    int key;
    int value;
};

class LRUCache {
    int cap;
    int used; //timing counter
    LRUItem *least;
    std::unordered_map<int, LRUItem*> cache; //by 'key'
    std::unordered_map<int, LRUItem*> order; //by 'used'
    
public:
    LRUCache(int capacity) {
        cap = capacity;
        used = 0;
        least = NULL;
    }
    
    ~LRUCache() {
        for(std::unordered_map<int, LRUItem*>::iterator it = cache.begin(); it != cache.end(); it++)
            delete it->second;
    }
    
    void dump(std::unordered_map<int, LRUItem*> &m) {
        for(std::unordered_map<int, LRUItem*>::iterator it = cache.begin(); it != cache.end(); it++) {
            LRUItem *item = it->second;
            std::cout << it->first << ":=" << item->used << "," << item->key << "," << item->value << std::endl;
        }
        //std::cout << "used=" << used << "," << least << std::endl;
        //std::cout << "key=" << key << std::endl;
    }

    void next_least(int used) {
        for (int i=0; i < cap; i++) { //
            used++;
            least = order[used];
            if (least)break;
        }//least may be NULL, when empty cache
    }
    
    void update(LRUItem *item) {
        int used_old = item->used;
        order.erase(used_old);
        item->used = used++;
        order[item->used] = item;
        if (least == item)
            next_least(used_old);
    }

    int get(int key) {
        std::unordered_map<int, LRUItem*>::iterator it = cache.find(key);
        if (it == cache.end())
            return -1;
        LRUItem *item = it->second;
        update(item);
        return item->value;
    }
    
    void put(int key, int value) {
        std::unordered_map<int, LRUItem*>::iterator it = cache.find(key);
        if (it != cache.end()) {
            LRUItem *item = it->second;
            update(item);
            item->value = value;
            return;
        }
        
        //capacity checking
        if (cache.size() >= cap && least != NULL) {
            int least_key = least->key;
            int used_old = least->used;
            cache.erase(least_key);
            order.erase(used_old);
            delete least; least = NULL;
            next_least(used_old);
        }
        
        //new item
        LRUItem *item = new LRUItem;
        item->used = used++;
        item->key = key;
        item->value = value;
        cache[key] = item;
        order[item->used] = item;
        if (least == NULL) {
            least = item;
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache obj = new LRUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.put(key,value);
 */

//["LRUCache","put","get","put","get","get"]
//[[1],[2,1],[2],[3,2],[2],[3]]
int main1() {
    LRUCache c(1);
    c.put(2,1);
    std::cout << "get2=" << c.get(2) << std::endl;
    c.put(3,2);
    std::cout << "get2=" << c.get(2) << std::endl;
    std::cout << "get3=" << c.get(3) << std::endl;
    return 0;
}

//["LRUCache","put","put","get","put","get","put","get","get","get"]
//[[2],[1,1],[2,2],[1],[3,3],[2],[4,4],[1],[3],[4]]
int main() {
    LRUCache cache( 2 /* capacity */ );

    cache.put(1, 1);  std::cout << "put(1,1)" << std::endl;
    cache.put(2, 2);  std::cout << "put(2,2)" << std::endl;
    std::cout << "get(1)=1 <==> " << cache.get(1) << std::endl;       // returns 1
    cache.put(3, 3);  std::cout << "put(3,3)" << std::endl;  // evicts key 2
    std::cout << "get(2)=-1 <==> " << cache.get(2) << std::endl;       // returns -1 (not found)
    cache.put(4, 4);  std::cout << "put(4,4)" << std::endl;  // evicts key 1
    std::cout << "get(1)=-1 <==> " << cache.get(1) << std::endl;       // returns -1 (not found)
    std::cout << "get(3)=3 <==> " << cache.get(3) << std::endl;       // returns 3
    std::cout << "get(4)=4 <==> " << cache.get(4) << std::endl;       // returns 4
    
    return 0;
}


