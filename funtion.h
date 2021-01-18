#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define LINES 9
#define COLS 9

void game(void);    //遊戲內容
void game_start(void);  //設定邊界
void gotoxy(int,int);   //指定座標
void landboom_generate(void);   //隨機生成地雷
void landboom_tester(int,int);  //測試旁邊八格地雷
void mod_choose_1(void);        //"踩"
void mod_choose_2(void);        //標記
void print_game(void);          //介面