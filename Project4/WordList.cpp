#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <fstream>
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
    MyHash<string, vector<string>*> m_wordPatternList;
    // MyHash<string, string> m_wordToPattern;
    string pattern(const string& s) const;
    void lowerCase(string& s) const;
};

WordListImpl::WordListImpl()
:m_wordPatternList()
{ }

bool WordListImpl::loadWordList(string filename)
{
    if (m_wordPatternList.getNumItems() != 0)
        m_wordPatternList.reset();
    ifstream infile(filename);
    if(!infile)
    {
        // cerr << "Cannot open " << filename << endl;
        return false;
    }
    
    string s;
    while (getline(infile, s))
    {
        lowerCase(s);
//        unsigned int hash(const string& s);
//        int bucket = hash(s) % NUM_BUCK;
        // find the letter pattern of this word
        string p = pattern(s);
        // m_wordToPattern.associate(s, p);
        // check if the letter pattern has appeared in the hash table
        vector<string>** wordsPtr = m_wordPatternList.find(p);
        
        // if hasn't, create a vector of words
        // and add the word.
        if (wordsPtr == nullptr)
        {
            vector<string>* wordsWithSamePattern = new vector<string>;
            (*wordsWithSamePattern).push_back(s);
//            for (int i = 0; i < (*wordsWithSamePattern).size(); i++)
//                cerr << (*wordsWithSamePattern)[i] << endl;
            m_wordPatternList.associate(p, wordsWithSamePattern);
        }
        // if it has appeared, add the word to the vector
        else
        {
            (*wordsPtr)->push_back(s);
//            for (int i = 0; i < (**wordsPtr).size(); i++)
//                cerr << (**wordsPtr)[i] << endl;
            // add/update the vector of words of the letter pattern
            m_wordPatternList.associate(p, *(wordsPtr));
        }
    }
    // number of patterns
    // cerr << m_wordPatternList.getNumItems() << endl;
    // cerr << m_wordToPattern.getNumItems() << endl;
    return true;
    
}

bool WordListImpl::contains(string word) const
{
    // Case-insensitive
    lowerCase(word);
    
    string p = pattern(word);
    // cerr << "pattern: " << p << endl;
    vector<string>* const* wordsPtr = m_wordPatternList.find(p);   // O(1)
    if (wordsPtr == nullptr)
    {
        //cerr << "The Pattern is not found" << endl;
        return false;
    }
    else
    {
//        for (int i = 0; i < (**wordsPtr).size(); i++)
//            cerr << (**wordsPtr)[i] << endl;
        if ((find((**wordsPtr).begin(), (**wordsPtr).end(), word)) != (**wordsPtr).end())
        {
            // cerr << "Found" << endl;
            return true;
        }
        else
        {
            // cerr << "The pattern is found, but the word is not found" << endl;
            return false;
        }
    }
    
    // return (m_wordToPattern.find(word) != nullptr);
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    if (cipherWord.size() != currTranslation.size())
        return vector<string> ();
    for (int i = 0; i < cipherWord.size(); i++)
    {
        if (cipherWord[i] != '\'' && !isalpha(cipherWord[i]))
            return vector<string> ();
        if (currTranslation[i] != '?' && currTranslation[i] != '\'' && !isalpha(currTranslation[i]))
            return vector<string> ();
    }
    
    lowerCase(cipherWord);
    lowerCase(currTranslation);
    vector<string> candidates;
    vector<string> potentialCand;
    string p;
    p = pattern(cipherWord);
    // cerr << "Pattern of cipherword: " << p << endl;
    vector<string>* const* vsp = m_wordPatternList.find(p);
    if (vsp != nullptr)
        for (int i = 0; i < (**vsp).size(); i++)
            potentialCand.push_back((**vsp)[i]);
    
    for (int i = 0; i < currTranslation.size(); i++)
    {
        if (isalpha(currTranslation[i]) && !isalpha(cipherWord[i]))
            return vector<string>();
        if (currTranslation[i] == '?' && !isalpha(cipherWord[i]))
            return vector<string>();
        if (currTranslation[i] == '\'' && cipherWord[i] != '\'')
            return vector<string>();
    }
    for (int i = 0; i < potentialCand.size(); i++)
    {
        bool isCand = true;
        if (potentialCand[i].size() != currTranslation.size())
            isCand = false;
        for (int j = 0; j < currTranslation.size(); j++)
        {
            if (isalpha(currTranslation[j]) && currTranslation[j] != potentialCand[i][j])
                isCand = false;
            else if (currTranslation[j] == '?' && !isalpha(potentialCand[i][j]))
                isCand = false;
            else if (currTranslation[j] == '\'' && currTranslation[j] != potentialCand[i][j])
                isCand = false;
        }
        if (isCand)
        {
            candidates.push_back(potentialCand[i]);
        }
    }
    return candidates;
}

string WordListImpl::pattern(const string& s) const
{
    string pattern;
    // pattern has the same length of the word
    
    for (int i = 0; i < s.size(); i++)
        pattern += '0';
    
    int i = 0, j  = 0, k = 0;
    for (i = 0, j = 0; i < s.size(); i++)
    {
        if (pattern[i] == '0')
        {
            pattern[i] = 'a' + j;
            for (k = i + 1; k < s.size(); k++)
            {
                if (s[i] == s[k])
                    pattern[k] = pattern[i];
            }
            j++;
        }
    }
    return pattern;
}

void WordListImpl::lowerCase(string &s) const
{
    for (int i = 0; i < s.size(); i++)
    {
        if (isalpha(s[i]))
            s[i] = tolower(s[i]);
    }
}

//***** hash functions for string, int, and char *****

unsigned int hasher(const string& s)
{
    return hash<string>()(s);
}

unsigned int hasher(const int& i)
{
    return hash<int>()(i);
}

unsigned int hasher(const char& c)
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




//const string FILENAME = "/Users/hermmy/Documents/2017-2018/CS32/Project4/Project4/wordlist.txt";
//int main()
//{
//    WordList w;
//    w.loadWordList(FILENAME);
//    assert(w.contains("Balloon"));
//    assert(w.contains("'em"));
//    assert(w.contains("arbItrariness"));
//    assert(w.contains("sinuateS"));
//    assert(!w.contains("csdjhwdc"));
//    assert(!w.contains("dasc"));
//    assert(!w.contains("vrwjy"));
//    vector<string> cand = w.findCandidates("bdttook", "???????");
//    for (int i = 0; i < cand.size(); i++)
//        cerr << cand[i] << endl;
//    return 0;
//}

