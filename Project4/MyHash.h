// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.
#ifndef MYHASH_INCLUDED
#define MYHASH_INCLUDED

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
        bool used;
        Node* next;
    };
    int m_size;
    int m_nItems;
    double m_maxLF;
    Node** m_buckets;
    void resizeArray();
    void swap(MyHash& mh1, MyHash& mh2);
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
            Node* kill = new Node;
            kill = p;
            p = p->next;
            delete kill;
        }
    }
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
    // Find the bucket that the key corresponds to
    unsigned int hasher(const KeyType& key);
    unsigned int bucket = hasher(key) % m_size;
    Node* n = new Node;
    n->value = value;
    n->key = key;
    n->used = true;
    // Look at the linked list in the bucket
    if (m_buckets[bucket] == nullptr)
    {
        m_buckets[bucket] = new Node;
        m_buckets[bucket] = n;
        m_nItems++;
    }
    else if (find(key) != nullptr)
    {
        *(find(key)) = value;
        delete n;
        n = nullptr;
    }
    else
    {
        n->next = m_buckets[bucket];
//        if (m_buckets[bucket]->used == false)
//        {
//            // delete m_buckets[bucket];
//            n->next = nullptr;
//        }
        m_buckets[bucket] = n;
        m_nItems++;
    }
    
    if (getLoadFactor() > m_maxLF)
    {
        resizeArray();
    }
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resizeArray()
{
    Node** newArray = new Node* [2 * m_size];
    for (int i = 0; i < 2 * m_size; i++)
    {
        newArray[i] = nullptr;
    }
    
    for (int i = 0; i < m_size; i++)
    {
        Node* tempN = m_buckets[i];
        while (tempN != nullptr)
        {
            KeyType tempK = tempN->key;
            ValueType tempV = tempN->value;
        
            // Get the node ready to be added
            Node* n = new Node;
            n->value = tempV;
            n->key = tempK;
            n->used = true;
            n->next = nullptr;
        
            // find out where the target bucket is in newArray
            unsigned int hasher(const KeyType& key);
            unsigned int bucket = hasher(tempK) % (2 * m_size);
            // cerr << "Key: " << tempK << ". At the new bucket: " << bucket << endl;
        
            if (m_buckets[bucket] == nullptr)
            {
                // If the bucket has never been used,
                // Assign the bucket to the new node
                m_buckets[bucket] = new Node;
                m_buckets[bucket] = n;
            }
            else if (find(tempK) != nullptr)
            {
                *(find(tempK)) = tempV;
                delete n;
                n = nullptr;
            }
            else
            {
                n->next = m_buckets[bucket];
//                if (m_buckets[bucket]->used == false)
//                {
//                    // delete m_buckets[bucket];
//                    n->next = nullptr;
//                }
                m_buckets[bucket] = n;
            }
            tempN = tempN->next;
        }
        delete tempN;
    }
    m_size *= 2;
    delete [] m_buckets;
    m_buckets = newArray;
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

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::swap(MyHash<KeyType, ValueType>& mh1, MyHash<KeyType, ValueType>& mh2)
{
    // swap everything
    Node** tempNode = mh1.m_buckets;
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

#endif



