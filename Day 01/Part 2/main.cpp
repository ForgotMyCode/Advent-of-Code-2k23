#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>


std::vector<std::string> readAllLines() {
	std::vector<std::string> lines;
	std::string line;

	while(std::getline(std::cin, line)) {
		lines.emplace_back(line);
	}

	return lines;
}

using Tint = int;

template<typename It>
bool getFirstDigit(It first, It last, Tint& out) {
	if(first == last) {
		return false;
	}

	if(std::isdigit(*first)) {
		out = (*first) - '0';
		return true;
	}


	std::string_view str(first, last);

	static constexpr std::array<std::string_view, 9> spelledDigits{
		"one",
		"two",
		"three",
		"four",
		"five",
		"six",
		"seven",
		"eight",
		"nine"
	};

	for(int i = 0; i < 9; ++i) {
		if(str.starts_with(spelledDigits[i])) {
			out = i + 1;
			return true;
		}
	}

	return false;
}

Tint getCalibrationValue(std::string_view str) {
	Tint firstDigit = -1;

	for(auto it = str.begin(); it != str.end(); ++it) {
		if(getFirstDigit(it, str.end(), firstDigit)) {
			break;
		}
	}

	assert(firstDigit != -1);

	Tint lastDigit = -1;

	for(auto it = str.rbegin(); it != str.rend(); ++it) {
		if(getFirstDigit(it.base()-1, str.end(), lastDigit)) {
			break;
		}
	}

	assert(lastDigit != -1);

	return firstDigit*10 + lastDigit;
}

int main() {
	auto lines = readAllLines();

	auto calibrationsRange = lines |
		std::views::transform(&getCalibrationValue);

	auto result = std::ranges::fold_left(calibrationsRange, Tint{}, std::plus<Tint>{});

	std::cout << result << "\n";

	return 0;
}
