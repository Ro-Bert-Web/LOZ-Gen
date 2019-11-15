#include "boolean.h"


bool isNum(char c){
	return c >= '0' && c <= '9';
}

bool isVar(char c){
	return c >= 'a' && c <= 'z';
}

bool isOpr(char c){
	return c == '~' || c == '&' || c == '^' || c == '|';
}

bool isPrn(char c){
	return c == '(' || c == ')';
}

int getExpect(char c){
	int rslt = 0;
	switch(c){
		case '&':
		case '^':
		case '|':
			rslt++;
		case '~':
		case ' ':
			rslt++;
		default:
			return rslt;
	}
}



//GetInt - gets leading integer from string
//@params
//	string str - input string
//	bool rev - whether string is to be reversed or not
//@return
//	int - leading integer

int getInt(string str, bool rev){
	if(rev){
		reverse(str.begin(), str.end());		//If rev is true, reverse the strings to run the command from the back
	}

	int sign = 1;
	int rslt = 0;
	int pos = 1;
	int i = 0;
	size_t len = str.size();

	if(!rev && str[0] == '-'){					//If string is forward and there is a negative, set the sign and move past
		sign = -1;
		i++;
	}

	for(; i < len && isNum(str[i]); i++){		//Cycle through characters until you find either the end or a non-numeric
		if(!rev){								//Shift number before if forward
			rslt *= 10;
		}

		rslt += (str[i] - '0') * pos;			//Add in position

		if(rev){								//Shift position after if backward (this builds the reversed number backwards)
			pos *= 10;
		}
	}

	if(rev && str[i] == '-'){					//If string was reversed and the negative now tails, set sign
		sign = -1;
	}

	return rslt * sign;
}



//GetOperands - returns the operands of a string after its operator
//@params
//	string exp - input expression
//	bool getAll - tells whether it should gather all operands if nested operators
//@return
//	vector<vector<size_t>> - a vector of start and ends to the operand

vector<vector<size_t>> getOperands(string exp, bool getAll){
	int xpct = getExpect(exp.back());
	vector<vector<size_t>> opers;

	if(isNum(exp.back())){																			//If it is a number return that number
		int x = getInt(exp, true);
		if(x < 0){
			x *= -10;
		}
		int i = (int)fmax(log10(x), 0) + 1;
		vector<size_t> e;
		e.push_back(exp.size() - i);
		e.push_back(i);
		opers.push_back(e);
		return opers;
	}
	else if(isVar(exp.back())){																		//If it is a variable return it
		vector<size_t> e;
		e.push_back(exp.size() - 1);
		e.push_back(1);
		opers.push_back(e);
		return opers;
	}

	int strt = exp.size() - 1, back;

	while(opers.size() < xpct){																		//Starting from the back, get each operand
		back = strt - 1;
		int srch = 1;
		for(strt = back - 1; srch > 0; strt--){														//Each time cycle back until a full operand has been achieved
			srch += getExpect(exp[strt]) - 1;
			if(isNum(exp[strt])){
				int x = getInt(exp.substr(0, strt + 1), true);
				if(x < 0){
					x *= -10;
				}
				strt -= (int)fmax(log10(x), 0);
			}
		}
		strt++;
		vector<size_t> e;
		e.push_back(strt);
		e.push_back(back - strt);																	//Push operand to back of vector and get next one
		opers.push_back(e);
	}
	reverse(opers.begin(), opers.end());															//Reverse the operands to be in proper order

	if(getAll && opers.size() > 1 && exp.back() == exp[opers.back()[0] + opers.back()[1] - 1]){		//If the operator on the back was nested, unravel it and push to back
		vector<vector<size_t>> sOpers = getOperands(exp, opers.back()[0], opers.back()[1], getAll);
		opers.pop_back();
		for(int i = 0; i < sOpers.size(); i++){
			opers.push_back(sOpers[i]);
		}
	}
	return opers;
}

vector<vector<size_t>> getOperands(string exp, size_t pos, size_t len, bool getAll){				//This allows you to call it not on a substring but a part of a string
	vector<vector<size_t>> opers = getOperands(exp.substr(pos, len), getAll);
	for(int i = 0; i < opers.size(); i++){
		opers[i][0] += pos;
	}
	return opers;
}



//InToPost - converts infix expression to postfix expression
//@params
//	string infix - infix expression
//@return
//	string - postfix expression

