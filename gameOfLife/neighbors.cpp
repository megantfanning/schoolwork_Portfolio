//Megan Fanning

if cell is live
call check neighbors
/*************************************
 check neighbors
***************************************/
checkNeighbors(){
    int neighbors=0;
  //  coordinates: row, column

    for(i=-1;i<1;i++){
        if (board[(row+i),(column)]) {
            +=neighbors;
        }
    }
    cout<<"current neighbors"<<neighbors;
}
/**************************/

    if(row, column+1, column-1)
        +=neighbors;

    if (neighbors<=1|| neighbors>3){
        row,column =false;
        //futureboard.pushback;
    }else if (neighbors=3){
       // find dead cell, deadcell=birth
       // pushback futureboard;
    }
}
