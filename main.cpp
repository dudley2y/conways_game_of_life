#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <future>

using namespace std;

struct board{
	int SIZE; //dimensions of the square
	int **board_data; // a 2d array that contains all the data

	board(int size){ //initalized with a peramter 
		//mallocs the board
		board_data =(int**) malloc(size*sizeof(int*));
		for(int z=0;z<size;z++){
			board_data[z] =(int*) malloc(size*sizeof(int));
		}
		
		//intitalizes all the data to zero
		for(int i=0;i<size;i++){
			for(int x=0;x<size;x++){
				board_data[i][x] = 0;
			}
		}
		//sets internal size state
		this->SIZE = size;
	}

	//displays the content of the data
	void displayData(){
		for(int i=0;i<SIZE;i++){
			for(int x=0;x<2*SIZE+1;x++){
				cout << "-";
			}
			cout << endl;
			for(int z=0;z<SIZE;z++){
				cout << "|";
				if(board_data[i][z] == 1){
					cout << "*";
				}
				else{
					cout << " ";
				}
			}
			cout << "|" <<endl;
		}
		for(int i=0;i<2*SIZE+1;i++){
			cout << "-";
		}
		cout << endl;
	}

	//for a given alive number, generates alive number
	//of points that are going to be alive
	void init(int alive){
		srand(time(NULL)); //initalizes random

		if(alive<1){ //Makes sure the number generated is positive number
			return;
		}

		//interates alive time creating unique pairs
		for(int i=0;i<alive;i++){
			while(true){
				int y = rand() % SIZE;  //random y value
				int x = rand() % SIZE;  //random x value;

				if(board_data[x][y] == 1){
					//the value already exists, so this is a repetition
					//we look for another one
					continue;
				}
				else{
					//sets the value in the array to be alive
					board_data[x][y] = 1;
					break;
				}
			}

		}
	}
	
	//gets the nearby alive (disincluding self) and return the integer answer
	/*
	 *****************************
	 *(x-1,y-1)*(x,y-1)*(x+1,y-1)*
	 *****************************
	 *(x-1, y )*( x, y)*(x+1, y )*
	 *****************************
	 *(x-1,y+1)*(x,y+1)*(x+1,y+1)*
	 *****************************
	 */
	//seraches like ^ 
	int nearBy(int x,int y){
		int count = 0;
		for(int i = x-1; i< x+2;i++){
			for(int z = y-1;z<y+2;z++){
				if(i < 0 || i>=SIZE || z>= SIZE || z<0){
					continue;
				}
				if(!(i==x && z==y)){
					if(board_data[i][z] == 1){
						count++;
						continue;
					}
				}

			}
		}
		return count;
	}

	bool isEmpty(){
		for(int i=0;i<SIZE;i++){
			for(int z=0;z<SIZE;z++){
				if(board_data[i][z] == 1){
					return false;
				}
			}
		}
		return true;
	}

	//this function applies the rules of the game to the board
	/* Rules:
	 * 	1. Any live cell with fewer than two neighbours die, as if by underpopulation
	 * 	2. Any live cell with two or three live neighbors lives on to the next generation
	 * 	3. Any live cell with more than three live neighbors dies, as if by overpopulation
	 * 	4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction
	 */

	void ofLife(){
		board nextRound(SIZE); //new board to copy to

		for(int i=0;i<SIZE;i++){
			for(int z=0;z<SIZE;z++){
				int near = nearBy(i,z);
						
				//Rule 1
				if( near < 2 && board_data[i][z] == 1){
					nextRound.board_data[i][z] = 0;
				}
				//Rule 2
				else if( (near == 2 || near == 3) && board_data[i][z] == 1){
					nextRound.board_data[i][z] = 1;
				}
				//Rule 3
				else if( near > 3 && board_data[i][z] == 1){
					nextRound.board_data[i][z] = 0;
				}
				//Rule 4
				else if( near == 3 && board_data[i][z] == 0){
					nextRound.board_data[i][z] = 1;
				}			
			}
		}
		free(board_data);
		board_data = nextRound.board_data;
	}
	
	//runs the game in a permanent game loop
	void game(int points){
		this->init(points);
		while(true){
			system("clear");
			this->displayData();
			this->ofLife();
			if(isEmpty()){
				cout << "ALL CELLS ARE DEAD!!!!!!!" << endl;
				this_thread::sleep_for(chrono::seconds{5});
				game(points);
			}
			this_thread::sleep_for(chrono::seconds{1});
		}
	}
};

int main(){
	system("clear");
	int size, points;
	cout << "what Size Rectangle: ";
	cin >> size;

	cout << "How many random points: ";
	cin >> points;

	board game(size);
	game.game(points);
	return 0;
}
