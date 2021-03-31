#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <conio.h>

typedef enum {
//   超出範圍   輸入錯誤      超出地圖       已標記位置
    OverRange, EnterError, OverCoordinate, MarkCoordinate
} Warning;

typedef enum {
    Map, Mark
} Type;

struct SET{
    int lines;  //地圖幾行
    int cols;   //地圖幾列
    int landboom;   //地雷數量
};

struct GAME{
    bool error;  //遊戲錯誤(輸入)
    bool over;   //遊戲結束
    bool restart;   //遊戲重新開始
    bool sweep_landboom;    //踩到炸彈
    bool exit;  //離開遊戲
    int flag;   //遊戲中旗子數量
    char continue_q;     //遊戲繼續詢問
    struct SET set;
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
bool compare_coor(Coor, Coor);   //比較座標是否一樣
void degree_of_difficulty(void);    //自訂地圖資訊
void game_dif_choose(void); //遊戲困難度選則
void gotoxy(int, int);  //指定座標
void is_hide_cursor(bool);  //是否隱藏游標
int location_value(MAP *, Coor, Type);  //計算地圖位置數值
int map_location(Coor, Type);  //地圖座標
bool over_range(Coor);   //計算有無超出地圖範圍
void print_space(void); //印空白(10個)
void print_warning(Warning);    //警告
void reset(void);       //重置遊戲參數
void reset_game(void);  //重設game參數
void reset_set(void);   //重設set參數

//遊戲內容(game_content.c)
void game_content(MAP *);    //遊戲內容
void game_start(MAP *);  //設定邊界
void interactive_mod(MAP *);     //互動模式
void is_win(MAP *);  //測試是否贏了
void landboom_generate(Coor, int *);   //隨機生成地雷
void landboom_tester(Coor, MAP *);   //測試旁邊八格地雷
bool mod_choose(int, MAP *, bool);    //選擇模式
bool sweep(MAP *, bool);    //"踩"
bool map_mark(MAP *, bool);    //標記
void print_game(MAP *, bool);      //介面
void text_mod(MAP *);     //文字模式
