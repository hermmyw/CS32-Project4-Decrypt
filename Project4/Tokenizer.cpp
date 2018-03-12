#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s);
private:
    vector<char> m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++)
        m_separators.push_back(separators[i]);
}

vector<string> TokenizerImpl::tokenize(const string& s)
{
    vector<string> tokens;
    string w;
    for (int i = 0; i < s.size(); i++)
    {
        if (find(m_separators.begin(), m_separators.end(), s[i]) == m_separators.end())
            w += s[i];
        else
        {
            if (w != "")
                tokens.push_back(w);
            w = "";
        }
    }
    if (w != "")
        tokens.push_back(w);
    return tokens;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}






//
//
//int main()
//{
//    Tokenizer t(" ,.!");
//    vector<string> v = t.tokenize("This,, is a test! It's the... best!");
//    for (int i = 0; i < v.size(); i++)
//        cout << v[i] << endl;
//
//    string s = "!!!!!";
//    v = t.tokenize(s);
//    for (int i = 0; i < v.size(); i++)
//        cout << v[i] << endl;
//
//}

