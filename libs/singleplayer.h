#ifndef AILIB_H_INCLUDED
#define AILIB_H_INCLUDED

namespace AIEngine {

    //Used to reset board to the start after a single-player match
    extern int TRUEreset[8][8];

    //Used to count current winner / player
    extern short Winner;
    extern short player;

    //Used for scoring purposes
    extern bool castledBlack;
    extern bool castledWhite;

    //Used to call the bot to calculate its next move
    void AI( int side , int difficulty );
}

#endif // AILIB_H_INCLUDED
