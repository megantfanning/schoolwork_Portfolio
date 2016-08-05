#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

//Protoyypes
void printBoard();
int countNeighbors(int rows,int columns);
void lifeCycle();
void startGame(int time, int lifecycles);
void gliderSeed(int x, int y);

//variables
const int HEIGHT=22;//height of the visible array
const int WIDTH=80;//width of visible board
const int GHOSTY=88;
const int GHOSTX=30;
bool init_value=false;
const static char DEAD ='.';
const static char LIVE ='8';

//create vectors
vector< vector< bool > > board ( 30, vector<bool> ( 88, false ) );
vector <vector<bool> > futureBoard (30, vector <bool> (88, false ));

/**************************************************************
                    glider seed
*********************************************************/
//void gliderSeed(int x, int y{ 
int main(){
    int x=0;
    int y=0;
  
    board[12+x][12+y]=true;
    board[12+x][13+y]=true;
    board[13+x][12+y]=true;
    board[13+x][14+y]=true;
    board[14+x][12+y]=true;

    printBoard();
    return 0;
}

/******************************************
*           print board
* accepts no arguments, 
* prints board
*******************************************/
void printBoard(){
 
    system("clear");//clears screen
    for (int y=0;y<GHOSTX;y++){
        for (int x=0;x<GHOSTY;x++){
            if (board[x][y]){
                cout<<LIVE;
            }else{
                cout<<DEAD;
            }
            //cout<<x<<","<<y<<".";//TEST
        }
    }
    cout<<endl;
}

/****************************************************
*         life cycle function
* tells cells to live die or stay in status
****************************************************
void lifeCycle(){
    //note function starts at 1 and ends with one less than size 
    //to ensure that it doesn't access out of bounds cells
    
    for (int j=1; j<(board.size()-1); j++){
        for(int i=1;i< (board.size()-1); i++){

            //find out the number of neighbors
            int neighbors=countNeighbors(i,j);
        
  // delete everything in ghost cells 1 out from board
            //leftside columns
            for (int b=0; b<HEIGHT+4; b++){
                futureBoard[0][b]=false;
            }
            //rightside column
            for (int b=0; b<HEIGHT+4; b++){
                futureBoard[WIDTH+4][b]=false;
            }
            //bottom row
            for (int a=0; a<WIDTH+4; a++){
                futureBoard[a][0]=false;
            }
            //top row
            for (int a=0; a<WIDTH+4; a++){
                futureBoard[a][HEIGHT+4]=false;
            }
// FIXME
            //kill cells
            if (neighbors <= 1 ){
                futureBoard[i][j]=false;
            } 
            else if (neighbors > 3){
                futureBoard[i][j]=false;
            } //stasis cases
            else if( neighbors ==2) {
                futureBoard[i][j]= board[i][j];
            } 
            else if (neighbors ==3){
                if(board[i][j]){                
                    futureBoard[i][j]= board[i][j];
                }
                else {//cell birth
                futureBoard[i][j]=true;
                }
            }               
        }
    }
    board.swap(futureBoard);//swap current and future board
}*/

