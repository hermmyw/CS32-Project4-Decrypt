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
    vector<bool> m_wordsChosen;
    vector<string> crackHelper(const string& ciphertext, Translator& t, vector<string>& output);
    bool fullyTranslated(const string& s) const;
    bool completeMessage(const string& message) const;
};

DecrypterImpl::DecrypterImpl()
:m_wordsChosen(50, false)
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
    output = crackHelper(ciphertext, t, output);
    
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

vector<string> DecrypterImpl::crackHelper(const string& ciphertext, Translator& t, vector<string>& output)
{
    Tokenizer token(",;:.!()[]{}-\"#$%^& ");
    vector<string> words = token.tokenize(ciphertext);
    if (words.size() == 0)
        return vector<string> ();

    int maxQues = 0;
    size_t maxLength = 0;
    int index = 0;
    vector<string> partialTrans;
    for (int i = 0; i < words.size(); i++)
    {
        if (!m_wordsChosen[i])
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
    {
        // cerr << "Enter check " << endl;
        if (fullyTranslated(partialTrans[i]) && !m_wordList->contains(partialTrans[i]))
        {
            //            while (true)
            //            {
            //                if (t.popMapping())
            //                    continue;
            //                else
            //                    break;
            //            }
            // cerr << "Wrong map, go back to the first word with empty map" << endl;
            return vector<string>();
        }
    }
    // cerr << endl;
    // m_wordsChosen[index] = true;
    string w = words[index];
    string currTr = partialTrans[index];
    if (currTr == "e??????" && w == "dyrmjls")
        cerr << "Current mapping table: " << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
   
    vector<string> candidates;
    if (m_wordList != nullptr)
        candidates = m_wordList->findCandidates(w, currTr);
    if (candidates.empty())
    {
        if (w == "O?ease")
            cerr << "No candidates for " << w << endl;
        // cerr << "Current mapping table: " << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
        t.popMapping();
        return vector<string> ();
    }
    
    string translation;
    for (int i = 0; i < candidates.size(); i++)
    {
        // cerr << "Candidate " << i << candidates[i] << endl;
        if (w == "dyrmjls" && i > 16)
        {
            cerr << candidates[i] << endl;
            cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
        }
        bool push = t.pushMapping(w, candidates[i]);
        
        if (!push)
            continue;
        cerr << "CANDIDATE: " << candidates[i] << endl;
        
        // cerr << endl;
        translation = "";
        translation = t.getTranslation(ciphertext);
        // cerr << "Translation: " << translation << endl;
        Tokenizer token(",;:.!()[]{}-\"#$%^& ");
        vector<string> message = token.tokenize(translation);
        
        ///////EVALUATE///////
        if (completeMessage(translation))
        {
            if (candidates[i] == "eductor")
                cerr << "check complete message" << endl;
            // cerr << "Complete" << endl;
            cerr << "Complete with: " << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
            output.push_back(translation);
            t.popMapping();
            // m_wordsChosen[index] = false;
            // cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYC") << endl;
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
                    cerr << message[i] << " is not in the wordlist." << endl;
//                    while (true)
//                    {
//                        if (t.popMapping())
//                            continue;
//                        else
//                            break;
//                    }
                    if (candidates[i] == "eductor")
                        cerr << "check full word" << endl;
                    t.popMapping();
                    promising = false;
                }
//                else
//                    cerr << "Got one word: " << message[i] << endl;
            }
            for (int j = 0; j < message[i].size(); j++)
                if (message[i][j] == '?')
                    notComplete = true;
        }
        if (!promising)
            continue;
        if (notComplete)
        {
            if (candidates[i] == "eductor")
                cerr << "after check full word" << endl;
            cerr << t.getTranslation("ABCDEFGHIJKLMNOPQRSTUVWXYZ") << endl;
            crackHelper(ciphertext, t, output);
            continue;
        }
        
        
        for (int i = 0; i < message.size(); i++)
        {
            cerr << "Enter check " << endl;
            if (fullyTranslated(message[i]) && !m_wordList->contains(message[i]))
            {
//                while (true)
//                {
//                    if (t.popMapping())
//                        continue;
//                    else
//                        break;
//                }
                t.popMapping();
                cerr << "Wrong map, go back to the first word with empty map" << endl;
                return vector<string>();
            }
        }
        // output.push_back(translation);
    }
//    if (completeMessage(translation))
//    {
//        cerr << "Searched all candidates but " << endl;
//        t.popMapping();
//        cerr << "Return empty vector" << endl;
//        return vector<string> ();
////        for (int i = 0; i < 50; i++)
////            m_wordsChosen[index] = false;
//    }
    t.popMapping();
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

//const string FILENAME = "/Users/hermmy/Documents/2017-2018/CS32/Project4/Project4/testword.txt";
//int main()
//{
//    Decrypter d;
//    d.load(FILENAME);
//    vector<string> s1 = d.crack("smxsdg ,;:.!()[]{}-\"#$%^& SGOSDG gfvgx!!");
//    for (int i = 0; i < s1.size(); i++)
//    {
//        cerr << "Output: " << s1[i] << endl;
//    }
////    vector<string> s2 = d.crack("Lzdkgd dyrmjls shcg xdggkud fpm xd!!");
////    for (int i = 0; i < s2.size(); i++)
////    {
////        cerr << "Output: " << s2[i] << endl;
////    }
////    vector<string> s = d.crack("rweelycbb vmobcb");
////    for (int i = 0; i < s.size(); i++)
////    {
////        cerr << "Output: " << s[i] << endl;
////    }
//}

