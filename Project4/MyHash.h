// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.
#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

#include <functional>
#include <string>
using namespace std;

const int NUM_BUCK = 100;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* next;
    };
    
    struct Bucket
    {
        int slot;
        Node* m_head;
    };

    int m_size;
    int m_nItems;
    double m_maxLF;
    Bucket* m_buckets;
    void swap(MyHash& mh1, MyHash& mh2);
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    m_maxLF = maxLoadFactor;
    if (maxLoadFactor <= 0)
        m_maxLF = 0.5;
    else if (maxLoadFactor > 2)
        m_maxLF = 2.0;
    m_size = NUM_BUCK;
    m_nItems = 0;
    m_buckets = new Bucket[NUM_BUCK];
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    delete [] m_buckets;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    MyHash<KeyType, ValueType>* temp = new MyHash(getLoadFactor());
    swap(*this, *temp);
    delete temp;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    Node* n = new Node;
    n->key = key;
    n->value = value;
    n->next = nullptr;
    unsigned int hash(const KeyType& key);
    int bucket = hash(key);
    Node* p = m_buckets[bucket].m_head;
    while(p != nullptr && p->next != nullptr)
        p = p->next;
    p->next = n;
    m_nItems++;
    if (getLoadFactor() > m_maxLF)
    {
        // allocate new bigger dynamic array
        Bucket* temp = m_buckets;
        m_buckets = new Bucket[2 * NUM_BUCK];
        delete [] temp;
    }
    
}


template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    ValueType* vp = new ValueType;
    unsigned int hash(const KeyType& key);
    int bucket = hash(key);
    Node* p = m_buckets[bucket]->m_head;
    while (p != nullptr)
    {
        if  (p->key == key)
            vp = &(p->value);
        else
            p = p->next;
    }
    vp = nullptr;
}

// template <class KeyType, class ValueType>
// ValueType* MyHash<class KeyType, class ValueType>::find(const KeyType& key) const
// {
//     ValueType* vp = new ValueType;
//     unsigned int hash(const KeyType& key);
//     int bucket = hash(key);
//     Node* p = m_buckets[bucket]->m_head;
//     while (p != nullptr)
//     {
//         if  (p->key == key)
//             vp = &(p->value);
//         else
//             p = p->next;
//     }
//     vp = nullptr;
// }

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_nItems;
}

template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return (m_nItems * 1.0 / NUM_BUCK);
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::swap(MyHash<KeyType, ValueType>& mh1, MyHash<KeyType, ValueType>& mh2)
{
    // swap everything
    Bucket* tempB = mh1.m_buckets;
    mh1.m_buckets = mh2.m_buckets;
    mh2.m_buckets = tempB;
    
    int tempS = mh1.m_size;
    mh1.m_size = mh2.m_size;
    mh2.m_size = tempS;
    
    int tempN = mh1.m_nItems;
    mh1.m_nItems = mh2.m_nItems;
    mh2.m_nItems = tempN;
    
    double tempM = mh1.m_maxLF;
    mh1.m_maxLF = mh2.m_maxLF;
    mh2.m_maxLF = tempM;
}


#endif



