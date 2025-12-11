#include <iostream>
#include <set>
#include <string>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function prototypes
void findWords(string& word, const string& pattern, const string& mustUse,
    int idx, string stillNeed, const set<string>& dict, set<string>& found);

int countBlanks(const string& pattern, int start);

// primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> found;
    string word = in;
    
    findWords(word, in, floating, 0, floating, dict, found);
    return found;
}

// count how many blank spots are left starting from start index
int countBlanks(const string& pattern, int start) {
    int count = 0;
    for (int i = start; i < (int)pattern.size(); i++) {  // loop 1
        if (pattern[i] == '-') {
            count++;
        }
    }
    return count;
}

// recursive helper to try all the different combinations
void findWords(string& word, const string& pattern, const string& mustUse,
    int idx, string stillNeed, const set<string>& dict, set<string>& found)
{
    // base case - we've gone through the whole word
    if (idx == (int)pattern.size()) {
        // make sure we used all the required letters
        if (stillNeed.empty()) {
            if (dict.find(word) != dict.end()) {
                found.insert(word);
            }
        }
        return;
    }

    // if this spot is already filled in, just move on
    if (pattern[idx] != '-') {
        word[idx] = pattern[idx];
        
        // check if the fixed letter is one we needed anyway
        string updated = stillNeed;
        for (int i = 0; i < (int)stillNeed.size(); i++) {  // loop 2
            if (stillNeed[i] == pattern[idx]) {
                updated.erase(i, 1);
                break;
            }
        }
        
        findWords(word, pattern, mustUse, idx + 1, updated, dict, found);
        return;
    }

    // figure out how many blanks we still have to fill
    int numBlanks = countBlanks(pattern, idx);
    

    // if the number of blanks left equals the letters we still need,
    // then we can only use those letters (no choice)
    if (numBlanks == (int)stillNeed.size()) {
        for (int i = 0; i < (int)stillNeed.size(); i++) {  // loop 3
            word[idx] = stillNeed[i];
            
            string updated = stillNeed;
            updated.erase(i, 1);
            
            findWords(word, pattern, mustUse, idx + 1, updated, dict, found);
        }
    }
    else {
        // we have extra blanks so try every letter a-z
        for (char c = 'a'; c <= 'z'; c++) {  // loop 4
            word[idx] = c;
            
            // if this letter is one we needed, mark it as used
            string updated = stillNeed;
            for (int i = 0; i < (int)stillNeed.size(); i++) {  // loop 5
                if (stillNeed[i] == c) {
                    updated.erase(i, 1);
                    break;
                }
            }
            
            findWords(word, pattern, mustUse, idx + 1, updated, dict, found);
        }
    }
}