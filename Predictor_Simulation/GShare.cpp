#include "GShare.h"

GShare::GShare(int HistoryBits, int state_bits)
{
	if (hbits > 27)throw "history too big!";
	const int size = (int)pow(2, HistoryBits);
	hbits = HistoryBits;
	history = History(HistoryBits);
	states = std::vector<State>(size);

	for (auto& state : states) state = State(state_bits);
}

GShare::~GShare()
= default;

double GShare::RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data)
{
	int correct = 0;
	for(const auto& pair : trace_data)
	{
		const auto is_jump = pair.second;
		const auto address = get_index(pair.first, hbits);

		//binary XOR(address, history)
		auto& state = states[address ^ history.get_index()];
		
		if (state.is_true() == is_jump)correct++;
		state.change_state(is_jump);
	}

	return correct / (double)trace_data.size();
}
