//
//  pattern.cpp
//  Project4
//
//  Created by Huimin Wang on 3/10/18.
//  Copyright © 2018 Huimin Wang. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

string pattern(const string& s)
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

int main()
{
    string s = "indeedsssfindee";
    cerr << pattern(s) << endl;
}