string inToPost(string infix){
	infix = "(" + infix + ")";																	//For cleanup
	string pstfx = "";
	string opers = "";

	size_t i = -1;
	size_t len = infix.size();
	do{
		i = infix.find_first_of(all, i + 1);													//Go to next character that matters
		if(i == string::npos){
			break;
		}

		if(isNum(infix[i]) || infix[i] == '-'){													//If it is a number get that number
			int x = getInt(infix.substr(i, len - i));
			if(x < 0){
				x *= -10;
			}
			pstfx.insert(pstfx.size(), infix, i, (int)fmax(log10(x), 0) + 1);
			pstfx.push_back(' ');
			i += (int)fmax(log10(x), 0);
		}

		else if(isVar(infix[i])){																//If it is a variable get that variable
			pstfx.push_back(infix[i]);
			pstfx.push_back(' ');
		}

		else if(isOpr(infix[i])){																//If it is an operator, push it to stack; emptying larger ones
			while(isOpr(opers.back()) && oprs.rfind(infix[i]) <= oprs.find(opers.back())){
				pstfx.push_back(opers.back());
				pstfx.push_back(' ');
				opers.pop_back();
			}
			opers.push_back(infix[i]);
		}

		else if(infix[i] == '('){																//If it is a left paren, push to stack
			opers.push_back(infix[i]);
		}

		else if(infix[i] == ')'){																//If it is a right paren, pop until the next left paren
			while(opers.length() > 1 && isOpr(opers.back())){
				pstfx.push_back(opers.back());
				pstfx.push_back(' ');
				opers.pop_back();
			}
			opers.pop_back();
		}
	}while(i < len);
	pstfx.pop_back();																			//Remove the space and return
	return pstfx;
}







//Algbr - mutates an equation
//@params
//	string & exp - input postifx expression
//	size_t pos - position in expression
//	size_t len - length of expression
//	bool simpl - whether the equation is to be simplified
//@return
//	bool - whether expression was changed

bool algbr(string & exp, size_t pos, size_t len, bool simpl){
	if(isOpr(exp[pos + len - 1])){													//If it is not an operation, exit
		if(assoc(exp, pos, len)){													//Apply assoc, combn, and ident
			return true;
		}
		if(combn(exp, pos, len)){
			return true;
		}
		if(ident(exp, pos, len)){
			return true;
		}

		vector<vector<size_t>> opers = getOperands(exp, pos, len);
		for(int i = 0; i < opers.size(); i++){										//If unsuccessful, try on each operand
			if(algbr(exp, opers[i][0], opers[i][1], simpl)){
				return true;
			}
		}

		if((!simpl && distr(exp, pos, len)) || (simpl && factr(exp, pos, len))){	//If still unsuccessful, factor or distribute
			return true;
		}
	}
	return false;
}



//Factr - factors the given expression one operation
//@params
//	string & exp - input postifx expression
//	size_t pos - position in expression
//	size_t len - length of expression
//@return
//	bool - whether expression was changed

