//Including libraries
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <algorithm>
#include <new>
#include <fstream>
#include <math.h>

#define _WIN32_WINNT 0x0500
#include <windows.h>

#include "libs/chess.h"
#include "libs/singleplayer.h"
#include "libs/button.h"
#include "libs/game.h"
#include "libs/label.h"
#include "libs/translator.h"

//Defining name-spaces
using namespace chessEngine;
using namespace AIEngine;
using namespace sf;
using namespace std;

//Board and Figure textures / sprites
Texture boardTexture , figureT;
Sprite boardI , figures[32];

//Variables used for the Draw Request button
short PressedByTheOther = 2;
short PlPressed = 0;

//Variable used to check if the 'Select Difficulty' screen is to be dispalyed
bool difSelect = false;

//The A.I. difficulty that has beed selected
int difficulty = 0;

//Used for the tutorial
int moves = 0;

//For one-time runs
bool oneTimers = false;

//Used to check if a player should promote a pawn
bool promotion = false;

//Used for replay
int Amoves = 0;
int B2moves = 0;
int total = 0;

//Tutorial progress
int tutorialStep = 0;

//Function that loads the textures of the board / pieces based on board
void LoadTextures()
{
    //Set the kings' initial positon
    KingWhitePos[0] = 7;
    KingWhitePos[1] = 4;
    KingBlackPos[0] = 0;
    KingBlackPos[1] = 4;

    //Load from file
    boardTexture.loadFromFile("res/images/board0.png");
    figureT.loadFromFile("res/images/figures.png");

    //Apply board texture
    boardI.setTexture(boardTexture);
    boardI.setPosition(100,20);
    boardI.setScale(1.5,1.5);

    //Number of pieces
    int k = 0;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            //For each piece set its texture

            int n = board[i][j];
            if(!n) continue;

            int x = abs(n)-1;
            int y = n>0?1:0;
            figures[k].setTextureRect( IntRect(56*x,56*y,56,56));
            figures[k].setTexture(figureT);
            figures[k].setPosition(85*j+115,85*i+35);
            k++;
        }
    }
}

//Function used to display board
void DrawBoard(RenderTarget& target)
{
    int k = 0;

    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            int n = board[i][j];
            if(!n) continue;

            int x = abs(n)-1;
            int y = n>0?1:0;
            figures[k].setTextureRect( IntRect(56*x,56*y,56,56));
            figures[k].setTexture(figureT);
            figures[k].setPosition(85*j+115,85*i+35);
            k++;
        }
    }
    for(int i = 0 ; i < k ; i++)
        target.draw(figures[i]);
}

