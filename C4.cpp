//*********************************************************
//Shaun Mbateng
//This program is a game of Connect Four.
//*********************************************************

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

void drawboard(ostream &, char [][7], int8_t, int8_t); // This Function Draws the Board.
int8_t humanmove(ostream & out, bool player, char board[][7], int8_t rows, int8_t columns); //This Function Handles the Human Move
int8_t compmove(ostream & out, int8_t diff, bool player, char board[][7], int8_t rows, int8_t columns); //This Function Handles the Dumb Computer Move
bool verticalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn); //Function That Checks for Vertical Win
bool horizontalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn); //Function That Checks for Horizontal Win
bool diagonalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn); //Function That Checks for Diagonal Win
bool draw(char board[][7], int8_t rows, int8_t columns); //Function That Checks for Draw Game
int8_t checkboard(char symbol, char board[][7], int8_t rows, int8_t columns); //Function for Computer to Check for Win or Loss
bool * checkmoves(char symbol, char board[][7],int8_t rows, int8_t columns); //Function to Check if Computer Move Will Allow Human to Win


int main () 
{
	int nop; //Number of Human Players
	int8_t diff; //Computer Difficulty Level, Will be 1, 2, or 3
	int8_t win = 0; //Whether Player has Won, or Quit, Will be 0, 1, or 2 When Returned
	bool player = 1; //Player Number, Initialized to Second Player
	bool first; //In 1 Human Player Game, Pick Who Goes First
	char board [6][7] = {0}; //Game Board
	ofstream outfile; //File to Print Results to

	outfile.open("GameLog.txt"); //Open GameLog
	
	//Menu of Game
	cout<<"Welcome to the Connect Four Game"<<endl;
	cout<<"Made by Shaun Mbateng\n"<<endl;
	cout<<"How many players will there be?"<<endl;
	
	//Loop Till Correct Input for Number of Players
	do
	{
		cout<<"Enter 0, 1, or 2: ";
		cin>>nop;
		/*
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
    	cin.clear(); //clear bad input flag
		*/
	}
	while (nop<0 || nop>2);
	
	if (nop<2) //If at Least One Computer Player
	{
		srand(time(NULL));	//Will Need Random Number Generator
		int btmp; //Temp Variable to Handle Human Error in Inputs
		cout<<"\nWhat is the difficulty level of the computer(s)?"<<endl;
		
		do //Loop Till Correct Input for Difficulty Level
		{
			cout<<"1=Easy, 2=Medium, 3=Hard: ";
			cin>>btmp;
			/*
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
    		cin.clear(); //clear bad input flag
			*/
		}
		while (btmp<1 || btmp>3);
		
		diff = btmp; //Set Difficulty Level
		
		if (nop==1) //One Human, One Computer
		{
			int ltmp; //Temp Variable to Handle Human Error in Inputs
			cout<<"\nDo you want to go first?"<<endl;
			
			do
			{
				cout<<"Enter 0 for no, 1 for yes: ";
				cin>>ltmp;
			}
			while (ltmp<0 || ltmp>1);
			
			first = ltmp; //Pick who Goes First
		}
	}
	
	for (int8_t x = 0; x < 6; x++) //Draw Board, Initialize With "-"
	{
		for (int8_t y = 0; y < 7; y++)
			board[x][y] = '-';
	}
	
	//Display Start Messages
	cout<<"\n*** Game Start! ***"<<endl;
	outfile<<"*** Game Start! ***"<<endl;
	
	while (win==0) //While Game Unfinished
	{
		drawboard(cout, board, 6, 7); //Draw Board on Screen
		drawboard(outfile, board, 6, 7); //Draw Board on File
		player ^= 1; //XOR With 1, Switch Player from 0 to 1, or Vice Versa, Game Will Start at First Player
		
		//Call Move Function Depending on Number of Human Players
		switch (nop)
		{
			case 0: //No Humans
				win = compmove(outfile, diff, player, board, 6, 7); //Check for Win
				break;
			case 1: //One Human
				if (player==first) //Booleans Match, Computer's Turn
					win = compmove(outfile, diff, player, board, 6, 7); //Check for Win
				else //Boolean's Don't Match, Human's Turn
					win = humanmove(outfile, player, board, 6, 7); //Check for Win
				break;
			case 2: //Both Human
				win = humanmove(outfile, player, board, 6, 7); //Check for Win
				break;
			default: //Shouldn't Happen, but Just in Case
				win = -1;
				break;
		}
	}
	
	//Print Board Last Time
	drawboard(cout, board,6, 7);
	drawboard(outfile, board, 6, 7);
	
	//Display Message Depending on Game Result
	switch (win)
	{
		case -1: //Player Chose to Quit
			cout<<"\nGame Quit\n"<<endl;
			outfile<<"\nGame Quit\n"<<endl;
			break;
		case 1: //Game was Won
			cout<<"\nPlayer "<<(player+1)<<" wins!"<<endl;
			outfile<<"\nPlayer "<<(player+1)<<" wins!"<<endl;
			break;
		case 2: //Draw Game
			cout<<"\nDraw Game."<<endl;
			outfile<<"\nDraw Game."<<endl;
			break;
		default: //Shouldn't Happen, but Just in Case
			break;
	}
	
	outfile.close(); //Close File
	
	return 0;
}

