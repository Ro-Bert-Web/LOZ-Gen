#include "dungeon.h"

int main(){
	int swtchs;
	cout << "How many switches? ";
	cin >> swtchs;
	Dungeon d = generateDungeon(swtchs);
	while(true){
		int rslt = d.move();
		switch(rslt){
			case -1:
			case 1:
				return 0;
				break;
			default:
				break;
		}
	}
}