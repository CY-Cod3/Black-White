#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

/** The following handler will perform all operations with specific games : SAVE / LOAD / REPLAY / DELETE . */

class gameHandler {

    public:

        void replay_game( const unsigned int id );  //Used to replay a specific game
        void save_game();   //Used to save a game into its respective file

};

#endif // GAME_H_INCLUDED
