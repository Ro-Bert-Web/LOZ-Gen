#include "dungeon.h"

//GetPos - gets a door's position in specific room
//@params
//	int room - specific room
//@return
//	Pos - position in room
//@throw
//	if not in room

Pos Door::getPos(int r){
	if(!inRoom(r)){
		throw "Not in room";
	}
	return (p1.r == r) ? p1 : p2;
}



//GetExit - returns a doors exit
//@params
//	int r - current room
//@return
//	Pos - exit position
//@throw
//	if not in room

Pos Door::getExit(int r){
	if(!inRoom(r)){
		throw "Not in room";
	}
	if(dir == 0){
		return getPos(r);
	}
	if(dir == -1 && p2.r == r){
		return getPos(r);
	}
	return (p1.r == r) ? p2 : p1;
}



//AddSwitch - add a switch to the dungeon
//@params
//	Pos p - position of switch

void Dungeon::addSwitch(Pos p){
	s.push_back(Switch(p));
	if(r.size() > p.r){
		r[p.r].s.push_back(s.size() - 1);
	}
}



//AddDoor - add a door to the dungeon
//@params
//	Pos p1 - entrance
//	Pos p2 - exit
//	int swtch - which switch to look at
//	bool inv - whether the door is inverted

void Dungeon::addDoor(Pos p1, Pos p2, int sw, bool inv, int dir){
	d.push_back(Door(p1, p2, sw, inv, dir));
	if(r.size() > p1.r){
		r[p1.r].d.push_back(d.size() - 1);
	}
	if(r.size() > p2.r){
		r[p2.r].d.push_back(d.size() - 1);
	}
}



//AddRoom - adds a room to the dungeon
//@params
//	int w_ - the width of the room
//	int h_ - the height of the room
//	int l_ - the thickness of the left wall
//	int r_ - the thickness of the right wall

void Dungeon::addRoom(int w_, int h_, int l_, int r_){
	Room room(w_, h_, l_, r_);
	for(int i = 0; i < s.size(); i++){
		if(s[i].inRoom(r.size())){
			room.s.push_back(i);
		}
	}
	for(int i = 0; i < d.size(); i++){
		if(d[i].inRoom(r.size())){
			room.d.push_back(i);
		}
	}
	r.push_back(room);
}





//Collide - checks if the players position collides with anything
//@return
//	bool - whether there was a collision
//		 - if player collided with a door, it will return false, but move the player to the exit of the door

bool Dungeon::collide(){
	for(int i = 0; i < r[p.r].s.size(); i++){												//Collide with switch
		if(s[r[p.r].s[i]].p == p){
			s[r[p.r].s[i]].on ^= true;
			return true;
		}
	}
	for(int i = 0; i < r[p.r].d.size(); i++){												//Collide with door
		Door door = d[r[p.r].d[i]];
		if(door.getPos(p.r) == p){
			if(doorState(door)){
				p = door.getExit(p.r);
				return false;
			}
			return true;
		}
	}
	if(p.y <= 0 || p.y >= r[p.r].h - 1 || p.x < r[p.r].l || p.x >= r[p.r].w - r[p.r].r){	//Collide with wall
		return true;
	}
	return false;
}



//Move - moves the player according to key presses
//@return
//	int - commands backward
//		(-1) - exit game

int Dungeon::move(){
	bool change = false;
	Pos op = p;
	if(GetAsyncKeyState(VK_UP)){		//Move up
		p.y--;
		if(collide()){
			p = op;
		}
		change = true;
	}
	op = p;
	if(GetAsyncKeyState(VK_DOWN)){		//Move down
		p.y++;
		if(collide()){
			p = op;
		}
		change = true;
	}
	op = p;
	if(GetAsyncKeyState(VK_LEFT)){		//Move left
		p.x--;
		if(collide()){
			p = op;
		}
		change = true;
	}
	op = p;
	if(GetAsyncKeyState(VK_RIGHT)){		//Move right
		p.x++;
		if(collide()){
			p = op;
		}
		change = true;
	}
	if(GetAsyncKeyState(VK_ESCAPE)){	//End game
		return -1;
	}
	if(p.r == f){
		return 1;
	}
	if(change){
		show();
	}
	Sleep(100);
	return 0;
}



