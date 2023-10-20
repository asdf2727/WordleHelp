#pragma once

std::vector <std::pair <float, std::string>> sort_words();
unsigned char get_hint (const std::string &chosen, const std::string &correct);
unsigned char hint_read (const std::string &hints);
bool hint(const std::string &letters, const unsigned char &hints);
std::vector <std::string> get_choices();

void init();
void reset();