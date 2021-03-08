#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
/*  #define LINES_EASY 9
    #define COLS_EASY 9
    #define LINES_NORMAL 16
    #define COLS_NORMAL 16
    #define LINES_HARD 30
    #define COLS_HARD 16 */

typedef enum {false, true} bool;

struct GAME{
    bool error;  //遊戲錯誤(輸入)
    bool over;   //遊戲結束
    int flag;   //遊戲中旗子數量
    char continue_q;     //遊戲繼續詢問
} game;

struct SET{
    int lines;  //地圖幾行
    int cols;   //地圖幾列
    int landboom;   //地雷數量
} set;

//重要函式(important_funt.c)
void color_set(int);    //改變顏色
void degree_of_difficulty(void);
void game_dif_choose(void); //遊戲困難度選則
void gotoxy(int,int);   //指定座標

//遊戲內容(game_content.c)
void game_content(void);    //遊戲內容
void game_start(int *,int *);  //設定邊界
void landboom_generate(int,int,int *);   //隨機生成地雷
void landboom_tester(int,int,int *);   //測試旁邊八格地雷
void mod_choose_1(int *,int *);        //"踩"
void mod_choose_2(int *,int *);        //標記
void print_game(int *,int *);          //介面
