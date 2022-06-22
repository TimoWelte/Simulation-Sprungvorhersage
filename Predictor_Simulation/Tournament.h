#pragma once
#include <map>

#include "TwoLevelGlobal.h"
#include "Predictors.h"

class LocalHistoryPredictor
{
private:
	int abits;//adress bits
	int hbits;//history bits
	int sbits;//state bits
	std::vector<std::pair<History, std::vector<State>>> table;
public:
	LocalHistoryPredictor(int address_bits, int history_bits, int state_bits)
	{
		abits = address_bits;
		sbits = state_bits;
		hbits = history_bits;

		const int table_size = static_cast<int>(pow(2, abits));
		const int state_vector_size = static_cast<int>(pow(2, hbits));

		table = std::vector<std::pair<History, std::vector<State>>>(table_size);
		for(auto& row : table)
		{
			row = std::pair(History(hbits), std::vector<State>(state_vector_size));
			for (auto& state : row.second) state = State(sbits);
		}
	}
	~LocalHistoryPredictor() = default;

	bool Action(address_t address, bool jump)
	{
		const auto index = get_index(address, abits);
		auto& data = table[index];
		auto& state = data.second[data.first.get_index()];
		const bool prediction = state.is_true();

		//change state of table
		data.first.move(jump);
		state.change_state(jump);

		return prediction == jump;
	}

	double RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data)
	{
		int correct = 0;

		for(auto& line : trace_data)
		{
			auto& address = line.first;
			const auto is_jump = line.second;

			auto& branch = table[get_index(address, abits)];
			auto& state = branch.second[branch.first.get_index()];
			const bool prediction = state.is_true();

			if (prediction == is_jump)correct++;

			branch.first.move(is_jump);
			state.change_state(is_jump);
		}

		return correct / (double)trace_data.size();
	}
};

class Tournament
{
private:
	TwoLevelGlobal* globalPredictor;
	LocalHistoryPredictor* localPredictor;
	State state;//predictor predictor bits

public:
	Tournament(TwoLevelGlobal* global, LocalHistoryPredictor* local, int state_bits);
	~Tournament();
	double RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data);
};

