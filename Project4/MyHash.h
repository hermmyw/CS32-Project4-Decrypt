#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

#include <cassert>
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
        ValueType value;
        KeyType key;
        Node* next;
    };
    int m_size;
    int m_nItems;
    double m_maxLF;
    Node** m_buckets;
    void resizeArray();
};

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
:m_maxLF(maxLoadFactor), m_size(NUM_BUCK), m_nItems(0)
{
    if (maxLoadFactor <= 0)
        m_maxLF = 0.5;
    else if (maxLoadFactor > 2.0)
        m_maxLF = 2.0;
    m_buckets = new Node*[NUM_BUCK];
    for (int i = 0; i < NUM_BUCK; i++)
    {
        m_buckets[i] = nullptr;
    }
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    for (int i = 0; i < m_size; i++)
    {
        Node* p = m_buckets[i];
        while (p != nullptr)
        {
            Node* n = p->next;
            delete p;
            p = n;
        }
    }
    delete [] m_buckets;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    // Delete the old array
    for (int i = 0; i < m_size; i++)
    {
        Node* p = m_buckets[i];
        while (p != nullptr)
        {
            Node* n = p->next;
            delete p;
            p = n;
        }
    }
    delete [] m_buckets;
    
    // Allocate a new Array
    m_buckets = new Node*[NUM_BUCK];
    for (int i = 0; i < NUM_BUCK; i++)
        m_buckets[i] = nullptr;
    
    // Reset the data members
    m_nItems = 0;
    m_size = NUM_BUCK;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hasher(const KeyType& key);
    unsigned int bucket = hasher(key) % m_size;
    
    Node* p = m_buckets[bucket];
    if (p == nullptr)
    {
        m_buckets[bucket] = new Node;
        m_buckets[bucket]->key = key;
        m_buckets[bucket]->value = value;
        m_buckets[bucket]->next = nullptr;
    }
    else
    {
        while(p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                return;
            }
            p = p->next;
        }
        Node* n = new Node;
        n->key = key;
        n->value = value;
        n->next = m_buckets[bucket];
        m_buckets[bucket] = n;
    }
    m_nItems++;
    
    if (getLoadFactor() > m_maxLF)
        resizeArray();
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resizeArray()
{
    Node** newArray = new Node*[2 * m_size];
    for (int i = 0; i < 2 * m_size; i++)
        newArray[i] = nullptr;
    for (int i = 0; i < m_size; i++)
    {
        if (m_buckets[i] != nullptr)
        {
            unsigned int hasher(const KeyType& key);
            unsigned int bucket = hasher(m_buckets[i]->key) % (2 * m_size);
            newArray[bucket] = m_buckets[i];
        }
    }
    delete [] m_buckets;
    m_buckets = newArray;
    m_size *= 2;
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& key);
    unsigned int bucket = hasher(key) % m_size;
    Node* p = m_buckets[bucket];
    while (p != nullptr)
    {
        if  (p->key == key)
            return &(p->value);
        else
            p = p->next;
    }
    return nullptr;
}

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_nItems;
}

template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return (m_nItems * 1.0 / m_size);
}
#endif