//Show - displays the dungeon at the right position

void Dungeon::show(){
	for(int i = 0; i < 100; i++){															//Clear board
		cout << endl;
	}
	vector<string> dsp;
	for(int i = 0; i < r[p.r].h; i++){
		string row = "";
		for(int j = 0; j < r[p.r].w; j++){
			if(i == 0 || i == r[p.r].h - 1 || j < r[p.r].l || j >= r[p.r].w - r[p.r].r){	//Draw walls
				row.push_back('#');
			}
			else{
				row.push_back(' ');
			}
		}
		dsp.push_back(row);
	}
	for(int i = 0; i < r[p.r].s.size(); i++){												//Draw switches
		Switch sw = s[r[p.r].s[i]];
		if(sw.on){
			dsp[sw.p.y][sw.p.x] = '/';
		}
		else{
			dsp[sw.p.y][sw.p.x] = '\\';
		}
	}
	for(int i = 0; i < r[p.r].d.size(); i++){												//Draw doors
		Door door = d[r[p.r].d[i]];
		if(doorState(door)){
			dsp[door.getPos(p.r).y][door.getPos(p.r).x] = ':';
		}
		else{
			dsp[door.getPos(p.r).y][door.getPos(p.r).x] = '|';
		}
	}
	dsp[p.y][p.x] = '8';																	//Draw player
	for(int i = 0; i < dsp.size(); i++){													//Print
		cout << "\t" << dsp[i] << endl;
	}
	for(int i = 0; i < 3; i++){
		cout << endl;
	}
}







//GenerateDungeon - generates a random dungeon with the given amount of switches
//@params
//	int swtch - amount of switches
//@return
//	Dungeon - generated dungeon

Dungeon generateDungeon(int s){
	Truth tbl(s, s + 1);
	Truth rch(s, s + 1);

	srand(time(NULL));

	int state = 0;
	rch.setOut(s, state, true);
	int n = 1;
	while(n < 1 << s >> 1){													//Generate correct path
		int swtch = rand() % s;
		int nState = state ^ 1 << swtch;
		if(!rch.getOut(s, nState)){
			tbl.setOut(swtch, state, true);
			rch.setOut(swtch, state, true);
			state = nState;
			tbl.setOut(swtch, state, true);
			rch.setOut(swtch, state, true);
			rch.setOut(s, state, true);
			n++;
		}
	}
	tbl.setOut(s, state, true);

	while(true){															//Generate branches
		int pcks = 0;
		for(int i = 0; i < 1 << s; i++){									//Find every possible branch point
			if(rch.getOut(s, i)){
				for(int j = 0; j < s; j++){
					pcks += !rch.getOut(j, i);
				}
			}
		}
		if(pcks == 0){
			break;
		}
		int pck = rand() % pcks;
		int slt = 0;
		for(state = 0; slt <= pck && state < 1 << s; state++){				//Operate on random branch point
			if(rch.getOut(s, state)){
				for(int swtch = 0; slt <= pck && swtch < s; swtch++){
					if(!rch.getOut(swtch, state)){
						if(slt == pck){
							int nState = state ^ 1 << swtch;
							bool pass = !rch.getOut(s, nState);
							tbl.setOut(swtch, state, pass);
							rch.setOut(swtch, state, true);
							state = nState;
							tbl.setOut(swtch, state, pass);
							rch.setOut(swtch, state, true);
							rch.setOut(s, state, true);
						}
						slt++;
					}
				}
			}
		}
	}

	Dungeon rslt;
	rslt.addRoom(s * 3 + 3, s * 2 + 5, s, s);
	rslt.moveTo(Pos(0, s + 1, s + 2));

	for(int i = 0; i <= s; i++){
		BoolExp exp = tbl.getEq(i);
		exp.algebra();
		string smp = exp.getStr();
		if(i == s){
			rslt.setFinish(rslt.getRooms());
		}
		rslt.addRoom(s * 3 + 3, 5, s, s);
		rslt.addSwitch(Pos(rslt.getRooms() - 1, s + 1, 2));
		generateRoom(rslt, Pos(rslt.getRooms() - 1, s * 2 + 2, 2), smp, s);
		rslt.addDoor(Pos(0, s - 1, i * 2 + 2), Pos(rslt.getRooms() - 1, s * 2 + 3, 2), -1);
	}

	for(int i = 0; i < 10000, i++;){
		cout << endl;
	}
	rslt.show();

	return rslt;
}



