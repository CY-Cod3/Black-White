#include "../game.h"
#include "../chess.h"
#include "../singleplayer.h"

//Link to graphics library
#include <SFML/Graphics.hpp>

//Link to FSTREAM library
#include <fstream>

//Link to CSTRING library
#include <cstring>

//Link to STRING library
#include <string>

using namespace chessEngine;
using namespace AIEngine;
using namespace std;

bool bigC = false;
bool smallC = false;

string Encode( unsigned int move_to_encode[4] , int player ) {

    char piece = ' ';
    char col;
    char startCOL = ' ';
    char line = (8 - move_to_encode[1]) + '0';

    switch( move_to_encode[3] ){
        case 1:
            col = 'b';
            break;
        case 2:
            col = 'c';
            break;
        case 3:
            col = 'd';
            break;
        case 4:
            col = 'e';
            break;
        case 5:
            col = 'f';
            break;
        case 6:
            col = 'g';
            break;
        case 7:
            col = 'h';
        default:
            col = 'a';
            break;
    }

    switch( abs(board[move_to_encode[0]][move_to_encode[2]]) ){
        case 1:
            piece = 'R';
            break;
        case 2:
            piece = 'N';
            break;
        case 3:
            piece = 'B';
            break;
        case 4:
            piece = 'Q';
            break;
        case 5:
            piece = 'K';
            break;
        default:
            break;
    }

    GetPosMoves(player);

    for(int i = 0 ; i  < NrMoves ; i++ ){

        if( board[AvailableMoves[i][0]][AvailableMoves[i][2]] == board[move_to_encode[0]][move_to_encode[2]] ) {

            if( AvailableMoves[i][0] != move_to_encode[0] || AvailableMoves[i][2] != move_to_encode[2] ) {

                if( AvailableMoves[i][1] == move_to_encode[1] && AvailableMoves[i][3] == move_to_encode[3] ) {

                    switch( move_to_encode[2] ){

                        case 1:
                            startCOL = 'b';
                            break;
                        case 2:
                            startCOL = 'c';
                            break;
                        case 3:
                            startCOL = 'd';
                            break;
                        case 4:
                            startCOL = 'e';
                            break;
                        case 5:
                            startCOL = 'f';
                            break;
                        case 6:
                            startCOL = 'g';
                            break;
                        case 7:
                            startCOL = 'h';
                        default:
                            startCOL = 'a';
                            break;

                    }

                }

            }

        }

    }

    if(piece == ' ' && board[move_to_encode[1]][move_to_encode[3]] != 0 ){
        switch( move_to_encode[2] ){

            case 1:
                startCOL = 'b';
                break;
            case 2:
                startCOL = 'c';
                break;
            case 3:
                startCOL = 'd';
                break;
            case 4:
                startCOL = 'e';
                break;
            case 5:
                startCOL = 'f';
                break;
            case 6:
                startCOL = 'g';
                break;
            case 7:
                startCOL = 'h';
            default:
                startCOL = 'a';
                break;

        }
    }

    string s = "";
    if(piece != ' ')
        s += piece;
    if(startCOL != ' ')
        s += startCOL;
    if( board[move_to_encode[1]][move_to_encode[3]] != 0 )
        s += "x";
    s += col;
    s += line;

    if( abs( board[move_to_encode[0]][move_to_encode[2]] ) == 5) {

        if(move_to_encode[2] - move_to_encode[3] == -2)
            s = "O-O";
        if(move_to_encode[2] - move_to_encode[3] == 2)
            s = "O-O-O";
    }

    InterChange(move_to_encode[0] , move_to_encode[1] , move_to_encode[2] , move_to_encode[3] );

    if( player == 1 ){
        GetPosMoves(2);
        if(NrMoves == 0 && isCheck(KingBlackPos[0] , KingBlackPos[1] , 2))
            s += "++";
        else if(isCheck(KingBlackPos[0] , KingBlackPos[1] , 2))
            s += "+";
    } else if( player == 2 ){
        GetPosMoves(1);
        if(NrMoves == 0 && isCheck(KingWhitePos[0] , KingWhitePos[1] , 1))
            s += "++";
        else if(isCheck(KingWhitePos[0] , KingWhitePos[1] , 1))
            s += "+";
    }

    return s;
}

