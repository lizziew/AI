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
}; 

queue<state> q; 
set<state> visited; 

bool valid_state(state s) {
	return (s.m == 0 || s.m >= s.c)  &&  ((3-s.m)==0 || (3-s.m) >= (3-s.c));
}

int main() {
	state s; 
	s.m = 3, s.c = 3, s.b = 0; 

	q.push(s); 
	while(!q.empty()) {
		printf("\n\n\n");
		state curr_state = q.front(); 
		q.pop();
		
		if(visited.find(curr_state) == visited.end()) { 
			printf("STATE: m-%d c-%d %d\n", curr_state.m, curr_state.c, curr_state.b);

			if(curr_state.m == 0 && curr_state.b == 0 && curr_state.c == 1) {
				printf("FOUND SOLUTION\n");
			} 

	  		visited.insert(curr_state); 

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
	} 

	return 0; 
}