//GenerateRoom - generates a chain of rooms based off of an equation
//@params
//	Dungeon & d - the dungeon being generated
//	Pos exit - the exit room
//	string exp - the expression
//	int s - amount of switches

void generateRoom(Dungeon & d, Pos exit, string exp, int s){
	if(exp.back() == '&'){
		vector<vector<size_t>> opers = getOperands(exp, true);
		int vars = 0;
		for(int i = opers.size() - 1; i >= 0; i--){
			string a = exp.substr(opers[i][0], opers[i][1]);
			bool inv = (a.back() == '~');
			if(inv){
				a.pop_back();
				a.pop_back();
			}
			if(isVar(a.back())){
				char c = a[0];
				int sw = c - 'a';
				vars |= 1 << sw;
				if(sw == 0){
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), sw, inv, -1);
				}
				else if(sw == s - 1){
					d.addDoor(Pos(exit.r, s * 2 + 3 + sw, exit.y), Pos(d.getRooms(), sw, 2), sw, inv, -1);
				}
				else{
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), sw, inv, 0);
				}
			}
		}
		for(int sw = 0; sw < s; sw++){
			if(!(vars >> sw & 1)){
				if(sw == 0){
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), -1, false, -1);
				}
				else if(sw == s - 1){
					d.addDoor(Pos(exit.r, s * 2 + 3 + sw, exit.y), Pos(d.getRooms(), sw, 2), -1, false, -1);
				}
				else{
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), -1, false, 0);
				}
			}
		}
		d.addRoom(s * 3 + 3, 5, s, s);
	}

	else if(exp.back() == '|'){
		vector<vector<size_t>> opers = getOperands(exp, true);
		int vars = 0;
		int room = d.getRooms();
		d.addDoor(Pos(d.getRooms(), s - 1, 2), Pos(exit.r, s * 2 + 3, exit.y), -1);
		d.addRoom(s * 3 + 3, opers.size() * 2 + 3, s, s);
		vector<int> rooms;
		for(int i = 0; i < opers.size(); i++){
			generateRoom(d, Pos(room, s * 2 + 2, i * 2 + 2), exp.substr(opers[i][0], opers[i][1]), s);
			rooms.push_back(d.getRooms() - 1);
		}
		for(int i = 0; i < rooms.size(); i++){
			d.addDoor(Pos(d.getRooms(), s - 1, i * 2 + 2), Pos(rooms[i], s * 2 + 3, 2), -1);
		}
		d.addRoom(s * 3 + 3, opers.size() * 2 + 3, s, s);
	}

	else{
		string a = exp;
		bool inv = (a.back() == '~');
		if(inv){
			a.pop_back();
			a.pop_back();
		}
		int vars = 0;
		if(isVar(a.back())){
			char c = a.back();
			int sw = c - 'a';
			vars |= 1 << sw;
			if(sw == 0){
				d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), sw, inv, -1);
			}
			else if(sw == s - 1){
				d.addDoor(Pos(exit.r, s * 2 + 3 + sw, exit.y), Pos(d.getRooms(), sw, 2), sw, inv, -1);
			}
			else{
				d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), sw, inv, 0);
			}
		}
		for(int sw = 0; sw < s; sw++){
			if(!(vars >> sw & 1)){
				if(sw == 0){
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), -1, false, -1);
				}
				else if(sw == s - 1){
					d.addDoor(Pos(exit.r, s * 2 + 3 + sw, exit.y), Pos(d.getRooms(), sw, 2), -1, false, -1);
				}
				else{
					d.addDoor(Pos(d.getRooms(), sw, 2), Pos(exit.r, s * 2 + 3 + sw, exit.y), -1, false, 0);
				}
			}
		}
		d.addRoom(s * 3 + 3, 5, s, s);
	}
}