//Megan Fanning
//Implimentation of Conway's Game of Life
//1/13/2015

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <stdlib.h> 

//Protoyypes
void printBoard();
int countNeighbors(int rows,int columns);
void lifeCycle();
void randomSeed();
void startGame(int time, int lifecycles);
void gliderSeed(int x, int y);
void gliderGunSeed(int x, int y);
void blinkerSeed(int x, int y);
int moveShapeX();
int moveShapeY();

void startGame(int time);
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::getline;

//variables
const int HEIGHT=22;//height of the array
const int WIDTH=80;//width of board
bool init_value=false;
const static char DEAD ='.';
const static char LIVE ='8';

//create vectors
vector< vector< bool > > board ( WIDTH, vector<bool> ( HEIGHT, false ) );
vector <vector<bool> > futureBoard (WIDTH, vector <bool> (HEIGHT, false ));

/************************************************************
*               main function 
* contains menu for user
***************************************************************/
int main(){

    int userChoice=0;
    std::string confirmation;
    
        cout<<endl;
        cout<<"           Menu"<<endl;
        cout<<"Welcome to Conway's Game of Life"<<endl;
        cout<<"Would you like to play?"<<endl;
        getline(cin,confirmation);

        while (confirmation.at(0)!='n'){
            int time;
            int lifecycles;
            cout<<"How many milliseconds would you like the board displayed each lifecycle?"<<endl;
            cin>>time;
            cout<<"How many lifecycles would you like the game to display?"<<endl;
            cin>>lifecycles;


            cout<<"Choose a starting design"<<endl;
            cout<<"(1) Blinker"<<endl;
            cout<<"(2) Glider"<<endl;
            cout<<"(3) Glider Gun"<<endl;
            cout<<"enter the number of your choice:"<<endl;
            cin>>userChoice;
            
            
            switch (userChoice){
            case 1:
                blinkerSeed(moveShapeX(),moveShapeY());
                startGame(time, lifecycles);
                break;
            case 2:
                gliderSeed(moveShapeX(),moveShapeY());
                startGame(time, lifecycles);
                break;
            case 3:
                gliderGunSeed(moveShapeX(),moveShapeY());
                startGame(time, lifecycles);
                break;
            }
            //need code for bad input
        }
    return 0;
}
/************************************************************************
*                start new game
* runs the game until the user tells the game to stop
************************************************************************/
void startGame(int time, int lifecycles){
      
    for (int i=1; i<lifecycles; i++){
        printBoard();
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        lifeCycle();
    }
}
/********************************************************************
*                      move seed
* this function accepts user input of how many spaces 
* to shift the design on the X axis 
********************************************************************/
int moveShapeX(){  
    std::string confirm;
    int x=7;

    cout<<"All designs start in the bottom left corner. ";
    
    while (x<0 ||x>5){
        cin.ignore();
        cout<<"how many spaces up would you like to move the design right? (choose 0-5)"<<endl; 
        cin>>x;      
    }                
    return x;
}

/*****************************************************************
*              move shape Y
* this function accepts user input of how many spaces 
* to shift the design on the Y axis 
*****************************************************************/
int moveShapeY(){  
    std::string confirm;
    int y=7;

    while (y<0 || y>5){   
        cin.ignore();     
        cout<<"how many spaces like to move the design up? (choose 0-5)"<<endl;               
        cin>>y;
    }
    return y;
}

/********************************************
*       count Neighbors function
* finds the number of neighbors a cell has
*********************************************/
int countNeighbors(int x, int y){
    int neighbors=0;

    for(int i=-1;i<=1;i++){
        for(int j=-1;j<=1;j++){
            if (board[x+i][y+j]) {
                neighbors++;
            }
        }
    }

    if (board [x][y]){
        neighbors--;//doesn't allow cell to count itself if live.
    }

    return neighbors;
}
/******************************************
*           print board
* accepts no arguments, 
* prints board
*******************************************/
void printBoard(){
 
    system("clear");//clears screen

    for (int y=0;y<HEIGHT;y++){
        for (int x=0;x<WIDTH;x++){
            if (board[x][y]){
                cout<<LIVE;
            }else{
                cout<<DEAD;
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

/****************************************************
*         life cycle function
* tells cells to live die or stay in status
****************************************************/
void lifeCycle(){
    //note function starts at 1 and ends with one less than size 
    //to ensure that it doesn't access out of bounds cells
    
    for (int j=1; j<(board.size()-1); j++){
        for(int i=1;i< (board.size()-1); i++){

            //find out the number of neighbors
            int neighbors=countNeighbors(i,j);
        
            //kill cells that are lonely or crowded
            
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
}

/********************************************************************
                    glider seed
************************************************************************/
void gliderSeed(int x, int y){   
    board[12+x][12+y]=true;
    board[12+x][13+y]=true;
    board[13+x][12+y]=true;
    board[13+x][14+y]=true;
    board[14+x][12+y]=true;
}

/***********************************************************************
*         blinker seed
*************************************************************************/
void blinkerSeed(int x,int y){
    board[12+x][12+y]=true;
    board[12+x][13+y]=true;
    board[13+x][12+y]=true;
    board[13+x][13+y]=true;
    board[14+x][14+y]=true;
    board[14+x][15+y]=true;
    board[15+x][14+y]=true;
    board[15+x][15+y]=true;

}
/*************************************************************************
                 Glider Gun seed
**************************************************************************/
void gliderGunSeed(int x, int y){
    board[9+x][5+y] = true;
    board[9+x][6+y] = true;
    board[10+x][5+y] = true;
    board[10+x][6+y] = true;

    board[19+x][5+y] = true;
    board[19+x][6+y] = true;
    board[19+x][7+y] = true;

    board[20+x][4+y] = true;
    board[20+x][8+y] = true;

    board[21+x][3+y] = true;
    board[21+x][9+y] = true;
    board[22+x][3+y] = true;
    board[22+x][9+y] = true;

    board[23+x][6+y] = true;

    board[24+x][4+y] = true;
    board[24+x][8+y] = true;

    board[25+x][5+y] = true;
    board[25+x][6+y] = true;
    board[25+x][7+y] = true;

    board[26+x][6+y] = true;

    board[29+x][3+y] = true;
    board[30+x][3+y] = true;
    board[29+x][4+y] = true;
    board[30+x][4+y] = true;
    board[29+x][5+y] = true;
    board[30+x][5+y] = true;

    board[31+x][2+y] = true;
    board[31+x][6+y] = true;

    board[33+x][2+y] = true;
    board[33+x][6+y] = true;
    board[33+x][1+y] = true;
    board[33+x][7+y] = true;

    board[43+x][3+y] = true;
    board[44+x][3+y] = true;
    board[43+x][4+y] = true;
    board[44+x][4+y] = true;
}