//Function called when program starts and builds the menu
int main(){

    //Load textures
    LoadTextures();

    //Declaring variables

    //If the exit button was pressed
    bool Epressed = false;

    //If a move was made
    bool MoveMade = false;

    //Current screen to be displayed
    short Cscreen = 0;

    int n = -1;

    //Used to check if mouse is over any of the buttons
    bool Btn1Hover = false;
    bool Btn2Hover = false;
    bool Btn3Hover = false;
    bool Btn4Hover = false;
    bool Btn5Hover = false;
    bool Btn6Hover = false;
    bool Btn7Hover = false;
    bool Btn8Hover = false;
    bool Btn9Hover = false;
    bool Btn10Hover = false;
    bool Btn11Hover = false;
    bool Btn12Hover = false;
    bool DrawBtnHover = false;
    bool ResignBtnHover = false;
    bool prevHover = false;
    bool nxtHover = false;
    bool rookBtnHover = false;
    bool knightBtnHover = false;
    bool bishopBtnHover = false;
    bool queenBtnHover = false;
    bool AbtnHover = false;
    bool PbtnHover = false;
    bool saved = false;
    bool loadHover = false;
    bool backHover = false;
    bool back8Hover = false;
    bool pzHover[5];
    bool pzBackHover = false;

    int pzId = -1;

    int resultPart = 0;

    //Positions of the mouse
    float posX , posY;

    //Piece offset
    int offset = 100;

    //Coordiantes of new and old positon of the moving piecee
    int Xcoord, Ycoord;
    int PrevX , PrevY;

    languages Clang = EN;

    //Hide the console window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    //Declaring our window
    RenderWindow _window(VideoMode(900,900) , "ChessGame 2.0");

    Font font;

    //Our game handler
    gameHandler game;

    sf::String playerInput;

    if( !font.loadFromFile( "res/fonts/Raleway-Medium.ttf" ) )
        _window.close();

    //GameHandler
    while(_window.isOpen())
    {
        Event event;

        //Initialize MoveMade to 0
        MoveMade = false;

        //Calculate mouse coordinates
        posX = Mouse::getPosition(_window).x;
        posY = Mouse::getPosition(_window).y;

        //See if mouse is hover any of the buttons
        if(Cscreen == 0 && !difSelect){

            //Set promotion to false
            promotion = false;

            //Initialize totalMoves to 0
            total = 0;

            //Reset the board
            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    board[i][j] = TRUEreset[i][j];
                }
            }

            //Update the 'HOVERS'
            if((posX > 390 && posX < 390 + 195) && (posY > 150 && posY < 195))
                Btn1Hover = true;
            else
                Btn1Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 230 && posY < 275))
                Btn2Hover = true;
            else
                Btn2Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 310 && posY < 355))
                Btn12Hover = true;
            else
                Btn12Hover = false;

            if((posX > 430 && posX < 430+95) && (posY > 550 && posY < 595))
                Btn3Hover = true;
            else
                Btn3Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 390 && posY < 435))
                AbtnHover = true;
            else
                AbtnHover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 470 && posY < 515))
                PbtnHover = true;
            else
                PbtnHover = false;
        }
        else if(Cscreen == 1 && !difSelect){
            if((posX > 390 && posX < 390 + 195) && (posY > 150 + offset && posY < 195 + offset))
                Btn4Hover = true;
            else
                Btn4Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 230 + offset && posY < 275 + offset))
                Btn5Hover = true;
            else
                Btn5Hover = false;

            if((posX > 430 && posX < 430+95) && (posY > 310 + offset && posY < 355 + offset))
                Btn6Hover = true;
            else
                Btn6Hover = false;
        }
        else if(difSelect){
            if((posX > 390 && posX < 390 + 195) && (posY > 50 + offset && posY < 95 + offset))
                Btn7Hover = true;
            else
                Btn7Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 130 + offset && posY < 175 + offset))
                Btn8Hover = true;
            else
                Btn8Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 210 + offset && posY < 255 + offset))
                Btn9Hover = true;
            else
                Btn9Hover = false;

            if((posX > 390 && posX < 390 + 195) && (posY > 290 + offset && posY < 335 + offset))
                Btn10Hover = true;
            else
                Btn10Hover = false;

            if((posX > 435 && posX < 435 + 195) && (posY > 370 + offset && posY < 415 + offset))
                Btn11Hover = true;
            else
                Btn11Hover = false;
        }
        else if(Cscreen == 5 || Cscreen == 6){
            if((posX > 80 && posX < 195) && (posY > 700 + offset && posY < 745 + offset))
                prevHover = true;
            else
                prevHover = false;

            if((posX > 680 && posX < 855) && (posY > 700 + offset && posY < 745 + offset))
                nxtHover = true;
            else
                nxtHover = false;
        }
        else if(Cscreen == 7){
            if((posX > 100 && posX < 195) && (posY > 400 && posY < 445))
                backHover = true;
            else
                backHover = false;

            if((posX > 700 && posX < (Clang==EN)?795:825) && (posY > 400 && posY < 445))
                loadHover = true;
            else
                loadHover = false;
        }
        else if(Cscreen == 8){
            if(posX > 390 && posX < 390+195){
                for(int i = 0 ; i < 5 ; i++){
                    if(posY > 150+80*i && posY < 150+45+80*i){
                        pzHover[i] = true;
                    }
                    else
                        pzHover[i] = false;
                }

                if(posX > 445 && posX < 445+95){
                    if(posY > 550 && posY < 550+45){
                        back8Hover = true;
                    }
                    else{
                        back8Hover = false;
                    }
                }
            }
        }
        else if(Cscreen == 9){
            if(posX > 100 && posX < 195 && posY > 720 && posY < 765)
                pzBackHover = true;
            else
                pzBackHover = false;
        }
        else{
            if((posX > 420 && posX < 640) && (posY > 730 && posY < 775))
                DrawBtnHover = true;
            else
                DrawBtnHover = false;

            if((posX > 670 && posX < 790) && (posY > 730 && posY < 775))
                ResignBtnHover = true;
            else
                ResignBtnHover = false;

            if(promotion){
                if((posX > 110 && posX < 260) && (posY > 800 && posY < 845))
                    rookBtnHover = true;
                else
                    rookBtnHover = false;

                if((posX > 300 && posX < 450) && (posY > 800 && posY < 845))
                    knightBtnHover = true;
                else
                    knightBtnHover = false;

                if((posX > 490 && posX < 640) && (posY > 800 && posY < 845))
                    bishopBtnHover = true;
                else
                    bishopBtnHover = false;

                if((posX > 680 && posX < 830) && (posY > 800 && posY < 845))
                    queenBtnHover = true;
                else
                    queenBtnHover = false;
            }
        }

        //EventHandler
        while(_window.pollEvent(event))
        {
            //Close the window if ESCAPE / EXIT is pressed
            if(event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) || Epressed)
                _window.close();
            else if(event.type == Event::KeyPressed && event.key.code == Keyboard::Z && (Cscreen == 2 || Cscreen == 3 || Cscreen == 4)){
                //Undo last move

                if(total >= 1){
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = Copy2[total-1][i][j];
                        }
                    }

                    total--;

                    if(player == 2){
                        player = 1;
                        NBmoves--;
                    }
                    else{
                        player = 2;
                        NWmoves--;
                    }
                }
            }
            else if(event.type == Event::KeyPressed && event.key.code == Keyboard::X && (Cscreen == 2 || Cscreen == 3 || Cscreen == 4)){
                //Undo last set of moves

                if(total >= 2){
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = Copy2[total-2][i][j];
                        }
                    }

                    total-=2;

                    NWmoves--;
                    NBmoves--;
                }
            }
            else if(event.type == Event::TextEntered && Cscreen == 7){
                if(event.text.unicode != '\b' && event.text.unicode < 58 && event.text.unicode > 47)
                    playerInput += event.text.unicode;
                else if(event.text.unicode == '\b' && !playerInput.isEmpty()){
                    playerInput.erase(playerInput.getSize()-1,1);
                }
            }
            if(event.type == Event::MouseButtonPressed){
                //Handle mouse input

                if(event.key.code == Mouse::Left){
                     if(Cscreen == 0){
                        if((posX > 830 && posX < 880) && (posY > 10 && posY < 30)){
                            if(Clang == RO)
                                Clang = EN;
                            else if(Clang == EN)
                                Clang = RO;
                        }

                        if(Btn1Hover)
                            Cscreen = 1;
                        else if(Btn2Hover){
                            //Re-initialize variables

                            NWmoves =0;
                            NBmoves = 0;

                            Cscreen = 2;

                            player = 1;
                            Winner = 2;

                            SCastlingWhite = true;
                            BCastlingWhite = true;
                            SCastlingBlack = true;
                            BCastlingBlack = true;

                            castledWhite = false;
                            castledBlack = false;
                        }
                        else if(Btn3Hover)
                            Epressed = true;
                        else if(Btn12Hover){
                            tutorialStep = 0;
                            Cscreen = 5;
                        }
                        else if(AbtnHover)
                            Cscreen = 7;
                        else if(PbtnHover)
                            Cscreen = 8;
                    }
                    else if(Cscreen == 1){
                        if(Btn4Hover){
                            //Re-initialize variables

                            NWmoves = 0;
                            NBmoves = 0;

                            Cscreen = 3;
                            difSelect = true;

                            player = 1;
                            Winner = 2;

                            SCastlingWhite = true;
                            BCastlingWhite = true;
                            SCastlingBlack = true;
                            BCastlingBlack = true;

                            castledWhite = false;
                            castledBlack = false;
                        }
                        else if(Btn5Hover){
                            //Re-initialize variables

                            NWmoves = 0;
                            NBmoves = 0;

                            Cscreen = 4;
                            difSelect = true;

                            player = 1;
                            Winner = 2;

                            SCastlingWhite = true;
                            BCastlingWhite = true;
                            SCastlingBlack = true;
                            BCastlingBlack = true;

                            castledWhite = false;
                            castledBlack = false;
                        }
                        else if(Btn6Hover){
                            Btn6Hover = false;
                            Cscreen = 0;
                        }
                    }
                    else if(Cscreen == 7){
                        if(backHover)
                            Cscreen = 0;
                        if(loadHover){
                            Cscreen = 6;

                            Amoves = 0;
                            B2moves = 0;
                            total = 0;

                            string s = playerInput.toAnsiString();
                            unsigned id = 0;

                            for(int i = 0 ; i < s.length() ; i++){

                                id += (s[i] - '0') * pow(10 , s.length() - i - 1);

                            }

                            game.replay_game(id);

                            for(int i = 0 ; i < 8 ; i++){
                                for(int j = 0 ; j < 8 ; j++){
                                    board[i][j] = TRUEreset[i][j];
                                }
                            }

                        }
                    }
                    else if(Cscreen == 8){
                        if(back8Hover)
                            Cscreen = 0;

                        for(int i = 0 ; i < 5 ; i++){
                            if(pzHover[i]){
                                Cscreen = 9;
                                pzId = i;
                                moves = 0;
                            }
                        }
                    }
                    else if(Cscreen == 9 && pzBackHover){
                        if(moves == 0 && pzBackHover)
                            Cscreen = 8;
                        else if(pzBackHover){
                            moves = 0;
                            resultPart = 0;
                        }
                    }
                    else if(difSelect){
                        //Update difficulty and start game

                        if(Btn7Hover){
                            difficulty = 0;
                            difSelect = false;
                        }
                        else if(Btn8Hover){
                            difficulty = 1;
                            difSelect = false;
                        }
                        else if(Btn9Hover){
                            difficulty = 2;
                            difSelect = false;
                        }
                        else if(Btn10Hover){
                            difficulty = 3;
                            difSelect = false;
                        }
                        else if(Btn11Hover){
                            Cscreen = 1;
                            difSelect = false;
                        }
                    }
                    else if (!promotion){
                        if(Cscreen == 5){
                            //Update tutorial progress

                            if(prevHover){
                                if(tutorialStep == 0)
                                    Cscreen = 0;
                                else
                                    tutorialStep--;

                                moves = 0;
                            }

                            if(nxtHover){
                                if(tutorialStep == 28)
                                    Cscreen = 0;
                                else
                                    tutorialStep++;

                                moves = 0;
                            }
                        }
                        else if(Cscreen == 6){
                            //Match replay

                            if(prevHover && (Amoves == 0 && B2moves == 0)){
                                Cscreen = 0;

                                Amoves = 0;
                                B2moves = 0;

                                NWmoves = 0;
                                total = 0;
                                NBmoves = 0;

                                Winner = 2;
                                player = 1;

                                Endgame = false;
                                DrawReq = false;
                            }
                            else if(prevHover){
                                for(int i = 0 ; i < 8 ; i++){
                                    for(int j = 0  ; j < 8 ; j++){
                                        board[i][j] = Copy2[total-1][i][j];
                                    }
                                }

                                if(player == 2){
                                    if(Amoves >= 1)
                                        Amoves--;
                                    player = 1;
                                }
                                else if(player == 1){
                                    if(B2moves >= 1)
                                        B2moves--;
                                    player = 2;
                                }
                                total--;
                            }

                            if(nxtHover && Amoves == NWmoves && B2moves == NBmoves){
                                Amoves = 0;
                                B2moves = 0;

                                NWmoves = 0;
                                total = 0;
                                NBmoves = 0;

                                Cscreen = 0;

                                Winner = 2;
                                player = 1;

                                Endgame = false;
                                DrawReq = false;
                            }
                            else if(nxtHover){
                                if(!oneTimers)
                                    SaveCopy(total);
                                oneTimers = true;

                                if(player == 2){
                                    InterChange(Bmoves[B2moves][0],Bmoves[B2moves][1],Bmoves[B2moves][2],Bmoves[B2moves][3]);

                                    player = 1;
                                    B2moves++;
                                }
                                else if(player == 1){
                                    InterChange(Wmoves[Amoves][0],Wmoves[Amoves][1],Wmoves[Amoves][2],Wmoves[Amoves][3]);

                                    player = 2;
                                    Amoves++;
                                }

                                oneTimers = false;
                                total++;
                            }
                        }

                        n = -1; //Selected piece ID
                        for(int i = 0 ; i < 32 ; i++){
                            //Look if Any piece is to be moved

                            if(figures[i].getGlobalBounds().contains(posX,posY))
                            {

                                //Update selected piece
                                n = i;

                                //Get its old positions
                                PrevY = (figures[i].getPosition().x - 115)/ 85;
                                PrevX = (figures[i].getPosition().y - 35) / 85;
                            }
                        }

                        //Update end-game
                        if(DrawBtnHover && DrawReq && player == PressedByTheOther && !Endgame)
                            Endgame = true;
                        else if(ResignBtnHover && !Endgame){
                            Endgame = true;
                            DrawReq = false;
                        }
                        else if(DrawBtnHover && !Endgame){
                            DrawReq = true;
                            PressedByTheOther = Winner;
                        }
                        else if(Endgame && ResignBtnHover && Cscreen != 6){
                            //Re-initialize variables on end-game

                            Cscreen = 0;

                            Amoves = 0;
                            B2moves = 0;

                            player = 1;
                            PlPressed = 0;
                            Winner = 2;

                            Endgame = false;
                            DrawReq = false;
                        }
                        else if(Endgame && DrawBtnHover && Cscreen != 6){
                            //Replay
                            for(int i = 0 ; i < 8 ; i++){
                                for(int j = 0 ; j < 8 ; j++){
                                    board[i][j] = TRUEreset[i][j];
                                }
                            }

                            Cscreen = 6;

                            Amoves = 0;
                            B2moves = 0;
                            total = 0;

                            DrawReq = false;
                            Endgame = false;

                            player = 1;
                        }

                        if(PlPressed > 2){
                            PlPressed = 0;
                            DrawReq = false;
                        }
                    }
                    else if(promotion){
                        int NewCode = 0;

                        if(rookBtnHover)
                            NewCode = 1;
                        else if(knightBtnHover)
                            NewCode = 2;
                        else if(bishopBtnHover)
                            NewCode = 3;
                        else if(queenBtnHover)
                            NewCode = 4;

                        if(player == 1)
                            NewCode *= -1;

                        board[Xcoord][Ycoord] = NewCode;
                        promotion = false;
                    }
                }
            }
            if(event.type == Event::MouseButtonReleased){
                if(event.key.code == Mouse::Left){
                    if(n >= 0){
                        //When mouse released get new coordinates

                        Ycoord = (posX - 115)/ 85;
                        Xcoord = (posY - 35) / 85;

                        MoveMade = false;
                        for(int k = 0 ; k < NrMoves ; k++){
                            if(AvailableMoves[k][0] == PrevX && AvailableMoves[k][1] == Xcoord){
                                if(AvailableMoves[k][2] == PrevY && AvailableMoves[k][3] == Ycoord){
                                    //Check if the move is valid and update castling conditions if needed

                                    if(player == 1){
                                        if(board[PrevX][PrevY] == 1){
                                            if(PrevX == 7 && PrevY == 0){
                                                BCastlingWhite = false;
                                            }
                                            else if(PrevX == 7 && PrevY == 7){
                                                SCastlingWhite = false;
                                            }
                                        }
                                        else if(board[PrevX][PrevY] == 5){
                                            if(PrevX == 7 && PrevY == 4){
                                                if(Xcoord != 7){
                                                    SCastlingWhite = false;
                                                    BCastlingWhite = false;
                                                }
                                                else{
                                                    if(Ycoord != 2 && Ycoord != 6){
                                                        SCastlingWhite = false;
                                                        BCastlingWhite = false;
                                                    }
                                                    else if(Ycoord == 2){
                                                        SCastlingWhite = false;
                                                        BCastlingWhite = false;
                                                        castledWhite = true;
                                                    }
                                                    else if(Ycoord == 6){
                                                        SCastlingWhite = false;
                                                        BCastlingWhite = false;
                                                        castledWhite = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else{
                                        if(board[PrevX][PrevY] == -1){
                                            if(PrevX == 0 && PrevY == 0){
                                                BCastlingBlack = false;
                                            }
                                            else if(PrevX == 0 && PrevY == 7){
                                                SCastlingBlack = false;
                                            }
                                        }
                                        else if(board[PrevX][PrevY] == -5){
                                            if(PrevX == 0 && PrevY == 4){
                                                if(Xcoord != 0){
                                                    SCastlingBlack = false;
                                                    BCastlingBlack = false;
                                                }
                                                else if(Xcoord == 0 && (Ycoord != 2 && Ycoord != 6)){
                                                    SCastlingBlack = false;
                                                    BCastlingBlack = false;
                                                }
                                                else{
                                                    if(Ycoord == 2){
                                                        SCastlingBlack = false;
                                                        BCastlingBlack = false;
                                                        castledBlack =true;
                                                    }
                                                    else if(Ycoord == 6){
                                                        SCastlingBlack = false;
                                                        BCastlingBlack = false;
                                                        castledBlack = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if(player == 1){
                                        Wmoves[NWmoves][0] = PrevX;
                                        Wmoves[NWmoves][1] = Xcoord;
                                        Wmoves[NWmoves][2] = PrevY;
                                        Wmoves[NWmoves][3] = Ycoord;
                                        NWmoves++;
                                    }
                                    else{
                                        Bmoves[NBmoves][0] = PrevX;
                                        Bmoves[NBmoves][1] = Xcoord;
                                        Bmoves[NBmoves][2] = PrevY;
                                        Bmoves[NBmoves][3] = Ycoord;
                                        NBmoves++;
                                    }

                                    //Save copy
                                    if(!oneTimers)
                                        SaveCopy(total);
                                    oneTimers = true;
                                    total++;

                                    //Make move
                                    InterChange(PrevX,Xcoord,PrevY,Ycoord);
                                    MoveMade = true;
                                    oneTimers = false;
                                }
                            }
                        }
                    }
                }
            }
        }

        ///Clear and update window
        _window.clear();

        if(Cscreen >= 2 && Cscreen != 5){
            //Get possible moves
            GetPosMoves(player);

            //Look for end-game conditions

            if(NrMoves == 0){
                if(player == 1){
                    if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                        Endgame = true;
                        DrawReq = false;
                    }
                    else{
                        Endgame = true;
                        DrawReq = true;
                    }
                }
                else{
                    if(isCheck(KingBlackPos[0] , KingBlackPos[1] , 2)){
                        Endgame = true;
                        DrawReq = false;
                    }
                    else{
                        Endgame = true;
                        DrawReq = true;
                    }
                }
            }

            int ok5 = 0;
            if(NWmoves + NBmoves >= 6 && NWmoves >= 3 && NBmoves >= 3){
                int contor = 0;
                for(int i = 0 ; i < min(NWmoves,NBmoves) ; i++){
                    contor = 0;
                    if(i+4 < NWmoves){
                        for(int j = 0 ; j < 4 ; j++){
                            if(Wmoves[i][j] == Wmoves[i+2][j] && Wmoves[i+4][j] == Wmoves[i][j]){
                                contor++;
                            }
                            if(i+2 < NBmoves && Bmoves[i][j] == Bmoves[i+2][j]){
                                contor++;
                            }
                        }
                        if(contor == 8){
                            ok5 = 1;
                        }
                    }
                    if(i+4 < NBmoves){
                        for(int j = 0 ; j < 4 ; j++){
                            if(Bmoves[i][j] == Bmoves[i+2][j] && Bmoves[i][j] == Bmoves[i+4][j]){
                                contor++;
                            }
                            if(i+2 < NWmoves && Wmoves[i][j] == Wmoves[i+2][j]){
                                contor++;
                            }
                        }
                        if(contor == 8 && ok5){
                            Endgame = true;
                            DrawReq = true;
                        }
                    }
                }
            }

            int DNR = 0;
            for(int i = 0 ; i < 8 ; i++){
                for(int j = 0 ; j < 8 ; j++){
                    if(board[i][j] == 3 || board[i][j] == -3){
                        DNR++;
                    }
                    else if(board[i][j] != 3 && board[i][j] != -3 && board[i][j] != 5 && board[i][j] != -5 && board[i][j] != 0){
                        DNR = 0;
                        break;
                    }
                }
            }

            if(DNR > 1){
                Endgame = true;
                DrawReq = true;
            }
            else{
                if(DNR == 0){
                    int DRR = 0;
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            if(board[i][j] != 5 && board[i][j] != -5 && board[i][j] != 0){
                                DRR = 1;
                            }
                        }
                    }
                    if(!DRR){
                        Endgame = true;
                        DrawReq = true;
                    }
                    else{
                        int DCR = 0;
                        for(int i = 0 ; i < 8 ; i++){
                            for(int j = 0 ; j < 8 ; j++){
                                if(board[i][j] != 2 && board[i][j] != -2 && board[i][j]  != 5 && board[i][j] != -5 && board[i][j] != 0){
                                    DCR = 1;
                                }
                            }
                        }
                        if(!DCR){
                            Endgame = true;
                            DrawReq = true;
                        }
                    }
                }
            }

            if(Endgame && !saved){
                game.save_game();
                saved = true;
            }
        }

        if(Cscreen == 0){
            saved = false;

            //Main menu
            Button Tutorial , Btn1 , Btn2 , Btn3 , Abtn , Pbtn;
            Label Title , Btn1Txt , Btn2Txt , Btn3Txt , TutorialTxt , Atxt , Ptxt, langTxt;

            ///LANGUAGE
            if(Clang == EN)
                langTxt.create(830,10,30,"EN",font);
            else if(Clang == RO)
                langTxt.create(830,10,30,"RO",font);

            ///TITLE
            Title.create( 250 , 50 , 48 , translateText( Clang , s0_title ) , font );

            ///Button 1 Text
            Btn1Txt.create( (Clang==EN)?400:410 , 150 , 30 , translateText( Clang , s0_button01 ) , font );

            ///Button 2 Text
            Btn2Txt.create( 410 , 230 , 30 , translateText( Clang , s0_button02 ) , font );

            ///Button 3 Text
            Btn3Txt.create( (Clang==EN)?450:440 , 554 , 30 , translateText( Clang , s0_button06 ) , font );

            ///Tutorial Text
            TutorialTxt.create( 430 , 314 , 30 , translateText( Clang , s0_button03 ) , font );

            ///Archives Text
            Atxt.create( (Clang==EN)?430:440 , 394 , 30 , translateText( Clang , s0_button04 ) , font );

            ///Puzzles Text
            Ptxt.create( (Clang==EN)?440:420 , 474 , 30 , translateText( Clang , s0_button05 ) , font);

            ///Tutorial Button
            Tutorial.create( 390 , 310 , 195 , 45 );
            Tutorial.update( Btn12Hover );

            ///Button 1
            Btn1.create( 390 , 150 , 195 , 45 );
            Btn1.update( Btn1Hover );

            ///Button 2
            Btn2.create( 390 , 230 , 195 , 45 );
            Btn2.update( Btn2Hover );

            ///Button 3
            Btn3.create( 430 , 550 , 95 , 45 );
            Btn3.update( Btn3Hover );

            ///Archives button
            Abtn.create( 390 , 390 , 195 , 45 );
            Abtn.update( AbtnHover );

            ///Puzzles button
            Pbtn.create( 390 , 470 , 195 , 45 );
            Pbtn.update( PbtnHover );

            ///DRAW EVERYTHING
            _window.draw(Title);

            _window.draw(Btn1);
            _window.draw(Btn1Txt);

            _window.draw(Btn2);
            _window.draw(Btn2Txt);

            _window.draw(Tutorial);
            _window.draw(TutorialTxt);

            _window.draw(Btn3);
            _window.draw(Btn3Txt);

            _window.draw(Abtn);
            _window.draw(Atxt);

            _window.draw(Pbtn);
            _window.draw(Ptxt);

            _window.draw(langTxt);

        }
        else if(Cscreen == 1){
            ///Second menu
            Label Title2 , Btn4Txt , Btn5Txt , Btn6Txt;
            Button Btn4 , Btn5 , Btn6;

            ///TITLE 2
            Title2.create( 300 , 50 , 48 , translateText( Clang , s1_title ) , font );

            ///Button 4 Text
            Btn4Txt.create( (Clang==EN)?450:465 , 150+offset , 30 , translateText( Clang , s1_button01 ) , font );

            ///Button 5 Text
            Btn5Txt.create( 445 , 230+offset , 30 , translateText( Clang , s1_button02 ) , font );

            ///Button 6 Text
            Btn6Txt.create( (Clang==EN)?445:433 , 314+offset , 30 , translateText( Clang , s1_button03 ) , font );

            ///Button 4
            Btn4.create( 390 , 150+offset , 195 , 45 );
            Btn4.update( Btn4Hover );

            ///Button 5
            Btn5.create( 390 , 230+offset , 195 , 45 );
            Btn5.update( Btn5Hover );

            ///Button 6
            Btn6.create( 430 , 310+offset , 95 , 45 );
            Btn6.update( Btn6Hover );

            ///DRAW EVERYTHING
            _window.draw(Title2);
            _window.draw(Btn4);
            _window.draw(Btn4Txt);

            _window.draw(Btn5);
            _window.draw(Btn5Txt);

            _window.draw(Btn6);
            _window.draw(Btn6Txt);
        }
        else if(Cscreen == 2){
            //Multi-player

            ///ASK PLAYER TO PROMOTE THEIR PAWN IF POSSIBLE
            if(CheckPromo(player)){
                promotion = true;
            }

            ///Draw the board
            _window.draw(boardI);

            ///WARN THE PLAYER HE IS IN CHECK.
            RectangleShape Warn;
            Warn.setFillColor(Color::Red);
            Warn.setSize(Vector2f(70,70));

            if(player == 1){
                if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                    Warn.setPosition(85 * KingWhitePos[1] + 105 , 85 * KingWhitePos[0] + 30);
                    _window.draw(Warn);
                }
            }
            else{
                if(isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                    Warn.setPosition(85 * KingBlackPos[1] + 105 , 85 * KingBlackPos[0] + 30);
                    _window.draw(Warn);
                }
            }

            Text CPlayer , ResignBtnT , DrawBtnT , DrawTxt , WinTxt;
            RectangleShape ResignBtn , DrawBtn;

            ///SET FONTS
            CPlayer.setFont(font);
            ResignBtnT.setFont(font);
            DrawBtnT.setFont(font);
            WinTxt.setFont(font);
            DrawTxt.setFont(font);

            ///DISPLAY CURRENT PLAYER'S TURN
            if(player == 1 && !promotion){
                CPlayer.setString( translateText( Clang , g_cplayer[0] ) );
            }
            else if(player == 2 && !promotion){
                CPlayer.setString( translateText( Clang , g_cplayer[1] ) );
            }
            else if(promotion){
                Label rookTxt , knightTxt , bishopTxt , queenTxt;
                Button rookBtn , knightBtn , bishopBtn , queenBtn;

                CPlayer.setString( translateText( Clang , prom_title ) );

                rookTxt.create( 150 , 804 , 30 , translateText(Clang,prom_buttons[0]) , font );
                knightTxt.create( 330 , 804 , 30 , translateText(Clang,prom_buttons[1]) , font );
                bishopTxt.create( 515 , 804 , 30 , translateText(Clang,prom_buttons[2]) , font );
                queenTxt.create( 715 , 804 , 30 , translateText(Clang,prom_buttons[3]) , font );

                rookBtn.create( 110 , 800 , 150 , 45 );
                rookBtn.update( rookBtnHover );

                knightBtn.create( 300 , 800 , 150 , 45 );
                knightBtn.update( knightBtnHover );

                bishopBtn.create( 490 , 800 , 150 , 45 );
                bishopBtn.update( bishopBtnHover );

                queenBtn.create( 680 , 800 , 150 , 45 );
                queenBtn.update( queenBtnHover );

                _window.draw(rookBtn);
                _window.draw(bishopBtn);
                _window.draw(knightBtn);
                _window.draw(queenBtn);

                _window.draw(rookTxt);
                _window.draw(bishopTxt);
                _window.draw(knightTxt);
                _window.draw(queenTxt);
            }

            CPlayer.setCharacterSize(30);
            CPlayer.setColor(Color::White);
            CPlayer.setPosition(110,730);

            ///RESIGN BUTTON TEXT
            ResignBtnT.setString( translateText( Clang , g_button02[0] ) );
            ResignBtnT.setCharacterSize(30);
            ResignBtnT.setColor(Color::White);
            ResignBtnT.setPosition(680,730);

            ///RESIGN BUTTON
            ResignBtn.setPosition(670,730);
            ResignBtn.setSize(Vector2f(120,45));

            if(ResignBtnHover)
                ResignBtn.setFillColor(Color::Red);
            else
                ResignBtn.setFillColor(Color(168,168,57));

            ///REQUEST DRAW BUTTON TEXT
            if(DrawReq && player == PressedByTheOther && DrawReq)
                DrawBtnT.setString( translateText( Clang , g_button01[1] ) );
            else
                DrawBtnT.setString( translateText( Clang , g_button01[0] ) );
            DrawBtnT.setCharacterSize(30);
            DrawBtnT.setColor(Color::White);
            DrawBtnT.setPosition(430,730);

            ///REQUEST BUTTON
            DrawBtn.setPosition(420,730);
            DrawBtn.setSize(Vector2f(220,45));

            if(DrawBtnHover)
                DrawBtn.setFillColor(Color::Red);
            else
                DrawBtn.setFillColor(Color(168,168,57));

            ///CHECK FOR ENDGAME SITUATIONS
            if(Endgame && !DrawReq){
                if(Winner == 1){
                    CPlayer.setString( translateText( Clang , g_cplayer[2] ) );
                }
                else if(Winner == 2){
                    CPlayer.setString( translateText( Clang , g_cplayer[3] ) );
                }
            }
            else if(Endgame && DrawReq){
                CPlayer.setString( translateText( Clang , g_cplayer[4] ) );
            }

            if(Endgame){
                DrawBtnT.setString( translateText( Clang , g_button01[2] ) );
                ResignBtnT.setString( translateText( Clang , g_button02[1] ) );
                ResignBtnT.setPosition(700,730);
            }

            ///DRAW EVERYTHING
            _window.draw(CPlayer);

            _window.draw(ResignBtn);
            _window.draw(ResignBtnT);

            _window.draw(DrawBtn);
            _window.draw(DrawBtnT);

            DrawBoard(_window);

            if(player == 1 && MoveMade){
                Winner = 1;
                player = 2;
                PlPressed++;
            }
            else if (MoveMade){
                Winner = 2;
                player = 1;
                PlPressed++;
            }
        }
        else if(Cscreen == 3 && !difSelect){
            //Vs AI WHITE

            Text ResignBtnT , DrawBtnT , CPlayer;
            RectangleShape ResignBtn , DrawBtn;

            CPlayer.setFont(font);
            ResignBtnT.setFont(font);
            DrawBtnT.setFont(font);

            CPlayer.setCharacterSize(30);
            CPlayer.setColor(Color::White);
            CPlayer.setPosition(110,730);

            ///Draw the board
            _window.draw(boardI);

            ///WARN THE PLAYER HE IS IN CHECK.
            RectangleShape Warn;
            Warn.setFillColor(Color::Red);
            Warn.setSize(Vector2f(70,70));

            if(player == 1){
                if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                    Warn.setPosition(85 * KingWhitePos[1] + 105 , 85 * KingWhitePos[0] + 30);
                    _window.draw(Warn);
                }
            }
            else{
                if(isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                    Warn.setPosition(85 * KingBlackPos[1] + 105 , 85 * KingBlackPos[0] + 30);
                    _window.draw(Warn);
                }
            }

            if(CheckPromo(1)){
                promotion = true;
            }

            if(promotion){
                Label rookTxt , knightTxt , bishopTxt , queenTxt;
                Button rookBtn , knightBtn , bishopBtn , queenBtn;

                CPlayer.setString(translateText(Clang,prom_title));

                rookTxt.create( 150 , 804 , 30 , translateText(Clang,prom_buttons[0]) , font );
                knightTxt.create( 330 , 804 , 30 , translateText(Clang,prom_buttons[1]) , font );
                bishopTxt.create( 515 , 804 , 30 , translateText(Clang,prom_buttons[2]) , font );
                queenTxt.create( 715 , 804 , 30 , translateText(Clang,prom_buttons[3]) , font );

                rookBtn.create( 110 , 800 , 150 , 45 );
                rookBtn.update( rookBtnHover );

                knightBtn.create( 300 , 800 , 150 , 45 );
                knightBtn.update( knightBtnHover );

                bishopBtn.create( 490 , 800 , 150 , 45 );
                bishopBtn.update( bishopBtnHover );

                queenBtn.create( 680 , 800 , 150 , 45 );
                queenBtn.update( queenBtnHover );

                _window.draw(rookBtn);
                _window.draw(bishopBtn);
                _window.draw(knightBtn);
                _window.draw(queenBtn);

                _window.draw(rookTxt);
                _window.draw(bishopTxt);
                _window.draw(knightTxt);
                _window.draw(queenTxt);
            }

            ///RESIGN BUTTON TEXT
            ResignBtnT.setString( translateText( Clang , g_button02[0] ) );
            ResignBtnT.setCharacterSize(30);
            ResignBtnT.setColor(Color::White);
            ResignBtnT.setPosition(680,730);

            ///RESIGN BUTTON
            ResignBtn.setPosition(670,730);
            ResignBtn.setSize(Vector2f(120,45));

            if(ResignBtnHover)
                ResignBtn.setFillColor(Color::Red);
            else
                ResignBtn.setFillColor(Color(168,168,57));

            ///REQUEST DRAW BUTTON TEXT
            DrawBtnT.setString( translateText( Clang , g_button01[0] ) );
            DrawBtnT.setCharacterSize(30);
            DrawBtnT.setColor(Color::White);
            DrawBtnT.setPosition(430,730);

            ///REQUEST BUTTON
            DrawBtn.setPosition(420,730);
            DrawBtn.setSize(Vector2f(220,45));

            if(DrawBtnHover)
                DrawBtn.setFillColor(Color::Red);
            else
                DrawBtn.setFillColor(Color(168,168,57));

            if(Endgame && !DrawReq){
                if(Winner == 1){
                    CPlayer.setString( translateText( Clang , g_cplayer[2] ) );
                }
                else if(Winner == 2){
                    CPlayer.setString( translateText( Clang , g_cplayer[3] ) );
                }
            }
            else if(Endgame && DrawReq){
                CPlayer.setString( translateText( Clang , g_cplayer[4] ) );
            }

            if(Endgame){
                DrawBtnT.setString( translateText( Clang , g_button01[2] ) );
                ResignBtnT.setString( translateText( Clang , g_button02[1] ) );
                ResignBtnT.setPosition(700,730);

                _window.draw(CPlayer);
            }

            ///DRAW EVERYTHING
            _window.draw(ResignBtn);
            _window.draw(ResignBtnT);

            _window.draw(DrawBtn);
            _window.draw(DrawBtnT);

            DrawBoard(_window);

            if(player == 1 && MoveMade){
                Winner = 1;
                if(DrawReq)
                    PlPressed++;

                player = 2;
            }
            else if(player == 2 && !Endgame){
                SaveCopy(total);
                total++;
                AI(2,difficulty);
                if(DrawReq)
                    PlPressed++;
            }
        }
        else if(Cscreen == 4 && !difSelect){
            //Vs AI BLACK

            Text ResignBtnT , DrawBtnT , CPlayer;
            RectangleShape ResignBtn , DrawBtn;

            ResignBtnT.setFont(font);
            CPlayer.setFont(font);
            DrawBtnT.setFont(font);

            CPlayer.setCharacterSize(30);
            CPlayer.setColor(Color::White);
            CPlayer.setPosition(110,730);

            ///Draw the board
            _window.draw(boardI);

            ///WARN THE PLAYER HE IS IN CHECK.
            RectangleShape Warn;
            Warn.setFillColor(Color::Red);
            Warn.setSize(Vector2f(70,70));

            if(player == 1){
                if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                    Warn.setPosition(85 * KingWhitePos[1] + 105 , 85 * KingWhitePos[0] + 30);
                    _window.draw(Warn);
                }
            }
            else{
                if(isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                    Warn.setPosition(85 * KingBlackPos[1] + 105 , 85 * KingBlackPos[0] + 30);
                    _window.draw(Warn);
                }
            }

            ///RESIGN BUTTON TEXT
            ResignBtnT.setString( translateText( Clang , g_button02[0] ) );
            ResignBtnT.setCharacterSize(30);
            ResignBtnT.setColor(Color::White);
            ResignBtnT.setPosition(680,730);

            ///RESIGN BUTTON
            ResignBtn.setPosition(670,730);
            ResignBtn.setSize(Vector2f(120,45));

            if(ResignBtnHover)
                ResignBtn.setFillColor(Color::Red);
            else
                ResignBtn.setFillColor(Color(168,168,57));

            if(CheckPromo(2)){
                promotion = true;
            }

            else if(promotion){
                Label rookTxt , knightTxt , bishopTxt , queenTxt;
                Button rookBtn , knightBtn , bishopBtn , queenBtn;

                CPlayer.setString(translateText(Clang,prom_title));

                rookTxt.create( 150 , 804 , 30 , translateText(Clang,prom_buttons[0]) , font );
                knightTxt.create( 330 , 804 , 30 , translateText(Clang,prom_buttons[1]) , font );
                bishopTxt.create( 515 , 804 , 30 , translateText(Clang,prom_buttons[2]) , font );
                queenTxt.create( 715 , 804 , 30 , translateText(Clang,prom_buttons[3]) , font );

                rookBtn.create( 110 , 800 , 150 , 45 );
                rookBtn.update( rookBtnHover );

                knightBtn.create( 300 , 800 , 150 , 45 );
                knightBtn.update( knightBtnHover );

                bishopBtn.create( 490 , 800 , 150 , 45 );
                bishopBtn.update( bishopBtnHover );

                queenBtn.create( 680 , 800 , 150 , 45 );
                queenBtn.update( queenBtnHover );

                _window.draw(rookBtn);
                _window.draw(bishopBtn);
                _window.draw(knightBtn);
                _window.draw(queenBtn);

                _window.draw(rookTxt);
                _window.draw(bishopTxt);
                _window.draw(knightTxt);
                _window.draw(queenTxt);
            }

            ///REQUEST DRAW BUTTON TEXT
            DrawBtnT.setString( translateText( Clang , g_button01[0] ) );
            DrawBtnT.setCharacterSize(30);
            DrawBtnT.setColor(Color::White);
            DrawBtnT.setPosition(430,730);

            ///REQUEST BUTTON
            DrawBtn.setPosition(420,730);
            DrawBtn.setSize(Vector2f(220,45));

            if(DrawBtnHover)
                DrawBtn.setFillColor(Color::Red);
            else
                DrawBtn.setFillColor(Color(168,168,57));

            if(Endgame && !DrawReq){
                if(Winner == 1){
                    CPlayer.setString( translateText( Clang , g_cplayer[2] ) );
                }
                else if(Winner == 2){
                    CPlayer.setString( translateText( Clang , g_cplayer[3] ) );
                }
            }
            else if(Endgame && DrawReq){
                CPlayer.setString( translateText( Clang , g_cplayer[4] ) );
            }

            if(Endgame){
                DrawBtnT.setString( translateText( Clang , g_button01[2]));
                ResignBtnT.setString( translateText( Clang , g_button02[1]));
                ResignBtnT.setPosition(700,730);

                _window.draw(CPlayer);
            }

            ///DRAW EVERYTHING
            _window.draw(ResignBtn);
            _window.draw(ResignBtnT);

            _window.draw(DrawBtn);
            _window.draw(DrawBtnT);

            DrawBoard(_window);

            if(player == 2 && MoveMade){
                Winner = 2;
                if(DrawReq)
                    PlPressed++;
                player = 1;
            }
            else if(player == 1 && !Endgame){
                SaveCopy(total);
                total++;
                AI(1,difficulty);
                if(DrawReq)
                    PlPressed++;
            }
        }
        else if(difSelect){
            ///Select difficulty
            Label Title , dif1T , dif2T , dif3T , dif4T , backT;
            Button dif1 , dif2 , dif3 , dif4 , backBtn;

            ///TEXTS
            dif1T.create( 420 , 54+offset , 30 , translateText(Clang,dif_buttons[0]) , font );
            dif2T.create( 455 , 134+offset , 30 , translateText(Clang,dif_buttons[1]) , font );
            dif3T.create( 430 , 214+offset , 30 , translateText(Clang,dif_buttons[2]) , font );
            dif4T.create( 445 , 294+offset , 30 , translateText(Clang,dif_buttons[3]) , font );

            backT.create( (Clang==EN)?450:440 , 374+offset , 30 , translateText( Clang , s1_button03 ) , font );

            Title.create( 250 , 50 , 48 , translateText(Clang,dif_title) , font );

            ///FIRST DIFFICULTY
            dif1.create( 390 , 50+offset , 195 , 45 );
            dif1.update( Btn7Hover );

            ///SECOND DIFFICULTY
            dif2.create( 390 , 130+offset , 195 , 45 );
            dif2.update( Btn8Hover );

            ///THIRD DIFFICULTY
            dif3.create( 390 , 210+offset , 195 , 45 );
            dif3.update( Btn9Hover );

            ///LAST DIFFICULTY
            dif4.create( 390 , 290+offset , 195 , 45 );
            dif4.update( Btn10Hover );

            ///BACK BUTTON
            backBtn.create( 435 , 370+offset , 95 , 45 );
            backBtn.update( Btn11Hover );

            _window.clear();

            _window.draw(Title);

            _window.draw(dif1);
            _window.draw(dif2);
            _window.draw(dif3);
            _window.draw(dif4);
            _window.draw(backBtn);

            _window.draw(dif1T);
            _window.draw(dif2T);
            _window.draw(dif3T);
            _window.draw(dif4T);
            _window.draw(backT);

        }
        else if(Cscreen == 5){
            Text Title , nxtTxt , prevTxt;
            RectangleShape nxtBtn , prevBtn , showCase[32];

            Title.setFont(font);
            Title.setString("Tutorial");
            Title.setColor(Color::White);
            Title.setCharacterSize(30);
            Title.setPosition(100,720);

            //Display the tutorial messages
            Title.setString(translateText(Clang,t_info[tutorialStep]));

            nxtTxt.setFont(font);
            nxtTxt.setString(translateText(Clang,r_button02));
            nxtTxt.setColor(Color::White);
            nxtTxt.setCharacterSize(30);
            nxtTxt.setPosition(700,800);

            prevTxt.setFont(font);
            prevTxt.setString(translateText(Clang,s1_button03));
            prevTxt.setColor(Color::White);
            prevTxt.setCharacterSize(30);
            prevTxt.setPosition(100,800);

            prevBtn.setPosition(80,700 + offset);

            if(prevHover)
                prevBtn.setFillColor(Color::Red);
            else
                prevBtn.setFillColor(Color(168,168,57));

            prevBtn.setSize(Vector2f(115,45));

            nxtBtn.setPosition(680,700 + offset);

            if(nxtHover)
                nxtBtn.setFillColor(Color::Red);
            else
                nxtBtn.setFillColor(Color(168,168,57));

            nxtBtn.setSize(Vector2f(175,45));

            _window.clear();

            _window.draw(Title);
            _window.draw(boardI);

            _window.draw(nxtBtn);
            _window.draw(nxtTxt);

            _window.draw(prevBtn);
            _window.draw(prevTxt);

            //Do something different based on each tutorial step
            if(tutorialStep >= 1 && tutorialStep < 9){
                for(int i = 0 ; i < 8 ; i++){
                    for(int j = 0 ; j < 8 ; j++){
                        board[i][j] = TRUEreset[i][j];
                    }
                }

                if(tutorialStep == 2){
                    for(int i = 0 ; i < 8 ; i++){
                        showCase[i].setPosition(i * 85 + 105 , 6 * 85 + 25);
                        showCase[i].setFillColor(Color::Green);
                        showCase[i].setSize(Vector2f(70,70));
                        _window.draw(showCase[i]);
                    }
                }
                else if(tutorialStep >= 3 && tutorialStep < 6){
                    for(int i = 0 ; i < 8 ; i++){
                        showCase[i].setPosition(i * 85 + 105 , 7 * 85 + 25);
                        showCase[i].setFillColor(Color::Green);
                        showCase[i].setSize(Vector2f(70,70));
                    }
                    _window.draw(showCase[tutorialStep - 3]);
                    _window.draw(showCase[7 - tutorialStep + 3]);
                }
                else if(tutorialStep == 6){
                    showCase[0].setPosition(3 * 85 + 105 , 7 * 85 + 25);
                    showCase[0].setFillColor(Color::Green);
                    showCase[0].setSize(Vector2f(70,70));
                    _window.draw(showCase[0]);
                }
                else if(tutorialStep == 7){
                    showCase[0].setPosition(4 * 85 + 105 , 7 * 85 + 25);
                    showCase[0].setFillColor(Color::Green);
                    showCase[0].setSize(Vector2f(70,70));
                    _window.draw(showCase[0]);
                }
                DrawBoard(_window);
            }
            else if(tutorialStep == 9){
                showCase[0].setPosition(figures[20].getPosition().x , figures[20].getPosition().y - 85);
                showCase[0].setFillColor(Color::Green);
                showCase[0].setSize(Vector2f(35,35));

                if(moves == 0){
                    figures[20].setPosition(85*4+115 , 85*6+35);

                    for(int i = 0 ; i < 4 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[6][4] = 6;

                    showCase[1].setPosition(figures[20].getPosition().x , figures[20].getPosition().y - 85 * 2);
                    showCase[1].setFillColor(Color::Green);
                    showCase[1].setSize(Vector2f(35,35));
                    _window.draw(showCase[1]);
                }

                GetPosMoves(1);

                _window.draw(showCase[0]);
                _window.draw(figures[20]);

                if(MoveMade){
                    figures[20].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);
                    _window.draw(figures[20]);
                    moves++;
                }
            }
            else if(tutorialStep == 10){
                if(moves == 0){
                    figures[24].setPosition(115 , 85*7 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[7][0] = 1;
                }

                GetPosMoves(1);

                for(int i = 0 ; i < NrMoves ; i++){
                    showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                    showCase[i].setFillColor(Color::Green);
                    showCase[i].setSize(Vector2f(35,35));
                    _window.draw(showCase[i]);
                }

                _window.draw(figures[24]);

                if(MoveMade){
                    figures[24].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[Xcoord][Ycoord] = 1;

                    _window.draw(figures[24]);
                    moves++;
                }
            }
            else if(tutorialStep == 11){
                if(moves == 0){
                    figures[25].setPosition(85 + 115 , 85*7 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[7][1] = 2;
                }

                GetPosMoves(1);

                for(int i = 0 ; i < NrMoves ; i++){
                    showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                    showCase[i].setFillColor(Color::Green);
                    showCase[i].setSize(Vector2f(35,35));
                    _window.draw(showCase[i]);
                }

                _window.draw(figures[25]);

                if(MoveMade){
                    figures[25].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[Xcoord][Ycoord] = 2;

                    _window.draw(figures[25]);
                    moves++;
                }
            }
            else if(tutorialStep == 12){
                if(moves == 0){
                    figures[26].setPosition(85*2+115 , 85*7+35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[7][2] = 3;
                }

                GetPosMoves(1);

                for(int i = 0 ; i < NrMoves ; i++){
                    showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                    showCase[i].setFillColor(Color::Green);
                    showCase[i].setSize(Vector2f(35,35));
                    _window.draw(showCase[i]);
                }

                _window.draw(figures[26]);

                if(MoveMade){
                    figures[26].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[Xcoord][Ycoord] = 3;

                    _window.draw(figures[26]);
                    moves++;
                }
            }
            else if(tutorialStep == 13){
                if(moves == 0){
                    figures[27].setPosition(85*3+115 , 85*7+35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[7][3] = 4;
                }

                GetPosMoves(1);

                for(int i = 0 ; i < NrMoves ; i++){
                    showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                    showCase[i].setFillColor(Color::Green);
                    showCase[i].setSize(Vector2f(35,35));
                    _window.draw(showCase[i]);
                }

                _window.draw(figures[27]);

                if(MoveMade){
                    figures[27].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[Xcoord][Ycoord] = 4;

                    _window.draw(figures[27]);
                    moves++;
                }
            }
            else if(tutorialStep > 13 && tutorialStep < 16){
                if(moves == 0 && tutorialStep == 15){
                    figures[28].setPosition(85*4+115 , 85*7+35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[7][4] = 5;
                }

                if(tutorialStep == 15){
                    GetPosMoves(1);

                    for(int i = 0 ; i < NrMoves ; i++){
                        showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                        showCase[i].setFillColor(Color::Green);
                        showCase[i].setSize(Vector2f(35,35));
                        _window.draw(showCase[i]);
                    }
                }

                _window.draw(figures[28]);

                if(MoveMade){
                    figures[28].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[Xcoord][Ycoord] = 5;

                    _window.draw(figures[28]);
                    moves++;
                }
            }
            else if((tutorialStep >= 16 && tutorialStep <= 18) || ( tutorialStep >= 20 && tutorialStep <= 23) || tutorialStep == 28 || tutorialStep == 27){
                for(int i = 0 ; i < 8 ; i++){
                    for(int j = 0 ; j < 8 ; j++){
                        board[i][j] = TRUEreset[i][j];
                    }
                }

                if( tutorialStep == 22 ){
                    showCase[0].setPosition(4 * 85 + 105 , 7 * 85 + 25);
                    showCase[0].setFillColor(Color::Red);
                    showCase[0].setSize(Vector2f(70,70));
                    _window.draw(showCase[0]);
                }

                DrawBoard(_window);
            }
            else if(tutorialStep == 19){
                if(moves == 0){
                    figures[20].setPosition(85*4+115 , 85*6+35);
                    figures[11].setPosition(85*3+115 , 85*1+35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }
                    board[1][3] = -6;
                    board[6][4] = 6;
                }
                GetPosMoves(1);

                for(int i = 0 ; i < NrMoves ; i++){
                    showCase[i].setPosition(AvailableMoves[i][3] * 85 + 115,AvailableMoves[i][1] * 85 + 25);
                    showCase[i].setFillColor(Color::Green);
                    showCase[i].setSize(Vector2f(35,35));
                    _window.draw(showCase[i]);
                }

                if(board[1][3] == -6)
                    _window.draw(figures[11]);
                _window.draw(figures[20]);

                if(MoveMade){
                    figures[20].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    if(Xcoord != 1 && Ycoord != 3)
                        board[1][3] = -6;

                    board[Xcoord][Ycoord] = 6;

                    _window.draw(figures[20]);
                    moves++;
                }
            }
            else if( tutorialStep == 24 ){

                if(moves == 0){

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j]  = 0;
                        }
                    }

                    board[5][2] = -1;
                    board[4][3] = -5;


                    board[5][5] = 5;
                    board[4][5] = 6;

                    showCase[10].setPosition(5 * 85 + 105 , 5 * 85 + 25);
                    showCase[10].setFillColor(Color::Red);
                    showCase[10].setSize(Vector2f(70,70));
                    _window.draw(showCase[10]);

                }

                DrawBoard(_window);

                if(moves != 1){
                    GetPosMoves(1);

                    for(int i = 0 ; i < NrMoves ; i++){
                        showCase[i].setPosition(AvailableMoves[i][3] * 85 + 105 , AvailableMoves[i][1] * 85 + 25);
                        showCase[i].setFillColor(Color::Green);
                        showCase[i].setSize(Vector2f(35,35));
                        _window.draw(showCase[i]);
                    }
                }

                if( MoveMade ){
                    figures[3].setPosition(Ycoord * 85 + 115 , Xcoord * 85 + 35);

                    board[5][5] = 0;
                    board[Xcoord][Ycoord]=5;

                    _window.draw(figures[3]);
                    moves++;
                }

            }
            else if(tutorialStep == 25){
                if(moves == 0){
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = TRUEreset[i][j];
                        }
                    }

                    board[0][1] = 0;
                    board[5][5] = -2;

                    board[6][0] = 0;
                    board[4][0] = 6;

                    board[6][1] = 0;
                    board[5][1] = 6;

                    board[7][2] = 0;
                    board[6][1] = 3;

                    showCase[10].setPosition(4 * 85 + 105 , 7 * 85 + 25);
                    showCase[10].setFillColor(Color::Red);
                    showCase[10].setSize(Vector2f(70,70));
                    _window.draw(showCase[10]);

                    GetPosMoves(1);

                    showCase[0].setPosition(5 * 85 + 105 , 5 * 85 + 25);
                    showCase[0].setFillColor(Color::Green);
                    showCase[0].setSize(Vector2f(35,35));
                    _window.draw(showCase[0]);
                }

                if(MoveMade){
                    moves++;

                    board[Xcoord][Ycoord] = board[PrevX][PrevY];
                    board[PrevX][PrevY] = 0;

                }

                DrawBoard(_window);
            }
            else if(tutorialStep == 26){
                if(moves == 0){
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = TRUEreset[i][j];
                        }
                    }

                    board[1][2] = 0;
                    board[2][2] = -6;

                    board[3][0] = -4;
                    board[0][3] = 0;

                    board[7][6] = 0;
                    board[5][5] = 2;

                    board[6][3] = 0;
                    board[4][3] = 6;

                    showCase[10].setPosition(4 * 85 + 105 , 7 * 85 + 25);
                    showCase[10].setFillColor(Color::Red);
                    showCase[10].setSize(Vector2f(70,70));
                    _window.draw(showCase[10]);

                    GetPosMoves(1);

                    for(int i = 0 ; i < NrMoves ; i++){
                        showCase[i].setPosition(AvailableMoves[i][3] * 85 + 105 , AvailableMoves[i][1] * 85 + 25);
                        showCase[i].setFillColor(Color::Green);
                        showCase[i].setSize(Vector2f(35,35));
                        _window.draw(showCase[i]);
                    }
                }

                if(MoveMade){
                    moves++;
                    board[Xcoord][Ycoord] = board[PrevX][PrevY];
                    board[PrevX][PrevY] = 0;
                }

                DrawBoard(_window);
            }
            else if(tutorialStep != 0){
                DrawBoard(_window);
            }

        }
        else if(Cscreen == 6){
            Label nxtTxt , prevTxt;
            Button nxtBtn , prevBtn;

            nxtTxt.create( 700 , 800 , 30 , translateText( Clang , r_button02 ) , font );
            prevTxt.create( 100 , 800 , 30 , translateText( Clang , s1_button03 ) , font );

            prevBtn.create( 80 , 700+offset , 115 , 45 );
            prevBtn.update( prevHover );

            nxtBtn.create( 680 , 700+offset , 175 , 45 );
            nxtBtn.update( nxtHover );

            _window.clear();

            _window.draw(boardI);

            RectangleShape Warn;
            Warn.setFillColor(Color::Red);
            Warn.setSize(Vector2f(70,70));

            if(player == 1){
                if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                    Warn.setPosition(85 * KingWhitePos[1] + 105 , 85 * KingWhitePos[0] + 30);
                    _window.draw(Warn);
                }
            }
            else{
                if(isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                    Warn.setPosition(85 * KingBlackPos[1] + 105 , 85 * KingBlackPos[0] + 30);
                    _window.draw(Warn);
                }
            }

            _window.draw(boardI);

            _window.draw(nxtBtn);
            _window.draw(nxtTxt);

            _window.draw(prevBtn);
            _window.draw(prevTxt);

            DrawBoard(_window);

        }
        else if( Cscreen == 7 ){

            //Archives

            int number_of_files = 0;

            ifstream fin("archive/settings.ini");

            /** Number of files from settings.ini will be the number of buttons */

            if(!fin.good())
                _window.close();
            else{
                if(fin.eof())
                    _window.close();
                else{
                    string buffer;

                    fin >> buffer >> buffer >> buffer >> number_of_files;
                    fin.close();
                }
            }

            Label Title , Info , Question , LoadTxt , BackTxt , playerTxt;
            Button LoadBtn , BackBtn;

            string info = translateText(Clang,s7_info[0]);
            if(number_of_files < 10)
                info += number_of_files + '0';
            else{
                int cifre[1000] , Ncifre=0;
                int aux = number_of_files;
                while(aux != 0){
                    cifre[Ncifre] = aux % 10;
                    Ncifre++;
                    aux = aux/10;
                }
                for(int i = Ncifre-1 ; i >= 0 ; i--){
                    info += cifre[i] + '0';
                }
            }
            info += translateText(Clang,s7_info[1]);

            Title.create( 350 , 50 , 48 , translateText(Clang,s7_title) , font);
            Info.create( 100 , 150 , 30 , info , font);
            Question.create( 100 , 200 , 30 , translateText(Clang,s7_question) , font);
            playerTxt.create( 640 , 200 , 30 , playerInput , font);

            LoadTxt.create(715 , 404 , 30 , translateText(Clang,s7_button01) , font);
            BackTxt.create((Clang==EN)?115:105 , 404 , 30 , translateText(Clang,s1_button03) , font);

            LoadBtn.create(700 , 400 , (Clang==EN)?95:125 , 45);
            LoadBtn.update( loadHover );

            BackBtn.create(100 , 400 , 95 , 45);
            BackBtn.update( backHover );

            _window.draw(Title);
            _window.draw(Info);
            _window.draw(Question);

            _window.draw(LoadBtn);
            _window.draw(BackBtn);

            _window.draw(LoadTxt);
            _window.draw(BackTxt);

            _window.draw(playerTxt);

        }
        else if( Cscreen == 8 ){

            //Puzzles menu

            //Labels
            Label title , backTxt , pzTxt[5];

            //Buttons
            Button backBtn , pz[5];

            title.create(400,50,48,translateText(Clang,s8_title),font);

            for(int i = 0 ; i < 5 ; i++){
                pz[i].create(390,150+80*i,195,45);
                pzTxt[i].create(420,154+80*i,30,translateText(Clang,s8_buttons[i]),font);
            }

            backBtn.create(445,550,95,45);
            backBtn.update(back8Hover);
            backTxt.create((Clang==EN)?460:450,554,30,translateText(Clang,s1_button03),font);

            _window.draw(title);
            _window.draw(backBtn);

            for(int i = 0 ; i < 5 ; i++){
                pz[i].update(pzHover[i]);
                _window.draw(pz[i]);
                _window.draw(pzTxt[i]);
            }

            _window.draw(backTxt);
        }
        else if(Cscreen == 9){

            //Puzzle layouts

            int answer[4];

            if(pzId == 0){

                if(moves == 0){

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[0][4] = -3;
                    board[0][5] = -5;

                    board[1][0] = -6;
                    board[1][1] = -6;
                    board[1][2] = -6;
                    board[1][6] = -6;

                    board[2][2] = -2;
                    board[2][3] = -6;
                    board[2][7] = -6;

                    board[3][2] = -3;

                    board[4][2] = 3;
                    board[4][5] = 3;
                    board[4][6] = -2;
                    board[4][7] = -4;

                    board[5][2] = 2;

                    board[6][0] = 6;
                    board[6][1] = 6;
                    board[6][6] = 6;
                    board[6][7] = 6;

                    board[7][4] = 1;
                    board[7][5] = 1;
                    board[7][7] = 5;

                }

                answer[0] = 4;
                answer[1] = 2;
                answer[2] = 5;
                answer[3] = 3;

            } else if( pzId == 1 ){

                if(moves == 0){

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[0][0] = blackRook;
                    board[0][5] = blackRook;
                    board[0][6] = blackKing;

                    board[1][0] = blackPawn;
                    board[1][1] = blackPawn;
                    board[1][2] = blackPawn;
                    board[1][6] = blackPawn;

                    board[2][2] = blackBishop;
                    board[2][3] = blackPawn;

                    board[3][4] = whitePawn;
                    board[3][5] = whiteKnight;
                    board[3][7] = whiteQueen;

                    board[4][4] = blackQueen;

                    board[6][0] = whitePawn;
                    board[6][6] = whitePawn;
                    board[6][7] = whitePawn;

                    board[7][0] = blackBishop;
                    board[7][2] = whiteBishop;
                    board[7][5] = whiteRook;
                    board[7][7] = whiteKing;

                }

                answer[0] = 3;
                answer[1] = 1;
                answer[2] = 5;
                answer[3] = 4;

            } else if(pzId == 2){

                if(moves == 0){
                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[0][0] = blackRook;
                    board[0][5] = whiteBishop;
                    board[0][6] = blackRook;

                    board[1][0] = blackPawn;
                    board[1][2] = blackQueen;
                    board[1][5] = blackPawn;
                    board[1][7] = blackKing;

                    board[2][1] = blackPawn;
                    board[2][3] = whiteRook;

                    board[3][4] = whitePawn;
                    board[3][7] = blackPawn;

                    board[5][2] = whitePawn;

                    board[6][1] = whitePawn;
                    board[6][5] = whitePawn;
                    board[6][6] = whitePawn;
                    board[6][7] = whitePawn;

                    board[7][6] = whiteKing;

                    answer[0] = 2;
                    answer[1] = 2;
                    answer[2] = 3;
                    answer[3] = 7;
                }
            } else if(pzId == 3){

                if(moves == 0){

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[0][0] = blackRook;
                    board[0][2] = blackBishop;
                    board[0][3] = blackKing;
                    board[0][7] = blackRook;

                    board[1][0] = blackPawn;
                    board[1][2] = blackQueen;
                    board[1][4] = blackBishop;
                    board[1][6] = blackPawn;

                    board[2][7] = blackPawn;

                    board[3][0] = blackKnight;
                    board[3][1] = whiteBishop;
                    board[3][3] = blackPawn;
                    board[3][4] = blackPawn;
                    board[3][6] = whiteKnight;

                    board[5][3] = whitePawn;
                    board[5][4] = whiteBishop;

                    board[6][0] = whitePawn;
                    board[6][1] = whitePawn;
                    board[6][2] = whitePawn;
                    board[6][5] = whitePawn;
                    board[6][6] = whitePawn;
                    board[6][7] = whitePawn;

                    board[7][0] = whiteRook;
                    board[7][4] = whiteKing;
                    board[7][7] = whiteRook;

                    answer[0] = 3;
                    answer[1] = 1;
                    answer[2] = 6;
                    answer[3] = 5;

                }

            } else if(pzId == 4){

                if(moves == 0){

                    for(int i = 0 ; i < 8 ; i++){
                        for(int j = 0 ; j < 8 ; j++){
                            board[i][j] = 0;
                        }
                    }

                    board[0][2] = blackKing;
                    board[0][4] = blackRook;

                    board[1][0] = blackPawn;
                    board[1][4] = whitePawn;

                    board[2][1] = blackPawn;
                    board[2][3] = blackQueen;

                    board[3][6] = blackPawn;

                    board[4][3] = whitePawn;
                    board[4][4] = whiteQueen;

                    board[5][2] = whitePawn;
                    board[5][6] = blackPawn;
                    board[5][7] = whitePawn;

                    board[6][0] = whitePawn;
                    board[6][6] = whiteBishop;

                    board[7][6] = whiteKing;

                    answer[0] = 4;
                    answer[1] = 1;
                    answer[2] = 4;
                    answer[3] = 1;
                }

            }

            if(moves == 0 && pzId >= 0){
                GetPosMoves(1);

                if(MoveMade){

                    board[Xcoord][Ycoord] = board[PrevX][PrevY];
                    board[PrevX][PrevY] = 0;

                    if(answer[0] == PrevX  && answer[1] == Xcoord && answer[2] == PrevY && answer[3] == Ycoord){
                        resultPart = 1;
                    }
                    else{
                        resultPart = 2;
                        AI(2,1);
                    }

                    moves++;

                }
            }

            Label result ,  backTxt;
            Button backBtn;

            backBtn.create(100,720,95,45);
            backBtn.update(pzBackHover);

            backTxt.create((Clang==EN)?115:105 , 724 , 30 ,  translateText(Clang , s1_button03),font);

            if(resultPart == 1)
                result.create(400,720,30,translateText(Clang,pz_res[0]),font);
            else if(resultPart == 2)
                result.create(400,720,30,translateText(Clang,pz_res[1]),font);
            else
                result.create(400,720,30,translateText(Clang,pz_res[2]),font);

            RectangleShape Warn;

            Warn.setFillColor(Color::Red);
            Warn.setSize(Vector2f(70,70));

            _window.draw(boardI);

            if(isCheck(KingWhitePos[0],KingWhitePos[1],1)){
                Warn.setPosition(85 * KingWhitePos[1] + 105 , 85 * KingWhitePos[0] + 30);
                _window.draw(Warn);
            }
            if(isCheck(KingBlackPos[0],KingBlackPos[1],2)){
                Warn.setPosition(85 * KingBlackPos[1] + 105 , 85 * KingBlackPos[0] + 30);
                _window.draw(Warn);
            }

            DrawBoard(_window);

            _window.draw(result);
            _window.draw(backBtn);
            _window.draw(backTxt);

        }
        else{
            //Error
            cout << "ERROR 01 : INVAILID SCREEN !";
            _window.close();
        }

        _window.display();
    }

    //Close the console window at the end.
    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);

    //End of execution
    return 0;
}
