#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList m_wordList;
    vector<bool> m_wordsChosen;
    vector<string> m_tokens;
    vector<string> crackHelper(const string& ciphertext, vector<string>& words, Translator& t);
    bool fullyTranslated(const string& s);
};

DecrypterImpl::DecrypterImpl()
:m_wordList(), m_wordsChosen(50, false)
{ }

bool DecrypterImpl::load(string filename)
{
    return m_wordList.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    Translator t;
    vector<string> output;
    Tokenizer token(",;:.!()[]{}-\"#$%^& ");
    vector<string> cipherWords = token.tokenize(ciphertext);
    output = crackHelper(ciphertext, cipherWords, t);
    return output;
}

vector<string> DecrypterImpl::crackHelper(const string& ciphertext,vector<string>& words, Translator& t)
{
    vector<string> output;
    if (words.size() == 0)
        return vector<string> ();
    int maxQues = 0;
    int index = 0;
    vector<string> partialTrans;
    for (int i = 0; i < words.size(); i++)
    {
        partialTrans.push_back(t.getTranslation(words[i]));
        int nQues = 0;
        for (int j = 0; j < partialTrans[i].size(); j++)
        {
            if (partialTrans[i][j] == '?')
                nQues++;
        }
        if (nQues > maxQues)
        {
            index = i;
            maxQues = nQues;
        }
        cerr << words[i] << " ";
    }
    cerr << endl;
    string w = words[index];
    string currTr = partialTrans[index];
    vector<string> candidates = m_wordList.findCandidates(w, currTr);
    if (candidates.size() == 0)
    {
        cerr << "No candidates" << endl;
        while (true)
        {
            if (t.popMapping())
                continue;
            else
                break;
        }
        return vector<string> ();
    }
    
    
    for (int i = 0; i < candidates.size(); i++)
    {
        cerr << "Candidate " << i << candidates[i] << endl;
        Translator* tempTable = &t;
        bool push = tempTable->pushMapping(w, candidates[i]);
        if (!push)
            continue;
        vector<string> message;
        for (int j = 0; j < words.size(); j++)
        {
            message.push_back(tempTable->getTranslation(words[j]));
            cerr << message[j] << " " << endl;
        }
        cerr << endl;
        string translation = tempTable->getTranslation(ciphertext);
        
        
        ///////EVALUATE///////
        bool promising = true;
        for (int i = 0; i < message.size(); i++)
        {
            if (fullyTranslated(message[i]))
            {
                cerr << "Fully translated: " << message[i] << endl;
                if (!m_wordList.contains(message[i]))
                    promising = false;
            }
        }
        if (promising)
        {
            cerr << "Enter crackHelper" << endl;
            if (words.size() > 1)
                words.erase(words.begin() + index);
            else
                words.clear();
            vector<string> r = crackHelper(translation, words, *tempTable);
            if (r.empty())
                continue;
            else
            {
                for (int i = 0; i < r.size(); i++)
                    output.push_back(r[i]);
            }
            continue;
        }
        
        
        bool valid = true;
        for (int i = 0; i < message.size(); i++)
        {
            if (fullyTranslated(message[i]) && !m_wordList.contains(message[i]))
            {
                tempTable->popMapping();
                valid = false;
            }
        }
        if (!valid)
        {
            cerr << "Not valid" << endl;
            continue;
        }
        
        bool complete = true;
        for (int i = 0; i < message.size(); i++)
        {
            if (!fullyTranslated(message[i]) || !m_wordList.contains(message[i]))
                complete = false;
        }
        if (complete)
        {
            // record this as one of the potentials
            cerr << "complete" << endl;
            output.push_back(translation);
        }
    }
    return output;
}

bool DecrypterImpl::fullyTranslated(const string& s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '?')
            return false;
    }
    return true;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}

const string FILENAME = "/Users/hermmy/Documents/2017-2018/CS32/Project4/Project4/wordlist.txt";
int main()
{
    Decrypter d;
    d.load(FILENAME);
    vector<string> s = d.crack("y qook ra bdttook yqkook");
    for (int i = 0; i < s.size(); i++)
        cerr << s[i] << endl;
}

