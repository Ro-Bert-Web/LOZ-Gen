#ifndef Boolean_H
#define Boolean_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

const string vars = "abcdefghijklmnopqrstuvwxyz";
const string nums = "-0123456789";
const string oprs = "|^&~~";
const string prns = "()";
const string all = vars + nums + oprs + prns;

bool isNum(char c);
bool isVar(char c);
bool isOpr(char c);
bool isPrn(char c);

int getExpect(char c);

int getInt(string str, bool rev = false);
vector<vector<size_t>> getOperands(string exp, bool getAll = false);
vector<vector<size_t>> getOperands(string exp, size_t pos, size_t len, bool getAll = false);
string inToPost(string infix);

bool algbr(string & exp, size_t pos, size_t len, bool simpl = true);
bool factr(string & exp, size_t pos, size_t len);
bool distr(string & exp, size_t pos, size_t len);
bool grtr(string a, string b);
bool assoc(string & exp, size_t pos, size_t len);
bool ident(string & exp, size_t pos, size_t len);
bool combn(string & exp, size_t pos, size_t len);

int solve(string exp, int vals[26]);

class BoolExp{
private:
	string exp;
public:
	BoolExp(string exp_){exp = inToPost(exp_);}
	void algebra(bool simpl = true, bool cmplt = false);
	int given(int vals[]){return solve(exp, vals);}
	string getStr(){return exp;}
};

#endif