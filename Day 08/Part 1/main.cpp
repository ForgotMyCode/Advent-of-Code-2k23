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
#include <unordered_map>
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
static constexpr std::string_view nums{ "0123456789" };
static constexpr std::string_view alpha{ "qwertzuiopasdfghjklyxcvbnm" };

Tint strToInt(std::string_view str) {
	Tint ret{};

	if(std::from_chars(str.data(), str.data() + str.length(), ret).ec == std::errc{}) {
		return ret;
	}

	throw;
}

struct Graph {
	struct Node {
		std::string_view left{};
		std::string_view right{};
	};

	std::unordered_map<std::string_view, Node> nodes;
};

struct ExtractionResult {
	std::string_view instructions{};
	Graph graph{};
};

ExtractionResult extract(std::vector<std::string> const& lines) {
	ExtractionResult extractionResult{};

	extractionResult.instructions = lines.front();

	for(auto const& map : lines | std::views::drop(2)) {
		std::string_view node(map.begin(), std::next(map.begin(), 3));
		std::string_view l(std::next(map.begin(), 7), std::next(map.begin(), 10));
		std::string_view r(std::next(map.begin(), 12), std::next(map.begin(), 15));

		extractionResult.graph.nodes[node].left = l;
		extractionResult.graph.nodes[node].right = r;
	}

	return extractionResult;
}

Tint solve(Graph const& graph, std::string_view instructions) {
	Tint result = 0;
	Tint instructionIdx = 0;
	std::string_view currentNode = "AAA";

	while(currentNode != "ZZZ") {
		++result;

		if(instructions[instructionIdx] == 'L') {
			currentNode = graph.nodes.at(currentNode).left;
		}
		else {
			assert(instructions[instructionIdx] == 'R');
			currentNode = graph.nodes.at(currentNode).right;
		}

		instructionIdx = (instructionIdx + 1) % std::ssize(instructions);
	}

	return result;
}

int main() {
	auto lines = readAllLines();

	auto [instructions, graph] = extract(lines);

	auto solution = solve(graph, instructions);

	std::cout << solution << "\n";
	
	return 0;
}
