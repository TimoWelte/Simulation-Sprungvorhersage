#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

#include "GShare.h"
#include "TwoBit_Local_Limited.h"
#include "TwoLevelGlobal.h"
#include "Tournament.h"
#include "Local_unlimited.h"

using namespace std;
namespace fs = std::filesystem;

auto test_set = vector<vector<pair<address_t, int>>>();
std::mutex globalMutex;

int convert_char(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}

	return -1;
}

address_t convert(string address)
{
	address_t ret = 0;
	int count = 0;
	for(int i = address.size() - 1; i >= 0; i--)
	{
		address_t d = convert_char(address[i]);
		for (int j = 0; j < count; j++) d *= 16;
		count++;
		ret += d;
	}

	return ret;
}

void Local_history_prediction(int state_bits, int adress_bits, int history_bits)
{
	stringstream s;
	ofstream file;
	file.open("Local_history_results.txt", ios::app);
	s << "state bits: " << state_bits << " address bits: " << adress_bits << "history bits: " << history_bits << "\n";
	const size_t memory_size = (size_t)pow(2, adress_bits) * static_cast<size_t>(pow(2, history_bits)) * (size_t)state_bits / 8.0;
	s << memory_size << " bytes of memory necessary\n\n";

	auto predictor = LocalHistoryPredictor(state_bits, adress_bits, history_bits);
	int count = 0;
	for (auto& data : test_set)
	{
		count++;
		s << "Precision test set " << count << ": " << predictor.RunTestSet(data) << "\n";
	}

	cout << s.str();
	file << s.str();

	file.close();
}

void Local_Limited_prediction(int state_bits, int adress_bits)
{
	stringstream s;
	ofstream file;
	file.open("Local_limited_results.txt", ios::app);
	s << "state bits: " << state_bits << " address bits: " << adress_bits << "\n";
	const size_t memory_size = (size_t)pow(2, adress_bits)*(size_t)state_bits/8.0;
	s <<  memory_size << " bytes of memory necessary\n\n";

	auto NBL_limited = NBitLocal_limited(state_bits, adress_bits);
	int count = 0;
	for (auto& data : test_set)
	{
		count++;
		s << "Precision test set " << count << ": " << NBL_limited.RunTestSet(data) << "\n";
	}

	cout << s.str();
	file << s.str();

	file.close();
}

void Local_Unlimited_prediction(int state_bits)
{
	stringstream s;
	ofstream file;
	file.open("Local_unlimited_results.txt", ios::app);
	s << "state bits: " << state_bits << " address bits: " << 32 << "\n";
	const size_t memory_size = (size_t)pow(2, 32) * (size_t)state_bits / 8.0;
	s << memory_size << " bytes of memory necessary\n\n";

	auto predictor = Local_unlimited(state_bits);
	int count = 0;
	for (auto& data : test_set)
	{
		count++;
		s << "Precision test set " << count << ": " << predictor.RunTestSet(data) << "\n\n";
	}
	cout << s.str();
	file << s.str();

	file.close();
}

void Global_prediction(int history_bits, int state_bits)
{
	stringstream s;
	ofstream file;
	file.open("Global_results.txt", ios::app);

	s << "Global predictor with " << state_bits << " state bits and " << history_bits << " history bits\n\n";

	//Größe des GHR wird hier vernachlässigt
	const size_t memory_size = (size_t)pow(2, history_bits) * (size_t)state_bits / 8.0;
	s << memory_size << " bytes of memory necessary\n\n";

	auto predictor = TwoLevelGlobal(history_bits, state_bits);

	int count = 0;
	for (const auto& data : test_set)
	{
		count++;
		s << "Precision test set " << count << ": " << predictor.RunTestSet(data) << "\n";
	}

	cout << s.str();
	file << s.str();

	file.close();
}

void GShare_prediction(int history_bits, int state_bits)
{
	stringstream s;
	ofstream file;
	file.open("Gshare_results.txt", ios::app);

	s << "Gshare predictor with " << state_bits << " state bits and " << history_bits << " history bits\n\n";

	const size_t memory_size = (size_t)pow(2, history_bits) * (size_t)state_bits / 8.0;
	s << memory_size << " bytes of memory necessary\n\n";

	auto predictor = GShare(history_bits, state_bits);

	int count = 0;
	for (const auto& data : test_set)
	{
		count++;
		s << "Precision test set " << count << ": " << predictor.RunTestSet(data) << "\n";
	}

	cout << s.str();
	file << s.str();

	file.close();
}

