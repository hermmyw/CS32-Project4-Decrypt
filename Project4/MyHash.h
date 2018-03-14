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
    if (key == "abacdefbfaghifj")
    {
        cerr << "FOUND with a bucket number " << bucket << endl;
        cerr << p->key << endl;
    }
    if (p == nullptr)
    {
        m_buckets[bucket] = new Node;
        m_buckets[bucket]->key = key;
        m_buckets[bucket]->value = value;
        m_buckets[bucket]->next = nullptr;
        return;
    }
    else
    {
        while(p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                if (key == "abacdefbfaghifj")
                {
                    cerr << "****FOUND" << endl;
                    cerr << p->key << endl;
                }
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
    cerr << "******RESIZING*******" << endl;
    m_size *= 2;
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
    
    // Delete every node in the old buckets
//    for (int i = 0; i < m_size; i++)
//    {
//        if (m_buckets[i] != nullptr)
//        {
//            Node* p = m_buckets[i];
//            while (p != nullptr)
//            {
//                Node* n = p->next;
//                delete p;
//                p = nullptr;
//                p = n;
//            }
//        }
//    }
    delete [] m_buckets;
    
    m_buckets = newArray;
    cerr << "*****FINISH RESIZING*****" << endl;
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int bucket = hashfunc(key);
    if (key == "abacdefbfaghifj")
        cerr << "*******************************" << bucket << endl;
    Node* p = m_buckets[bucket];
    if (p == nullptr)
        cerr << "你他妈怎么能是null" << endl;
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

//#ifndef MYHASH_INCLUDED
//#define MYHASH_INCLUDED
//
//#include <unordered_map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//#include <algorithm>
//
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL containers, and this
//// implementation uses std::unordered_map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyHash class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyHash class template.
//
//template<typename KeyType, typename ValueType>
//class MyHash
//{
//public:
//    MyHash(const MyHash&) = delete;
//    MyHash& operator=(const MyHash&) = delete;
//    using O=KeyType;
//    using maxloadfactor=float;
//    using O10=int;
//    void reset()
//    {
//        maxloadfactor max_1oad_factor=l01.max_load_factor();l01.clear();l01.
//        max_load_factor(max_1oad_factor);l01.rehash(doub1e+doub1e);
//
//    }
//    using String=double;
//    using l0=ValueType;
//    using l1O=O10 const;
//    MyHash(String d0uble=doub1e/10):
//    l01(doub1e+doub1e)
//    {
//        l01.max_load_factor(std::min<maxloadfactor>(doub1e/5/5,std::max<maxloadfactor>((double)doub1e/(doub1e+doub1e),d0uble)));}using l10=O
//    const;using Const=MyHash<O,l0>;String getLoadFactor()const{return l01.
//        load_factor();}using ll0=l0 const;O10 getNumItems()const{return l01.size(
//            );}using l00=Const const;void associate(l10&Using,ll0&first){l01[Using]=
//                first;}using l1=std::unordered_map<O,l0>;ll0*find(l10&l11)const{auto first(l01.
//                                                                                           find(l11));return(first!=l01.end()?&first->second:0);}l0*find(l10&l01){return(
//                                                                                                                                                                         l0*)(*(l00*)(this)).find(l01);}private:static l1O doub1e{50};l1 l01;
//};
//
// #endif // MYHASH_INCLUDED

