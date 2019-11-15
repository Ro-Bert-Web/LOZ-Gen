#include "truth.h"

//Truth - Constructor
//@params
//	int in - number of inputs
//	int out - number of outputs

Truth::Truth(int ins_, int out_) : out(out_, 0){
	ins = ins_;
	row = 1 << ins;
}



//SetOut - set a specific slot in a given output to a certain value
//@params
//	int out - specified output
//	int row - specified slot
//	bool val - whether that is true or false

void Truth::setOut(int out_, int row_, bool val){
	if(val){
		out[out_] |= 1 << row_;
	}
	else{
		out[out_] &= ~(1 << row_);
	}
}



//GetOut - returns the value in a specific slot of an output
//@params
//	int out - specified output
//	int row - specified row
//@return
//	bool - whether that slot is true or not

bool Truth::getOut(int out_, int row_){
	return (out[out_] >> row_ & 1) == 1;
}



//GetEq - Get equation for specified output
//@params
//	int out - specified output
//@return
//	BoolExp - returned expression

BoolExp Truth::getEq(int out_){
	string minTerms = "";
	for(int i = 0; i < row; i++){
		if(getOut(out_, i)){
			if(minTerms.size() != 0){
				minTerms.push_back('|');
			}
			for(int j = 0; j < ins; j++){
				if(j != 0){
					minTerms.push_back('&');
				}
				if((i >> j & 1) == 0){
					minTerms.push_back('~');
				}
				minTerms.push_back('a' + j);
			}
		}
	}
	BoolExp exp(minTerms);
	return exp;
}
