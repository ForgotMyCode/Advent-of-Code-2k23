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
#include <unordered_set>
#include <vector>

#include <omp.h>


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
	struct Mapping {
		std::string_view targetState;
		Tint nSteps{};
	};

	auto findNextZ = [&graph, &instructions](std::string_view state, Tint instruction) {	
		Mapping result{};

		Tint steps = 0;

		do {
			++steps;

			if(instructions[instruction] == 'L') {
				state = graph.nodes.at(state).left;
			}
			else {
				state = graph.nodes.at(state).right;
			}

			instruction = (instruction + 1) % std::ssize(instructions);
		} while(state.back() != 'Z');

		result.targetState = state;
		result.nSteps = steps;

		return result;
	};

	std::unordered_map<std::string_view, std::unordered_map<Tint, Mapping>> cache;

	struct State {
		std::string_view state;
		Tint instructionIdx{};
		Tint step{};
	};

	auto increment = [&cache, &instructions, &findNextZ](State& state) mutable {
		auto it1 = cache.find(state.state);
		if(it1 != cache.end()) {
			auto it2 = it1->second.find(state.instructionIdx);

			if(it2 != it1->second.end()) {
				auto [target, nInstructions] = it2->second;
				state.state = target;
				state.step += nInstructions;
				state.instructionIdx = (state.instructionIdx + nInstructions) % std::ssize(instructions);
				return;
			}
		}

		auto result = findNextZ(state.state, state.instructionIdx);

		cache[state.state][state.instructionIdx] = result;
		state.state = result.targetState;
		state.step += result.nSteps;
		state.instructionIdx = (state.instructionIdx + result.nSteps) % std::ssize(instructions);
	};

	std::vector<State> currentNodes;

	for(auto const& [node, _] : graph.nodes) {
		if(node.back() == 'A') {
			currentNodes.emplace_back(State{node, 0});
		}
	}

	Tint currentStep = 1;

	while(true) {
		bool canBreak{true};

#pragma omp parallel for
		for(size_t  i = 0; i < currentNodes.size(); ++i) {
			auto& currentNode = currentNodes[i];

			if(currentNode.step >= currentStep) {
				continue;
			}

#pragma omp atomic write
			canBreak = false;

			while(currentNode.step < currentStep) {
				increment(currentNode);
			}
		}

		if(canBreak) {
			break;
		}

		for(auto const& node : currentNodes) {
			currentStep = std::max(currentStep, node.step);
		}
	}

	return currentStep;
}

int main() {
	auto lines = readAllLines();

	auto [instructions, graph] = extract(lines);

	auto solution = solve(graph, instructions);

	std::cout << solution << "\n";
	
	return 0;
}