bool factr(string & exp, size_t pos, size_t len){
	vector<vector<size_t>> opers = getOperands(exp, pos, len, true);
	switch(exp[pos + len - 1]){
		case '|':																													//Check for only OR's
			for(int i = 0; i < opers.size() - 1; i++){
				if(string ("^|").find(exp[opers[i][0] + opers[i][1] - 1]) == string::npos){											//Find first operand that is either an AND or just an value
					vector<vector<size_t>> sOpersI;
					if(exp[opers[i][0] + opers[i][1] -1] == '~'){
						sOpersI.push_back(opers[i]);
					}
					else{
						sOpersI = getOperands(exp, opers[i][0], opers[i][1], true);
					}
					for(int j = i + 1; j < opers.size(); j++){
						if(string ("^|").find(exp[opers[j][0] + opers[j][1] - 1]) == string::npos){									//Find second operand that is as such
							vector<vector<size_t>> sOpersJ;
							if(exp[opers[j][0] + opers[j][1] - 1] == '~'){
								sOpersJ.push_back(opers[j]);
							}
							else{
								sOpersJ = getOperands(exp, opers[j][0], opers[j][1], true);
							}
							for(int m = 0; m < sOpersI.size(); m++){
								string a = exp.substr(sOpersI[m][0], sOpersI[m][1]);
								for(int n = 0; n < sOpersJ.size(); n++){
									string b = exp.substr(sOpersJ[n][0], sOpersJ[n][1]);

									if(a == b){																						//From those find first element that matches
										string newStr = "";
										for(int x = 0; x < opers.size(); x++){														//Copy every other operand from original OR
											if(x == i || x == j){
												continue;
											}
											newStr.insert(newStr.size(), exp.substr(opers[x][0], opers[x][1]));
											newStr.push_back(' ');
										}
										for(int x = 0; x < sOpersI.size(); x++){													//Copy every element from first operand (replacing like element with -1)
											if(x == m){
												newStr.insert(newStr.size(), "-1 ");
											}
											else{
												newStr.insert(newStr.size(), exp.substr(sOpersI[x][0], sOpersI[x][1]));
												newStr.push_back(' ');
											}
										}
										for(int x = 1; x < sOpersI.size(); x++){													//Add appropriate amount of AND's
											newStr.insert(newStr.size(), "& ");
										}
										for(int x = 0; x < sOpersJ.size(); x++){													//Copy every element from second operand
											if(x == n){
												newStr.insert(newStr.size(), "-1 ");
											}
											else{
												newStr.insert(newStr.size(), exp.substr(sOpersJ[x][0], sOpersJ[x][1]));
												newStr.push_back(' ');
											}
										}
										for(int x = 1; x < sOpersJ.size(); x++){													//Add appropriate amount of AND's
											newStr.insert(newStr.size(), "& ");
										}
										newStr.insert(newStr.size(), "| ");															//Add factored bit
										newStr.insert(newStr.size(), a + " & ");
										for(int x = 2; x < opers.size(); x++){														//Add remaining OR's
											newStr.insert(newStr.size(), "| ");
										}
										newStr.pop_back();

										exp.replace(pos, len, newStr);																//Replace old equation
										return true;
									}
								}
							}
						}
					}
				}
			}
			break;
		default:
			break;
	}
	return false;
}



//Distr - distributes the given expression one operation
//@params
//	string & exp - input postifx expression
//	size_t pos - position in expression
//	size_t len - length of expression
//@return
//	bool - whether expression was changed

bool distr(string & exp, size_t pos, size_t len){
	vector<vector<size_t>> opers = getOperands(exp, pos, len);
	switch(exp[pos + len - 1]){
		case '~':
			{
				vector<vector<size_t>> sOpers = getOperands(exp, opers[0][0], opers[0][1]);
				switch(exp[opers[0][0] + opers[0][1] - 1]){
					case '~':
						exp.replace(pos, len, exp.substr(sOpers[0][0], sOpers[0][1]));						//~~A = A
						return true;
					case '&':																				//~(A&B) = ~A|~B
						{
							string newStr = "";
							newStr.insert(newStr.size(), exp, sOpers[0][0], sOpers[0][1]);
							newStr.insert(newStr.size(), " ~ ");
							newStr.insert(newStr.size(), exp, sOpers[1][0], sOpers[1][1]);
							newStr.insert(newStr.size(), " ~ |");

							exp.replace(pos, len, newStr);
							return true;
						}
					case '|':																				//~(A|B) = ~A&~B
						{
							string newStr = "";
							newStr.insert(newStr.size(), exp, sOpers[0][0], sOpers[0][1]);
							newStr.insert(newStr.size(), " ~ ");
							newStr.insert(newStr.size(), exp, sOpers[1][0], sOpers[1][1]);
							newStr.insert(newStr.size(), " ~ &");

							exp.replace(pos, len, newStr);
							return true;
						}
					default:
						break;
				}
			}
			break;
		case '&':
			{
				switch(exp[opers[1][0] + opers[1][1] - 1]){
					case '^':																				//A&(B^C) = A&B^A&C
					case '|':																				//A&(B|C) = A&B|A&C
						{
							vector<vector<size_t>> sOpers = getOperands(exp, opers[1][0], opers[1][1]);
							string newStr = "";
							newStr.insert(newStr.size(), exp, opers[0][0], opers[0][1]);
							newStr.push_back(' ');
							newStr.insert(newStr.size(), exp, sOpers[0][0], sOpers[0][1]);
							newStr.push_back(' ');
							newStr.push_back(exp[pos + len - 1]);
							newStr.push_back(' ');
							newStr.insert(newStr.size(), exp, opers[0][0], opers[0][1]);
							newStr.push_back(' ');
							newStr.insert(newStr.size(), exp, sOpers[1][0], sOpers[1][1]);
							newStr.push_back(' ');
							newStr.push_back(exp[pos + len - 1]);
							newStr.push_back(' ');
							newStr.push_back(exp[opers[1][0] + opers[1][1] - 1]);

							exp.replace(pos, len, newStr);
							return true;
						}
					default:
						break;
				}
				break;
			}
		case '^':																							//A^B = ~A&B|A&~B
			{
				string newStr = "";
				newStr.insert(newStr.size(), exp, opers[0][0], opers[0][1]);
				newStr.push_back(' ');
				newStr.insert(newStr.size(), exp, opers[1][0], opers[1][1]);
				newStr.insert(newStr.size(), " ~ & ");
				newStr.insert(newStr.size(), exp, opers[1][0], opers[1][1]);
				newStr.push_back(' ');
				newStr.insert(newStr.size(), exp, opers[0][0], opers[0][1]);
				newStr.insert(newStr.size(), " ~ & |");

				exp.replace(pos, len, newStr);
				return true;
			}
		default:
			break;
	}
	return false;
}



