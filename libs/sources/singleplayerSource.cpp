#include "../singleplayer.h"
#include "../chess.h"

//Linking other libraries
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

//The A.I. code requires the functions defined in chessRules.h
using namespace chessEngine;
using namespace std;

namespace AIEngine{

    /**
     * HERE A.I. MECHANICS ARE DEFINED.
     * THIS IS VITAL FOR SINGLE-PLAYER AS ANY ERRORS WILL STOP IT FROM FUNCTIONING PROPERLY.
     * CHANGE ONLY IF YOU KNOW WHAT YOU'RE DOING.
     */

    //Variables used to store the current winner & player. Both set to White by default.
    short player = 1; // WHITE
    short Winner = 2; //WHITE

    //Variables used for board scoring purposes
    bool castledWhite = false;
    bool castledBlack = false;

    //The initial set of moves the A.I. can make
    int PossibleMoves[100][5];

    //The number of moves the A.I. can make
    int NPmoves = 0;

    //The moves that achieved the best score after the Minimax algorithm
    int Fmoves[100][4];

    //Number of moves that achieved the best score after the Minimax algorithm
    int NFmoves = 0;

    //A copy of the initial board status used to completley reset the board at the end of a game.
    int TRUEreset[8][8] = {
        {-1,-2,-3,-4,-5,-3,-2,-1},
        {-6,-6,-6,-6,-6,-6,-6,-6},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {6,6,6,6,6,6,6,6},
        {1,2,3,4,5,3,2,1}
    };


    /* The following tables are used in board evaluation. They can be changed freely.
    They give each piece a score based on its position on the board. */

    int kingTable[8][8] = {
        {-3,-4,-4,-5,-5,-4,-4,-3},
        {-3,-4,-4,-5,-5,-4,-4,-3},
        {-3,-4,-4,-5,-5,-4,-4,-3},
        {-3,-4,-4,-5,-5,-4,-4,-3},
        {-2,-3,-3,-4,-4,-3,-3,-2},
        {-1,-2,-2,-3,-3,-2,-2,-1},
        {2,2,0,0,0,0,2,2},
        {2,3,1,0,0,1,3,2}
    };

    int queenTable[8][8] = {
        {-3,-2,-2,-1,-1,-2,-2,-3},
        {-2,0,0,0,0,0,0,-2},
        {-2,0,1,1,1,1,0,-2},
        {-1,0,1,1,1,1,0,-1},
        {0,0,1,1,1,1,0,-1},
        {-2,1,1,1,1,1,0,-2},
        {-2,0,1,0,0,0,0,-2},
        {-3,-2,-2,-1,-1,-2,-2,-3}
    };

    int rookTable[8][8] = {
        {0,0,0,0,0,0,0,0},
        {1,2,2,2,2,2,2,1},
        {-1,0,0,0,0,0,0,-1},
        {-1,0,0,0,0,0,0,-1},
        {-1,0,0,0,0,0,0,-1},
        {-1,0,0,0,0,0,0,-1},
        {-1,0,0,0,0,0,0,-1},
        {0,0,0,1,1,0,0,0}
    };

    int bishopTable[8][8] = {
        {-3,-2,-2,-2,-2,-2,-2,-3},
        {-2,0,0,0,0,0,0,-2},
        {-2,0,1,2,2,1,0,-2},
        {-2,1,1,2,2,1,1,-2},
        {-2,0,2,2,2,2,0,-2},
        {-2,2,2,2,2,2,2,-2},
        {-2,1,0,0,0,0,1,-2},
        {-3,-2,-2,-2,-2,-2,-2,-3}
    };

    int knightTable[8][8] = {
        {-6,-5,-4,-4,-4,-4,-5,-6},
        {-4,-2,0,1,1,0,-2,-4},
        {-4,1,2,3,3,2,1,-4},
        {-4,0,3,4,4,3,0,-4},
        {-4,0,3,4,4,3,0,-4},
        {-4,1,2,3,3,2,1,-4},
        {-4,-2,0,1,1,0,-2,-4},
        {-6,-5,-4,-4,-4,-4,-5,-6}
    };

