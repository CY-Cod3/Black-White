#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include <iostream>
#include <string>

enum languages { RO , EN , DE , IT };

const int s0_title = 0;
const int s0_button01 = 1;
const int s0_button02 = 2;
const int s0_button03 = 3;
const int s0_button04 = 4;
const int s0_button05 = 5;
const int s0_button06 = 6;

const int s1_title = 7;
const int s1_button01 = 8;
const int s1_button02 = 9;
const int s1_button03 = 10;

const int g_cplayer[5] = { 11 , 12 , 13 , 14 , 15 };
const int g_button01[3] = { 16 , 17 , 18 };
const int g_button02[2] = { 19 , 20 };

const int r_button02 = 21;

const int t_info[29] = { 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36 , 37 , 38 , 39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 49 , 50};

const int s7_title = 51;
const int s7_info[2] = { 52 , 53 };
const int s7_question = 54;
const int s7_button01 = 55;

const int s8_title = 56;
const int s8_buttons[5] = { 57 , 58 , 59 , 60 , 61 };

const int dif_buttons[4] = { 62 , 63 , 64 , 65 };
const int dif_title = 71;

const int prom_buttons[4] = { 66 , 67 , 68 , 69 };
const int prom_title = 70;

const int pz_res[3] = { 72 , 73 , 74 };

std::string translateText ( const languages lang , const int text_to_translate );

#endif // TRANSLATOR_H_INCLUDED