//Grtr - compares two expressions
//@params
//	string a - first postfix
//	string b - second postfix
//@return
//	bool - whether a is greater in order than b

bool grtr(string a, string b){
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	if(isOpr(a[0]) != isOpr(b[0])){							//If one is an operation, move the operation forward
		return isOpr(a[0]);
	}
	if(isOpr(a[0])){										//If they are both operations, sort by expected inputs
		if(getExpect(a[0]) != getExpect(b[0])){
			return getExpect(a[0]) > getExpect(b[0]);
		}
	}
	return a > b;											//Sort by alphabet
}



//Assoc - associates the given expression one operation
//@params
//	string & exp - input postifx expression
//	size_t pos - position in expression
//	size_t len - length of expression
//@return
//	bool - whether expression was changed

bool assoc(string & exp, size_t pos, size_t len){
	vector<vector<size_t>> opers = getOperands(exp, pos, len, true);
	for(int i = 0; i < opers.size() - 1; i++){
		if(exp[opers[i][0] + opers[i][1] - 1] == exp[pos + len - 1]){	//Move operands of the same operator to the front. This also merges them
			string a = exp.substr(opers.back()[0], opers.back()[1]);
			string b = exp.substr(opers[i][0], opers[i][1]);
			exp.replace(opers.back()[0], opers.back()[1], b);
			exp.replace(opers[i][0], opers[i][1], a);
			return true;
		}
		string a = exp.substr(opers[i][0], opers[i][1]);
		string b = exp.substr(opers[i + 1][0], opers[i + 1][1]);
		if(grtr(a, b)){													//If element is greater than the next, sort those two
			exp.replace(opers[i + 1][0], opers[i + 1][1], a);
			exp.replace(opers[i][0], opers[i][1], b);
			return true;
		}
	}
	return false;
}



//Ident - identities the given expression one operation
//@params
//	string & exp - input postifx expression
//	size_t pos - position in expression
//	size_t len - length of expression
//@return
//	bool - whether expression was changed

