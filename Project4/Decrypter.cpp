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
    // 0. Base case
    if (ciphertext.size() == 0)
        return;
    // 1. Break up the ciphertext
    Tokenizer token("1234567890,;:.!()[]{}-\"#$%^& ");
    vector<string> words = token.tokenize(ciphertext);
    
    // 2. Pick the word
         // (1) Not chosen
         // (2) Has the most '?'
    int maxQues = 0;
    size_t maxLength = 0;
    int index = 0;
    vector<string> partialTrans;
    for (int i = 0; i < words.size(); i++)
    {
        partialTrans.push_back(t.getTranslation(words[i]));
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
    string w = words[index];
    string currTr = partialTrans[index];
   
    // 4. Find candidates
    vector<string> candidates;
    if (m_wordList != nullptr)
        candidates = m_wordList->findCandidates(w, currTr);
    
    // 5. Empty candidates
    if (candidates.empty())
    {
        t.popMapping();
        return;
    }
    
    // 6. Examine the candidates
    string translation;
    for (int i = 0; i < candidates.size(); i++)
    {
        // 6a. Push the new mapping
        Translator temp;
        string plaintext;
        string ct;
        string currMap = t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        for (int i = 0; i < currMap.size(); i++)
        {
            if (currMap[i] != '?' && isalpha(currMap[i]))
            {
                ct += 'A' + i;
                plaintext += currMap[i];
            }
        }
        if (!temp.pushMapping(ct, plaintext))
            exit(1);
        if (!temp.pushMapping(w, candidates[i]))
            continue;
        
        // 6b. Translate the ciphertext
        translation = temp.getTranslation(ciphertext);
        Tokenizer token(",;:.!()[]{}-\"#$%^& ");
        vector<string> message = token.tokenize(translation);
        
        // 6c. Evaluate
        if (completeMessage(translation))
        {
            // Message is complete
            output.push_back(translation);
            temp.popMapping();
            continue;
        }
        else
        {
            bool valid = true;
            for (int i = 0; i < message.size(); i++)
            {
                // Message is not complete, a fully translated word is not in the wordlist
                if (fullyTranslated(message[i]) && !m_wordList->contains(message[i]))
                {
                    temp.popMapping();
                    valid = false;
                }
            }
            if (!valid)
                continue;
            
            // Message is not complete, all fully translated words are in the wordlist
            crackHelper(ciphertext, temp, output);
        }
    }
    // 7. After checking all the candidates
    t.popMapping();
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
    if (message.size() == 0)
        return false;
    Tokenizer token("1234567890,;:.!()[]{}-\"#$%^& ");
    vector<string> words = token.tokenize(message);
    for (int i = 0; i < words.size(); i++)
    {
        if (fullyTranslated(words[i]) && !m_wordList->contains(words[i]))
            return false;
        for (int j = 0; j < words[i].size(); j++)
            if (words[i][j] == '?')
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

void testDe()
{
    Decrypter d;
    d.load(FILENAME);
    vector<string> s0 = d.crack("Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp");
    for (int i = 0; i < s0.size(); i++)
        cerr << "Output: " << s0[i] << endl;
    vector<string> s1 = d.crack("Xjzwq gjz cuvq xz huri arwqvudiy fuk ufjrqoq svquxiy. -Lzjk Nqkkqcy");
    for (int i = 0; i < s1.size(); i++)
        cerr << "Output: " << s1[i] << endl;
    vector<string> s2 = d.crack("Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. -Rcmcy Xcmmcn");
    for (int i = 0; i < s2.size(); i++)
        cerr << "Output: " << s2[i] << endl;
    vector<string> s3 = d.crack("Axevfvu lvnelvp bxqp mvpprjv rgl bvoop Grnxvgkvuj dqupb jvbp buvrbvl be lqggvu.");
    for (int i = 0; i < s3.size(); i++)
        cerr << "Output: " << s3[i] << endl;
    vector<string> s4 = d.crack("Lzdkgd dyrmjls shcg xdggkud fpm xd!!");
    for (int i = 0; i < s4.size(); i++)
        cerr << "Output: " << s4[i] << endl;
    vector<string> s5 = d.crack("Ojvgtv vcrpxok kfwt uvttgiv byp uv!!");
    for (int i = 0; i < s5.size(); i++)
        cerr << "Output: " << s5[i] << endl;
    vector<string> s6 = d.crack("y qook ra bdttook yqkook.");
    for (int i = 0; i < s6.size(); i++)
        cerr << "Output: " << s6[i] << endl;
    

}
//
//int main()
//{
//    testDe();
//}

