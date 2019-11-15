#ifndef Truth_H
#define Truth_H

#include "boolean.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Truth{
private:
	int row;
	int ins;
	vector<int> out;
public:
	Truth(int ins_, int out_);
	void setOut(int out_, int row_, bool val);
	bool getOut(int out_, int row_);
	BoolExp getEq(int out_);
};

#endif