//Megan Fanning
//Board implimentation for Conways Game of Life
//1/13/2015

using std::cout;
using std::endl;
using std::vector;

//variables
const int HEIGHT=22;//height of the array
const int WIDTH=80;//width of board
bool init_value=false;
const static char DEAD ='.';
const static char LIVE ='8';

vector< vector< bool > > board ( WIDTH, vector<bool> ( HEIGHT, false ) );
vector <vector<bool> > futureBoard (WIDTH, vector <bool> (HEIGHT, false ));

/*********************************************************************
                         random seed
**********************************************************************/
void randomSeed(){
    for(int i=0;i<12;i++){
        srand (time(NULL));
        board[[randomnum=rand() % 21 + 1]=][randomnum=rand() % 79 + 1]=true;
    }
}

/********************************************************************
                    glider seed
************************************************************************/
void gliderSeed(){   
    board[12][12]=true;
    board[12][13]=true;
    board[13][12]=true;
    board[13][14]=true;
    board[14][12]=true;
}
/*************************************************************************
                 rake seed
**************************************************************************/
void rakeSeed(){
    board[10][10]=true;
    board[8][10]=true;
    board[7][11]=true;
    board[7][12]=true;
    board[7][13]=true;
    board[7][14]=true;
    board[10][13]=true;
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
        //cout<<i; //prints rows OPTIONAL feature
        for (int x=0;x<WIDTH;x++){
            if (board[x][y]){
                cout<<countNeighbors(x,y);//Live cells
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
            if (neighbors > 3){
                futureBoard[i][j]=false;
            }

            //cell birth
            if (neighbors==3 && !(board[i][j]) ){
                futureBoard[i][j]=true;
            }
            //if cell is in status copy to future array
            if( neighbors ==2) {
                futureBoard[i][j]= board[i][j];
            }
        }
    }
    board.swap(futureBoard);//swap current and future board
}
