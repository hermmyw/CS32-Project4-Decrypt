#include "provided.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

const int MAX = 10;
class TranslatorImpl
{
public:
    TranslatorImpl();
    ~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    // Mapping table stack:
    struct Node
    {
        char m_map[2][26];
        Node* next;
    };
    struct Stack
    {
        Node* m_head;
        void push(const char data[2][26])
        {
            Node* n = new Node;
            for (int i = 0; i < 26; i++)
            {
                n->m_map[0][i] = data[0][i];
                n->m_map[1][i] = data[1][i];
            }
            if (m_head == nullptr)
            {
                m_head = n;
                n->next = nullptr;
            }
            else
            {
                n->next = m_head;
                m_head = n;
            }
        }
        void pop()
        {
            if (m_head == nullptr)
                return;
            else
            {
                Node* temp = m_head;
                m_head = m_head->next;
                delete temp;
            }
        }
        Node* top()
        {
            return m_head;
        }
    };
    // Mapping stack
    Stack m_mappingStack;
    
    // Current Mapping table
    char m_mappingTable[2][26];
    
    // All case-insensitive
    void upperCase(string& s);
};

TranslatorImpl::TranslatorImpl()
{
    m_mappingStack.m_head = new Node;
    m_mappingStack.m_head = nullptr;
    for (int i = 0; i < 26; i++)
    {
        m_mappingTable[0][i] = 'A' + i;
        m_mappingTable[1][i] = '?';
    }
}

TranslatorImpl::~TranslatorImpl()
{ }

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    upperCase(ciphertext);
    upperCase(plaintext);
    if (ciphertext.size() != plaintext.size())
        return false;
    for (int i = 0; i < ciphertext.size(); i++)
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
            return false;
    for (int i = 0; i < ciphertext.size(); i++)
    {
        int index = ciphertext[i] - 'A';
        if (m_mappingTable[1][index] != '?' && m_mappingTable[1][index] != plaintext[i])
            return false;
    }
    for (int i = 0; i < plaintext.size(); i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (plaintext[i] == m_mappingTable[1][j] && ciphertext[i] != m_mappingTable[0][j])
                return false;
        }
    }
    
    // Update the current mapping table
    for (int i = 0; i < ciphertext.size(); i++)
    {
        int index = ciphertext[i] - 'A';
        if (m_mappingTable[0][index] == ciphertext[i] && m_mappingTable[1][index] == '?')
            m_mappingTable[1][index] = plaintext[i];
    }
    
    cerr << "Pushed plain text table" << endl;
    for (int i = 0; i < 26; i++)
        cerr << m_mappingTable[0][i];
    cerr << endl;
    for (int i = 0; i < 26; i++)
        cerr << m_mappingTable[1][i];
    cerr << endl;
    m_mappingStack.push(m_mappingTable);
    
    
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (m_mappingStack.m_head == nullptr)
        return false;
    cerr << "Enter pop" << endl;
    m_mappingStack.pop();
    // Restoring the old one
    Node* top = m_mappingStack.top();
    if (top != nullptr)
    {
        for (int i = 0; i < 26; i++)
        {
            m_mappingTable[0][i] = top->m_map[0][i];
            m_mappingTable[1][i] = top->m_map[1][i];
        }
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            m_mappingTable[0][i] = 'A' + i;
            m_mappingTable[1][i] = '?';
        }
    }
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    cerr << "Current plain text table" << endl;
    for (int i = 0; i < 26; i++)
        cerr << m_mappingTable[0][i];
    cerr << endl;
    for (int i = 0; i < 26; i++)
        cerr << m_mappingTable[1][i];
    cerr << endl;
    
    string result;
    for (int i = 0; i < ciphertext.size(); i++)
        result += '?';
    
    for (int i = 0; i < ciphertext.size(); i++)
    {
        if (isalpha(ciphertext[i]))
        {
            if (islower(ciphertext[i]))
            {
                int index = ciphertext[i] - 'a';
                result[i] = m_mappingTable[1][index];
                result[i] = tolower(result[i]);
            }
            else if (isupper(ciphertext[i]))
            {
                int index = ciphertext[i] - 'A';
                result[i] = m_mappingTable[1][index];
                result[i] = toupper(result[i]);
            }
        }
        else
            result[i] = ciphertext[i];
    }
    return result;
}


void TranslatorImpl::upperCase(string &s)
{
    for (int i = 0; i < s.size(); i++)
        if (isalpha(s[i]))
            s[i] = toupper(s[i]);
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}







//int main()
//{
//    Translator t;
//    string s = "y qook ra bdttook yqkook!";
//    cerr << t.getTranslation(s) << endl;
//    t.pushMapping("bdttook", "BALLOON");
//    t.pushMapping("ra", "CD");
//    cerr << t.getTranslation(s) << endl;
//    assert(!t.pushMapping("a", "R"));
//    assert(!t.pushMapping("fd", "FD"));
//    t.popMapping();
//    t.popMapping();
//    assert(!t.popMapping());
//    cerr << t.getTranslation(s) << endl;
//}

