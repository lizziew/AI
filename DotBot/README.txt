DotBot

An agent for 2 player dot and boxes game. Minimax with alpha beta pruning.
Maximum board size 10*10.

Instructions to play against DotBot on a 2 by 2 board:
$ javac DotBot.java
$ java DotBot
$ 2 2
$ 0 0 n
$ ... //etc

Instructions to run against another agent in environ.coffee on a 10 by 10 board:
$ coffee environ.coffee 'coffee naive.coffee' 'java DotBot' -r 5 
