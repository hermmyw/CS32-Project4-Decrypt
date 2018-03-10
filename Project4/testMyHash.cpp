//
//  testMyHash.cpp
//  Project4
//
//  Created by Huimin Wang on 3/9/18.
//  Copyright © 2018 Huimin Wang. All rights reserved.
//
#include <iostream>
#include <cassert>
#include <string>
// #include "MyHash.h"
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
    

    // An open hash table:
    // Buckets: Key, pointer to the values
    // Nodes: Value, pointer to the next node
    struct Node
    {
        ValueType value;
        KeyType key;
        Node* next;
    };
//    struct Bucket
//    {
//        Node* head;
//    };
    
    int m_size;
    int m_nItems;
    double m_maxLF;
    Node** m_buckets;
    void resizeArray(Node** oldArray, int n);
    void swap(MyHash& mh1, MyHash& mh2);
    private:
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
    m_buckets = new Node*[NUM_BUCK];
}

template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    cerr << "MyHash Destructor" << endl;
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
    cerr << "Enter associate" << endl;
    // Create a new node for the new key and value
    Node* n = new Node;
    n->value = value;
    n->key = key;
    n->next = nullptr;
    
    // Find the bucket that the key corresponds to
    unsigned int hash(const KeyType& key);
    int bucket = hash(key) % NUM_BUCK;
    cerr << bucket << endl;
    
    // Look at the linked list in the bucket
    Node* p = m_buckets[bucket];
    cerr << p << endl;
    if (p == nullptr)
    {
        // If the bucket has never been used,
        // Assign the bucket to the new node
        m_buckets[bucket] = n;
        cerr << "Successfully assign the new node to the head of the linked list" << endl;
    }
    else
    {
        while(p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                cerr << "Successfully update the value" << endl;
                return;
            }
            if (p->next != nullptr)
                p = p->next;
        }
        p->next = n;
        cerr << "Successfully assign the new node to the tail of the linked list" << endl;
    }
    m_nItems++;
    
    if (getLoadFactor() > m_maxLF)
    {
        cerr << "Load factor is too big" << endl;
        // allocate new bigger dynamic array
        resizeArray(m_buckets, NUM_BUCK);
    }
    cerr << "Leave associate" << endl << endl;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resizeArray(Node** oldArray, int size)
{
    Node** newArray = new Node* [2 * size];
    for (int i = 0; i < size; i++)
    {
        if (oldArray[i] != nullptr)
        {
            KeyType tempK = oldArray[i]->key;
            ValueType tempV = oldArray[i]->value;
            Node* n = new Node;
            n->value = tempV;
            n->key = tempK;
            n->next = nullptr;
            
            unsigned int hash(const KeyType& key);
            // find out where the target bucket is in newArray
            int bucket = hash(tempK) % (2 * size);
            cerr << tempK << tempV << bucket << endl;
            Node* p = newArray[bucket];
            cerr << p << endl;
            if (p == nullptr)
            {
                // If the bucket has never been used,
                // Assign the bucket to the new node
                newArray[bucket] = n;
            }
            else
            {
                while(p != nullptr)
                {
                    if (p->key == tempK)
                    {
                        p->value = tempV;
                        return;
                    }
                    if (p->next != nullptr)
                        p = p->next;
                }
                p->next = n;
            }
        }
    }
    delete [] oldArray;
    oldArray = newArray;
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    ValueType* vp = new ValueType;
    unsigned int hash(const KeyType& key);
    int bucket = hash(key) % NUM_BUCK;
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
    Node* tempNode = mh1.m_buckets;
    mh1.m_buckets = mh2.m_buckets;
    mh2.m_buckets = tempNode;
    
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

void test()
{
    MyHash<string, string> test1(0.05);
    
    test1.associate("indeed", "abcddc");
    test1.associate("kayuuy", "abcddc");
    test1.associate("hafsrxc", "abcdefg");
    test1.associate("hafsrxc", "abfdefg");
    test1.associate("ysahha", "abcddc");
    test1.associate("cmyaldw", "abcdefg");
    test1.associate("xycakld", "abcdefg");
    
    assert(test1.getNumItems() == 6);
    cerr << test1.getLoadFactor() << endl;
    
    for (int i = 0; i < NUM_BUCK; i++)
    {
        if (test1.m_buckets[i] == nullptr)
            cout << "Empty bucket " << i << endl;
        else
        {
            cout << "Key: " << test1.m_buckets[i]->key << endl;
            MyHash<string, string>::Node* p = test1.m_buckets[i];
            while (p != nullptr)
            {
                cout << "   Value: " << p->value << endl;
                p = p->next;
            }
        }
    }
    cout << "Passed" << endl;
    
}
int main()
{
    test();
}