void gameHandler::save_game() {

    /** Read settings file to determine how many files there are */

    unsigned int current_id = 0;
    unsigned cifre[1000]  , Ncifre=0;

    ifstream fin( "archive/settings.ini" );

    string buffer;   //This will read settings

    fin >> buffer >> buffer >> buffer >> current_id;    //Read section , variable , = and ID
    fin.close();

    /** Begin encoding match into its file */

    string file = "archive/game";

    if(current_id > 9) {
        int aux = current_id+1;
        while(aux != 0){
            cifre[Ncifre] = aux % 10;
            Ncifre++;
            aux = aux/10;
        }
        for(int i = Ncifre-1 ; i >= 0 ; i--){
            file += cifre[i] + '0';
        }
    }
    else
        file += current_id + 1 + '0';
    file += ".pgn";

    ofstream fout( file.c_str() );

    fout << "[White] \n[Black] \n[Result ";

    if( Endgame && DrawReq )
        fout << "1/2 - 1/2]\n\n";
    else if(Endgame && !DrawReq ) {
        if(Winner == 1)
            fout << "1 - 0]\n\n";
        else
            fout << "0 - 1]\n\n";
    }

    //Reset board
    for(int i = 0 ; i < 8 ; i++) {
        for(int j = 0 ; j < 8 ; j++) {
            board[i][j] = TRUEreset[i][j];
        }
    }

    for(int i = 1 ; i <= min(NWmoves , NBmoves) ; i++) {

        fout << i << ". " << Encode(Wmoves[i-1],1) << " " << Encode(Bmoves[i-1],2) << " ";

        if(i % 6 == 0)
            fout << "\n";

    }

    if( NWmoves - NBmoves == 1) {

        fout << NWmoves << ". " << Encode(Wmoves[NWmoves-1],1) << " ";
    }

    if( Endgame && DrawReq )
        fout << "1/2 - 1/2";
    else if(Endgame && !DrawReq ) {
        if(Winner == 1)
            fout << "1 - 0";
        else
            fout << "0 - 1";
    }

    fout.close();

    /** Update config file */

    fout.open("archive/settings.ini");
    fout.close();
    fout.open("archive/settings.ini");
    fout << "[Settings] \nNumber_of_files = " << current_id+1;
    fout.close();
}