// This function prints the board to the indicated ostream object.
// The values in the board are 'X' for player 1, 'O' (the letter) for player 2, and 
// the ASCII value of 0 (zero) for an empty space.
void drawboard(ostream & out, char b[][7], int8_t r, int8_t c) 
{
	out << endl;
	
	for (int8_t i=1; i<=c; i++) //Display Column Numbers
		out<<(int) i<<" "; //Display as Integers
	out<<"\n"<<endl;	
	for (int8_t x = r-1; x >= 0; x--)  //Output Game Board
	{
		for (int8_t y = 0; y < c; y++)
		{
			if (b[x][y] == 0)
				out << "  ";
			else
				out << b[x][y] << " ";
		}
		
		out << endl;
	}
	out << endl << endl;
} // end drawboard function

int8_t humanmove(ostream & out, bool player, char board[][7], int8_t rows, int8_t columns) //Function to Handle Human Move
{
	int col; //Playing Column
	int8_t r = 0; //Playing Row, Initialized to Zero
	char symbol; //Playing Symbol
	
	if (player== 0) //First Player is X
		symbol = 'X';
	else //Second is O
		symbol = 'O';
		
	cout<<"What column does player "<<(player+1)<<" want to play?"<<endl;
	
	do //Loop Till Valid Input
	{
		cout<<"(1-7, or 0 to Quit)"<<endl;
		cin>>col;
	}
	while (col<0 || col>7);
	
	if (col==0) //Return If Player Chose to Quit
		return -1;
	
	col-= 1; //To Adjust for Zero-Indexing
	
	if (board[rows-1][col] != '-') //Display if Row is Filled, Select Again
	{
		cout<<"\nThat column is filled.\n"<<endl;
		return humanmove(out, player, board, rows, columns); //Call Function Again
	}
	
	while (board[r][col] != '-' && r<rows) //Loop Till Open Space in Column Found
		r++;

	board[r][col] = symbol; //Play Piece
	cout<<"\nPlayer "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Display Play Message
	out<<"Player "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Outfile Play Message
	
	if //Check for Game Win
	(
		verticalwin(board, rows, columns, r, col) || 
		horizontalwin(board, rows, columns, r, col) || 
		diagonalwin(board, rows, columns, r, col)
	)
		return 1;
	if (draw(board, rows, columns)) //Check for Draw Game
		return 2;
	
	return 0;
}

