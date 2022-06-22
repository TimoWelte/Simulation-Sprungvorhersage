#include "Tournament.h"

Tournament::Tournament(TwoLevelGlobal* global, LocalHistoryPredictor* local, const int state_bits)
{
	state = State(state_bits);
	globalPredictor = global;
	localPredictor = local;
}

Tournament::~Tournament()
{
	delete globalPredictor;
	delete localPredictor;
}

double Tournament::RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data)
{
	int correct = 0;
	const size_t total = trace_data.size();

	for(auto& line : trace_data)
	{
		const bool is_jump = line.second;
		const bool local_prediction = localPredictor->Action(line.first, is_jump);
		const bool global_prediction = globalPredictor->Action(is_jump);

		//if predictor predictor bits are "low" and local prediction is correct
		//or if predictor predictor bits are "high" and global prediction is correct -> correct prediction
		if((state.is_true() && global_prediction) || (!state.is_true() && local_prediction)) correct++;

		if (local_prediction && !global_prediction)state.change_state(false);//local right but not global
		if (!local_prediction && global_prediction)state.change_state(true);//global right but not local
	}

	return correct / (double)total;
}
