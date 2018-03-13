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
        delete m_buckets[i];
    delete [] m_buckets;
}

template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    MyHash<KeyType, ValueType>* temp = new MyHash(getLoadFactor());
    swap(*this, *temp);
    //    for (int i = 0; i < m_size; i++)
    //        cerr << m_buckets[i] << " " << i << endl;
    // cerr << "temp: ";
    delete temp;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    // cerr << "Enter associate" << endl;
    // Create a new node for the new key and value
    Node* n = new Node;
    n->value = value;
    n->key = key;
    n->next = nullptr;
    
    // Find the bucket that the key corresponds to
    unsigned int hasher(const KeyType& key);
    unsigned int bucket = hasher(key) % m_size;
    // cerr << bucket << endl;
    
    // Look at the linked list in the bucket
    Node* p = new Node;
    p = m_buckets[bucket];
    // cerr << p << endl;
    // cerr << "Key: " << n->key << " Bucket" << bucket << endl;
    if (p == nullptr)
    {
        // If the bucket has never been used,
        // Assign the bucket to the new node
        m_buckets[bucket] = n;
        //cerr << "ADD NEW" << endl;
    }
    else
    {
        while(p != nullptr)
        {
            if (p->key == key)
            {
                p->value = value;
                delete n;
                // cerr << "UPDATE" << endl << "Leave associate" << endl << endl;
                return;
            }
            p = p->next;
        }
        n->next = m_buckets[bucket];
        m_buckets[bucket] = n;
        // cerr << "ADD" << endl;
    }
    m_nItems++;
    
    if (getLoadFactor() > m_maxLF)
    {
        // allocate new bigger dynamic array
        resizeArray();
        //        for (int i = 0; i < m_size; i++)
        //            cerr << m_buckets[i] << " " << i << endl;
    }
    // cerr << "Leave associate" << endl << endl;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resizeArray()
{
    Node** temp = m_buckets;
    m_buckets = new Node* [2 * m_size];
    for (int i = 0; i < 2 * m_size; i++)
        m_buckets[i] = nullptr;
    //    for (int i = 0; i < 2 * m_size; i++)
    //        cerr << m_buckets[i] << " " << i << endl;
    //    for (int i = 0; i < m_size; i++)
    //        cerr << temp[i] << " " << i << endl;
    
    for (int i = 0; i < m_size; i++)
    {
        if (temp[i] != nullptr)
        {
            Node* tempN = new Node;
            tempN = temp[i];
            while (tempN != nullptr)
            {
                KeyType tempK = tempN->key;
                ValueType tempV = tempN->value;
                
                // Get the node ready to be added
                Node* n = new Node;
                n->value = tempV;
                n->key = tempK;
                n->next = nullptr;
                
                // find out where the target bucket is in newArray
                unsigned int hasher(const KeyType& key);
                unsigned int bucket = hasher(tempK) % (2 * m_size);
                // cerr << "Key: " << tempK << ". At the new bucket: " << bucket << endl;
                
                Node* p = new Node;
                p = m_buckets[bucket];
                // cerr << p << endl;
                if (p == nullptr)
                {
                    // If the bucket has never been used,
                    // Assign the bucket to the new node
                    m_buckets[bucket] = n;
                    // cerr << "New Array: ADD NEW" << endl;
                }
                else
                {
                    while(p != nullptr)
                    {
                        if (p->key == tempK)
                        {
                            p->value = tempV;
                            n = nullptr;
                            // // cerr << "New Array: UPDATE" << endl;
                            break;
                        }
                        p = p->next;
                    }
                    if (n != nullptr)
                    {
                        n->next = m_buckets[bucket];
                        m_buckets[bucket] = n;
                    }
                    else
                        delete n;
                    // cerr << "New Array: ADD" << endl;
                }
                tempN = tempN->next;
            }
        }
    }
    
    //    for (int i = 0; i < 2 * m_size; i++)
    //    {
    //        if (m_buckets[i] == nullptr)
    //            cerr << "Empty Bucket at " << i << endl;
    //        else
    //            cerr << "Key: " << m_buckets[i]->key << endl << "  Value:" << m_buckets[i]->value << endl;
    //    }
    //    cerr << "New load factor " << (m_nItems * 1.0 / (2 * m_size)) << endl;
    m_size *= 2;
    //    cerr << "New size " << m_size << endl;
    delete [] temp;
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hasher(const KeyType& key);
    unsigned int bucket = hasher(key) % m_size;
    // cerr << bucket << endl;
    Node* p = m_buckets[bucket];
    // cerr << p << endl;
    while (p != nullptr)
    {
        // cerr << "Key: " << p->key << endl;
        if  (p->key == key)
            return &(p->value);
        else
            p = p->next;
    }
    return nullptr;
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