int8_t compmove(ostream & out, int8_t diff, bool player, char board[][7], int8_t rows, int8_t columns) //This Function Handles the Dumb Computer Move
{
	int8_t col = -1; //Playing Column, Initialized to -1
	int8_t r = 0; //Playing Row, Initialized to Zero
	bool * moves; //Pointer to Hold Potential Computer Loss Columns
	char symbol; //Playing Symbol
	
	if (player== 0) //First Player is X
		symbol = 'X';
	else //Second is O
		symbol = 'O';
	
	if (diff>1) //Difficulty Level is More Than 1
	{
		if (diff==3 && board[0][3]=='-') //Max Difficulty
			col = 3; //Play Center Column If Bottom Space Open, Necessary to Assure Victory
		if (col==-1) //Space Already Filled
			col = checkboard(symbol, board, rows, columns); //Check If Computer can Win
		if (col==-1) //No Winning Move for Computer
		{
			char tmp = 'X'; //Temporary Variable for Symbol
			
			if (player==0) //Set to Opposite Player Symbol
				tmp = 'O';
				
			col = checkboard(tmp, board, rows, columns); //Check If Human can Win
		}
		if (col==-1 && diff==3) //Max Difficulty, Move Not Decided Yet
		{
			moves = checkmoves(symbol, board, rows, columns); //Check If Any Move Will Setup Human Win
			
			do //Choose Random Number, but Make Sure it Won't Result in Human Win
				col=rand()%6;
			while (board[rows-1][col] != '-' && moves[col]==1);
		}
	}
	if (col==-1)
	{
		//Create Random Number from 0-6, Loop Till Acceptable Input
		do
			col=rand()%6;
		while (board[rows-1][col] != '-');
	}
	
	while (board[r][col] != '-' && r<rows) //Loop Till Open Space in Column Found
		r++;
	
	board[r][col] = symbol; //Play Piece
	cout<<"\nComputer Player "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Display Play Message
	out<<"Computer Player "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Outfile Play Message
	
	if //Check for Game Win
	(
		verticalwin(board, rows, columns, r, col) || 
		horizontalwin(board, rows, columns, r, col) || 
		diagonalwin(board, rows, columns, r, col)
	)
		return 1;
	if (draw(board, rows, columns)) //Check for Draw Game
		return 2;
	
	return 0;
}

bool verticalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn) //Funtion That Checks for Vertical Win
{
	int8_t count = 1; //Hold Count of Consecutive Pieces Vertically
	int8_t r; //To Traverse Array to Check Pieces

	if (moverow>=3) //No Point in Checking if not High Enough
	{
		r = moverow-1; //Intialize Traverse Variable
		
		while (board[moverow][movecolumn] == board[r][movecolumn] && r>=0 && count<5) //Loop Till Not Matching or Enough Pieces
		{
			count++; //Increment Count
			r--; //Check Row Below
		}
		
		if (count>=4) //If Enough Consecutive Pieces
			return true;
	}
	
	return false; //Default, Return False
}

bool horizontalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn) //Funtion That Checks for Horizontal Win
{
	int8_t count = 1; //Keep Count of Consecutive Pieces
	int8_t c = movecolumn-1; //To Traverse Array to Check Pieces
	
	while (board[moverow][movecolumn] == board[moverow][c] && c>=0 && count<5) //Count Consecutive Pieces to the Left Till Not Matching or Enough Pieces
	{
			count++; //Increment Count
			c--; //Check Column Before
	}
	
	c = movecolumn+1; //Reinitialize c
	
	while (board[moverow][movecolumn] == board[moverow][c] && c<columns && count<5) //Count Consecutive Pieces to the Right Till Not Matching or Enough Pieces
	{
			count++; //Increment Count
			c++; //Check Column After
	}
		
	if (count>=4) //If Enough Consecutive Pieces
		return true;
	
	return false; //Default Return False
}

