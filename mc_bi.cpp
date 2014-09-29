#include <cstdio>
#include <queue> 
#include <set> 
#include <iostream> 

using namespace std;

struct state {
	int m; //num of missionaries on left side
	int c; //num of cannibals on left side
	int b; //0 on left side; 1 on right side
	bool operator==(const state &other) const {
		return (m == other.m && c == other.c && b == other.b); 
	}
	bool operator<(const state& other) const {
		if(m < other.m) return true; 
		if(m == other.m && c < other.c) return true; 
		if(m == other.m && c == other.c && b < other.b) return true; 
		return false;
	}
	state& operator=(const state& other) {
		m = other.m; 
		c = other.c;
		b = other.b; 
		return *this; 
	}
}; 

queue<state> q1, q2; 
set<state> visited1, visited2; 

bool valid_state(state s) {
	return (s.m == 0 || s.m >= s.c)  &&  ((3-s.m)==0 || (3-s.m) >= (3-s.c));
}

void gen_children(state curr_state, set<state> visited, queue<state> q) {
	if(curr_state.b == 0) { //boat on initial side
		for(int i = 0; i <= curr_state.m; i++) 
			for(int j = 0; j <= curr_state.c; j++) 
				if(i+j <= 2 && i+j > 0) { //choose people to put in a boat
					state next_state; 
					next_state.m = curr_state.m - i; 
					next_state.c = curr_state.c - j; 
					next_state.b = 1; 
					//printf("POSSIBLE NEXT: m-%d c-%d %d\n", next_state.m, next_state.c, next_state.b);
					if(valid_state(next_state) && visited.find(next_state)==visited.end()) {
						printf("NEXT: m-%d c-%d %d\n", next_state.m, next_state.c, next_state.b);
						q.push(next_state);  
					}
				}
	}
	else if(curr_state.b == 1){ //boat on other side
		for(int i = 0; i <= 3-curr_state.m; i++) 
			for(int j = 0; j <= 3-curr_state.c; j++) 
				if(i+j <= 2 && i+j > 0) { //choose people to put in a boat
					state next_state; 
					next_state.m = curr_state.m + i; 
					next_state.c = curr_state.c + j; 
					next_state.b = 0; 
					//printf("POSSIBLE NEXT: m-%d c-%d %d\n", next_state.m, next_state.c, next_state.b);
					if(valid_state(next_state) && visited.find(next_state)==visited.end()) {
						printf("NEXT: m-%d c-%d %d\n", next_state.m, next_state.c, next_state.b);
						q.push(next_state);  
					}
				}	
	}	
} 

int main() {
	state s; 
	s.m = 3, s.c = 3, s.b = 0; 

	state g; 
	g.m = 0, g.c = 0, g.b = 1; 

	q1.push(s); 
	q2.push(g); 
	while(!q1.empty() || !q2.empty()) {
		printf("\n\n\n");
		state curr_state1, curr_state2; 
		curr_state1.m = -1, curr_state1.c = -1, curr_state1.b = -1; 
		curr_state2.m = -1, curr_state2.c = -1, curr_state2.b = -1; 		 

		if(!q1.empty()) {
			curr_state1 = q1.front(); 			
			q1.pop(); 
		}	
		if(!q2.empty()) {
			curr_state2 = q2.front();
			q2.pop(); 
		}

		printf("STATE 1: m-%d c-%d %d\n", curr_state1.m, curr_state1.c, curr_state1.b);
		printf("STATE 2: m-%d c-%d %d\n", curr_state2.m, curr_state2.c, curr_state2.b);

		printf("forwards.........\n");
		if(visited1.find(curr_state1)==visited1.end()) gen_children(curr_state1, visited1, q1);

		printf("backwards.........\n");
		if(visited2.find(curr_state2)==visited2.end()) gen_children(curr_state2, visited2, q2); 

		visited1.insert(curr_state1);  
		visited2.insert(curr_state2);  

		if(curr_state1 == curr_state2) 
			printf("FOUND SOLUTION\n");	

		if(q1.empty()) printf("didn't save....\n");
	} 

	return 0; 
}