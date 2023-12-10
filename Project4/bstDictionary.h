#ifndef _BST_DICTIONARY_H
#define _BST_DICTIONARY_H

#include <cstdint>
#include <stdexcept>
#include <algorithm>

static const uint32_t
    NULL_INDEX = 0xffffffff,
    DEFAULT_INITIAL_CAPACITY = 16;

template <typename KeyType, typename ValueType>
class BSTDictionary {
public:
    explicit BSTDictionary(uint32_t _cap = DEFAULT_INITIAL_CAPACITY) {
        if (nTrees == 0) {
            counts = new uint32_t[DEFAULT_INITIAL_CAPACITY];
            heights = new uint32_t[DEFAULT_INITIAL_CAPACITY];
            left = new uint32_t[DEFAULT_INITIAL_CAPACITY];
            right = new uint32_t[DEFAULT_INITIAL_CAPACITY];
            keys = new KeyType[DEFAULT_INITIAL_CAPACITY];
            values = new ValueType[DEFAULT_INITIAL_CAPACITY];

            capacity = _cap;

        genFreeList(0, _cap - 1);

        }
        nTrees = nTrees +1;
        root = NULL_INDEX;
    }

    ~BSTDictionary() {
        nTrees = nTrees -1;

        if (nTrees == 0){
            delete[] counts;
			delete[] heights;
			delete[] left;
			delete[] right;
			delete[] keys;
			delete[] values;
        }

        else{
            prvClear(root);
        }
    }

    void genFreeList(uint32_t start, uint end){
        for (int i = start; i < end -1; i++){
            left[i] = i + 1;
        }

        left[end] = NULL_INDEX;
        freeListHead = start;
    }

    void clear() {
        prvClear(root);
        root = NULL_INDEX;
        nTrees = 0;
        freeListHead = 0;
    }

    uint32_t size() {
        if (root = NULL_INDEX) {
            return 0;
        }

        return counts[root];
    }

    uint32_t height() {
        if (root == NULL_INDEX)
            return -1; //could be a problem try NULL_INDEX if broken

        return heights[root];
    }

    bool isEmpty() {
        return root == NULL_INDEX;
    }

    ValueType &search(const KeyType &k) {
        uint32_t n = root;
        while (n != NULL_INDEX){
            if (k == keys[n]){
                    return values[n];
            }
            else if (k < keys[n]) {
                n = left[n];
            }
            else{
                n = right[n];
            }
        }
        throw std::domain_error("Search: Key not Found");
    }

    ValueType &operator[](const KeyType &k) {
        uint32_t tmp;
        uint32_t n;

        tmp = prvAllocate();
        n = tmp;
        root = prvInsert(root, n, k);

        if ( n != tmp){
            prvFree(tmp);
        }
        return values[n];
    }

    void remove(const KeyType &k) {

    }

private:
    uint32_t prvAllocate() {
        uint32_t tmp;
        if (freeListHead = NULL_INDEX){
            uint32_t* tmpcounts = new uint32_t[capacity * 2];
            uint32_t* tmpheights = new uint32_t[capacity * 2];
            uint32_t* tmpleft = new uint32_t[capacity * 2];
            uint32_t* tmpright = new uint32_t[capacity * 2];
            KeyType* tmpkeys = new KeyType[capacity * 2];
            ValueType* tmpvalues = new ValueType[capacity * 2];

            for (int i = 0; i < capacity; i++){
                tmpcounts[i] = counts[i];
                tmpheights[i] = heights[i];
                tmpleft[i] = left[i];
                tmpright[i] = right[i];
                tmpkeys[i] = keys[i];
                tmpvalues[i] = values[i];
            }

            delete[] counts;
			delete[] heights;
			delete[] left;
			delete[] right;
			delete[] keys;
			delete[] values;

            counts = tmpcounts;
            heights = tmpheights;
            left = tmpleft;
            right = tmpright;
            keys = tmpkeys;
            values = tmpvalues;

            genFreeList(capacity, capacity * 2 - 1);

            capacity = capacity * 2;
        }
        
        tmp = freeListHead;
        freeListHead = left[freeListHead];

        left[tmp] = NULL_INDEX;
        right[tmp] = NULL_INDEX;
        counts[tmp] = 1;
        heights[tmp] = 1;

        return tmp; 
    }

