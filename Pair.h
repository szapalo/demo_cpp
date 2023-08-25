
#ifndef NUNU_PAIR_H_
#define NUNU_PAIR_H_


template <typename K, typename V>
struct Pair {
    K key;
    V value;
    Pair(K k, V v):key(k),value(v){}
    
    bool operator>(const Pair<K,V>& p) const {
        return (key > p.key) || ( key == p.key && value > p.value );
    }
    bool operator<(const Pair<K,V>& p) const {
        return !operator>(p);
    }
    bool operator==(const Pair<K,V>& p) const {
        return (key == p.key && value == p.value);
    }
    bool operator!=(const Pair<K,V>& p) const {
        return (key != p.key || value != p.value);
    }
    bool operator>=(const Pair<K,V>& p) const {
        return operator==(p) || operator>(p);
    }
    bool operator<=(const Pair<K,V>& p) const {
        return operator==(p) || operator<(p);
    }

};


#endif