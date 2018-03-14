#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    ~DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList* m_wordList;
    MyHash<string, bool> m_visited;
    void crackHelper(const string& ciphertext, Translator& t, vector<string>& output);
    bool fullyTranslated(const string& s) const;
    bool completeMessage(const string& message) const;
};

DecrypterImpl::DecrypterImpl()
:m_visited(0.5)
{
    m_wordList = new WordList;
}

DecrypterImpl::~DecrypterImpl()
{
    delete m_wordList;
}
bool DecrypterImpl::load(string filename)
{
    return m_wordList->loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    Translator t;
    vector<string> output;
    Tokenizer token(",;:.!()[]{}-\"#$%^& ");
    vector<string> cipherWords = token.tokenize(ciphertext);
    for (int i = 0; i < cipherWords.size(); i++)
        m_visited.associate((cipherWords[i]), false);
    
    crackHelper(ciphertext, t, output);
    
    // Insertion sort
    for (int i = 2; i < output.size() + 1; i++)
    {
        string sort = output[i-1];
        int index = i - 2;
        while (index >= 0 && sort < output[index])
        {
            output[index+1] = output[index];
            index--;
        }
        output[index+1] = sort;
    }
    return output;
}

void DecrypterImpl::crackHelper(const string& ciphertext, Translator& t, vector<string>& output)
{
    Tokenizer token(",;:.!()[]{}-\"#$%^& ");
    vector<string> words = token.tokenize(ciphertext);
    if (words.size() == 0 || ciphertext.size() == 0)
        return;
    bool finished = true;
    for (int i = 0; i < words.size(); i++)
        if (m_visited.find(words[i]) != nullptr && *(m_visited.find(words[i])) == false)
            finished = false;
    if (finished)
        return;
    int maxQues = 0;
    size_t maxLength = 0;
    int index = 0;
    vector<string> partialTrans;
    for (int i = 0; i < words.size(); i++)
    {
        
        {
            partialTrans.push_back(t.getTranslation(words[i]));
            //cerr << partialTrans[i] << endl;
            int nQues = 0;
            size_t size = partialTrans[i].size();
            for (int j = 0; j < partialTrans[i].size(); j++)
            {
                if (partialTrans[i][j] == '?')
                    nQues++;
            }
            if (nQues > maxQues && size > maxLength)
            {
                index = i;
                maxQues = nQues;
                maxLength = size;
            }
        }
    }
    for (int i = 0; i < partialTrans.size(); i++)
        if (fullyTranslated(partialTrans[i]) && !m_wordList->contains(partialTrans[i]))
        {
            t.popMapping();
            return;
        }
    if (completeMessage(t.getTranslation(ciphertext)))
        return;

    string w = words[index];
    string currTr = partialTrans[index];
   
    vector<string> candidates;
    if (m_wordList != nullptr)
        candidates = m_wordList->findCandidates(w, currTr);
    if (candidates.empty())
    {
        t.popMapping();
        return;
    }
    
    string translation;
    for (int i = 0; i < candidates.size(); i++)
    {
        if (candidates[i] == "aloofness")
            cerr << "******Start over with " << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
//        if (candidates[i] == "huffiness")
//            cerr << "******FOUND" << endl;
        if (!t.pushMapping(w, candidates[i]))
            continue;
        
        translation = t.getTranslation(ciphertext);
        Tokenizer token(",;:.!()[]{}-\"#$%^& ");
        vector<string> message = token.tokenize(translation);
        
        ///////EVALUATE///////
        if (completeMessage(translation))
        {
            output.push_back(translation);
            cerr << "Complete translation: " << translation << endl;
            if (!t.popMapping())
                return;
            cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
            continue;
        }
        
        bool notComplete = false;
        bool promising = true;
        for (int i = 0; i < message.size(); i++)
        {
            if (fullyTranslated(message[i]))
            {
                if (!m_wordList->contains(message[i]))
                {
                    if (!t.popMapping())
                        return;
                    promising = false;
                }

            }
            for (int j = 0; j < message[i].size(); j++)
                if (message[i][j] == '?')
                    notComplete = true;
        }
        if (!promising)
            continue;
        if (notComplete)
        {
            crackHelper(ciphertext, t, output);
            // t.popMapping();
            continue;
        }
        
        
        for (int i = 0; i < message.size(); i++)
        {
            if (fullyTranslated(message[i]) && !m_wordList->contains(message[i]))
            {
                t.popMapping();
                return;
            }
        }
    }
    bool* visited = m_visited.find(w);
    if (visited != nullptr)
        *visited = true;
    cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
    t.popMapping();
    cerr << "AFTER POPPING " << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
    return;
}

bool DecrypterImpl::fullyTranslated(const string& s) const
{
    for (int i = 0; i < s.size(); i++)
        if (s[i] == '?')
            return false;
    return true;
}

bool DecrypterImpl::completeMessage(const string& message) const
{
    Tokenizer token(",;:.!()[]{}-\"#$%^& ");
    vector<string> words = token.tokenize(message);
    for (int i = 0; i < words.size(); i++)
        if (!fullyTranslated(words[i]) || !m_wordList->contains(words[i]))
            return false;
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

void testDe()
{
    Decrypter d;
    d.load(FILENAME);
    vector<string> s3 = d.crack("Vxgvab sovi jh");
    for (int i = 0; i < s3.size(); i++)
        cerr << "Output: " << s3[i] << endl;
}

int main()
{
    testDe();
}

