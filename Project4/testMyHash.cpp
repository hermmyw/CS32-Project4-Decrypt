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
#include <vector>
#include "MyHash.h"
//using namespace std;
//
//const int NUM_BUCK = 100;
//
//template<typename KeyType, typename ValueType>
//class MyHash
//{
//public:
//    MyHash(double maxLoadFactor = 0.5);
//    ~MyHash();
//    void reset();
//    void associate(const KeyType& key, const ValueType& value);
//    int getNumItems() const;
//    double getLoadFactor() const;
//
//    // for a map that can't be modified, return a pointer to const ValueType
//    const ValueType* find(const KeyType& key) const;
//
//    // for a modifiable map, return a pointer to modifiable ValueType
//    ValueType* find(const KeyType& key)
//    {
//        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
//    }
//
//    // C++11 syntax for preventing copying and assignment
//    MyHash(const MyHash&) = delete;
//    MyHash& operator=(const MyHash&) = delete;
//
//
//    // An open hash table:
//    // Buckets: Key, pointer to the values
//    // Nodes: Value, pointer to the next node
//    struct Node
//    {
//        ValueType value;
//        KeyType key;
//        Node* next;
//    };
////    struct Bucket
////    {
////        Node* head;
////    };
//
//
//    int m_size;
//    int m_nItems;
//    double m_maxLF;
//    Node** m_buckets;
//    void resizeArray();
//    void swap(MyHash& mh1, MyHash& mh2);
//private:
//};
//
//template<typename KeyType, typename ValueType>
//MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
//{
//    m_maxLF = maxLoadFactor;
//    if (maxLoadFactor <= 0)
//        m_maxLF = 0.5;
//    else if (maxLoadFactor > 2)
//        m_maxLF = 2.0;
//    m_size = NUM_BUCK;
//    m_nItems = 0;
//    m_buckets = new Node*[NUM_BUCK];
//    for (int i = 0; i < NUM_BUCK; i++)
//    {
//        m_buckets[i] = nullptr;
//    }
//}
//
//template<typename KeyType, typename ValueType>
//MyHash<KeyType, ValueType>::~MyHash()
//{
//    cerr << "MyHash Destructor" << endl;
//}
//
//template<typename KeyType, typename ValueType>
//void MyHash<KeyType, ValueType>::reset()
//{
//    MyHash<KeyType, ValueType>* temp = new MyHash(getLoadFactor());
//    swap(*this, *temp);
////    for (int i = 0; i < m_size; i++)
////        cerr << m_buckets[i] << " " << i << endl;
//    cerr << "temp: ";
//    delete temp;
//}
//
//template <class KeyType, class ValueType>
//void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
//{
//    
//    // Create a new node for the new key and value
//    Node* n = new Node;
//    n->value = value;
//    n->key = key;
//    n->next = nullptr;
//
//    // Find the bucket that the key corresponds to
//    unsigned int hash(const KeyType& key);
//    unsigned int bucket = hash(key) % m_size;
//    cerr << bucket << endl;
//
//    // Look at the linked list in the bucket
//    Node* p = new Node;
//    p = m_buckets[bucket];
//    cerr << p << endl;
//    if (p == nullptr)
//    {
//        // If the bucket has never been used,
//        // Assign the bucket to the new node
//        m_buckets[bucket] = n;
//        cerr << "Successfully assign the new node to the head of the linked list" << endl;
//    }
//    else
//    {
//        while(p != nullptr)
//        {
//            if (p->key == key)
//            {
//                p->value = value;
//                cerr << "Successfully update the value" << endl << "Leave associate" << endl << endl;
//                return;
//            }
//            if (p->next != nullptr)
//                p = p->next;
//        }
//        p->next = n;
//        cerr << "Successfully assign the new node to the tail of the linked list" << endl;
//    }
//    m_nItems++;
//
//    if (getLoadFactor() > m_maxLF)
//    {
//        cerr << "******Exceeds Max Load factor******" << endl << endl;
//        // allocate new bigger dynamic array
//        resizeArray();
////        for (int i = 0; i < m_size; i++)
////            cerr << m_buckets[i] << " " << i << endl;
//    }
//    cerr << "Leave associate" << endl << endl;
//}
//
//template <class KeyType, class ValueType>
//void MyHash<KeyType, ValueType>::resizeArray()
//{
//    cerr << "Resizing" << endl;
//    Node** temp = m_buckets;
//    m_buckets = new Node* [2 * m_size];
//    for (int i = 0; i < 2 * m_size; i++)
//        m_buckets[i] = nullptr;
////    for (int i = 0; i < 2 * m_size; i++)
////        cerr << m_buckets[i] << " " << i << endl;
////    for (int i = 0; i < m_size; i++)
////        cerr << temp[i] << " " << i << endl;
//
//    for (int i = 0; i < m_size; i++)
//    {
//        if (temp[i] != nullptr)
//        {
//            KeyType tempK = temp[i]->key;
//            ValueType tempV = temp[i]->value;
//
//            // Get the node ready to be added
//            Node* n = new Node;
//            n->value = tempV;
//            n->key = tempK;
//            n->next = nullptr;
//
//            // find out where the target bucket is in newArray
//            unsigned int hash(const KeyType& key);
//            unsigned int bucket = hash(tempK) % (2 * m_size);
//            cerr << "Key: " << tempK << ". Value: " << tempV << ". At the new bucket: " << bucket << endl;
//
//            Node* p = new Node;
//            p = m_buckets[bucket];
//            cerr << p << endl;
//            if (p == nullptr)
//            {
//                // If the bucket has never been used,
//                // Assign the bucket to the new node
//                m_buckets[bucket] = n;
//                cerr << "New Array: Successfully assign the node to the head of the linked list" << endl;
//            }
//            else
//            {
//                while(p != nullptr)
//                {
//                    if (p->key == tempK)
//                    {
//                        p->value = tempV;
//                        n = nullptr;
//                        cerr << "New Array: Successfully update the value" << endl;
//                        break;
//                    }
//                    if (p->next != nullptr)
//                        p = p->next;
//                }
//                if (n != nullptr && p->next == nullptr)
//                    p->next = n;
//                else
//                    delete n;
//                cerr << "New Array: Successfully assign the new node to the tail of the linked list" << endl;
//            }
//        }
//    }
//
////    for (int i = 0; i < 2 * m_size; i++)
////    {
////        if (m_buckets[i] == nullptr)
////            cerr << "Empty Bucket at " << i << endl;
////        else
////            cerr << "Key: " << m_buckets[i]->key << endl << "  Value:" << m_buckets[i]->value << endl;
////    }
////    cerr << "New load factor " << (m_nItems * 1.0 / (2 * m_size)) << endl;
//    m_size *= 2;
////    cerr << "New size " << m_size << endl;
//    delete [] temp;
//    cerr << "*****Finish Resizing******" << endl << endl;
//}
//
//template <class KeyType, class ValueType>
//const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
//{
//    unsigned int hash(const KeyType& key);
//    unsigned int bucket = hash(key) % m_size;
//    Node* p = m_buckets[bucket];
//    while (p != nullptr)
//    {
//        if  (p->key == key)
//            return &(p->value);
//        else
//            p = p->next;
//    }
//    return nullptr;
//}
//
//// template <class KeyType, class ValueType>
//// ValueType* MyHash<class KeyType, class ValueType>::find(const KeyType& key) const
//// {
////     ValueType* vp = new ValueType;
////     unsigned int hash(const KeyType& key);
////     int bucket = hash(key);
////     Node* p = m_buckets[bucket]->m_head;
////     while (p != nullptr)
////     {
////         if  (p->key == key)
////             vp = &(p->value);
////         else
////             p = p->next;
////     }
////     vp = nullptr;
//// }
//
//template <class KeyType, class ValueType>
//int MyHash<KeyType, ValueType>::getNumItems() const
//{
//    return m_nItems;
//}
//
//template <class KeyType, class ValueType>
//double MyHash<KeyType, ValueType>::getLoadFactor() const
//{
//    return (m_nItems * 1.0 / m_size);
//}
//
//template <class KeyType, class ValueType>
//void MyHash<KeyType, ValueType>::swap(MyHash<KeyType, ValueType>& mh1, MyHash<KeyType, ValueType>& mh2)
//{
//    // swap everything
//    Node** tempNode = mh1.m_buckets;
//    mh1.m_buckets = mh2.m_buckets;
//    mh2.m_buckets = tempNode;
//
//    int tempS = mh1.m_size;
//    mh1.m_size = mh2.m_size;
//    mh2.m_size = tempS;
//
//    int tempN = mh1.m_nItems;
//    mh1.m_nItems = mh2.m_nItems;
//    mh2.m_nItems = tempN;
//
//    double tempM = mh1.m_maxLF;
//    mh1.m_maxLF = mh2.m_maxLF;
//    mh2.m_maxLF = tempM;
//}

//void test()
//{
//    MyHash<string, vector<string>> test1(0.05);
//    vector<string> vs;
//    vs.push_back("'em'");
//    vs.push_back("any");
//    // test1.associate("abc", vs);
//    // assert(test1.find("abc") != nullptr);
//    for (int i = 0; i < 100000; i++)
//        vs.push_back("huy");
//    // test1.associate("abc", vs)
//    if (test1.find("abc") == nullptr)
//    {
//        vector<string> wordsWithSamePattern;
//        wordsWithSamePattern.push_back("'em");
//        test1.associate("abc", wordsWithSamePattern);
//    }
//    for (int i = 0; i < 9000; i++)
//        test1.associate("ghds" + to_string(i), vs);
//    for (int i = 0; i < 100000; i++)
//        (*test1.find("abc")).push_back(to_string(i));
//    for (int i = 0; i < (*test1.find("abc")).size(); i++)
//        cerr << (*test1.find("abc"))[i] << endl;
//}


//int main()
//{
//    test();
//}

