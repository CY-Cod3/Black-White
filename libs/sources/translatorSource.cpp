#include "../translator.h"

std::string translateText( const languages lang , const int text_to_translate ){

    /** Translate text */
  if( lang == RO ) {

  // ***************
  // *** Limba romana
  // ***************

    switch( text_to_translate ){

        case 0:     return "Bun venit la Sah 2.0 !";
        case 1:     return "Un Jucator";
        case 2:     return "Doi Jucatori";
        case 3:     return "Tutorial";
        case 4:     return "Arhive";
        case 5:     return "Puzzle-uri";
        case 6:     return "Iesire";
        case 7:     return "Alegeti-va culoarea !";
        case 8:     return "Alb";
        case 9:     return "Negru";
        case 10:    return "Inapoi";
        case 11:    return "Albul muta.";
        case 12:    return "Negrul muta.";
        case 13:    return "Albul castiga.";
        case 14:    return "Negrul castiga.";
        case 15:    return "Remiza.";
        case 16:    return "Oferiti remiza";
        case 17:    return "Acceptati";
        case 18:    return "Revedeti jocul";
        case 19:    return "Cedati";
        case 20:    return "Iesire";
        case 21:    return "Continuati";
        case 22:    return "Aceasta este tabla de sah, un patrat de 8 pe 8 \npe care se afla piesele de sah.";
        case 23:    return "Intr-un joc de sah sunt 32 de piese :\n  16 albe si 16 negre";
        case 24:    return "Piesele fiecarei tabere sunt : 8 Pioni";
        case 25:    return "2 Turnuri";
        case 26:    return "2 Cai";
        case 27:    return "2 Nebuni";
        case 28:    return "O Regina";
        case 29:    return "Si un Rege";
        case 30:    return "Fiecare piesa se poate deplasa \npe tabla de sah in modul sau unic.";
        case 31:    return "Pionul se poate deplasa in fata cu cate o casuta.Cand \neste mutat prima oara, el poate sari 2 casute. Incearca!";
        case 32:    return "Turnul se misca de-a lungul liniilor si coloanelor.\n     Incearca !";
        case 33:    return "Calul se deplaseaza in forma literei L si \npoate sari peste alte piese.Incearca sa-l misti !";
        case 34:    return "Nebunul merge doar de-a lungul diagonalelor.\n  Incearca !";
        case 35:    return "Regina e considerata cea mai puternica piesa.\n Ea se misca ca un nebun si un turn la un loc. Incearca !";
        case 36:    return "Regele, pe de alta parte, e cea mai importanta piesa.\n Daca pierzi regele, pierzi jocul!";
        case 37:    return "Regele se deplaseaza doar in casutele adiacente.\n Incearca !";
        case 38:    return "Acum urmeaza sa vorbim despre capturarea pieselor.";
        case 39:    return "Capturarea unei piese inamice consta in deplasarea \npiesei tale pe casuta ocupata de piesa inamica.";
        case 40:    return "Singura piesa care captureaza diferit\n de cum se deplaseaza e Pionul.";
        case 41:    return "Pionul captureaza piesele din diagonala sa, in casutele\n adiacente. Incearca sa capturezi pionul negru.";
        case 42:    return "Tine minte ca majoritatea pieselor nu se pot deplasa\n prin alte piese.";
        case 43:    return "De asemenea, nu uita ca pionii merg doar inainte.";
        case 44:    return "Cand o piesa inamica iti poate captura regele se \nnumeste SAH si casuta regelui tau va fi colorata in rosu.";
        case 45:    return "Cand esti in SAH esti obligat sa faci \nmutari care te scot din SAH.";
        case 46:    return "Poti iesi din SAH in 3 moduri :\n 1. Deplasandu-ti regele pe o casuta sigura.";
        case 47:    return "2. Capturand piesa inamica ce-ti ameninta regele.";
        case 48:    return "3. Blocand atacul piesei inamice.";
        case 49:    return "Daca nu poti iesi din SAH se numeste SAH MAT\n si pierzi jocul.";
        case 50:    return "Acestea sunt regulile de baza.\n Mult success!";
        case 51:    return "Arhive";
        case 52:    return "Sunt ";
        case 53:    return " jocuri salvate";
        case 54:    return "Care joc doriti sa-l vedeti ?";
        case 55:    return "Incarca";
        case 56:    return "Puzzle-uri";
        case 57:    return "Puzzle #1";
        case 58:    return "Puzzle #2";
        case 59:    return "Puzzle #3";
        case 60:    return "Puzzle #4";
        case 61:    return "Puzzle #5";
        case 62:    return "Foarte usor";
        case 63:    return "Usor";
        case 64:    return "Mediu";
        case 65:    return "Greu";
        case 66:    return "Turn";
        case 67:    return "Cal";
        case 68:    return "Nebun";
        case 69:    return "Regina";
        case 70:    return "Promoveaza in :";
        case 71:    return "Alegeti dificultatea";
        case 72:    return "Felicitari !!";
        case 73:    return "Mai incearca.";
        case 74:    return "Mat in o mutare";
        default:    return "" + text_to_translate;
    }
  } else if(lang == EN) {

  // ***************
  // *** Traducere limba engleza
  // ***************

    switch( text_to_translate ){

        case 0:     return "Welcome to Chess 2.0 !";
        case 1:     return "SinglePlayer";
        case 2:     return "MultiPlayer";
        case 3:     return "Tutorial";
        case 4:     return "Archives";
        case 5:     return "Puzzles";
        case 6:     return "Exit";
        case 7:     return "Choose your side !";
        case 8:     return "White";
        case 9:     return "Black";
        case 10:    return "Back";
        case 11:    return "White's turn.";
        case 12:    return "Black's turn.";
        case 13:    return "White wins.";
        case 14:    return "Black wins.";
        case 15:    return "Draw.";
        case 16:    return "Propose Draw";
        case 17:    return "Accept Draw";
        case 18:    return "Review match";
        case 19:    return "Resign";
        case 20:    return "Exit";
        case 21:    return "Continue";
        case 22:    return "This is the chess board. It is an 8 x 8 square on which \n the chess pieces are placed.";
        case 23:    return "In a game of chess , there are 32 pieces :\n  16 for black and 16 for white.";
        case 24:    return "The pieces of either side are : 8 Pawns";
        case 25:    return "2 Rooks";
        case 26:    return "2 Knights";
        case 27:    return "2 Bishops";
        case 28:    return "1 Queen";
        case 29:    return "And 1 King";
        case 30:    return "Each piece can move across the chess board\n   in its own unique way.";
        case 31:    return "The pawn, only moves forward by 1 square.\n On its first move only it can move 2 squares. Give it a try!";
        case 32:    return "The rook, moves alongside rows and columns.\n     Give it a try !";
        case 33:    return "The knight, moves in the shape of an L.\n     Try moving it !";
        case 34:    return "The bishop, moves only alongside diagonals.\n  Try it !";
        case 35:    return "The Queen is the most powerful.\n It moves like a rook and bishop combined.Try it !";
        case 36:    return "The King is the most important piece.\n If you loose the king you loose the game.";
        case 37:    return "The King can only move in the nearby squares. Try it !";
        case 38:    return "Next up we will talk about capturing pieces.";
        case 39:    return "When you move your piece on a square occupied by\n an enemy piece it's called capturing.";
        case 40:    return "The only piece that captures different than it moves\n is the pawn.";
        case 41:    return "The pawn captures pieces in its diagonals,\n that are next to him.";
        case 42:    return "Before we explain the 'Check' remember that\n most pieces cannot move through other pieces.";
        case 43:    return "And also remember, that pawns cannot move backwards.\n They only move forward.";
        case 44:    return "When an enemy piece can capture your king it's called \nCHECK and your king's square will be highlighted in red.";
        case 45:    return "When you are in CHECK , you are forced to make moves\n that will get you out of it.";
        case 46:    return "You can get out of CHECK in 3 ways.\n 1. Moving your king.";
        case 47:    return "2. Capturing the enemy piece that\n threatens your king.";
        case 48:    return "3. Blocking the enemy piece's attack.";
        case 49:    return "If you can't get out of check it's called CHECKMATE\n and you loose the game.";
        case 50:    return "Those are the basic rules of chess.\n And here we conclude our tutorial. Good luck!";
        case 51:    return "Archives";
        case 52:    return "There are ";
        case 53:    return " games saved.";
        case 54:    return "Which game would you like to load ?";
        case 55:    return "Load";
        case 56:    return "Puzzles";
        case 57:    return "Puzzle #1";
        case 58:    return "Puzzle #2";
        case 59:    return "Puzzle #3";
        case 60:    return "Puzzle #4";
        case 61:    return "Puzzle #5";
        case 62:    return "Very Easy";
        case 63:    return "Easy";
        case 64:    return "Medium";
        case 65:    return "Hard";
        case 66:    return "Rook";
        case 67:    return "Knight";
        case 68:    return "Bishop";
        case 69:    return "Queen";
        case 70:    return "Promote to :";
        case 71:    return "Choose A.I. Difficulty";
        case 72:    return "Congratulations !";
        case 73:    return "Try Again !";
        case 74:    return "Mate in 1";
        default:    return "" + text_to_translate;
    }

  }
}
