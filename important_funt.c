#include "funtion.h"

void color_set(int color_num) {     //https://baike.baidu.com/item/SetConsoleTextAttribute 顏色對應值
    HANDLE hOut;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,color_num);
}

void game_dif_choose() {
    int choose, error_in=0, game_end;
    while (1) {
        printf("請問要玩怎樣的難度呢\n"
               "    簡單:1\n"
               "    普通:2\n"
               "    困難:3\n"
               "  請輸入: ");
        scanf("%d", &choose);
        fflush(stdin);  //清理輸入緩衝區
        switch (choose) {
            case 1:
                game_easy();
                game_end=1;
                break;
            case 2:
                game_normal();
                game_end=1;
                break;
            case 3:
                game_hard();
                game_end=1;
                break;
            default:
                color_set(12);
                printf("請不要亂輸入喔!\n");
                color_set(7);
                Sleep(3000);    //延遲
                system("cls");  //刷新螢幕
                break;
        }
        if (game_end==1) {
            break;
        }
        
    }
}

void gotoxy(int x,int y) {  //指定座標(x和y從0開始算，所以減1，比較直觀)
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}
