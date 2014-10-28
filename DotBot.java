import java.util.Scanner; 
import java.util.ArrayList;
import java.io.PrintStream; 
import java.util.Random;
import java.lang.Math; 

//AI agent for 2 player dot and boxes game with maximum board size 10*10

//RUN CMD 
//coffee environ.coffee 'coffee naive.coffee' 'java DotBot' -r 5

public class DotBot {

  static char numToDir[] = new char[]{'n', 'e', 's', 'w'}; //map int to direction
  static int w=11, h=11; //size of board
  static PrintStream fout; //file for debugging
  static Random rand = new Random(); 

  static int mapdir(char d) { //map direction to int
    if (d=='n') 
      return 0;
    if (d=='e') 
      return 1;
    if (d=='s') 
      return 2;
    return 3;
  }

  static void outputMove(int x, int y, char d) { //send back to environ.coffee
    System.out.printf("%d %d %c\n",x,y,d);
  }

  static void logData(String msg) { //to a file for debugging
    fout.println(msg);
  }

  static void logMove(String header, int x, int y, char d) { //to a file for debugging
    fout.print(header);
    fout.print(x);
    fout.print(" ");
    fout.print(y);
    fout.print(" ");
    fout.println(d);
  }
  
  static int rand(int n) {
    return rand.nextInt(n);
  }

  static class Box {
    int lines[]; //n = 0, e = 1, s = 2, w = 3
    int numLines; //num of lines drawn in Box
    int winner; //1 if our agent wins box, -1 if other agent wins box

    public Box() {
      lines = new int[4];
    }
  }

  static class Move {
    int x, y;
    char d; 

    public Move(int x, int y, char d) {
      this.x=x;
      this.y=y;
      this.d=d;
    }

    public Move(String x, String y, String dir) {
      this.x=Integer.parseInt(x);
      this.y=Integer.parseInt(y);
      this.d=dir.charAt(0);
    }

    public String toString() {
      return "x:"+x+" y:"+y+" d:"+d;
    }
  }