bool ident(string & exp, size_t pos, size_t len){
	vector<vector<size_t>> opers = getOperands(exp, pos, len);
	switch(exp[pos + len - 1]){
		case '&':
			{
				string a = exp.substr(opers[0][0], opers[0][1]);
				string b = exp.substr(opers[1][0], opers[1][1]);
				if(a == "0"){												//0&A = 0
					exp.replace(pos, len, "0");
					return true;
				}
				else if(b == "0"){											//A&0 = 0
					exp.replace(pos, len, "0");
					return true;
				}
				else if(a == "-1"){											//1&A = A (-1 to make every bit 1)
					exp.replace(pos, len, b);
					return true;
				}
				else if(b == "-1"){											//A&1 = A
					exp.replace(pos, len, a);
					return true;
				}
				break;
			}
		case '^':
			{
				string a = exp.substr(opers[0][0], opers[0][1]);
				string b = exp.substr(opers[1][0], opers[1][1]);
				if(a == "0"){												//0^A = A
					exp.replace(pos, len, b);
					return true;
				}
				else if(b == "0"){											//A^0 = A
					exp.replace(pos, len, a);
					return true;
				}
				else if(a == "-1"){
					exp.replace(pos, len, b + " ~");						//1^A = ~A
					return true;
				}
				else if (b == "-1"){
					exp.replace(pos, len, a + " ~");						//A^1 = ~A
					return true;
				}
				break;
			}
		case '|':
			{
				string a = exp.substr(opers[0][0], opers[0][1]);
				string b = exp.substr(opers[1][0], opers[1][1]);
				if(a == "0"){												//0|A = A
					exp.replace(pos, len, b);
					return true;
				}
				else if(b == "0"){											//A|0 = A
					exp.replace(pos, len, a);
					return true;
				}
				else if(a == "-1"){											//1|A = 1
					exp.replace(pos, len, "-1");
					return true;
				}
				else if (b == "-1"){										//A|1 = 1
					exp.replace(pos, len, "-1");
					return true;
				}
				break;
			}
		default:
			break;
	}
	vector<int> vals;
	for(int i = 0; i < opers.size(); i++){
		if(isNum(exp[opers[i][0] + opers[i][1] - 1])){						//If all operands are numbers, solve it
			vals.push_back(getInt(exp.substr(opers[i][0], opers[i][1])));
		}
		else{
			break;
		}
	}
	if(vals.size() == opers.size()){
		switch(exp[pos + len - 1]){
			case '~':
				exp.replace(pos, len, to_string(~ vals[0]));
				return true;
			case '&':
				exp.replace(pos, len, to_string(vals[0] & vals[1]));
				return true;
			case '^':
				exp.replace(pos, len, to_string(vals[0] ^ vals[1]));
				return true;
			case '|':
				exp.replace(pos, len, to_string(vals[0] | vals[1]));
				return true;
			default:
				break;
		}
	}
	return false;
}



//Combn - combines like terms
//@params
//	string & exp - input postfix expression
//	size_t pos - position in expression
//	size_t len - length of expression
//@return
//	bool - whether expression was changed

bool combn(string & exp, size_t pos, size_t len){
	vector<vector<size_t>> opers = getOperands(exp, pos, len, true);
	string a = exp.substr(opers[0][0], opers[0][1]);
	for(int i = 1; i < opers.size(); i++){
		string b = exp.substr(opers[i][0], opers[i][1]);
		switch(exp[pos + len - 1]){
			case '&':
				if(a == b){
					exp.replace(opers[0][0], opers[0][1], "-1");		//A&A = A
					return true;
				}
				if((a + " ~") == b){
					exp.replace(opers[0][0], opers[0][1], "0");			//A&~A = 0
					return true;
				}
				break;
			case '^':													//A^A = 0
				if(a == b){
					exp.replace(pos, len, "0");
					return true;
				}
				if((a + " ~") == b){									//A^~A = -1
					exp.replace(pos, len, "-1");
					return true;
				}
				break;
			case '|':													//A|A = A
				if(a == b){
					exp.replace(opers[0][0], opers[0][1], "0");
					return true;
				}														//A|~A = 1
				if((a + " ~") == b){
					exp.replace(opers[0][0], opers[0][1], "-1");
					return true;
				}
				break;
			default:
				break;
		}
	}
	return false;
}





//Solve - solves a given equation with the given inputs
//@params
//	string exp - input postfix expression
//	int vals[26] - value for each variable
//@return
//	int - result

int solve(string exp, int vals[]){
	vector<vector<size_t>> opers = getOperands(exp);
	int rslt = 0;
	switch(exp.back()){
		case '~':
			rslt = ~ solve(exp.substr(opers[0][0], opers[0][1]), vals);
			break;
		case '&':
			rslt = solve(exp.substr(opers[0][0], opers[0][1]), vals) & solve(exp.substr(opers[1][0], opers[1][1]), vals);
			break;
		case '^':
			rslt = solve(exp.substr(opers[0][0], opers[0][1]), vals) ^ solve(exp.substr(opers[1][0], opers[1][1]), vals);
			break;
		case '|':
			rslt = solve(exp.substr(opers[0][0], opers[0][1]), vals) | solve(exp.substr(opers[1][0], opers[1][1]), vals);
			break;
		default:
			if(isNum(exp.back())){
				rslt = getInt(exp, true);
			}
			if(isVar(exp.back())){
				rslt = vals[exp.back() - 'a'];
			}
			break;
	}
	return rslt;
}



//Algebra - algebraically changes expression

void BoolExp::algebra(bool simpl, bool cmplt){
	while(cmplt && algbr(exp, 0, exp.size(), !simpl)){
	}
	while(algbr(exp, 0, exp.size(), simpl)){
	}
}