void Tournament_prediction(int labits, int lhbits, int lsbits, int ghbits, int gsbits, int tsbits)
{
	cout << "Tournament:\n";

	auto localPredictor = new LocalHistoryPredictor(labits, lhbits, lsbits);
	auto globalPredictor = new TwoLevelGlobal(ghbits, gsbits);
	auto predictor = Tournament(globalPredictor, localPredictor, tsbits);

	ofstream file;
	file.open(("Tournament_results"), ios::app);
	stringstream s;
	s << "Local predictor spec: " << "history bits: " << lhbits << " address bits: " << labits << " state bits: " << lsbits << "\n";
	s << "Global predictor spec: " << "history bits: " << ghbits << " state bits: " << gsbits << "\n";
	s << "Tournament state bits: " << tsbits << "\n\n";

	const size_t memory_size = (size_t)pow(2, labits)*(size_t)pow(2, lhbits) * lsbits + (size_t)pow(2, ghbits)*gsbits + tsbits;
	s << memory_size/8 << " bytes of memory necessary\n\n";

	cout << s.str();
	file << s.str();
	

	int count = 0;
	for (const auto& data : test_set)
	{
		count++;
		stringstream str;
		str << "Precision test set " << count << ": " << predictor.RunTestSet(data) << "\n";
		if (file.is_open())
		{
			file << str.str();
		}
		cout << str.str();
	}
	file.close();
}

void read_file(string path)
{
	auto data = vector<pair<address_t, int>>();

	fstream file;
	file.open(path, ios::in);

	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			string address;
			//get adress:
			for (int i = 0; i < 8; i++)address += line[i];
			int jmp;
			if (line[9] == '0')jmp = 0;
			else
			{
				jmp = 1;
			}
			data.emplace_back(convert(address), jmp);
		}
		file.close();
	}

	globalMutex.lock();
	test_set.emplace_back(data);
	globalMutex.unlock();
}

void read_files(string path)
{
	auto threads = vector<thread>();

	for (const auto& entry : fs::directory_iterator(path))
	{
		string file_path = entry.path().string();
		threads.emplace_back(read_file, file_path);
	}

	for (auto& t : threads)
	{
		t.join();
	}
}

int main()
{
	string path = "Trace files";

	cout << "Reading trace_files...\n";

	read_files(path);

	cout << "Trace_files successfully read!\n\n";

	int n = 6;
	enum Predictors { Local_limited, Local_unlimited, Global, Gshare, Tournament, Local_history };
	const string predictors[] = { "(Nbit) local", "Unlimited local", "Two level global", "Gshare", "Tournament", "Local history"};

	while (true)
	{
		int selected;

		cout << "Available predictors:\n\n";
		for (int i = 0; i < 6; i++) cout << i << " : " << predictors[i] << "\n";
		cout << "\nSelect a predictor (Enter number between 0 and 5): ";

		cin >> selected;
		if (selected >= 6)continue;

		cout << "\nPredictor \"" << predictors[selected] << "\" was selected - please enter parameters for predictor:\n";

		if (selected == Predictors::Local_limited)
		{
			int state_bits = 2, address_bits = 10;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> state_bits;
			//address bits
			cout << "\nEnter number of address bits (number of bits from address used as index): ";
			cin >> address_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			Local_Limited_prediction(state_bits, address_bits);
		}
		else if (selected == Predictors::Local_unlimited)
		{
			int state_bits = 2;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> state_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			Local_Unlimited_prediction(state_bits);
		}
		else if (selected == Predictors::Global)
		{
			int history_bits = 4, state_bits = 2;
			//history bits
			cout << "\nEnter number of history bits (size of history): ";
			cin >> history_bits;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> state_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			Global_prediction(history_bits, state_bits);
		}
		else if (selected == Predictors::Gshare)
		{
			int history_bits = 4, state_bits = 2;
			//history bits
			cout << "\nEnter number of history bits (size of history): ";
			cin >> history_bits;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> state_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			GShare_prediction(history_bits, state_bits);
		}
		else if (selected == Predictors::Tournament)
		{
			int local_address_bits = 10, local_history_bits = 4, local_state_bits = 2, global_history_bits = 4, global_state_bits = 2, tournament_state_bits = 1;
			cout << "\nLocal predictor:";
			//address bits
			cout << "\nEnter number of address bits (number of bits from address used as index): ";
			cin >> local_address_bits;
			//history bits
			cout << "\nEnter number of history bits (size of history for each branch): ";
			cin >> local_history_bits;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> local_state_bits;

			cout << "\n\nGlobal predictor:";
			//history bits
			cout << "\nEnter number of history bits (size of history for each branch): ";
			cin >> global_history_bits;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> global_state_bits;

			//Tounamentstate bits
			cout << "\n\nEnter number of TOURNAMENT state bits (number of \"state bits\" to choose from predictors): ";
			cin >> tournament_state_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			Tournament_prediction(local_address_bits, local_history_bits, local_state_bits, global_history_bits, global_state_bits, tournament_state_bits);
		}
		else if(selected == Predictors::Local_history)
		{
			int state_bits, address_bits, history_bits;
			//state bits
			cout << "\nEnter number of state bits (size of each state): ";
			cin >> state_bits;
			//address bits
			cout << "\nEnter number of address bits (number of bits from address used as index): ";
			cin >> address_bits;

			//history bits
			cout << "\nEnter number of history bits (size of history for each branch): ";
			cin >> history_bits;

			cout << "\n";
			cout << "Running test sets...\n\n";

			Local_history_prediction(state_bits, address_bits, history_bits);
		}
		else
		{
			cout << "Invalid input!\n";
		}
	}
}