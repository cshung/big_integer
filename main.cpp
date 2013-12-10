#include "stdafx.h"
#include "big_integer.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	big_integer big1 = big_integer::create("1");
	big_integer big2 = big_integer::create("1");
	big_integer big3 = big1 + big2;
	for (int i = 3; i <= 1000; i++)
	{
		big1 = big2;
		big2 = big3;
		big3 = big1 + big2;
	}

	cout << "Fib(100) = " << big3 << endl;

	return 0;
}

