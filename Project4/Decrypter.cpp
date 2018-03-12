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
    bool fullyTranslated(const string& s) const;
    bool completeMessage(const string& message) const;
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
    for (int i = 0; i < m_wordsChosen.size(); i++)
        cerr << m_wordsChosen[i] << endl;
    return output;
}

vector<string> DecrypterImpl::crackHelper(const string& ciphertext,vector<string>& words, Translator& t)
{
    vector<string> output;
    if (words.size() == 0)
        return vector<string> ();
    int maxQues = 0;
    int maxLength = 0;
    int index = 0;
    vector<string> partialTrans;
    for (int i = 0; i < words.size(); i++)
    {
        if (m_wordsChosen[i] == false)
        {
            partialTrans.push_back(t.getTranslation(words[i]));
            cerr << partialTrans[i] << endl;
            cerr << "Not chosen" << endl;
            int nQues = 0;
            int size = partialTrans[i].size();
            for (int j = 0; j < partialTrans[i].size(); j++)
            {
                if (partialTrans[i][j] == '?')
                    nQues++;
            }
            if (nQues >= maxQues)
            {
                index = i;
                maxQues = nQues;
                maxLength = size;
            }
        }
    }
    cerr << endl;
    string w = words[index];
    string currTr = partialTrans[index];
    // m_wordsChosen[index] = true;
    vector<string> candidates = m_wordList.findCandidates(w, currTr);
    if (candidates.size() == 0)
    {
        cerr << "No candidates for " << w << endl;
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
        bool push = t.pushMapping(w, candidates[i]);
        if (!push)
            continue;
        cerr << candidates[i] << endl;
//        vector<string> message;
//        for (int j = 0; j < words.size(); j++)
//        {
//            message.push_back(t.getTranslation(words[j]));
//            //cerr << message[j] << " " << endl;
//        }
        cerr << endl;
        string translation = t.getTranslation(ciphertext);
        cerr << "Translation: " << translation << endl;
        Tokenizer token(",;:.!()[]{}-\"#$%^& ");
        vector<string> message = token.tokenize(translation);
        
        ///////EVALUATE///////
        if (completeMessage(translation))
        {
            cerr << "Complete" << endl;
            output.push_back(translation);
            m_wordsChosen[index] = false;
            t.popMapping();
            cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYC") << endl;
            continue;
        }
        
        bool notComplete = false;
        for (int i = 0; i < message.size(); i++)
        {
            if (fullyTranslated(message[i]))
            {
                if (!m_wordList.contains(message[i]))
                {
                    while (true)
                    {
                        if (t.popMapping())
                            continue;
                        else
                            break;
                    }
                    return vector<string>();
                }
                else
                    cerr << "Got one word: " << message[i] << endl;
            }
            for (int j = 0; j < message[i].size(); j++)
                if (message[i][j] == '?')
                    notComplete = true;
        }
        if (notComplete)
        {
            cerr << "Enter crackHelper" << endl;
            vector<string> r = crackHelper(ciphertext, words, t);
            if (r.empty())
                continue;
            else
            {
                for (int i = 0; i < r.size(); i++)
                    output.push_back(r[i]);
            }
        }
        
        
        for (int i = 0; i < message.size(); i++)
        {
            cerr << "Enter check " << endl;
            if (fullyTranslated(message[i]) && !m_wordList.contains(message[i]))
            {
                while (true)
                {
                    if (t.popMapping())
                        continue;
                    else
                        break;
                }
                cerr << "Wrong map, go back to the first word with empty map" << endl;
                return vector<string>();
            }
        }
        // output.push_back(translation);
    }
    return output;
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
        if (!fullyTranslated(words[i]) || !m_wordList.contains(words[i]))
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

const string FILENAME = "/Users/hermmy/Documents/2017-2018/CS32/Project4/Project4/testword.txt";
int main()
{
    Decrypter d;
    d.load(FILENAME);
    vector<string> s = d.crack("y qook ra bdttook yqkook");
    for (int i = 0; i < s.size(); i++)
    {
        cerr << "Output: " << s[i] << endl;
    }
}

