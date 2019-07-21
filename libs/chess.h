#ifndef CHESSRULES_H_INCLUDED
#define CHESSRULES_H_INCLUDED

namespace chessEngine {

    /** Here are definitions for all chess-related mechanics */

    //The board
    extern int board[8][8];

    //The position of the kings
    extern short KingWhitePos[2];
    extern short KingBlackPos[2];

    //The number of available moves
    extern unsigned NrMoves;

    //The list of available moves
    extern unsigned AvailableMoves[80][4];

    //If draw was requested
    extern bool DrawReq;

    //If we reached endgame
    extern bool Endgame;

    //The list of each player's moves
    extern unsigned Wmoves[130][4];
    extern unsigned Bmoves[130][4];

    //The number of each player's moves
    extern unsigned NWmoves;
    extern unsigned NBmoves;

    //Used to undo moves
    extern int Copy2[50][8][8];

    //Variables used to validate castling
    extern bool BCastlingWhite;
    extern bool SCastlingWhite;
    extern bool BCastlingBlack;
    extern bool SCastlingBlack;

    //Piece codes
    const int blackPawn = -6;
    const int blackRook = -1;
    const int blackKnight = -2;
    const int blackBishop = -3;
    const int blackQueen = -4;
    const int blackKing = -5;

    const int whitePawn = 6;
    const int whiteRook = 1;
    const int whiteKnight = 2;
    const int whiteBishop = 3;
    const int whiteQueen = 4;
    const int whiteKing = 5;

    //Functions used to operate with pieces
    void GetPosMoves( int player );
    void InterChange( int pos1L , int pos2L , int pos1C , int pos2C );
    void SaveCopy( int x );

    //Functions used to check for CHECK / PAWN PROMOTION
    bool isCheck( int posX , int posY , int Cplayer );
    bool CheckPromo( int player );

}

#endif // CHESSRULES_H_INCLUDED
