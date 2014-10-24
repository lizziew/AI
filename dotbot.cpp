#include <iostream>
#include <cstdio> 
#include <cstdlib> 
#include <fstream> 

using namespace std;

//AI agent for 2 player dot and boxes game with maximum board size 10*10

//RUN CMD 
//coffee environ.coffee 'coffee naive.coffee' './a.out' -r 5

typedef struct Box {
	int x, y; 
	int myBox; //1 if our agent has won this box, 0 otherwise
	int lines[4]; //n = 0, e = 1, s = 2, w = 3
	int numLines; //num of lines in Box
} Box;

int w, h;
char numToDir[4] = {'n', 'e', 's', 'w'}; 
ofstream fout; 

//mark a line as drawn
void drawLine(Box board[][11], int x, int y, int dir) {
	board[x][y].lines[dir] = 1; //mark line in curr box as drawn
	board[x][y].numLines++;  
	fout << "DRAW LINE " << x << " " << y << " " << dir << endl; 

	if(dir == 0 && y-1 >= 0) { //mark line in upper box as drawn
		board[x][y-1].lines[2] = 1; 
		board[x][y-1].numLines++; 
		fout << "DRAW LINE " << x << " " << y-1 << " " << "2" << endl; 
	}
	else if(dir == 1 && x+1 < w) { //mark line in right box as drawn
		board[x+1][y].lines[3] = 1; 
		board[x+1][y].numLines++;  
		fout << "DRAW LINE " << x+1 << " " << y << " " << "3" << endl; 
	}
	else if(dir == 2 && y+1 < h) { //mark line in lower box as drawn
		board[x][y+1].lines[0] = 1; 
		board[x][y+1].numLines++; 
		fout << "DRAW LINE " << x << " " << y+1 << " " << "0" << endl; 	
	}
	else if(dir == 3 && x-1 >= 0) { //mark box in left box as drawn
		board[x-1][y].lines[1] = 1;
		board[x-1][y].numLines++; 
		fout << "DRAW LINE " << x-1 << " " << y << " " << "1" << endl; 
	}
}

//un-mark a line
void eraseLine(Box board[][11], int x, int y, int dir) {
	board[x][y].lines[dir] = 0; //erase line in current box
	board[x][y].numLines--;  
	fout << "ERASE LINE " << x << " " << y << " " << dir << endl; 

	if(dir == 0 && y-1 >= 0) { //erase line in upper box
		board[x][y-1].lines[2] = 0; 
		board[x][y-1].numLines--; 
		fout << "ERASE LINE " << x << " " << y-1 << " " << "2" << endl; 
	}
	else if(dir == 1 && x+1 < w) { //erase line in right box
		board[x+1][y].lines[3] = 0; 
		board[x+1][y].numLines--;  
		fout << "ERASE LINE " << x+1 << " " << y << " " << "3" << endl; 
	}
	else if(dir == 2 && y+1 < h) { //erase line in lower box
		board[x][y+1].lines[0] = 0; 
		board[x][y+1].numLines--; 
		fout << "ERASE LINE " << x << " " << y+1 << " " << "0" << endl; 	
	}
	else if(dir == 3 && x-1 >= 0) { //erase line in left box
		board[x-1][y].lines[1] = 0;
		board[x-1][y].numLines--; 
		fout << "ERASE LINE " << x-1 << " " << y << " " << "1" << endl; 
	}
}

//make a random move (in beginning of games)
void randomMove(Box board[][11]) {
	int x = 0, y = 0; 

	//will only make random move if: 
	//--line hasn't been drawn yet
	//--line won't be the third line in a box (so opponent won't take that box)

	while(1) {
		x = rand() % w;
		y = rand() % h; 

		if(board[x][y].numLines >= 2) 
			continue;

		fout << "has move already been done? " << x << " " << y << endl;
		
		//try 0 (north)
		if(board[x][y].lines[0] == 0 && !(y-1 >= 0 && board[x][y-1].numLines >= 2)) {
			cout << x << " " << y << " " << numToDir[0] << endl; 
			drawLine(board, x, y, 0); 
			fout << "our move: " << x << " " << y << " " << numToDir[0] << endl;   
			return; 
		}
		//try 1 (east)
		else if(board[x][y].lines[1] == 0 && !(x+1 < w && board[x+1][y].numLines >= 2)) {
			cout << x << " " << y << " " << numToDir[1] << endl; 
			drawLine(board, x, y, 1); 
			fout << "our move: " << x << " " << y << " " << numToDir[1] << endl;   
			return; 
		}
		//try 2 (south)
		else if(board[x][y].lines[2] == 0 && !(y+1 < h && board[x][y+1].numLines >= 2)) {
			cout << x << " " << y << " " << numToDir[2] << endl; 
			drawLine(board, x, y, 2); 
			fout << "our move: " << x << " " << y << " " << numToDir[2] << endl;   
			return; 
		}
		//try 3 (west)
		else if(board[x][y].lines[3] == 0 && !(x-1 >=0 && board[x-1][y].numLines >= 2)) {
			cout << x << " " << y << " " << numToDir[3] << endl; 
			drawLine(board, x, y, 3); 
			fout << "our move: " << x << " " << y << " " << numToDir[3] << endl;   
			return; 
		}
	}
}

