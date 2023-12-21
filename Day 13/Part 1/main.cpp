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
#include <unordered_set>
#include <vector>


std::vector<std::string> readAllLines() {
	std::vector<std::string> lines;
	std::string line;

	while(std::getline(std::cin, line)) {
		lines.emplace_back(line);
	}

	return lines;
}

using Tint = long long;
static constexpr std::string_view nums{ "0123456789" };
static constexpr std::string_view alpha{ "qwertzuiopasdfghjklyxcvbnm" };

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

std::vector<std::vector<std::string>> extractMaps(std::vector<std::string> const& lines) {
	std::vector<std::vector<std::string>> result;

	auto from = lines.begin();
	auto to = from;

	while(true) {
		if(to == lines.end() || to->empty()) {
			result.emplace_back(from, to);

			if(to == lines.end()) {
				break;
			}

			from = ++to;
		}
		++to;
	}

	return result;
}

bool findSymmetry(std::vector<std::string> const& arr, Tint& outIdx) {
	std::vector<size_t> hashes;

	static std::hash<std::string> hashString{};

	for(auto const& str : arr) {
		hashes.emplace_back(hashString(str));
	}

	for(Tint i = 0; i < std::ssize(arr) - 1; ++i) {
		Tint l = i;
		Tint r = i + 1;

		while(l >= 0 && r < std::ssize(arr)) {
			if(hashes[l] != hashes[r]) {
				goto ContinueForLoop;
			}
			--l;
			++r;
		}

		l = i;
		r = i + 1;

		while(l >= 0 && r < std::ssize(arr)) {
			if(arr[l] != arr[r]) {
				goto ContinueForLoop;
			}
			--l;
			++r;
		}

		outIdx = i;
		return true;

	ContinueForLoop: {}
	}

	return false;
}

std::vector<std::string> transpose(std::vector<std::string> const& original) {
	std::vector<std::string> result(original.front().size(), std::string(original.size(), '\0'));

	for(Tint i = 0; i < std::ssize(original); ++i) {
		for(Tint j = 0; j < std::ssize(original[i]); ++j) {
			result[j][i] = original[i][j];
		}
	}

	return result;
}

Tint solve(std::vector<std::vector<std::string>>& maps) {
	Tint result{};

	for(auto const& map : maps) {
		Tint subResult{};

		if(findSymmetry(map, subResult)) {
			result += (subResult + 1) * 100;
		}
		else if(findSymmetry(transpose(map), subResult)) {
			result += subResult + 1;
		}
		else {
			throw;
		}
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto maps = extractMaps(lines);

	auto solution = solve(maps);

	std::cout << solution << "\n";
	
	return 0;
}