  static Box[][] cloneBoard(Box[][] board) {
    Box[][] clone=new Box[w][h];

    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++) 
        clone[i][j]=new Box();

    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++) {
        clone[i][j].numLines = board[i][j].numLines; 
        clone[i][j].winner = board[i][j].winner; 
        for(int k = 0; k < 4; k++)
          clone[i][j].lines[k]=board[i][j].lines[k]; 
      }

    return clone;
  }

  static Box[][] createBoard(int w, int h) {
    Box[][] board=new Box[w][h];

    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++) 
        board[i][j]=new Box();

    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++) {
        board[i][j].numLines = 0; 
        board[i][j].winner = 0; 
        for(int k = 0; k < 4; k++)
          board[i][j].lines[k] = 0; 
      }
    return board;
  }

  static void logBoard(Box[][] board) {
    for(int i =0; i < w; i++) {//todo to check if i < w and j < h in every for loop
      for(int j = 0; j < h; j++) {
        fout.print(board[i][j].winner);
        fout.print(" "); 
      }
      fout.println(); 
    }
  }

  static Move[] parseMoves(String line) {
    String[] toks=line.split("| ");
    ArrayList<Move> moves = new ArrayList<Move>();
    /*
       example: 
       1 1 s ==> "1", " ", "1", " ","s"
       1 1 s|2 2 n ==> "1", " ", "1", " ","s","|","2","","2","","n"
    */
    for (int i=0; i+4<toks.length; ) {
      Move move = new Move(toks[i], toks[i+2], toks[i+4]);
      moves.add(move);
      if (i+6<toks.length)
        i+=6;
      else 
        break;
    }
    return moves.toArray(new Move[moves.size()]);
  }
  
  static void drawLine(Box[][] board, int x, int y, int dir, int player) {
    drawLine(board, new Move(x,y,numToDir[dir]), player);
  }

  static void drawLine(Box[][] board, Move move, int player) {
    int x= move.x;
    int y= move.y;
    int dir=mapdir(move.d);
    
    board[x][y].lines[dir] = 1; //mark line in curr box as drawn
    board[x][y].numLines++; 
    if(board[x][y].numLines == 4) board[x][y].winner = player; 

    if(dir == 0 && y-1 >= 0) { //mark line in upper box as drawn
      board[x][y-1].lines[2] = 1; 
      board[x][y-1].numLines++; 
      if(board[x][y-1].numLines == 4) board[x][y-1].winner = player;
    }
    else if(dir == 1 && x+1 < w) { //mark line in right box as drawn
      board[x+1][y].lines[3] = 1; 
      board[x+1][y].numLines++;  
      if(board[x+1][y].numLines == 4) board[x+1][y].winner = player;
    }
    else if(dir == 2 && y+1 < h) { //mark line in lower box as drawn
      board[x][y+1].lines[0] = 1; 
      board[x][y+1].numLines++; 
      if(board[x][y+1].numLines == 4) board[x][y+1].winner = player;
    }
    else if(dir == 3 && x-1 >= 0) { //mark box in left box as drawn
      board[x-1][y].lines[1] = 1;
      board[x-1][y].numLines++; 
      if(board[x-1][y].numLines == 4) board[x-1][y].winner = player;
    }
  }

  static void eraseLine(Box[][] board, int x, int y, int dir) { //todo: need this? if so, erase specific player's line, unmark box as won over
    board[x][y].lines[dir] = 0; //erase line in current box
    board[x][y].numLines--;  

    if(dir == 0 && y-1 >= 0) { //erase line in upper box
      board[x][y-1].lines[2] = 0; 
      board[x][y-1].numLines--; 
    }
    else if(dir == 1 && x+1 < w) { //erase line in right box
      board[x+1][y].lines[3] = 0; 
      board[x+1][y].numLines--;  
    }
    else if(dir == 2 && y+1 < h) { //erase line in lower box
      board[x][y+1].lines[0] = 0; 
      board[x][y+1].numLines--; 
    }
    else if(dir == 3 && x-1 >= 0) { //erase line in left box
      board[x-1][y].lines[1] = 0;
      board[x-1][y].numLines--; 
    }
  }

  //make a random move (in beginning of games)
  static void randomMove(Box[][] board) {
    int x = 0, y = 0; 

    //will only make random move if: 
    //--line hasn't been drawn yet
    //--line won't be the third line in a box (so opponent won't take that box)

    while(true) {
      x = rand(w);
      y = rand(h); 
      if(board[x][y].numLines >= 2) 
        continue;
		
      //try 0 (north)
      if(board[x][y].lines[0] == 0 && !(y-1 >= 0 && board[x][y-1].numLines >= 2)) {
        outputMove(x,y,numToDir[0]);
        drawLine(board, x, y, 0, 1); 
        logMove("random...our move: ",x,y,numToDir[0]);
        return; 
      }
      //try 1 (east)
      else if(board[x][y].lines[1] == 0 && !(x+1 < w && board[x+1][y].numLines >= 2)) {
        outputMove(x,y,numToDir[1]);
        drawLine(board, x, y, 1, 1); 
        logMove("random...our move: ",x,y,numToDir[1]);
        return; 
      }
      //try 2 (south)
      else if(board[x][y].lines[2] == 0 && !(y+1 < h && board[x][y+1].numLines >= 2)) {
        outputMove(x,y,numToDir[2]);
        drawLine(board, x, y, 2, 1); 
        logMove("random...our move: ",x,y,numToDir[2]);
        return; 
      }
      //try 3 (west)
      else if(board[x][y].lines[3] == 0 && !(x-1 >=0 && board[x-1][y].numLines >= 2)) {
        outputMove(x,y,numToDir[3]);
        drawLine(board, x, y, 3, 1); 
        logMove("random...our move: ",x,y,numToDir[3]); 
        return; 
      }
    }
  }

    //make a random move (in end of games)
  static void randomEndMove(Box[][] board) {
    int x = 0, y = 0; 

    //will only make random move if: 
    //--line hasn't been drawn yet

    while(true) {
      x = rand(w);
      y = rand(h); 
    
      //try 0 (north)
      if(board[x][y].lines[0] == 0) {
        outputMove(x,y,numToDir[0]);
        drawLine(board, x, y, 0, 1); 
        logMove("random...our move: ",x,y,numToDir[0]);
        return; 
      }
      //try 1 (east)
      else if(board[x][y].lines[1] == 0) {
        outputMove(x,y,numToDir[1]);
        drawLine(board, x, y, 1, 1); 
        logMove("random...our move: ",x,y,numToDir[1]);
        return; 
      }
      //try 2 (south)
      else if(board[x][y].lines[2] == 0) {
        outputMove(x,y,numToDir[2]);
        drawLine(board, x, y, 2, 1); 
        logMove("random...our move: ",x,y,numToDir[2]);
        return; 
      }
      //try 3 (west)
      else if(board[x][y].lines[3] == 0) {
        outputMove(x,y,numToDir[3]);
        drawLine(board, x, y, 3, 1); 
        logMove("random...our move: ",x,y,numToDir[3]); 
        return; 
      }
    }
  }

  static boolean startSmartPlay(Box[][] board) { //begin minimax near end of game
    int freeBoxes = 0; //num of boxes with 0 or 1 lines 

    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++) 
        if(board[i][j].numLines < 2) freeBoxes++;

    if(freeBoxes <= 10) return true; 
    return false; 
  }

  static boolean isLeaf(Box[][] board) { //check if board is leaf node?
    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++)
        if(board[i][j].numLines < 4) return false;
    return true;  
  }

  static int counter = 0; 

  static int minimax(Box[][] board, int depth, int player, int alpha, int beta) { 
    logData("counter " + counter); 

    if(counter++ >= 1e5) { 
      logData("abandon ship!");
      return -5000; 
    }

    logData("minimax..." + depth + " " + player); 
    if(depth == 0 || isLeaf(board)) { //or node is leaf node!? 
      int numBoxes = 0; 
      for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++) 
          numBoxes += board[i][j].winner; //rough heuristic value

      //logData("heuristic: " + numBoxes);
      return numBoxes; 
    }

    int currVal = 0; 
    if(player == 1) { //our agent's move
      for(int i = 0; i < w; i++) 
        for(int j = 0; j < h; j++) 
          for(int k = 0; k < 4; k++) 
            if(board[i][j].lines[k] == 0) {
              Box[][] clone=cloneBoard(board);
              drawLine(clone, i, j, k, player); 

              /*String indent = ""; 
              for(int t = depth; t < 2; t++)
                indent += "___________";
              logData(indent + "try our move " + i + " " + j + " " + k); 
              logBoard(clone); 
              logData(""); */ 

              currVal = minimax(clone, depth-1, -1, alpha, beta);
              if(currVal == -5000) {
                logData("Got here...");
                return -5000; 
              }
              alpha = Math.max(alpha, currVal); 
              if(alpha > beta) { //find max score if our agent's turn
                return beta; 
              }
            eraseLine(clone, i, j, k); 
          }
      return alpha; 
    }
    else if(player == -1) { //other agent's move
      for(int i = 0; i < w; i++) 
        for(int j = 0; j < h; j++) 
          for(int k = 0; k < 4; k++) 
            if(board[i][j].lines[k] == 0) {
              Box[][] clone=cloneBoard(board);
              drawLine(clone, i, j, k, player); 

              /*String indent = ""; 
              for(int t = depth; t < 2; t++)
                indent += "___________";
              logData(indent + "try their move " + i + " " + j + " " + k);
              logBoard(clone); 
              logData(""); */ 

              currVal = minimax(clone, depth-1, 1, alpha, beta); 
              if(currVal == -5000) {
                logData("got here...");
                return -5000; 
              }
              beta = Math.min(currVal, beta); 
              if(beta < alpha) { //find min score if other agent's turn
                return alpha; 
              }
              eraseLine(clone, i, j, k); 
          }
      return beta; 
    }

    logData("ERROR IN MINIMAX.");
    return -1; //error in minimax
  }

  public static void main(String[] args) throws Exception {
    fout=new PrintStream("moves.txt");
    Scanner sc = new Scanner(System.in);
    w = sc.nextInt();
    h = sc.nextInt();
    sc.nextLine(); //skip line return
    Box[][] board = createBoard(w,h);
    int smartPlay = 0; 

    while (sc.hasNextLine()) {
      String line=sc.nextLine();
      Move[] moves=parseMoves(line);

      for (Move move: moves) {
        drawLine(board, move, -1);  
        logMove("their move: ", move.x, move.y, numToDir[mapdir(move.d)]); 
        //logBoard(board);       
      }
     
      if(smartPlay == 0) { //make a random move in beginning of game
        //logData("MAKE RANDOM MOVES..."); 
        boolean madeMove = false; 
        for(int i = 0; i < w && !madeMove; i++)
          for(int j = 0; j < h && !madeMove; j++)
            if(board[i][j].numLines == 3)  //if there's a box with 3 lines, take the box
              for(int k = 0; k < 4; k++) 
                if(board[i][j].lines[k] == 0) {
                  outputMove(i,j,numToDir[k]); 
                  drawLine(board, i, j, k, 1); 
                  //logBoard(board);    
                  //logMove("complete 4...our move: ",i,j,numToDir[k]);
                  madeMove = true; 
                  break;  
                }
        if(!madeMove) {
          long sTime = System.nanoTime(); 
          randomMove(board); //else if no box with 3 lines, make a random move 
          long eTime = System.nanoTime(); 
          if((eTime-sTime)/1e9 > 2.0) logData("OVERTIME");
        }

        if(startSmartPlay(board)) 
          smartPlay = 1; 
      } else if(smartPlay == 1) { //make a smarter move using minimax
        long startTime = System.nanoTime(); 
        logData("MINIMAX");
        int mx=-1, my=-1, md=-1;
        int maxScore = -1000; 
        int currScore = 0;
        counter = 0; 

        for(int i = 0; i < w && currScore != -5000; i++)
          for(int j = 0; j < h && currScore != -5000; j++)
            for(int k = 0; k < 4 && currScore != -5000; k++) 
              if(board[i][j].lines[k] == 0) {//try this move
                //logData("first try our move " + i + " " + j + " " + k);
                Box[][] clone=cloneBoard(board);
                drawLine(clone, i, j, k, 1); //try line
                currScore = minimax(clone, 2, -1, -1000, 1000); //look 2 levels deep
                if(currScore == -5000) {
                  logData("switch to random moves"); 
                  randomEndMove(board);
                  break;  
                } 
                if(currScore > maxScore) {
                  maxScore = currScore; 
                  mx = i; my = j; md = k; 
                }
              }

        if(currScore != -5000) {
          drawLine(board, mx, my, md, 1); //draw final line on board
          long endTime = System.nanoTime(); 
          outputMove(mx,my,numToDir[md]);
          //logBoard(board);    
          logMove("our move: ",mx,my,numToDir[md]);
          if((endTime-startTime)/1e9 > 2.0) logData("OVERTIME"); 
        }
      }
      
    }
    fout.close();
  }

}