//begin minimax if there are 5 remaining freeBoxes 
bool startSmartPlay(Box board[][11]) {
	int freeBoxes = 0; //num of boxes with 0 or 1 lines 

	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++) 
			if(board[i][j].numLines < 2) freeBoxes++;

	if(freeBoxes <= 5) return true; 
	return false; 
}

//add a-b pruning
int minimax(Box board[][11], int player) { 
    int x = -1, y = -1, dir = -1; 
    bool foundMove = false; 
    int optScore = -2000*player; //optimal score

    for(int i = 0; i < w; i++) 
    	for(int j = 0; j < h; j++) 
    		for(int k = 0; k < 4; k++) 
    			if(board[i][j].lines[k] == 0) {
    				fout << "draw Line asdfasdfasdfasdfasdfasdfasdf" << endl; 
  					drawLine(board, i, j, k); 
    				int currScore = minimax(board, -1*player); 
    				if(player==-1 && currScore < optScore) { //find min score if other agent's turn
    					optScore = currScore;
    					x = i, y = j, dir = k; 
    					foundMove = true; 
    				}
    				else if(player==1 && currScore > optScore) { //find max score if our agent's turn
    					optScore = currScore; 
    					x = i, y = j, dir = k; 
    					foundMove = true; 
    				}
    				fout << "erase LIne asdfasdfasdfasdfasdfasdfasdf" << endl; 
    				eraseLine(board, i, j, k); 
    			}
    		
    if(!foundMove) {
    	fout << "SOMETHING WENT WRONG WITH MINIMAX. COULD NOT FIND MAXSCORE" << endl; 
    	fout << endl; 
    	fout << endl; 
    	return -1; 
    }
    return optScore;
}

int main() {
	fout.open("moves.txt");
	fout << "Output file" << endl; 

	scanf("%d %d\n", &w, &h); 

	Box board[11][11]; 
	
	for(int i = 0; i < w; i++)
		for(int j = 0; j < h; j++) {
			board[i][j].myBox = 0; 
			board[i][j].numLines = 0; 
			for(int k = 0; k < 4; k++)
				board[i][j].lines[k] = 0; 
		}

	int len = 80;
	char *line = (char*) malloc(len);
	size_t linecap = 0;
	ssize_t linelen; 

	int smartPlay = 0; 

	while((linelen = getline(&line, &linecap, stdin)) > 0) {
		//read in input
		char *head = line;
		char *end = line; 
		int x, y; char dir;
		int last = 0;
		while(*end!=0) {
			if(*end!='|') {
				end++;
				if(*end!=0)
					continue; 
				last = 1; 
			}

			*end = 0; 

			sscanf(head, "%d %d %c", &x, &y, &dir); 
			//printf("read in %d %d %c\n", x, y, dir);
			if(dir == 'n') drawLine(board, x, y, 0); 
			else if(dir == 'e') drawLine(board, x, y, 1); 
			else if(dir == 's') drawLine(board, x, y, 2); 			
			else if(dir == 'w') drawLine(board, x, y, 3); 

			fout << "their move: " << x << " " << y << " " << dir << endl; 
			if(last)
				break;
			head=++end; 
		}

		//make a random move
		if(smartPlay == 0) {
			bool madeMove = false; 
			for(int i = 0; i < w && !madeMove; i++)
				for(int j = 0; j < h && !madeMove; j++)
					if(board[i][j].numLines == 3)  //if there's a box with 3 lines, take the box
						for(int k = 0; k < 4; k++) 
							if(board[i][j].lines[k] == 0) {
								cout << i << " " << j << " " << numToDir[k] << endl; 
								drawLine(board, i, j, k); 
								fout << "our move: " << i << " " << j << " " << numToDir[k] << endl;  
								madeMove = true; 
								break;  
							}
						
			if(!madeMove) randomMove(board); //else if no box with 3 lines, make a random move 

			if(startSmartPlay(board)) 
				smartPlay = 1; 
		}
		else if(smartPlay == 1) { //make a smarter move using minimax
		    int mx=-1, my=-1, md=-1;
		    int maxScore = -2000;

		    for(int i = 0; i < w; i++)
		    	for(int j = 0; j < h; j++)
		   			for(int k = 0; k < 4; k++) 
		   				if(board[i][j].lines[k] == 0) {//try this move
		   					drawLine(board, i, j, k); //try line
		   					int currScore = minimax(board, -1); 
		   					eraseLine(board, i, j, k); //erase line
		   					if(currScore > maxScore) {
		   						maxScore = currScore; 
		   						mx = i, my = j, md = k; 
		   					}
		   				}

			cout << mx << " " << my << " " << numToDir[md] << endl; 
			fout << "our move: " << mx << " " << my << " " << numToDir[md] << endl; 
		}
	}

	fout.close(); 
	return 0; 
}