    int pawnTable[8][8] = {
        {0,0,0,0,0,0,0,0},
        {6,6,6,6,6,6,6,6},
        {2,2,3,4,4,3,2,2},
        {1,1,2,3,3,2,1,1},
        {0,0,0,2,2,0,0,0},
        {1,-1,-2,0,0,-2,-1,1},
        {1,2,2,-3,-3,2,2,1},
        {0,0,0,0,0,0,0,0},
    };

    /*The following function evaluates the board and returns a score.
        0 = Equal position
        + ( positive score ) = Black is advantaged
        - ( negative score ) = White is advantaged
    The reason black is positive and white negative is because initially the A.I. could only play as black.*/

    int GetScore()
    {
        //Asume the position is equal
        int score = 0;

        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                //For each square on the board

                if(board[i][j] == 6){
                    //If it's a white pawn

                    score -= pawnTable[i][j];
                    score -= 100;
                }
                else if(board[i][j] == -6){
                    //If it's a black pawn

                    score += pawnTable[7-i][7-j];
                    score += 100;
                }
                else if(board[i][j] == 1){
                    //If it's a white rook

                    score -= rookTable[i][j];
                    score -= 500;
                }
                else if(board[i][j] == -1){
                    //If it's a black rook

                    score += rookTable[7-i][7-j];
                    score += 500;
                }
                else if(board[i][j] == 2){
                    //If it's a white knight

                    score -= knightTable[i][j];
                    score -= 300;
                }
                else if(board[i][j] == -2){
                    //If it's a black knight

                    score += knightTable[7-i][7-j];
                    score += 300;
                }
                else if(board[i][j] == 3){
                    //If it's a white bishop

                    score -= bishopTable[i][j];
                    score -= 320;
                }
                else if(board[i][j] == -3){
                    //If it's a black bishop

                    score += bishopTable[7-i][7-j];
                    score += 320;
                }
                else if(board[i][j] == 4){
                    //If it's a white queen

                    score -= queenTable[i][j];
                    score -= 900;
                }
                else if(board[i][j] == -4){
                    //If it's a black queen

                    score += queenTable[7-i][7-j];
                    score += 900;
                }

                /* If there are two pawns 'Doubled' ( one in front of the other )
                reduce their score by 50 as one of them is less active */

                if(i-1 >= 0 && board[i][j] == 6 && board[i-1][j] == 6)
                    score += 50;
                else if(i+1 < 8 && board[i][j] == -6 && board[i+1][j] == -6)
                    score -= 50;
            }
        }

        //Give points to each side if they are castled
        if(castledWhite)
            score -= 80;
        else if(castledBlack)
            score += 80;

        //Evaluate the Kings' position
        score -= kingTable[KingWhitePos[0]][KingWhitePos[1]];
        score += kingTable[7 - KingBlackPos[0]][7 - KingBlackPos[1]];

        //After done evaluating return the score
        return score;
    }

    /* This function evaluates one position by calculating several positions ahead using a 'Mini-Max' type algorithm
    with 'Alfa-Beta pruning' to increase the speed of the calculation. Changes to this function are not recommended !*/

    int Minimax(int depth , int isMaximizingPlayer , int alpha , int beta)
    {
        //At the start of each function allocate free space to avoid Stack Overflow.
        char* space = new char[50];

        //A copy of the board that is only used for this algorithm to prevent real board modifications
        int TTcopy[8][8];

        //Used to store the inital number of available moves as 'GetPosMoves(int side)' is called several times
        int TMOVES = 0;

        //The side of the player. White = minimizer / Black = maximizer
        int side;

        //Get the side required by the function 'GetPosMoves(int side)' found in chessRules.h
        if(isMaximizingPlayer)
            side = 2;
        else
            side = 1;

        //If we reached the end return the score.
        if(depth == 0)
            return GetScore();

        //Calculate all possible moves
        GetPosMoves(side);

        //Save their number
        TMOVES = NrMoves;

        if(isMaximizingPlayer){
            //bestMove is the variable that will hold the highest / lowest score found by the algorithm for the said position.
            int bestMove = -100000;

            //Save the current board state
            for(int j = 0 ; j < 8 ; j++){
                for(int k = 0 ; k < 8 ; k++){
                    TTcopy[j][k] = board[j][k];
                }
            }

            for(int i = 0 ; i < TMOVES ; i++){
                //For each possible move

                //Re-calculate all possible moves
                GetPosMoves(side);

                //Make the next move available
                InterChange(AvailableMoves[i][0],AvailableMoves[i][1],AvailableMoves[i][2],AvailableMoves[i][3]);

                //Get bestMove for the next set of positions
                bestMove = max(bestMove , Minimax(depth - 1, 0,alpha,beta));

                //Update alpha
                alpha = max(bestMove , alpha);

                //Remove redundant positions
                if(beta <= alpha)
                    return bestMove;

                //If the position was not removed by Alpha-Beta restore the board to its previous state
                for(int j = 0 ; j < 8 ; j++){
                    for(int k = 0 ; k < 8 ; k++){
                        board[j][k] = TTcopy[j][k];
                    }
                }
            }

            //At the end return the highest score
            return bestMove;
        }
        else{
            /* I am not going to comment all the following lines as they are identical to the ones above
            as the same thing is done for the Minimizer */

            int bestMove = 100000;

            //Save the current board state
            for(int j = 0 ; j < 8 ; j++){
                for(int k = 0 ; k < 8 ; k++){
                    TTcopy[j][k] = board[j][k];
                }
            }

            for(int i = 0 ; i < TMOVES ; i++){
                //For each available move

                //Recalculate the number of moves
                GetPosMoves(1);

                //Make the next move available
                InterChange(AvailableMoves[i][0] , AvailableMoves[i][1] , AvailableMoves[i][2] , AvailableMoves[i][3]);

                //Recalculate bestMove for the next set of positions
                bestMove = min(bestMove , Minimax(depth - 1 , 1,alpha,beta));

                //Update beta
                beta = min(bestMove , beta);

                //Remove redundant positions
                if(beta <= alpha)
                    return bestMove;

                //If the position was not removed by Alpha-Beta restore the board
                for(int j = 0 ; j < 8 ; j++){
                    for(int k = 0 ; k < 8 ; k++){
                        board[j][k] = TTcopy[j][k];
                    }
                }
            }

            //At the end return bestMove
            return bestMove;
        }
    }

    //This function compares the scores returned by the Mini-Max algorithm for each possible move of the initial set and gets the highest / lowest
    void GetBestMove(int side , int difficulty)
    {
        //Yet another copy of the board
        int TTc[8][8];

        //Identical to the one used inside the Mini-Max algorithm. They both have the same usage.
        int bestMove = 0;

        //Again simillar to the one found above.
        int TMOVES = 0;

        //Initialize the number of moves on 0
        NPmoves = 0;
        NFmoves = 0;

        //Save the board to preserve its state
        for(int j = 0 ; j < 8 ; j++){
            for(int k = 0 ; k < 8 ; k++){
                TTc[j][k] = board[j][k];
            }
        }

        //Calculate all possible moves
        GetPosMoves(side);

        //Save initial number of moves
        TMOVES = NrMoves;

        for(int i = 0 ; i < TMOVES ; i++){
            //For each move

            //Recalculate all moves
            GetPosMoves(side);

            //Save the current move returned by the function
            PossibleMoves[NPmoves][0] = AvailableMoves[i][0];
            PossibleMoves[NPmoves][1] = AvailableMoves[i][1];
            PossibleMoves[NPmoves][2] = AvailableMoves[i][2];
            PossibleMoves[NPmoves][3] = AvailableMoves[i][3];

            //Make the current move
            InterChange(AvailableMoves[i][0] , AvailableMoves[i][1] , AvailableMoves[i][2] , AvailableMoves[i][3]);

            //Call the Mini-Max algorithm and save the score for the current position
            if(side == 1)
                PossibleMoves[NPmoves][4] = Minimax(difficulty,1,-20000,20000);
            else
                PossibleMoves[NPmoves][4] = Minimax(difficulty,0,-20000,20000);

            //Restore the board
            for(int j = 0 ; j < 8 ; j++){
                for(int k = 0 ; k < 8 ; k++){
                    board[j][k] = TTc[j][k];
                }
            }

            //Increase the number of analysed moves by one
            NPmoves++;
        }

        //Initialize bestMove on the score of the first positon
        bestMove = PossibleMoves[0][4];

        for(int i = 0 ; i < NPmoves ; i++){
            //For each position update bestMove

            if(PossibleMoves[i][4] > bestMove && side == 2)
                bestMove = PossibleMoves[i][4];
            else if(PossibleMoves[i][4] < bestMove && side == 1)
                bestMove = PossibleMoves[i][4];
        }

        for(int i = 0 ; i < NPmoves ; i++){
            //After we found the highest / lowest score look for all moves that have it

            if(PossibleMoves[i][4] == bestMove){
                //If found one move save it to Fmoves ( Final moves )

                Fmoves[NFmoves][0] = PossibleMoves[i][0];
                Fmoves[NFmoves][1] = PossibleMoves[i][1];
                Fmoves[NFmoves][2] = PossibleMoves[i][2];
                Fmoves[NFmoves][3] = PossibleMoves[i][3];

                //Increase their number by 1
                NFmoves++;
            }
        }
    }

    //This function works as the A.I. itself . No changes are recommended.
    void AI(int side , int difficulty)
    {
        //If the A.I. is in dis-advantage and a draw is proposed, It will accept it.
        if(GetScore() < 0 && side == 2 && DrawReq){
            Endgame = true;
            DrawReq = true;
        }
        else if(GetScore() > 0 && side == 1 && DrawReq){
            DrawReq = true;
            Endgame = true;
        }

        //Initialize the randomizer
        srand((int)time(0));

        //The ID of the move the A.I. will make
        int Move = 0;

        //Calculate the best moves to make
        GetBestMove(side , difficulty);

        //Out of all best moves make a random one.
        Move = rand() % NFmoves;

        if(side == 1){
            //If it's white save it to white moves

            for(int i = 0 ; i < 4 ; i++){
                Wmoves[NWmoves][i] = Fmoves[Move][i];
            }

            //Increase the number of white moves by 1
            NWmoves++;
        }
        else{

            //Same thing for black

            for(int i = 0 ; i < 4 ; i++){
                Bmoves[NBmoves][i] = Fmoves[Move][i];
            }

            NBmoves++;
        }

        //If the A.I. is moving a rook , make the coresponding castling be unavailable
        if(board[Fmoves[Move][0]][Fmoves[Move][2]] == 1 || board[Fmoves[Move][0]][Fmoves[Move][2]] == -1){
            if(side == 1){
                if(Fmoves[Move][2] == 0)
                    BCastlingWhite = false;
                else if(Fmoves[Move][2] == 7)
                    SCastlingWhite = false;
            }
            else{
                if(Fmoves[Move][2] == 0)
                    BCastlingBlack = false;
                else if(Fmoves[Move][2] == 7)
                    SCastlingBlack = false;
            }
        }
        else if(board[Fmoves[Move][0]][Fmoves[Move][2]] == -5 || board[Fmoves[Move][0]][Fmoves[Move][2]] == 5){

            //If the king is moved make all castling be unavailable

            if(side == 1){
                SCastlingWhite = false;
                BCastlingWhite = false;
            }
            else{
                SCastlingBlack = false;
                BCastlingBlack = false;
            }
        }


        //Finally make the move and update the player and winner.
        if(side == 1){
            InterChange(Fmoves[Move][0] , Fmoves[Move][1] , Fmoves[Move][2] , Fmoves[Move][3]);
            player = 2;
            Winner = 1;
        }
        else{
            InterChange(Fmoves[Move][0] , Fmoves[Move][1] , Fmoves[Move][2] , Fmoves[Move][3]);
            player = 1;
            Winner = 2;
        }

        //If a promotion is available after the move, promote to queen
        if(CheckPromo(side)){
            for(int i = 0 ; i < 8 ; i++){
                if(board[7][i] == -6 && side == 2){
                    board[7][i] = -4;
                }
                else if(side == 1 && board[0][i] == 6){
                    board[0][i] = 4;
                }
            }
        }
    }
}