void gameHandler::replay_game( const unsigned int id ){

    string s = "archive/game";

    /** Convert ID to game file . */

    int aux = id;
    int cifre[1000] , Ncifre=0;
    while(aux != 0){
        cifre[Ncifre] = aux%10;
        aux = aux / 10;
        Ncifre++;
    }
    for(int i = Ncifre-1 ; i >= 0 ; i--){
        s += cifre[i] + '0';
    }
    s += ".pgn";

    /** Attempt to open file. */

    ifstream fin(s.c_str());
    ofstream fout("archive/log.txt");

    string buffer , wmove , bmove;

    fin >> buffer >> buffer >> buffer >> buffer >> buffer >> buffer;    //Read redundant data

    NWmoves = 0;
    NBmoves = 0;

    while(!fin.eof()){
        fin >> buffer >> wmove >> bmove;

        if(wmove.length() > 1){
            unsigned pieceCode = 0;
            int startingCol=-1 , finalCol;
            unsigned startingLine , finalLine;

            if(wmove == "O-O"){

                Wmoves[NWmoves][0] = 7;
                Wmoves[NWmoves][1] = 7;
                Wmoves[NWmoves][2] = 4;
                Wmoves[NWmoves][3] = 6;
                NWmoves++;

            } else if(wmove == "O-O-O"){

                Wmoves[NWmoves][0] = 7;
                Wmoves[NWmoves][1] = 7;
                Wmoves[NWmoves][2] = 4;
                Wmoves[NWmoves][3] = 2;
                NWmoves++;

            } else if(wmove.length() > 2){
                if(wmove[0] == 'R')
                    pieceCode = 1;
                else if(wmove[0] == 'N')
                    pieceCode = 2;
                else if(wmove[0] == 'B')
                    pieceCode = 3;
                else if(wmove[0] == 'Q')
                    pieceCode = 4;
                else if(wmove[0] == 'K')
                    pieceCode = 5;
                else {
                    pieceCode = 6;

                    if(wmove.length() > 2)
                        startingCol = (wmove[0] - 'a');
                }

                if(wmove[1] != 'x'){
                    if(wmove[2] != 'x'){
                        //No starting col

                        finalCol = (wmove[1] - 'a');
                        finalLine = 8 - (wmove[2] - '0');

                    } else{
                        //Starting col

                        startingCol = (wmove[1] - 'a');
                        finalCol = (wmove[3] - 'a');
                        finalLine = 8 - (wmove[4] - '0');

                    }
                } else {

                    finalCol = (wmove[2] - 'a');
                    finalLine = 8 - (wmove[3] - '0');

                }
            } else if(wmove.length() == 2){

                finalCol = wmove[0] - 'a';
                finalLine = 8 - (wmove[1] - '0');
                pieceCode = 6;

            }

            //Got all data . Search for move
            GetPosMoves(1);

            for(int i = 0 ; i < NrMoves ; i++){
                if(board[AvailableMoves[i][0]][AvailableMoves[i][2]] == pieceCode){
                    //The piece required
                    if(startingCol == -1){
                        if(AvailableMoves[i][1] == finalLine && AvailableMoves[i][3] == finalCol){

                            Wmoves[NWmoves][0] = AvailableMoves[i][0];
                            Wmoves[NWmoves][1] = AvailableMoves[i][1];
                            Wmoves[NWmoves][2] = AvailableMoves[i][2];
                            Wmoves[NWmoves][3] = AvailableMoves[i][3];
                            NWmoves++;

                        }
                    } else {

                        if(AvailableMoves[i][2] == startingCol){
                            if(AvailableMoves[i][1] == finalLine && AvailableMoves[i][3] == finalCol){

                                Wmoves[NWmoves][0] = AvailableMoves[i][0];
                                Wmoves[NWmoves][1] = AvailableMoves[i][1];
                                Wmoves[NWmoves][2] = AvailableMoves[i][2];
                                Wmoves[NWmoves][3] = AvailableMoves[i][3];
                                NWmoves++;

                            }
                        }
                    }
                }
            }

            InterChange(Wmoves[NWmoves-1][0] , Wmoves[NWmoves-1][1] , Wmoves[NWmoves-1][2] , Wmoves[NWmoves-1][3]);
        }


        if(bmove.length() > 1){
            int pieceCode = 0;
            int startingCol=-1 , finalCol;
            unsigned startingLine , finalLine;

            if(bmove == "O-O"){

                Bmoves[NBmoves][0] = 0;
                Bmoves[NBmoves][1] = 0;
                Bmoves[NBmoves][2] = 4;
                Bmoves[NBmoves][3] = 6;
                NBmoves++;

            } else if(bmove == "O-O-O"){

                Bmoves[NBmoves][0] = 0;
                Bmoves[NBmoves][1] = 0;
                Bmoves[NBmoves][2] = 4;
                Bmoves[NBmoves][3] = 2;
                NBmoves++;

            } else if(bmove.length() > 2){
                if(bmove[0] == 'R')
                    pieceCode = -1;
                else if(bmove[0] == 'N')
                    pieceCode = -2;
                else if(bmove[0] == 'B')
                    pieceCode = -3;
                else if(bmove[0] == 'Q')
                    pieceCode = -4;
                else if(bmove[0] == 'K')
                    pieceCode = -5;
                else {
                    pieceCode = -6;

                    if(bmove.length() > 2)
                        startingCol = (bmove[0] - 'a');
                }

                if(bmove[1] != 'x'){
                    if(bmove[2] != 'x'){
                        //No starting col

                        finalCol = (bmove[1] - 'a');
                        finalLine = 8 - (bmove[2] - '0');

                    } else{
                        //Starting col

                        startingCol = (bmove[1] - 'a');
                        finalCol = (bmove[3] - 'a');
                        finalLine = 8 - (bmove[4] - '0');

                    }
                } else {

                    finalCol = (bmove[2] - 'a');
                    finalLine = 8 - (bmove[3] - '0');

                }
            } else if(bmove.length() == 2){

                finalCol = (bmove[0] - 'a');
                finalLine = 8 - (bmove[1] - '0');
                pieceCode = -6;

            }

            //Got all data . Search for move
            GetPosMoves(2);

            for(int i = 0 ; i < NrMoves ; i++){
                if(board[AvailableMoves[i][0]][AvailableMoves[i][2]] == pieceCode){
                    //The piece required
                    if(startingCol == -1){
                        if(AvailableMoves[i][1] == finalLine && AvailableMoves[i][3] == finalCol){

                            Bmoves[NBmoves][0] = AvailableMoves[i][0];
                            Bmoves[NBmoves][1] = AvailableMoves[i][1];
                            Bmoves[NBmoves][2] = AvailableMoves[i][2];
                            Bmoves[NBmoves][3] = AvailableMoves[i][3];
                            NBmoves++;

                        }
                    } else {

                        if(AvailableMoves[i][2] == startingCol){
                            if(AvailableMoves[i][1] == finalLine && AvailableMoves[i][3] == finalCol){

                                Bmoves[NBmoves][0] = AvailableMoves[i][0];
                                Bmoves[NBmoves][1] = AvailableMoves[i][1];
                                Bmoves[NBmoves][2] = AvailableMoves[i][2];
                                Bmoves[NBmoves][3] = AvailableMoves[i][3];
                                NBmoves++;

                            }
                        }
                    }
                }
            }

            InterChange(Bmoves[NBmoves-1][0] , Bmoves[NBmoves-1][1] , Bmoves[NBmoves-1][2] , Bmoves[NBmoves-1][3]);
        }
    }

}
