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

bool findSymmetry(std::vector<std::string> const& arr, Tint badIdx, Tint& outIdx) {
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

		if(outIdx != badIdx) {
			return true;
		}

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

bool tryFindDifferentSymmetry(std::vector<std::string> const& map, std::vector<std::string> const& transposedMap, Tint badIdx, bool badIsHorizontal, Tint& outIdx, bool& isHorizontal) {
	if(findSymmetry(map, badIsHorizontal ? badIdx : -1, outIdx)) {
		isHorizontal = true;
		return true;
	}
	else if(findSymmetry(transposedMap, badIsHorizontal ? -1 : badIdx, outIdx)) {
		isHorizontal = false;
		return true;
	}

	return false;
}

void flip(char& x) {
	if(x == '.') {
		x = '#';
		return;
	}
	if(x == '#') {
		x = '.';
		return;
	}

	throw;
}

Tint solve(std::vector<std::vector<std::string>>& maps) {
	Tint result{};

	for(auto& map : maps) {
		Tint subResult{};
		bool isHorizontal{};

		auto transposedMap = transpose(map);

		if(!tryFindDifferentSymmetry(map, transposedMap, -1, false, subResult, isHorizontal)) {
			throw;
		}

		for(int i = 0; i < std::ssize(map); ++i) {
			for(int j = 0; j < std::ssize(transposedMap); ++j) {
				Tint subResult2{};
				bool isHorizontal2{};

				flip(map[i][j]);
				flip(transposedMap[j][i]);

				if(tryFindDifferentSymmetry(map, transposedMap, subResult, isHorizontal, subResult2, isHorizontal2)) {
					if(isHorizontal2) {
						result += (subResult2 + 1) * 100;
					}
					else {
						result += subResult2 + 1;
					}

					goto ContinueOuter;
				}

				flip(map[i][j]);
				flip(transposedMap[j][i]);
			}
		}

		throw;

	ContinueOuter: {}
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
