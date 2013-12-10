#include "stdafx.h"
#include "big_integer.h"

#include <iostream>
using namespace std;

/* static */ big_integer big_integer::create(char* c, bool is_positive)
{
	big_integer result;
	int length = 0;
	char* d = c;
	while (*d != '\0')
	{
		d++;
		length++;
	}
	int remainder = length % 9;

	d = c;
	int current = 0;
	if (remainder != 0)
	{	
		current = 0;
		for (int i = 0; i < remainder; i++)
		{
			current = current * 10;
			current += (*d) - '0';
			d++;
		}
		result.words.push_back(current);
	}

	while (*d != '\0') 
	{
		current = 0;
		for (int i = 0; i < 9; i++)
		{
			current = current * 10;
			current += (*d) - '0';
			d++;
		}
		result.words.push_back(current);
	}

	result.is_positive = is_positive;
	return result;
}

big_integer::big_integer()
{
	this->is_positive = true;
}

big_integer big_integer::operator+(const big_integer& that) const
{
	if (this->is_positive == that.is_positive)
	{
		big_integer result = this->unsigned_add(that);
		result.is_positive = this->is_positive;
		return result;
	}
	else
	{
		int compare_result = absolute_compare(that);
		if (compare_result == 0)
		{
			big_integer result;
			result.words.push_back(0);
			return result;
		}
		
		if (compare_result > 0)
		{
			big_integer unsigned_subtract_result = unsigned_sub(that);
			if (!this->is_positive) 
			{
				// -this + that case
				unsigned_subtract_result.is_positive = false;
			}

			return unsigned_subtract_result;
		}
		else
		{
			big_integer unsigned_subtract_result = that.unsigned_sub(*this);
			if (this->is_positive) 
			{
				// this - that case
				unsigned_subtract_result.is_positive = false;
			}

			return unsigned_subtract_result;
		}
	}
}

big_integer big_integer::operator-() const
{
	big_integer result = *this;
	result.is_positive = !result.is_positive;
	return result;
}

big_integer big_integer::operator-(const big_integer& that) const
{
	return operator+(-that);
}

ostream& operator<<(ostream& os, const big_integer& operand)
{
	bool first = true;
	if (!operand.is_positive)
	{
		os << '-';
	}
	for (vector<int>::const_iterator words_iterator = operand.words.cbegin(); words_iterator != operand.words.cend(); words_iterator++)
	{
		if (first)
		{
			os << *words_iterator;
			first = false;
		}
		else
		{
			int value = *words_iterator;
			os << value / 100000000;
			value = value % 100000000;

			os << value / 10000000;
			value = value % 10000000;

			os << value / 1000000;
			value = value % 1000000;

			os << value / 100000;
			value = value % 100000;

			os << value / 10000;
			value = value % 10000;

			os << value / 1000;
			value = value % 1000;

			os << value / 100;
			value = value % 100;

			os << value / 10;
			value = value % 10;

			os << value;
		}

	}

	return os;
}

big_integer big_integer::operator=(const big_integer& that)
{
	if (this != &that)
	{
		this->is_positive = that.is_positive;
		this->words.clear();
		for(vector<int>::const_iterator words_iterator = that.words.cbegin(); words_iterator != that.words.cend(); words_iterator++)
		{
			this->words.push_back(*words_iterator);
		}
	}

	return *this;
}

int big_integer::absolute_compare(const big_integer& that) const
{
	if (this->words.size() > that.words.size())
	{
		return 1;
	}
	else
	{
		vector<int>::const_iterator this_iterator = this->words.cbegin();
		vector<int>::const_iterator that_iterator = that.words.cbegin();

		for (unsigned int i = 0; i < this->words.size(); i++)
		{
			if ((*this_iterator) > (*that_iterator))
			{
				return 1;
			}
			else if ((*this_iterator) < (*that_iterator))
			{
				return -1;
			}
		}

		return 0;
	}
}

big_integer big_integer::unsigned_add(const big_integer& that) const
{
	big_integer result;
	vector<int>::const_iterator thisLast = this->words.cend();
	vector<int>::const_iterator thatLast = that.words.cend();
	bool thisEnd = false;
	bool thatEnd = false;
	int carry = 0;
	while (!thisEnd || !thatEnd) 
	{	
		int thisValue;
		int thatValue;

		if (thisEnd)
		{
			thisValue = 0;
		}
		else
		{
			thisLast--;		
			if (thisLast == this->words.cbegin())
			{
				thisEnd = true;
			}

			thisValue = (*thisLast);
		}

		if (thatEnd)
		{
			thatValue = 0;
		}
		else
		{
			thatLast--;		
			if (thatLast == that.words.cbegin())
			{
				thatEnd = true;
			}

			thatValue = (*thatLast);
		}
		
		int sum = (thisValue + thatValue + carry);
		int current = sum % 1000000000;
		carry = (sum >= 1000000000) ? 1 : 0;
		result.words.insert(result.words.begin(), current);
	}

	if (carry == 1)
	{
		result.words.insert(result.words.begin(), 1);
	}

	return result;
}

big_integer big_integer::unsigned_sub(const big_integer& that) const
{
	// Assumption: this > that
	big_integer result;
	vector<int>::const_iterator thisLast = this->words.cend();
	vector<int>::const_iterator thatLast = that.words.cend();
	bool thisEnd = false;
	bool thatEnd = false;
	int borrow = 0;
	while (!thisEnd || !thatEnd) 
	{	
		int thisValue;
		int thatValue;

		if (thisEnd)
		{
			thisValue = 0;
		}
		else
		{
			thisLast--;		
			if (thisLast == this->words.cbegin())
			{
				thisEnd = true;
			}

			thisValue = (*thisLast);
		}

		if (thatEnd)
		{
			thatValue = 0;
		}
		else
		{
			thatLast--;		
			if (thatLast == that.words.cbegin())
			{
				thatEnd = true;
			}

			thatValue = (*thatLast);
		}
		
		int difference = (thisValue - borrow - thatValue);
		if (difference < 0)
		{
			difference += 1000000000;
			borrow = 1;
		}
		else
		{
			borrow = 0;
		}

		result.words.insert(result.words.begin(), difference);
	}

	return result;
}