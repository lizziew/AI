#include <cstdio>
#include <queue> 
#include <set> 
#include <iostream> 
#include <vector> 

using namespace std;

struct state {
	int m; //num of missionaries on left side
	int c; //num of cannibals on left side
	int b; //0 on left side; 1 on right side
	vector<state> path; //the state path

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

queue<state> q1, q2; 
set<state> visited1, visited2; 

bool valid_state(state s) {
	return (s.m == 0 || s.m >= s.c)  &&  ((3-s.m)==0 || (3-s.m) >= (3-s.c));
}

void genChildren(state &curr_state, set<state> &visited, queue<state> &q) {
	bool on_initial_side = (curr_state.b+1)%2; 
	int m = on_initial_side? curr_state.m : 3-curr_state.m; 
	int c = on_initial_side? curr_state.c : 3-curr_state.c; 

	for(int i = 0; i <= m; i++)
		for(int j = 0; j <= c; j++) 
			if(i+j <= 2 && i+j > 0) { //choose people to put in boat
				state next_state; 
				next_state.m = on_initial_side? curr_state.m-i : curr_state.m+i; 
				next_state.c = on_initial_side? curr_state.c-j : curr_state.c+j;
				next_state.b = on_initial_side; 
				if(valid_state(next_state) && visited.find(next_state)==visited.end()) {
    				//printf("NEXT: m-%d c-%d %d\n", next_state.m, next_state.c, next_state.b);
					next_state.path = curr_state.path; 
					next_state.path.push_back(curr_state);  

					q.push(next_state); 
				}
			}		
} 

int main() {
	int checks = 0; 

	state s; //initial state
	s.m = 3, s.c = 3, s.b = 0; 

	state g; //goal state
	g.m = 0, g.c = 0, g.b = 1; 

	q1.push(s); 
	q2.push(g); 
	while(!q1.empty() || !q2.empty()) {
		state curr_state1, curr_state2; 

		if(!q1.empty()) {
			curr_state1 = q1.front(); 			
			q1.pop(); 
			if(visited1.find(curr_state1)==visited1.end()) 
				genChildren(curr_state1, visited1, q1);
				visited1.insert(curr_state1); 
		}	
		if(!q2.empty()) {
			curr_state2 = q2.front();
			q2.pop(); 
			if(visited2.find(curr_state2)==visited2.end()) 
				genChildren(curr_state2, visited2, q2); 
			visited2.insert(curr_state2); 
		} 

		if(visited1.find(curr_state2)!=visited1.end() || visited2.find(curr_state1)!=visited2.end()) {
			printf("SOLUTION PATH: \n");	
			for(int i = 0; i < curr_state1.path.size(); i++)
				printf("%d %d %d\n", curr_state1.path[i].m, curr_state1.path[i].c, curr_state1.path[i].b);
			for(int i = curr_state2.path.size()-1; i >= 0; i--)
				printf("%d %d %d\n", curr_state2.path[i].m, curr_state2.path[i].c, curr_state2.path[i].b);
			break; 
		}
	} 

	return 0; 
}