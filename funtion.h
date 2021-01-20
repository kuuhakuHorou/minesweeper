#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define LINES_EASY 9
#define COLS_EASY 9
#define LINES_NORMAL 16
#define COLS_NORMAL 16
#define LINES_HARD 30
#define COLS_HARD 16

//重要函式(important_funt.c)
void color_set(int);    //改變顏色
void game_dif_choose(void); //遊戲困難度選則
void gotoxy(int,int);   //指定座標

//9×9的地圖(簡單)(funt_dif_1.c)
void game_easy(void);    //遊戲內容
void game_start_easy(void);  //設定邊界
void landboom_generate_easy(int *,int *);   //隨機生成地雷
void landboom_tester_easy(int,int);  //測試旁邊八格地雷
void mod_choose_1_easy(void);        //"踩"
void mod_choose_2_easy(void);        //標記
void print_game_easy(void);          //介面

//16×16的地圖(普通)(funt_dif_2.c)
void game_normal(void);    //遊戲內容
void game_start_normal(void);  //設定邊界
void landboom_generate_normal(int *,int *);   //隨機生成地雷
void landboom_tester_normal(int,int);  //測試旁邊八格地雷
void mod_choose_1_normal(void);        //"踩"
void mod_choose_2_normal(void);        //標記
void print_game_normal(void);          //介面

//16×30的地圖(困難)(funt_dif_3.c)
void game_hard(void);    //遊戲內容
void game_start_hard(void);  //設定邊界
void landboom_generate_hard(int *,int *);   //隨機生成地雷
void landboom_tester_hard(int,int);  //測試旁邊八格地雷
void mod_choose_1_hard(void);        //"踩"
void mod_choose_2_hard(void);        //標記
void print_game_hard(void);          //介面
