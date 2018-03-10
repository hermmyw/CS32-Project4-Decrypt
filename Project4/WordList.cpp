#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <iostream>
#include <cassert>
using namespace std;

class WordListImpl
{
public:
    WordListImpl();
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    // Key: word; Value: pattern
    MyHash<string, string> m_wordPatterList;
};

WordListImpl::WordListImpl()
:m_wordPatterList()
{ }

bool WordListImpl::loadWordList(string filename)
{
    if (m_wordPatterList.getNumItems() != 0)
        m_wordPatterList.reset();
    ifstream infile(filename);
    if(!infile)
    {
        cerr << "Cannot open " << filename << endl;
        return false;
    }
    
    string s;
    while (getline(infile, s))
    {
//        unsigned int hash(const string& s);
//        int bucket = hash(s) % NUM_BUCK;
        m_wordPatterList.associate(s, "");
    }
    cerr << m_wordPatterList.getNumItems() << endl;
    return true;
    
}

bool WordListImpl::contains(string word) const
{
    for (int i = 0; i < word.size(); i++)
        word[i] = tolower(word[i]);
    if (m_wordPatterList.find(word) != nullptr)
        return true;
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    return vector<string>();  // This compiles, but may not be correct
}

//***** hash functions for string, int, and char *****

unsigned int hash(const string& s)
{
    return hash<string>()(s);
}

unsigned int hash(const int& i)
{
    return hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}




const string FILENAME = "/Users/hermmy/Documents/2017-2018/CS32/Project4/Project4/wordlist.txt";
int main()
{
    WordList w;
    w.loadWordList(FILENAME);
    assert(w.contains("ABDuctors"));
    return 0;
}
