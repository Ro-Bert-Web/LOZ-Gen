#ifndef Dungeon_H
#define Dungeon_H

#include <windows.h>
#include "boolean.h"
#include <iostream>
#include <stdlib.h>
#include "truth.h"
#include <string>
#include <vector>
#include <time.h>
using namespace std;

struct Pos{
	int r;
	int x;
	int y;
	Pos(int r_ = 0, int x_ = 0, int y_ = 0) : r(r_), x(x_), y(y_) {}
	bool operator==(Pos b){return r == b.r && x == b.x && y == b.y;}
	bool operator!=(Pos b){return !(*this == b);}
};

struct Switch{
	Pos p;
	bool on = false;
	Switch(Pos p_) : p(p_) {}
	bool inRoom(int r){return p.r == r;}
};

struct Door{
	Pos p1;
	Pos p2;
	int s;
	bool inv;
	int dir;
	Door(Pos p1_, Pos p2_, int sw, bool inv_ = false, int dir_ = 1) : p1(p1_), p2(p2_), s(sw), inv(inv_), dir(dir_) {}
	bool inRoom(int r){return p1.r == r || p2.r == r;}
	Pos getPos(int r);
	Pos getExit(int r);
};

struct Room{
	int w;
	int h;
	int l;
	int r;
	vector<int> s;
	vector<int> d;
	Room(int w_, int h_, int l_, int r_) : w(w_), h(h_), l(l_), r(r_) {}
};

class Dungeon{
private:
	Pos p;
	int f;
	vector<Switch> s;
	vector<Door> d;
	vector<Room> r;
public:
	Dungeon(){}
	void addSwitch(Pos p);
	void addDoor(Pos p1, Pos p2, int sw, bool inv = false, int dir = 1);
	void addDoor(Pos p, int sw, bool inv = false, int dir = 1){addDoor(p, p, sw, inv, dir);}
	void addRoom(int w_, int h_, int l_, int r_);
	void setFinish(int f_){f = f_;}

	bool doorState(Door door){return (door.s == -1) || (s[door.s].on != door.inv);}
	int getSwitches(){return s.size();}
	int getDoors(){return d.size();}
	int getRooms(){return r.size();}

	bool collide();
	void moveTo(Pos np){p = np;}
	int move();
	void show();
};

void generateRoom(Dungeon & d, Pos exit, string exp, int s);
Dungeon generateDungeon(int s);

#endif