#include <string>
#include <bitset>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>
#include <fstream>
#include <cmath>
#include <iostream>

#include "logic.h"

#pragma GCC optimize("unroll-loops")
//#define HARD_MODE

std::vector <std::string> words;
std::vector <int> possibleIDs;
bool mod = true;
std::vector <std::vector <unsigned char>> lookup;

float score(const int &chosenID) {
	float ans = 0, floatsize = (float)possibleIDs.size(), floatfreq;
	int freq[243];
	int count = 0;
	memset(freq, 0, sizeof(freq));
	for(const int &correctID : possibleIDs) {
		freq[lookup[chosenID][correctID]]++;
	}
	for(int now : freq) {
		if (now) {
			count++;
			floatfreq = (float)now / floatsize;
			ans -= floatfreq * std::log2(floatfreq);
		}
	}
	return ans;
}
bool comp_words(const std::pair <float, std::string> &nr1, const std::pair <float, std::string> &nr2) {
	return nr1.first > nr2.first;
}
std::vector <std::pair <float, std::string>> sort_words() {
	static std::vector <std::pair <float, std::string>> ans;
	if (mod) {
		ans.clear();
#ifdef HARD_MODE
		for (int chosenID : possibleIDs) {
			ans.emplace_back(score(chosenID), words[chosenID]);
		}
#else
		for(int chosenID = 0; chosenID < words.size(); chosenID++) {
			ans.emplace_back(score(chosenID), words[chosenID]);
		}
#endif
		std::sort(ans.begin(), ans.end(), comp_words);
		mod = false;
	}
	return ans;
}

unsigned char get_hint (const std::string &chosen, const std::string &correct) {
	unsigned char ans1 = 0, ans2 = 0;
	unsigned char count[26];
	memset(count, 0, sizeof(count));
	// Check for green
	for(int index = 0; index < 5; index++) {
		ans1 += ans1 << 1;
		if (chosen[index] == correct[index]) {
			ans1 += 2;
		}
		else{
			count[correct[index] - 'a']++;
		}
	}
	// Check for yellow
	for(int index = 0; index < 5; index++) {
		ans2 += ans2 << 1;
		if(chosen[index] != correct[index] && count[chosen[index] - 'a'] > 0) {
			count[chosen[index] - 'a']--;
			ans2++;
		}
	}
	return ans1 + ans2;
}

unsigned char hint_read(const std::string &hints) {
	unsigned char hint_val = 0;
	for(int index = 0; index < 5; index++) {
		if('0' > hints[index] || hints[index] > '2'){
			return 0xff;
		}
		hint_val += hint_val << 1;
		hint_val += hints[index] - '0';
	}
	return hint_val;
}

bool hint(const std::string &word, const unsigned char &hint_val) {
	for(int index = 0; index < 5; index++) {
		if('a' > word[index] || word[index] > 'z'){
			return false;
		}
	}
	if (word.size() != 5 || hint_val >= 243) {
		return false;
	}
	int set_ptr = 0;
	for(int index = 0; index < possibleIDs.size(); index++) {
		if(get_hint(word, words[possibleIDs[index]]) == hint_val) {
			if(set_ptr != index) {
				std::swap(possibleIDs[set_ptr], possibleIDs[index]);
			}
			set_ptr++;
		}
	}
	possibleIDs.resize(set_ptr);
	mod = true;
	return true;
}

void init(){
	std::string str;
	possibleIDs.clear();
	lookup.clear();
	std::ifstream fin("valid-words.txt");
	while(fin >> str) {
		possibleIDs.push_back((int)words.size());
		words.push_back(str);
		lookup.emplace_back();
		for(int index = 0; index < words.size() - 1; index++) {
			lookup.back().push_back(get_hint(words.back(), words[index]));
			lookup[index].push_back(get_hint(words[index], words.back()));
		}
		lookup.back().push_back(242);
	}
	fin.close();
}

void reset() {
	possibleIDs.clear();
	for(int index = 0; index < words.size(); index++){
		possibleIDs.push_back(index);
	}
	mod = true;
}

std::vector <std::string> get_choices() {
	std::vector <std::string> ans;
	for(const int &ID : possibleIDs){
		ans.push_back(words[ID]);
	}
	return ans;
}