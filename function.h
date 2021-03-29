#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
/*  #define LINES_EASY 9
    #define COLS_EASY 9
    #define LINES_NORMAL 16
    #define COLS_NORMAL 16
    #define LINES_HARD 30
    #define COLS_HARD 16 */

typedef enum {              //超出地圖       已標記位置
    OverRange, EnterError, OverCoordinate, MarkCoordinate
} Warning;

typedef enum {
    Map, Mark
} Type;

struct GAME{
    bool error;  //遊戲錯誤(輸入)
    bool over;   //遊戲結束
    int flag;   //遊戲中旗子數量
    char continue_q;     //遊戲繼續詢問
};

struct SET{
    int lines;  //地圖幾行
    int cols;   //地圖幾列
    int landboom;   //地雷數量
};

typedef struct MAP {
    int* map;    //地圖
    int* mark;   //標記地圖
} MAP;

typedef struct COOR {
    int x;   //x
    int y;   //y
} Coor;

//重要函式(important_funt.c)
void clean_stdin(void); //清除輸入緩衝區
void color_set(int);    //改變顏色
void degree_of_difficulty(void);
void game_dif_choose(void); //遊戲困難度選則
void gotoxy(int, int);  //指定座標
void print_space(void); //印空白(10個)
void print_warning(Warning);    //警告
int map_location(Coor, Type);  //地圖座標
bool over_range(Coor);   //計算有無超出地圖範圍
void reset(void);       //重製遊戲參數
int location_value(MAP *, Coor, Type);  //計算地圖位置數值

//遊戲內容(game_content.c)
void game_content(MAP *);    //遊戲內容
void game_start(MAP *);  //設定邊界
void landboom_generate(Coor, int *);   //隨機生成地雷
void landboom_tester(Coor, MAP *);   //測試旁邊八格地雷
void mod_choose_1(MAP *);        //"踩"
void mod_choose_2(MAP *);        //標記
void print_game(MAP *);          //介面
