#ifndef __BIG_INTEGER_H__
#define __BIG_INTEGER_H__

#include <vector>
using namespace std;

/* The faciliate input/output, the internal representation is base 10^9 */
class big_integer
{
public:
	static big_integer create(char* c, bool is_positive = true);
	bool is_positive;
	big_integer operator+(const big_integer& that) const;
	big_integer operator-(const big_integer& that) const;
	big_integer operator-() const;
	big_integer operator=(const big_integer& that);
	friend ostream& operator<<(ostream& os, const big_integer& operand);
private:
	big_integer();
	int absolute_compare(const big_integer& that) const;
	big_integer unsigned_add(const big_integer& that) const;
	big_integer unsigned_sub(const big_integer& that) const;
	
	vector<int> words;

};

#endif