bool diagonalwin(char board[][7], int8_t rows, int8_t columns, int8_t moverow, int8_t movecolumn) //Funtion That Checks for Diagonal Win
{
	int8_t count = 1; //Keep Count of Consecutive Pieces
	int8_t r = moverow-1; //To Traverse Rows to Check Pieces
	int8_t c = movecolumn-1; //To Traverse Columns to Check Pieces
	
	while (board[moverow][movecolumn] == board[r][c] && r>=0 && c>=0 && count<5) //Count Consecutive Pieces to the Down-Left
	{
			count++; //Increment Count
			r--; //Check Row Below
			c--; //Check Column Before
	}
	
	r = moverow+1; //Reinitialize to Row Above
	c = movecolumn+1; //Reinitialize to Column After
	
	while (board[moverow][movecolumn] == board[r][c] && r<rows && c<columns && count<5) //Count Consecutive Pieces to the Up-Right
	{
			count++; //Increment Count
			r++; //Check Row Below
			c++; //Check Column Before
	}
	
	if (count>=4) //If Enough Consecutive Pieces
		return true;
	
	count = 1; //Reset Count
	r = moverow+1; //Reinitialize to Row Above
	c = movecolumn-1; //Reinitialize to Column Before
	
	while (board[moverow][movecolumn] == board[r][c] && r<rows && c>=0 && count<5) //Count Consecutive Pieces to the Up-Left
	{
			count++; //Increment Count
			r++; //Check Row Below
			c--; //Check Column Before
	}
	
	r = moverow-1; //Reinitialize to Row Below 
	c = movecolumn+1; //Reinitialize to Column After
	
	while (board[moverow][movecolumn] == board[r][c] && r>=0 && c<columns && count<5) //Count Consecutive Pieces to the Down-Right
	{
			count++; //Increment Count
			r--; //Check Row Below
			c++; //Check Column Before
	}
	
	if (count>=4) //If Enough Consecutive Pieces
		return true;
	
	return 0;
	
}

bool draw(char board[][7], int8_t rows, int8_t columns) //Function That Checks for Draw Game
{
	int8_t i = 0; //To Traverse Top Row
	
	while (board[rows-1][i] != '-') //Traverse Top Row
		i++;
	if (i>=columns) //If Filled Row, Draw Game
		return true;
	
	return false; //If Not, Return False
}

int8_t checkboard(char symbol, char board[][7], int8_t rows, int8_t columns) //Function for Computer to Check for Win or Loss
{
	int8_t r; //Playing Row
	int8_t c = 0; //Playing Column
	
	for (c; c<columns; c++) //Check Each Column
	{	
		if (board[rows-1][c] !='-') //Column is Filled, No Point in Checking
			continue;
		
		r = 0; //Initialize Row
		
		while (board[r][c] != '-' && r<rows) //Loop Till Open Space in Column Found
			r++;
		
		board[r][c] = symbol; //Place Piece
		
		if //Check for Game Win
		(
			verticalwin(board, rows, columns, r, c) || 
			horizontalwin(board, rows, columns, r, c) || 
			diagonalwin(board, rows, columns, r, c)
		)
		{
			board[r][c] = '-'; //Empty Spot
			return c; //Return Column Number
		}
		
		board[r][c] = '-'; //Empty Spot
	}
	
	return -1; //Return -1 on Default
}

bool * checkmoves(char symbol, char board[][7], int8_t rows, int8_t columns) //Function to Check if Computer Move Will Allow Human to Win
{
	///*static*/ bool moves[columns]; //Keep Track of Columns, Results in Size of moves "Not Being Constant"
	static bool moves[7]; //Keep Track of Columns
	char tmp = 'X'; //Human Player Symbol
	int8_t movsum = 0; //Keep Sum of Array
	int8_t r; //Playing Row
	int8_t c = 0; //Playing Column
	
	for (c; c<columns; c++) //Check Each Column
	{	
		if (board[rows-2][c] !='-') //Column Has Only Top Spot or Filled, No Point in Checking
		{
			if (board[rows-1][c] !='-') //If Column Filled
				moves[c] = 1; //Still Mark as Not Playable
				
			continue;
		}
		r = 0; //Initialize Row
		
		while (board[r][c] != '-' && r<rows-1) //Loop Till Open Space in Column Found
			r++;
		
		board[r][c] = symbol; //Place Piece
		
		if (symbol=='X') //If Computer was Player 1, Switch Symbol
			tmp = 'O';
			
		board[++r][c] = tmp; //Place Opposite Piece in Row Above
		
		if //Check for Game Win, No Need To Check Vertical Win Since Impossible
		(
			horizontalwin(board, rows, columns, r, c) || 
			diagonalwin(board, rows, columns, r, c)
		)
			moves[c] = 1;

		movsum += moves[c];
		board[r][c] = '-';
		board[r-1][c] = '-';
	}
	
	if (movsum>=columns) //If Any Move Results in Human Win
		fill_n(moves, 7, 0); //Reset moves, Doesn't Matter
		
	for (int i=0; i<columns; i++)
		cout<<moves[i]<<" ";
	cout<<endl;
	
	return moves;
}
