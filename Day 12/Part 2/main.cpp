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

struct Arrangement {
	std::string records{};
	std::vector<Tint> groups{};
};

std::vector<Arrangement> extractArrangements(std::vector<std::string> const& lines) {
	std::vector<Arrangement> arrangements;

	for(auto& line : lines) {

		Arrangement arrangement;
		
		auto const splitIdx = line.find_first_of(' ');

		auto const records = line.substr(0, splitIdx);

		arrangement.records = records;

#ifndef __INTELLISENSE__
		auto groups = line.substr(splitIdx + 1)
			| std::views::split(',')
			| std::views::transform([](auto&& range) -> Tint{return strToInt(std::string_view(range));})
			| std::ranges::to<std::vector>();
#endif

		arrangement.groups = groups;

		for(Tint i = 0; i < 4; ++i) {
			arrangement.records += '?' + records;
			arrangement.groups.insert(arrangement.groups.end(), groups.begin(), groups.end());
		}

		arrangements.emplace_back(arrangement);
	}

	return arrangements;
}

struct CacheEntry {
	Tint cacheValue{};
	bool isCached{false};
};

// [groupIdx][arrIdx]
using Cache = std::vector<std::vector<CacheEntry>>;

Tint solveArrangement(Arrangement& arrangement, Cache& cache, Tint groupIdx = 0, Tint arrIdx = 0) {
	if(arrIdx >= std::ssize(arrangement.records)) {
		// no more items -> are all groups present?
		return groupIdx == std::ssize(arrangement.groups) ? 1 : 0;
	}
	
	auto& cacheEntry = cache[groupIdx][arrIdx];

	if(cacheEntry.isCached) {
		// cached
		return cacheEntry.cacheValue;
	}

	auto setCacheValue = [&cacheEntry](Tint value) mutable -> Tint {
		cacheEntry.isCached = true;
		cacheEntry.cacheValue = value;
		return value;
	};

	if(arrangement.records[arrIdx] == '.') {
		// nothing to do, go to next index
		return setCacheValue(solveArrangement(arrangement, cache, groupIdx, arrIdx + 1));
	}

	if(arrangement.records[arrIdx] == '#') {
		// this group MUST be mapped

		if(groupIdx >= std::ssize(arrangement.groups) || arrIdx + arrangement.groups[groupIdx] > std::ssize(arrangement.records)) {
			// cannot map more groups, impossible
			return setCacheValue(0);
		}

		// check if the group can be mapped

		Tint i{};
		for(i = arrIdx; i < arrIdx + arrangement.groups[groupIdx]; ++i) {
			if(arrangement.records[i] == '.') {
				// cannot map it here, impossible
				return setCacheValue(0);
			}
		}

		if(i >= std::ssize(arrangement.records) || arrangement.records[i] != '#') {
			// ok, the group ends
			return setCacheValue(solveArrangement(arrangement, cache, groupIdx + 1, i + 1));
		}

		// the group does not end right, impossible
		return setCacheValue(0);
	}

	if(arrangement.records[arrIdx] == '?') {
		// the group MAY be mapped

		if(groupIdx >= std::ssize(arrangement.groups) || arrIdx + arrangement.groups[groupIdx] > std::ssize(arrangement.records)) {
			// cannot map more groups, consider it as '.' and continue
			return setCacheValue(solveArrangement(arrangement, cache, groupIdx, arrIdx + 1));
		}

		bool canBeMappedHere = true;

		Tint i{};
		for(i = arrIdx; i < arrIdx + arrangement.groups[groupIdx]; ++i) {
			if(arrangement.records[i] == '.') {
				// cannot map it here
				canBeMappedHere = false;
				break;
			}
		}

		if(i < std::ssize(arrangement.records) && arrangement.records[i] == '#') {
			// the group does not end right
			canBeMappedHere = false;
		}

		// first, consider the case '?' is actually '.'
		Tint result = solveArrangement(arrangement, cache, groupIdx, arrIdx + 1);

		if(canBeMappedHere) {
			// if the '?' being '#' is possible, add it to the result
			result += solveArrangement(arrangement, cache, groupIdx + 1, i + 1);
		}

		return setCacheValue(result);
	}

	// invalid symbol
	throw;
}

Tint solve(std::vector<Arrangement>& arrangements) {
	Tint result{};

	for(auto& arrangement : arrangements) {
		Cache cache(arrangement.groups.size() + 1, std::vector<CacheEntry>(arrangement.records.size()));
		result += solveArrangement(arrangement, cache);
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto arrangements = extractArrangements(lines);

	auto solution = solve(arrangements);

	std::cout << solution << "\n";
	
	return 0;
}
