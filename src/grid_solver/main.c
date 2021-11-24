#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "solver.h"


int main(int argc,  char *argv[]){

    if (argc>2||argc<2)
        errx(1,"The number of arguments is not valid");

    char toto[9][9];//create a temp array of the char in the origin file
    int good_one[9][9];//array that will contain the grid
    int i=0,j=0,c;

    FILE *file = NULL;
    file = fopen(argv[1], "r");
    if(file == NULL)
        exit(1);

    //read the file and take each caractere in a grid at the good place
    while((c = (fgetc(file))) != EOF){

        if(i==9){
            i=0;
            j+=1;
        }

        if(c == ' ' || c == '\r' || c=='\n' || c=='\f' || c=='\0')
            continue;
        else{
            toto[j][i]= c;
            i++;
        }
    }

    //put the char into int number
    for(int x=0; x<9; x++){
        for(int y=0; y<9; y++){
            good_one[x][y] = (change_dot(toto[x][y]));
        }
    }

    //resolve the grid if posible else return an error
    if(Solve_Sudoku(good_one, 0, 0))
    {
        //write into a new file the solve grid
        write_file(argv[1],good_one);
    }
    else errx(1,"NO SOLUTION! The given grid is wrong please change it !");

    return 0;
}