    void prvFree(uint32_t n) {
        left[n] = freeListHead;
        freeListHead = n;
    }

    void prvClear(uint32_t r) {
        if (r != NULL_INDEX){
            prvClear(left[r]);
            prvClear(right[r]);
            prvAdjust(r);
        }
    }

    void prvAdjust(uint32_t r) {
        counts[r] = getCount(left[r]) + getHeight(right[r]) + 1;

        heights[r] = std::max(getHeight(left[r]), getHeight(right[r])) + 1;
    }

    uint32_t prvInsert(uint32_t r,uint32_t &n,const KeyType &k) {
        if (r == NULL_INDEX){
            keys[n] = k;

            return n;
        }

        if (k == keys[r]){
            n = r;
        }
        else if (k < keys[r]){
            left[r] = prvInsert(left[r], n, k);
        }
        else
            right[r] = prvInsert(right[r], n, k);

        prvAdjust(r);

        return r;
    }

    uint32_t prvRemove(uint32_t r,uint32_t &ntbd,const KeyType &k) {
        uint32_t tmp;

        if (r == NULL_INDEX){
            throw std::domain_error("Remove: Key Not Found");
        }
        if (k < keys[r]){
            left[r] = prvRemove(left[r], ntbd, k);
        }
        else if (k > keys[r]){
            right[r] = prvRemove(right[r], ntbd, k);
        }
        else {
            ntbd = r;

            if (left[r] == NULL_INDEX){
                if (right[r] == NULL_INDEX){
                    r = NULL_INDEX;
                }

                else{
                    r = right[r];
                }
            }

            else{
                if (right[r] == NULL_INDEX){
                    r = left[r];
                }

                else{
                    if (getHeight(right[r]) > getHeight[r]){
                        tmp = right[r];

                        while(left[tmp] != NULL_INDEX){
                            tmp = left[tmp];
                        }

                        std::swap (keys[r],keys[tmp]);
                        std::swap (values[r],values[tmp]);

                        right[r] = prvRemove(right[r], ntbd, k);
                    }


                        else{
                            tmp = left[r];

                            while (right[tmp] != NULL_INDEX){
                                tmp = right[tmp];
                            }

                            std::swap (keys[r],keys[tmp]);
                            std::swap (values[r],values[tmp]);

                            left[r] = prvRemove(left[r], ntbd, k);
                        }
                    }
                if (r != NULL_INDEX){
                    prvAdjust(r);
                }

                return r;
                }
        }
    }

    uint32_t getCount(uint32_t index){
        if (index != NULL_INDEX){
            return counts[index];
        }

        else
            return 0;
    }

    uint32_t getHeight(uint32_t index){
        if (index != NULL_INDEX){
            return heights[index];
        }

        else
            return 0;
    }

    uint32_t
        root;

static uint32_t
		*counts,
		*heights,
		*left,
		*right,
		nTrees,
		capacity,
		freeListHead;
	
	static KeyType
		*keys;
	
	static ValueType
		*values;

};

template<typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType, ValueType>::counts = nullptr;

template<typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType, ValueType>::heights = nullptr;

template<typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType, ValueType>::left = nullptr;

template<typename KeyType, typename ValueType>
uint32_t *BSTDictionary<KeyType, ValueType>::right = nullptr;

template<typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType, ValueType>::nTrees = 0;

template<typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType, ValueType>::capacity = 0;

template<typename KeyType, typename ValueType>
uint32_t BSTDictionary<KeyType, ValueType>::freeListHead = 0;

template<typename KeyType, typename ValueType>
KeyType *BSTDictionary<KeyType, ValueType>::keys = nullptr;

template<typename KeyType, typename ValueType>
ValueType *BSTDictionary<KeyType, ValueType>::values = nullptr;

#endif