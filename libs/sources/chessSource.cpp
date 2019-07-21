#include "../chess.h"

namespace chessEngine{

    /**
     * THIS PART OF CODE DEALS WITH IMPLEMENTING THE CHESS GAME.
     * HERE ALL CHESS RULES , PIECE MOVEMENTS , ENDGAME CONDITIONS ARE DEFINED.
     * THIS IS A VITAL PART OF THE PROGRAMME AND IS NOT TO BE CHANGED !
     */

    //The chess board
    int board[8][8] = {
        {-1,-2,-3,-4,-5,-3,-2,-1},
        {-6,-6,-6,-6,-6,-6,-6,-6},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {6,6,6,6,6,6,6,6},
        {1,3,2,4,5,3,2,1}
    };

    //Copies of the chess board for move undo / validation
    int Copy[8][8];
    int Copy2[50][8][8];

    //A board used to mark all positions that are in check
    unsigned CheckBoard[8][8];

    //List of all possible moves
    unsigned AvailableMoves[80][4];

    //The number of all possible moves
    unsigned NrMoves = 0;

    //All moves that each player has made
    unsigned Wmoves[130][4];
    unsigned Bmoves[130][4];

    //The number of all moves made by each player
    unsigned NWmoves = 0;
    unsigned NBmoves = 0;

    //The X and Y coordinate of both kings
    short KingWhitePos[2] , KingBlackPos[2];

    //The Initial / Final X / Y coordinates of a moving piece
    short posL1 , posL2;
    short posC1 , posC2;

    //Temporary variable used to store data.
    short temp;

    //Variables that validate castling
    bool SCastlingWhite = true;
    bool BCastlingWhite = true;
    bool SCastlingBlack = true;
    bool BCastlingBlack = true;

    //Variables that validate End-Game
    bool DrawReq = false;
    bool Endgame = false;

    //The following functions check for any possible Pawn Promotions a player can make
    bool CheckPromo(int player)
    {
        //If a player can promote
        bool canPromote = false;

        if(player == 1){

            //The White player

            for(int i = 0 ; i < 8 ; i++){
                //If a pawn is found

                if(board[0][i] == 6)
                    canPromote = true;
            }
        }
        else{

            //The Black player

            for(int i = 0 ; i < 8 ; i++){
                //If a pawn is found

                if(board[7][i] == -6)
                    canPromote = true;
            }
        }

        //After the search return canPromote
        return canPromote;
    }

    //Function that adds all En'Passant moves to the Available moves list
    void CheckEnPassant(int player)
    {
        if(player == 1){

            //The White player

            for(int j = 0 ; j < 8 ; j++){
                if(board[3][j] == 6){
                    //For each valid pawn check to its left and right.

                    if(j-1 >= 0){
                        if(NBmoves-1 >= 0 && Bmoves[NBmoves-1][0] == 1 && Bmoves[NBmoves-1][1] == 3 && Bmoves[NBmoves-1][2] == j-1 && Bmoves[NBmoves-1][3] == j-1){
                            AvailableMoves[NrMoves][0] = 3;
                            AvailableMoves[NrMoves][1] = 2;
                            AvailableMoves[NrMoves][2] = j;
                            AvailableMoves[NrMoves][3] = j-1;
                            NrMoves++;
                        }
                    }
                    if(j+1 < 8){
                        if(NBmoves-1 >= 0 && Bmoves[NBmoves-1][0] == 1 && Bmoves[NBmoves-1][1] == 3 && Bmoves[NBmoves-1][2] == j+1 && Bmoves[NBmoves-1][3] == j+1){
                            AvailableMoves[NrMoves][0] = 3;
                            AvailableMoves[NrMoves][1] = 2;
                            AvailableMoves[NrMoves][2] = j;
                            AvailableMoves[NrMoves][3] = j+1;
                            NrMoves++;
                        }
                    }
                }
            }
        }
        else{

            //The Black Player

            for(int j = 0 ; j < 8 ; j++){
                if(board[4][j] == -6){
                    //For each valid pawn check its left and right.

                    if(j-1 >= 0){
                        if(NWmoves-1 >= 0 && Wmoves[NWmoves-1][0] == 6 && Wmoves[NWmoves-1][1] == 4 && Wmoves[NWmoves-1][2] == j-1 && Wmoves[NWmoves-1][3] == j-1){
                            AvailableMoves[NrMoves][0] = 4;
                            AvailableMoves[NrMoves][1] = 5;
                            AvailableMoves[NrMoves][2] = j;
                            AvailableMoves[NrMoves][3] = j-1;
                            NrMoves++;
                        }
                    }
                    if(j+1 < 8){
                        if(NWmoves-1 >= 0 && Wmoves[NWmoves-1][0] == 6 && Wmoves[NWmoves-1][1] == 4 && Wmoves[NWmoves-1][2] == j+1 && Wmoves[NWmoves-1][3] == j+1){
                            AvailableMoves[NrMoves][0] = 4;
                            AvailableMoves[NrMoves][1] = 5;
                            AvailableMoves[NrMoves][2] = j;
                            AvailableMoves[NrMoves][3] = j+1;
                            NrMoves++;
                        }
                    }
                }
            }
        }
    }

