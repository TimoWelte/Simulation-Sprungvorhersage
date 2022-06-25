#pragma once
#include <string>
#include <cmath>

typedef unsigned long long address_t;

class History
{
	int history;
	int size;

public:
	History()
	{
		history = 0;
		size = 0;
	}
	History(int hbits)
	{
		if (hbits > 27)throw "history too big";
		size = hbits;
		history = 0;
	}

	void move(bool jump)
	{
		//right shift history
		history = history >> 1;
		//set left most bit to 1 if jump
		if (jump)history += (int)pow(2, size - 1);
	}

	int get_index()const
	{
		return history;
	}
};

class State
{
private:
	int bits;
	int state;
public:
	State()
	{
		bits = 0;
		state = 0;
	}
	State(int Bits)
	{
		bits = Bits;
		state = 0;
	}
	bool is_true()
	{
		auto n = pow(2, bits);
		if ((int)n - state <= n / 2)return true;
		return false;
	}
	void change_state(bool jump)
	{
		auto n = pow(2, bits);
		if (jump)
		{
			//add 1
			if (state >= n - 1)return;
			state++;
		}
		else
		{
			if (state <= 0)return;
			state--;
		}
	}
};

inline address_t get_index(address_t address, int bits)
{
	const auto mask = static_cast<address_t>(pow(2, bits));

	return address % mask;
}