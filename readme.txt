Missionaries and Cannibals (with 3 missionaries, 3 cannibals)

===

mc.cpp is the bfs solution. in terminal, run the commands

g++ mc.cpp -O2 -o mc 
./mc

to print out the solution path.

===

mc_bi.cpp uses bidirectional search. in terminal, run the commands

g++ mc_bi.cpp -O2 -o mc_bi 
./mc_bi

to print out the solution path.

===

output should be

SOLUTION PATH: 
3 3 0 
3 1 1 
3 2 0 
3 0 1 
3 1 0 
1 1 1 
2 2 0 
0 2 1 
0 3 0 
0 1 1 
0 2 0 
0 0 1

the first number is the number of missionaries on the initial side, the second number is the number of cannibals on the initial side, and the third number marks which side the boat is on (0 on initial side, 1 on other side)

3 3 0 is the starting state. 0 0 1 is the goal state.