    //This function Restores the board to its previous configuration. Used only for validating moves without modifying the board
    void Restore()
    {
        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                //Save the board

                board[i][j] = Copy[i][j];

                //Update king positons
                if(board[i][j] == 5){
                    KingWhitePos[0] = i;
                    KingWhitePos[1] = j;
                }
                else if(board[i][j] == -5){
                    KingBlackPos[0] = i;
                    KingBlackPos[1] = j;
                }
            }
        }
    }

    //Function used to save a copy of each board. Used for move undo.
    void SaveCopy(int x)
    {
        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                Copy2[x][i][j] = board[i][j];
            }
        }
    }

    //The function that changes piece positions on the board
    void InterChange(int pos1L , int pos2L , int pos1C , int pos2C )
    {
        bool enPassant = false;

        //Initialize the copy to 0
        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                Copy[i][j] = 0;
            }
        }

        //Save the copy
        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                Copy[i][j] = board[i][j];
            }
        }

        if(board[pos2L][pos2C] == 0)
            enPassant=  true;

        //Change piece positons
        board[pos2L][pos2C] = board[pos1L][pos1C];
        board[pos1L][pos1C] = 0;

        //In case of En'Passant / Castling several other changes are required.
        if(enPassant && (board[pos1L][pos2C] == 6 || board[pos1L][pos2C] == -6) && (board[pos2L][pos2C] == 6 || board[pos2L][pos2C] == -6) && (pos2C != pos1C) && ((pos1L == 4 && board[pos2L][pos2C] == -6) || (pos1L == 3 && board[pos2L][pos2C] == 6))){
            board[pos1L][pos2C] = 0;
        }

        if(board[pos2L][pos2C] == 5 && pos2C == 6 && pos1C == 4){
            board[7][7] = 0;
            board[7][5] = 1;
        }
        else if(board[pos2L][pos2C] == 5 && pos2C == 2 && pos1C == 4){
            board[7][0] = 0;
            board[7][3] = 1;
        }

        if(board[pos2L][pos2C] == -5 && pos2C == 6 && pos1C == 4){
            board[0][7] = 0;
            board[0][5] = -1;
        }
        else if(board[pos2L][pos2C] == -5 && pos2C == 2 && pos1C == 4){
            board[0][0] = 0;
            board[0][3] = -1;
        }

        //If the king was moved then update its position
        if(board[pos2L][pos2C] == 5){
            KingWhitePos[0] = pos2L;
            KingWhitePos[1] = pos2C;
        }
        else if(board[pos2L][pos2C] == -5){
            KingBlackPos[0] = pos2L;
            KingBlackPos[1] = pos2C;
        }
    }

    //Function that updates the check board and returnes if a certain square is attacked by the enemy player
    bool isCheck(int posX , int posY , int Cplayer)
    {
        //Auxiliary variables
        int auxI = 0;
        int auxJ = 0;

        for(int i = 0 ; i < 8 ; i++){
            for(int j = 0 ; j < 8 ; j++){
                CheckBoard[i][j] = 0;
            }
        }

        if(Cplayer == 1){

            //For the white player

            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    //Update each square

                    if(board[i][j] == -6){
                        //Enemy pawns

                        if(j-1 >= 0 && i+1 < 8)
                            CheckBoard[i+1][j-1]++;
                        if(i+1 < 8 && j+1 < 8)
                            CheckBoard[i+1][j+1]++;
                    }
                    else if(board[i][j] == -1 || board[i][j] == -4){
                        //Enemy rooks / queens

                        auxI = i-1;
                        while(auxI >= 0){
                            CheckBoard[auxI][j]++;
                            if(board[auxI][j] < 0 || board[auxI][j] > 0)
                                auxI = -1;
                            else
                                auxI--;
                        }

                        auxI = i+1;
                        while(auxI < 8){
                            CheckBoard[auxI][j]++;
                            if(board[auxI][j] < 0 || board[auxI][j] > 0)
                                auxI = 10;
                            else
                                auxI++;
                        }

                        auxJ = j-1;
                        while(auxJ >= 0){
                            CheckBoard[i][auxJ]++;
                            if(board[i][auxJ] < 0 || board[i][auxJ] > 0)
                                auxJ = -1;
                            else
                                auxJ--;
                        }

                        auxJ = j+1;
                        while(auxJ < 8){
                            CheckBoard[i][auxJ]++;
                            if(board[i][auxJ] < 0 || board[i][auxJ] > 0)
                                auxJ = 10;
                            else
                                auxJ++;
                        }
                    }
                    else if(board[i][j] == -2){
                        //Enemy knights

                        if(i+2 < 8 && j-1 >= 0)
                            CheckBoard[i+2][j-1]++;
                        if(i+2 < 8 && j+1 < 8)
                            CheckBoard[i+2][j+1]++;
                        if(i+1 < 8 && j-2 >= 0)
                            CheckBoard[i+1][j-2]++;
                        if(i+1 < 8 && j+2 < 8)
                            CheckBoard[i+1][j+2]++;

                        if(i-2 >= 0 && j-1 >= 0)
                            CheckBoard[i-2][j-1]++;
                        if(i-2 >= 0 && j+1 < 8)
                            CheckBoard[i-2][j+1]++;
                        if(i-1 >= 0 && j-2 >= 0)
                            CheckBoard[i-1][j-2]++;
                        if(i-1 >= 0 && j+2 < 8)
                            CheckBoard[i-1][j+2]++;
                    }

                    if(board[i][j] == -3 || board[i][j] == -4){
                        //Enemy bishops / queens

                        auxI = i-1;
                        auxJ = j-1;
                        while(auxI >= 0 && auxJ >= 0){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = -1;
                            else{
                                auxI--;
                                auxJ--;
                            }
                        }

                        auxI = i-1;
                        auxJ = j+1;
                        while(auxI >= 0 && auxJ < 8){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = -1;
                            else{
                                auxI--;
                                auxJ++;
                            }
                        }

                        auxI = i+1;
                        auxJ = j-1;
                        while(auxI < 8 && auxJ >= 0){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = 10;
                            else{
                                auxI++;
                                auxJ--;
                            }
                        }

                        auxI = i+1;
                        auxJ = j+1;
                        while(auxI < 8 && auxJ < 8){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = 10;
                            else{
                                auxI++;
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == -5){
                        //Enemy king

                        if(j-1 >= 0)
                            CheckBoard[i][j-1]++;
                        if(j+1 < 8)
                            CheckBoard[i][j+1]++;

                        if(i-1 >= 0)
                            CheckBoard[i-1][j]++;
                        if(i-1 >= 0 && j-1 >= 0)
                            CheckBoard[i-1][j-1]++;
                        if(i-1 >= 0 && j+1 < 8)
                            CheckBoard[i-1][j+1]++;

                        if(i+1 < 8)
                            CheckBoard[i+1][j]++;
                        if(i+1 < 8 && j-1 >= 0)
                            CheckBoard[i+1][j-1]++;
                        if(i+1 < 8 && j+1 < 8)
                            CheckBoard[i+1][j+1]++;
                    }
                }
            }
        }
        else if(Cplayer == 2){

            //For the black player

            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    //Update each square

                    if(board[i][j] == 6){
                        //Enemy pawns

                        if(i-1 >= 0 && j-1 >= 0)
                            CheckBoard[i-1][j-1]++;
                        if(i-1 >= 0 && j+1 < 8)
                            CheckBoard[i-1][j+1]++;
                    }
                    else if(board[i][j] == 1 || board[i][j] == 4){
                        //Enemy rooks / queens

                        auxI = i-1;
                        while(auxI >= 0){
                            CheckBoard[auxI][j]++;
                            if(board[auxI][j] < 0 || board[auxI][j] > 0)
                                auxI = -1;
                            else
                                auxI--;
                        }

                        auxI = i+1;
                        while(auxI < 8){
                            CheckBoard[auxI][j]++;
                            if(board[auxI][j] < 0 || board[auxI][j] > 0)
                                auxI = 10;
                            else
                                auxI++;
                        }

                        auxJ = j-1;
                        while(auxJ >= 0){
                            CheckBoard[i][auxJ]++;
                            if(board[i][auxJ] < 0 || board[i][auxJ] > 0)
                                auxJ = -1;
                            else
                                auxJ--;
                        }

                        auxJ = j+1;
                        while(auxJ < 8){
                            CheckBoard[i][auxJ]++;
                            if(board[i][auxJ] < 0 || board[i][auxJ] > 0)
                                auxJ = 10;
                            else
                                auxJ++;
                        }
                    }
                    else if(board[i][j] == 2){
                        //Enemy Knights

                        if(i+2 < 8 && j-1 >= 0)
                            CheckBoard[i+2][j-1]++;
                        if(i+2 < 8 && j+1 < 8)
                            CheckBoard[i+2][j+1]++;
                        if(i+1 < 8 && j-2 >= 0)
                            CheckBoard[i+1][j-2]++;
                        if(i+1 < 8 && j+2 < 8)
                            CheckBoard[i+1][j+2]++;

                        if(i-2 >= 0 && j-1 >= 0)
                            CheckBoard[i-2][j-1]++;
                        if(i-2 >= 0 && j+1 < 8)
                            CheckBoard[i-2][j+1]++;
                        if(i-1 >= 0 && j-2 >= 0)
                            CheckBoard[i-1][j-2]++;
                        if(i-1 >= 0 && j+2 < 8)
                            CheckBoard[i-1][j+2]++;
                    }

                    if(board[i][j] == 3 || board[i][j] == 4){
                        //Enemy bishops / queens

                        auxI = i-1;
                        auxJ = j-1;
                        while(auxI >= 0 && auxJ >= 0){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = -1;
                            else{
                                auxI--;
                                auxJ--;
                            }
                        }

                        auxI = i-1;
                        auxJ = j+1;
                        while(auxI >= 0 && auxJ < 8){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = -1;
                            else{
                                auxI--;
                                auxJ++;
                            }
                        }

                        auxI = i+1;
                        auxJ = j-1;
                        while(auxI < 8 && auxJ >= 0){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = 10;
                            else{
                                auxI++;
                                auxJ--;
                            }
                        }

                        auxI = i+1;
                        auxJ = j+1;
                        while(auxI < 8 && auxJ < 8){
                            CheckBoard[auxI][auxJ]++;
                            if(board[auxI][auxJ] < 0 || board[auxI][auxJ] > 0)
                                auxI = 10;
                            else{
                                auxI++;
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == 5){
                        //Enemy kings

                        if(j-1 >= 0)
                            CheckBoard[i][j-1]++;
                        if(j+1 < 8)
                            CheckBoard[i][j+1]++;

                        if(i-1 >= 0)
                            CheckBoard[i-1][j]++;
                        if(i-1 >= 0 && j-1 >= 0)
                            CheckBoard[i-1][j-1]++;
                        if(i-1 >= 0 && j+1 < 8)
                            CheckBoard[i-1][j+1]++;

                        if(i+1 < 8)
                            CheckBoard[i+1][j]++;
                        if(i+1 < 8 && j-1 >= 0)
                            CheckBoard[i+1][j-1]++;
                        if(i+1 < 8 && j+1 < 8)
                            CheckBoard[i+1][j+1]++;
                    }
                }
            }
        }

        if(posX >= 0 && posX < 8 && posY >= 0 && posY < 8 && CheckBoard[posX][posY] >= 1)
            return true;
        else
            return false;
    }

    //Function used to calculate all possible moves
    void GetPosMoves(int player)
    {
        //Auxiliary variables
        int auxI , auxJ;

        //Initialize all previous moves to -1
        for(int i = 0 ; i < NrMoves ; i++){
            for(int j = 0 ; j < 4 ; j++){
                AvailableMoves[i][j] = -1;
            }
        }

        if(player == 2){

            //For the black player

            //Initialzie the number of moves to 0
            NrMoves = 0;

            //Check for En'Passant
            CheckEnPassant(player);
            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    //For each square

                    if(board[i][j] == -6){
                        //If it's a pawn

                        if(i == 1){
                            //If it hasn't moved yet

                            if(board[i+2][j] == 0 && board[i+1][j] == 0){
                                //If it can jump to squares

                                //Make the move and check for check
                                InterChange(i,i+2,j,j);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                    //If the move is valid add it

                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i+2;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }

                                //Restore the board
                                Restore();
                            }

                            if(board[i+1][j] == 0){
                                //If it can move one square

                                //Make the move and check for check
                                InterChange(i,i+1,j,j);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                    //If the move is valid add it

                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i+1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }

                                //Restore the board
                                Restore();
                            }

                            //If the pawn can capture an enemy piece

                            if(j-1 >= 0 && board[i+1][j-1] > 0){
                                InterChange(i,i+1,j,j-1);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i+1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j-1;
                                    NrMoves++;
                                }
                                Restore();
                            }

                            if(j+1 < 8 && board[i+1][j+1] > 0){
                                InterChange(i,i+1,j,j+1);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i+1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j+1;
                                    NrMoves++;
                                }
                                Restore();
                            }
                        }
                        else{
                            //If the pawn isn't on its initial position. --Same thing as above--

                            if(i+1 < 8 && board[i+1][j] == 0){
                                InterChange(i,i+1,j,j);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i+1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }
                                Restore();
                            }
                            if(i+1 < 8 && j-1 >= 0 && board[i+1][j-1] > 0){
                                InterChange(i,i+1,j,j-1);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                        AvailableMoves[NrMoves][0] = i;
                                        AvailableMoves[NrMoves][1] = i+1;
                                        AvailableMoves[NrMoves][2] = j;
                                        AvailableMoves[NrMoves][3] = j-1;
                                        NrMoves++;
                                }
                                Restore();
                            }
                            if( i+1 < 8 && j+1 < 8 && board[i+1][j+1] > 0){
                                InterChange(i,i+1,j,j+1);
                                if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                        AvailableMoves[NrMoves][0] = i;
                                        AvailableMoves[NrMoves][1] = i+1;
                                        AvailableMoves[NrMoves][2] = j;
                                        AvailableMoves[NrMoves][3] = j+1;
                                        NrMoves++;
                                }
                                Restore();
                            }
                        }
                    }
                    else if(board[i][j] == -1 || board[i][j] == -4){
                        //If it's a rook / queen

                        auxI = i;
                        auxJ = j;

                        auxI = i-1;
                        while((auxI >= 0) && (board[auxI][j] >= 0)){
                            InterChange(i,auxI,j,j);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][j] > 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                            }
                        }

                        auxI = i+1;
                        while((auxI < 8) && (board[auxI][j] >= 0)){
                            InterChange(i,auxI,j,j);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][j] > 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                            }
                        }

                        auxJ = j-1;
                        while((auxJ >= 0) && (board[i][auxJ] >= 0)){
                            InterChange(i,i,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[i][auxJ] > 0){
                                auxJ = -1;
                            }
                            else{
                                auxJ--;
                            }
                        }

                        auxJ = j+1;
                        while((auxJ < 8) && (board[i][auxJ] >= 0)){
                            InterChange(i,i,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[i][auxJ] > 0){
                                auxJ = 10;
                            }
                            else{
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == -2){
                        //If it's a knight

                        if(i+1 < 8 && j+2 < 8 && board[i+1][j+2] >= 0){
                            InterChange(i,i+1,j,j+2);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j-2 >= 0 && board[i+1][j-2] >= 0){
                            InterChange(i,i+1,j,j-2);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+2 < 8 && j-1 >= 0 && board[i+2][j-1] >= 0){
                            InterChange(i,i+2,j,j-1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+2 < 8 && j+1 < 8 && board[i+2][j+1] >= 0){
                            InterChange(i,i+2,j,j+1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-2 >= 0 && j-1 >= 0 && board[i-2][j-1] >= 0){
                            InterChange(i,i-2,j,j-1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-2 >= 0 && j+1 < 8 && board[i-2][j+1] >= 0){
                            InterChange(i,i-2,j,j+1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j-2 >= 0 && board[i-1][j-2] >= 0){
                            InterChange(i,i-1,j,j-2);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j+2 < 8 && board[i-1][j+2] >= 0){
                            InterChange(i,i-1,j,j+2);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+2;
                                NrMoves++;
                            }
                            Restore();
                        }
                    }

                    if(board[i][j] == -3 || board[i][j] == -4){
                        //If it's a bishop / queen

                        auxI = i-1;
                        auxJ = j-1;
                        while(auxI >= 0 && auxJ >= 0 && board[auxI][auxJ] >= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] > 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                                auxJ--;
                            }
                        }

                        auxI = i-1;
                        auxJ = j+1;
                        while(auxI >= 0 && auxJ < 8 && board[auxI][auxJ] >= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;++
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] > 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                                auxJ++;
                            }
                        }

                        auxI = i+1;
                        auxJ = j-1;
                        while(auxI < 8 && auxJ >= 0 && board[auxI][auxJ] >= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] > 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                                auxJ--;
                            }
                        }

                        auxI = i+1;
                        auxJ = j+1;
                        while(auxI < 8 && auxJ < 8 && board[auxI][auxJ] >= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] > 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == -5){
                        //For the king.

                        if(j-1 >= 0 && board[i][j-1] >= 0){
                            InterChange(i,i,j,j-1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if( j+1 < 8 && board[i][j+1] >= 0){
                            InterChange(i,i,j,j+1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && board[i-1][j] >= 0){
                            InterChange(i,i-1,j,j);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if( i-1 >= 0 && j-1 >= 0 && board[i-1][j-1] >= 0){
                            InterChange(i,i-1,j,j-1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j+1 < 8 && board[i-1][j+1] >= 0){
                            InterChange(i,i-1,j,j+1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && board[i+1][j] >= 0){
                            InterChange(i,i+1,j,j);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j-1 >= 0 && board[i+1][j-1] >= 0){
                            InterChange(i,i+1,j,j-1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j+1 < 8 && board[i+1][j+1] >= 0){
                            InterChange(i,i+1,j,j+1);
                            if(!isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                    }
                }
            }

            //At last, check for any available castling.

            //Big castling
            if(board[0][2] == 0 && board[0][3] == 0 && board[0][0] == -1  && board[0][4] == -5 && BCastlingBlack && !isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                if(!isCheck(0,2,2) && !isCheck(0,3,2)){
                    AvailableMoves[NrMoves][0] = KingBlackPos[0];
                    AvailableMoves[NrMoves][1] = KingBlackPos[0];
                    AvailableMoves[NrMoves][2] = KingBlackPos[1];
                    AvailableMoves[NrMoves][3] = 2;
                    NrMoves++;
                }
            }

            //Small Castling
            if(board[0][5] == 0 && board[0][6] == 0 && board[0][7] == -1 && board[0][4] == -5 && SCastlingBlack && !isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                if(!isCheck(0,5,2) && !isCheck(0,6,2)){
                    AvailableMoves[NrMoves][0] = KingBlackPos[0];
                    AvailableMoves[NrMoves][1] = KingBlackPos[0];
                    AvailableMoves[NrMoves][2] = KingBlackPos[1];
                    AvailableMoves[NrMoves][3] = 6;
                    NrMoves++;
                }
            }
        }
        else{

            //Same as above for the white player with minor changes to piece positions e.g. Castling positons / Initial pawn positions

            NrMoves = 0;
            CheckEnPassant(player);
            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    if(board[i][j] == 6){
                        if(i == 6){
                            if(board[i-2][j] == 0 && board[i-1][j] == 0){
                                InterChange(i,i-2,j,j);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i-2;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }
                                Restore();
                            }

                            if(board[i-1][j] == 0){
                                InterChange(i,i-1,j,j);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i-1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }
                                Restore();
                            }

                            if(j-1 >= 0 && board[i-1][j-1] < 0){
                                InterChange(i,i-1,j,j-1);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i-1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j-1;
                                    NrMoves++;
                                }
                                Restore();
                            }
                            if(j+1 < 8 && board[i-1][j+1] < 0){
                                InterChange(i,i-1,j,j+1);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i-1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j+1;
                                    NrMoves++;
                                }
                                Restore();
                            }
                        }
                        else{
                            if(i-1 >= 0 && board[i-1][j] == 0){
                                InterChange(i,i-1,j,j);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                    AvailableMoves[NrMoves][0] = i;
                                    AvailableMoves[NrMoves][1] = i-1;
                                    AvailableMoves[NrMoves][2] = j;
                                    AvailableMoves[NrMoves][3] = j;
                                    NrMoves++;
                                }
                                Restore();
                            }
                            if(i-1 >= 0 && j-1 >= 0 && board[i-1][j-1] < 0){
                                InterChange(i,i-1,j,j-1);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                        AvailableMoves[NrMoves][0] = i;
                                        AvailableMoves[NrMoves][1] = i-1;
                                        AvailableMoves[NrMoves][2] = j;
                                        AvailableMoves[NrMoves][3] = j-1;
                                        NrMoves++;
                                }
                                Restore();
                            }
                            if(i-1 >= 0 && j+1 < 8 && board[i-1][j+1] < 0){
                                InterChange(i,i-1,j,j+1);
                                if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                        AvailableMoves[NrMoves][0] = i;
                                        AvailableMoves[NrMoves][1] = i-1;
                                        AvailableMoves[NrMoves][2] = j;
                                        AvailableMoves[NrMoves][3] = j+1;
                                        NrMoves++;
                                }
                                Restore();
                            }
                        }
                    }
                    else if(board[i][j] == 1 || board[i][j] == 4){
                        auxI = i;
                        auxJ = j;

                        auxI = i-1;
                        while(auxI >= 0 && board[auxI][j] <= 0){
                            InterChange(i,auxI,j,j);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][j] < 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                            }
                        }

                        auxI = i+1;
                        while(auxI < 8 && board[auxI][j] <= 0){
                            InterChange(i,auxI,j,j);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][j] < 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                            }
                        }

                        auxJ = j-1;
                        while(auxJ >= 0 && board[i][auxJ] <= 0){
                            InterChange(i,i,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[i][auxJ] < 0){
                                auxJ = -1;
                            }
                            else{
                                auxJ--;
                            }
                        }

                        auxJ = j+1;
                        while(auxJ < 8 && board[i][auxJ] <= 0){
                            InterChange(i,i,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[i][auxJ] < 0){
                                auxJ = 10;
                            }
                            else{
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == 2){
                        if(i-2 >= 0 && j-1 >= 0 && board[i-2][j-1] <= 0){
                            InterChange(i,i-2,j,j-1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-2 >= 0 && j+1 < 8 && board[i-2][j+1] <= 0){
                            InterChange(i,i-2,j,j+1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j-2 >= 0 && board[i-1][j-2] <= 0){
                            InterChange(i,i-1,j,j-2);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j+2 < 8 && board[i-1][j+2] <= 0){
                            InterChange(i,i-1,j,j+2);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j+2 < 8 && board[i+1][j+2] <= 0){
                            InterChange(i,i+1,j,j+2);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j-2 >= 0 && board[i+1][j-2] <= 0){
                            InterChange(i,i+1,j,j-2);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-2;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+2 < 8 && j-1 >= 0 && board[i+2][j-1] <= 0){
                            InterChange(i,i+2,j,j-1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+2 < 8 && j+1 < 8 && board[i+2][j+1] <= 0){
                            InterChange(i,i+2,j,j+1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+2;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                    }

                    if(board[i][j] == 3 || board[i][j] == 4){
                        auxI = i-1;
                        auxJ = j-1;
                        while(auxI >= 0 && auxJ >= 0 && board[auxI][auxJ] <= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] < 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                                auxJ--;
                            }
                        }

                        auxI = i-1;
                        auxJ = j+1;
                        while(auxI >= 0 && auxJ < 8 && board[auxI][auxJ] <= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] < 0){
                                auxI = -1;
                            }
                            else{
                                auxI--;
                                auxJ++;
                            }
                        }

                        auxI = i+1;
                        auxJ = j-1;
                        while(auxI < 8 && auxJ >= 0 && board[auxI][auxJ] <= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] < 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                                auxJ--;
                            }
                        }

                        auxI = i+1;
                        auxJ = j+1;
                        while(auxI < 8 && auxJ < 8 && board[auxI][auxJ] <= 0){
                            InterChange(i,auxI,j,auxJ);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = auxI;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = auxJ;
                                NrMoves++;
                            }
                            Restore();

                            if(board[auxI][auxJ] < 0){
                                auxI = 10;
                            }
                            else{
                                auxI++;
                                auxJ++;
                            }
                        }
                    }
                    else if(board[i][j] == 5){
                        if(j-1 >= 0 && board[i][j-1] <= 0){
                            InterChange(i,i,j,j-1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(j+1 < 8 && board[i][j+1] <= 0){
                            InterChange(i,i,j,j+1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && board[i-1][j] <= 0){
                            InterChange(i,i-1,j,j);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j-1 >= 0 && board[i-1][j-1] <= 0){
                            InterChange(i,i-1,j,j-1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i-1 >= 0 && j+1 < 8 && board[i-1][j+1] <= 0){
                            InterChange(i,i-1,j,j+1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i-1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && board[i+1][j] <= 0){
                            InterChange(i,i+1,j,j);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j-1 >= 0 && board[i+1][j-1] <= 0){
                            InterChange(i,i+1,j,j-1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j-1;
                                NrMoves++;
                            }
                            Restore();
                        }
                        if(i+1 < 8 && j+1 < 8 && board[i+1][j+1] <= 0){
                            InterChange(i,i+1,j,j+1);
                            if(!isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                                AvailableMoves[NrMoves][0] = i;
                                AvailableMoves[NrMoves][1] = i+1;
                                AvailableMoves[NrMoves][2] = j;
                                AvailableMoves[NrMoves][3] = j+1;
                                NrMoves++;
                            }
                            Restore();
                        }
                    }
                }
            }

            //Big Castling
            if(board[7][2] == 0 && board[7][3] == 0 && board[7][0] == 1 && board[7][4] == 5 && BCastlingWhite && !isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                if(!isCheck(7,2,1) && !isCheck(7,3,1)){
                    AvailableMoves[NrMoves][0] = KingWhitePos[0];
                    AvailableMoves[NrMoves][1] = KingWhitePos[0];
                    AvailableMoves[NrMoves][2] = KingWhitePos[1];
                    AvailableMoves[NrMoves][3] = 2;
                    NrMoves++;
                }
            }

            //Small Castling
            if(board[7][5] == 0 && board[7][6] == 0 && board[7][7] == 1 && board[7][4] == 5 && SCastlingWhite && !isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                if(!isCheck(7,5,1) && !isCheck(7,6,1)){
                    AvailableMoves[NrMoves][0] = KingWhitePos[0];
                    AvailableMoves[NrMoves][1] = KingWhitePos[0];
                    AvailableMoves[NrMoves][2] = KingWhitePos[1];
                    AvailableMoves[NrMoves][3] = 6;
                    NrMoves++;
                }
            }
        }
    }

}
