#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "logic.h"

#define lower(chr) ('A' <= chr && chr <= 'Z' ? chr + 32 : chr)

struct command{
	std::string type;
	std::vector <std::string> params;

	void Set (const std::string &line){
		std::string::const_iterator start, now;
		bool first = true;
		start = now = line.begin();
		type = "";
		params.clear();
		while(true){
			if(now != line.end() && *now != ' ') {
				now++;
			}
			else{
				if(first){
					while(start != now) {
						type.push_back(lower(*start));
						start++;
					}
					first = false;
				}
				else {
					params.emplace_back();
					while(start != now) {
						params.back().push_back(lower(*start));
						start++;
					}
				}
				if (now == line.end()) {
					break;
				}
				now++;
				start++;
			}
		}
	}
};

bool toInt (int& ret, const std::string& number) {
	long long int ans = 0;
	bool neg = false;
	std::string::const_iterator it = number.begin();
	if(*it == '-'){
		neg = false;
		it++;
	}
	for(; it != number.end(); it++) {
		if('0' > *it || *it > '9') {
			return false;
		}
		ans = ans * 10 + *it - '0';
	}
	ret = ans;
	return true;
}

int main() {

	std::string str;
	std::vector <std::pair <float, std::string>> ans;
	std::vector <std::string> choices;

	std::cout << "Welcome to worlde help!\nInitializing..." << std::endl;
	init();
	choices = get_choices();
	std::cout << "Done!\n";

	command input;
	while(true) {
		std::cout << "> ";
		std::cout.flush();
		getline(std::cin, str);
		input.Set(str);
		if(input.type == "exit") {
			std::cout << "Goodbye!\n";
			break;
		}
		else if(input.type == "reset") {
			reset();
			choices = get_choices();
			std::cout << "Game reset!\n";
		}
		else if(input.type == "hint") {
			if(input.params.size() == 2) {
				if(!hint(input.params[0], hint_read(input.params[1]))) {
					std::cout << "Invalid arguments were given!\n";
					continue;
				}
				float prob = (float)choices.size();
				choices = get_choices();
				prob = (float)choices.size() / prob;
				std::cout << "Actual information: " << std::log2(1 / prob) << '\n';
				std::cout << choices.size() << " words remain.\n";
			}
			else {
				std::cout << "Command \'hint\' only accepts two arguments!\n";
			}
		}
		else if(input.type == "choices") {
			for(const std::string &choice : choices) {
				std::cout << choice << ' ';
			}
			std::cout << '\n' << choices.size() << " words remaining.\n";
		}
		else if(input.type == "next") {
			int count = 0;
			if(input.params.empty()) {
				count = 1;
			}
			else if(!toInt(count, input.params[0])) {
				std::cout << '\'' << input.params[0] << "\' is not a number!\n";
				continue;
			}
			ans = sort_words();
			count = std::min(count, (int)ans.size());
			for(int index = 0; index < count; index++){
				std::cout << index + 1 << ". " << ans[index].second << ' ' << ans[index].first << '\n';
			}
		}
		else if(input.type == "fast") {
			int show_max = 20;
			std::string best;
			if(input.params.size() == 1) {
				if(toInt(show_max, input.params[0])){
					std::cout << '\'' << input.params[0] << "\' is not a number!\n";
					continue;
				}
			}
			reset();
			choices = get_choices();
			while(choices.size() > 1) {
				if(choices.size() <= show_max){
					std::cout << "Choices: ";
					for(const std::string &choice : choices) {
						std::cout << choice << ' ';
					}
					std::cout << '\n';
				}
				best = sort_words()[0].second;
				std::cout << "Best choice: " << best << '\n';
				std::cout << "Hint (input reset to stop): ";
				while(std::cin >> str) {
					if(str == "reset") {
						std::cout << "Fast solver canceled.\n";
						break;
					}
					else if(str == "") {
						std::cout << "Hint (input reset to stop): ";
					}
					else if(!hint(best, hint_read(str))) {
						std::cout << "Invalid hint given!\n";
						std::cout << "Hint (input reset to stop): ";
					}
					else {
						break;
					}
				}
				if(str == "reset") {
					break;
				}
				choices = get_choices();
			}
			if(choices.size() == 0) {
				std::cout << "Invalid state found!\n";
			}
			else if(choices.size() == 1) {
				std::cout << "Final word: " << choices[0] << '\n';
			}
			reset();
			choices = get_choices();
			std::cout << "Game reset!\n";
			std::cin.get();
		}
		else if (input.type == "test") {
			std::cout << "Benchmark for current version...\n";
			int count, index = 0;
			std::vector <int> results;
			double avg = 0;
			reset();
			std::string first_choice = sort_words()[0].second, next_choice;
			choices = get_choices();
			for (std::string now : choices) {
				reset();
				hint(first_choice, get_hint(first_choice, now));
				count = 1;
				while (get_choices().size() != 1) {
					next_choice = sort_words()[0].second;
					hint(next_choice, get_hint(next_choice, now));
					count++;
				}
				if (get_choices()[0] != next_choice) {
					count++;
				}
				while(results.size() <= count) {
					results.push_back(0);
				}
				results[count]++;
				index++;
				std::cout << index << ": " << now << ' ' << count << '\n';
			}
			std::cout << "Results\n";
			for(int index = 0; index < results.size(); index++) {
				std::cout << "\tIn " << index << " tries: " << results[index] << '\n';
				avg += index * results[index];
			}
			reset();
			avg /= (double)choices.size();
			std::cout << "Average: " << avg << '\n';
		}
		else if(!input.type.empty()){
			std::cout << "Command \'" << input.type << "\' is unrecognised!\n";
		}
	}
	return 0;
}