#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

#include <cassert>
#include <string>
#include <iostream>
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
    int hashfunc(const KeyType& key) const
    {
        unsigned int hasher(const KeyType& key);
        return hasher(key) % m_size;
    }
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
    unsigned int bucket = hashfunc(key);
    Node* p = m_buckets[bucket];
    if (p == nullptr)
    {
        m_buckets[bucket] = new Node;
        m_buckets[bucket]->key = key;
        m_buckets[bucket]->value = value;
        m_buckets[bucket]->next = nullptr;
        m_nItems++;
    }
    else
    {
        bool update = false;
        while(p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                update = true;
            }
            p = p->next;
        }
        if (!update)
        {
            Node* n = new Node;
            n->key = key;
            n->value = value;
            n->next = m_buckets[bucket];
            m_buckets[bucket] = n;
            m_nItems++;
        }
    }
    if (getLoadFactor() >= 0.5)
    {
        resizeArray();
    }
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resizeArray()
{
    // cerr << "******RESIZING*******" << endl;
    m_size *= 2;
    cerr << m_size << endl;
    Node** newArray = new Node*[m_size];
    for (int i = 0; i < m_size; i++)
        newArray[i] = nullptr;
    for (int i = 0; i < m_size/2; i++)
    {
        // For every linked list in m_buckets
        if (m_buckets[i] != nullptr)
        {
            Node* p = m_buckets[i];
            while (p != nullptr)
            {
                unsigned int bucket = hashfunc(p->key);
                if (newArray[bucket] == nullptr)
                {
                    newArray[bucket] = new Node;
                    newArray[bucket]->key = p->key;
                    newArray[bucket]->value = p->value;
                    newArray[bucket]->next = nullptr;
                }
                else
                {
                    Node* n = new Node;
                    n->key = p->key;
                    n->value = p->value;
                    n->next = newArray[bucket];
                    newArray[bucket] = n;
                }
                Node* n = p->next;
                delete p;
                p = nullptr;
                p = n;
            }
            delete p;
            p = nullptr;
        }
    }
    delete [] m_buckets;
    m_buckets = newArray;
    // cerr << "*****FINISH RESIZING*****" << endl;
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int bucket = hashfunc(key);
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
    return (getNumItems() * 1.0 / m_size);
}
#endif
