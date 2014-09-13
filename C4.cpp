//*********************************************************
//Shaun Mbateng
//This program is a game of connect fout.
//***********************************************************

#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;

void drawboard(ostream &, char [][7], int, int); // This Function Draws the Board.
int8_t humanmove(ostream & out, bool player, char board[][7], int rows, int columns); //This Function Handles the Human Move
bool verticalwin(char board[][7], int rows, int columns, int moverow, int movecolumn); //Funtion That Checks for Vertical Win
bool horizontalwin(char board[][7], int rows, int columns, int moverow, int movecolumn); //Funtion That Checks for Horizontal Win
bool diagonalwin(char board[][7], int rows, int columns, int moverow, int movecolumn); //Funtion That Checks for Diagonal Win
bool draw(char board[][7], int rows, int columns); //Function That Checks for Draw Game

int main () 
{
	int col; //Player Move
	int nop; //Number of Human Players
	bool player = 1; //Player Number, Initialized to Second Player
	int diff; //Computer Difficulty Level
	char board [6][7] = {0}; //Game Board
	int8_t win = 0; //Whether Player has Won, or Quit, Will be 0, 1, or 2
	bool cfirst; //In 1 Player Game, Pick Who Goes First
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
		cout<<"\nWhat is the difficulty level of the computer(s)?"<<endl;
		
		do //Loop Till Correct Input for Difficulty Level
		{
			cout<<"1=Easy, 2=Medium, 3=Hard: ";
			cin>>diff;
			/*
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
    		cin.clear(); //clear bad input flag
			*/
		}
		while (diff<1 || diff>3);
	}
	
	for (int x = 0; x < 6; x++) //Draw Board, Initialize With "-"
	{
		for (int y = 0; y < 7; y++)
			board[x][y] = '-';
	}
	
	//Display Start Messages
	cout<<"\n*** Game Start! ***"<<endl;
	outfile<<"*** Game Start! ***"<<endl;
	
	while (win==0) //While Game Unfinished
	{
		drawboard(cout, board, 6, 7); //Draw Board on Screen
		drawboard(outfile, board, 6, 7); //Draw Board on File
		player ^= 1; //XOR With 1, Switch Player from 0 to 1, or Vice Versa, Game Will Start at 0
		win = humanmove(outfile, player, board, 6, 7); //Check for Win
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
			break; //End Program
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
void drawboard(ostream & out, char b[][7], int r, int c) 
{
	out << endl;
	
	for (int i=1; i<=c; i++) //Display Column Numbers
		out<<i<<" ";
	out<<"\n"<<endl;	
	for (int x = r-1; x >= 0; x--)  //Output Game Board
	{
		for (int y = 0; y < c; y++)
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

int8_t humanmove(ostream & out, bool player, char board[][7], int rows, int columns)
{
	int col; //Playing Column
	int r = 0; //Playing Row, Initialized to Zero
	char symbol;
	
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
	
	while (board[r][col] != '-' && r<=rows) //Loop Till Open Space in Column Found
		r++;
	
	if (r>rows) //Display if Row is Filled, Select Again
	{
		cout<<"\nThat column is filled.\n"<<endl;
		return humanmove(out, player, board, rows, columns); //Call Function Again
	}

	board[r][col] = symbol; //Play Piece
	cout<<"\nPlayer "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Display Play Message
	out<<"Player "<<(player+1)<<" plays in column "<<(col+1)<<endl; //Outfile Play Message
	
	if (verticalwin(board, rows, columns, r, col)) //Check for Vertical Win
		return 1;
	if (horizontalwin(board, rows, columns, r, col)) //Check for Horizontal Win
		return 1;
	if (diagonalwin(board, rows, columns, r, col)) //Check for Horizontal Win
		return 1;
	if (draw(board, rows, columns)) //Check for Draw Game
		return 2;
	
	return 0;
}

bool verticalwin(char board[][7], int rows, int columns, int moverow, int movecolumn) //Funtion That Checks for Vertical Win
{
	int count = 1; //Hold Count of Consecutive Pieces Vertically
	int r; //To Traverse Array to Check Pieces

	if (moverow>=3) //No Point in Checking if not High Enough
	{
		r = moverow-1; //Intialize Traverse Variable
		
		while (board[moverow][movecolumn] == board[r][movecolumn] && r>=0)
		{
			count++; //Increment Count
			r--; //Check Row Below
		}
		
		if (count>=4) //If Enough Consecutive Pieces
			return true;
	}
	
	return false; //Default, Return False
}

bool horizontalwin(char board[][7], int rows, int columns, int moverow, int movecolumn) //Funtion That Checks for Horizontal Win
{
	int count = 1; //Keep Count of Consecutive Pieces
	int c = movecolumn-1; //To Traverse Array to Check Pieces
	
	while (board[moverow][movecolumn] == board[moverow][c] && c>=0) //Count Consecutive Pieces to the Left
	{
			count++; //Increment Count
			c--; //Check Column Before
	}
	
	c = movecolumn+1; //Reinitialize c
	
	while (board[moverow][movecolumn] == board[moverow][c] && c<columns) //Count Consecutive Pieces to the Right
	{
			count++; //Increment Count
			c++; //Check Column After
	}
		
	if (count>=4) //If Enough Consecutive Pieces
		return true;
	
	return false; //Default Return False
}

bool diagonalwin(char board[][7], int rows, int columns, int moverow, int movecolumn) //Funtion That Checks for Diagonal Win
{
	int count = 1; //Keep Count of Consecutive Pieces
	int r = moverow-1; //To Traverse Rows to Check Pieces
	int c = movecolumn-1; //To Traverse Columns to Check Pieces
	
	while (board[moverow][movecolumn] == board[r][c] && r>=0 && c>=0) //Count Consecutive Pieces to the Down-Left
	{
			count++; //Increment Count
			r--; //Check Row Below
			c--; //Check Column Before
	}
	
	r = moverow+1; //Reinitialize to Row Above
	c = movecolumn+1; //Reinitialize to Column After
	
	while (board[moverow][movecolumn] == board[r][c] && r<rows && c<columns) //Count Consecutive Pieces to the Up-Right
	{
			count++; //Increment Count
			r++; //Check Row Below
			c++; //Check Column Before
	}
	
	if (count>=4) //If Enough Consecutive Pieces
		return true;
	
	return 0;
	
}

bool draw(char board[][7], int rows, int columns) //Function That Checks for Draw Game
{
	int i = 0; //To Traverse Top Row
	
	while (board[rows-1][i] != '-') //Traverse Top Row
		i++;
	if (i>=columns) //If Filled Row, Draw Game
		return true;
	
	return false; //If Not, Return False
}
