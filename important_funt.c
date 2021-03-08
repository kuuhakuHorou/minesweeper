#include "funtion.h"

void color_set(int color_num) {     //https://baike.baidu.com/item/SetConsoleTextAttribute 顏色對應值
    HANDLE hOut;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, color_num);
}

void degree_of_difficulty() {       //自訂難度
    int i, choose, change;
    bool exit = false, choose_exit = false, error = false;
    do {
        if (set.lines == 0) {
            choose = 1;
        }
        else if (set.cols == 0) {
            choose = 2;
        }
        else if (set.landboom == 0) {
            choose = 3;
        }
        switch (choose) {
            case 1:
            do {
                error = false;
                gotoxy(30,1);
                printf("請輸入長度x(最小9,最大30): ");
                for (i = 0; i < 10; i++) {
                    putchar(' ');
                }
                for (i = 0; i < 10; i++) {
                    putchar('\b');
                }
                scanf("%d", &set.lines);
                fflush(stdin);
                if (set.lines < 9||set.lines > 30) {
                    error = true;
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,1);
                    for (i = 0; i < 35; i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i = 0; i < 17; i++) {
                        putchar(' ');
                    }
                }
            } while (error);
            break;
            case 2:
            do {
                error = false;
                gotoxy(30,2);
                printf("請輸入寬度y(最小9,最大24): ");
                for (i = 0;i < 10; i++) {
                    putchar(' ');
                }
                for (i = 0; i < 10; i++) {
                    putchar('\b');
                }
                scanf("%d", &set.cols);
                fflush(stdin);
                if (set.cols < 9||set.cols > 24) {
                    error = true;
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,2);
                    for (i = 0; i < 35; i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i = 0; i < 17; i++) {
                        putchar(' ');
                    }
                }
            } while (error);
            break;
            case 3:
            do {
                error = false;
                gotoxy(30,3);
                printf("請輸入炸彈數(最小10,最大%-3d): ", (set.lines-1)*(set.cols-1));
                for (i = 0; i < 10; i++) {
                    putchar(' ');
                }
                for (i = 0; i < 10; i++) {
                    putchar('\b');
                }
                scanf("%d", &set.landboom);
                fflush(stdin);
                if (set.landboom < 10||set.landboom > (set.lines-1)*(set.cols-1)) {
                    error = true;
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,3);
                    for (i = 0; i < 40; i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i=0;i<17;i++) {
                        putchar(' ');
                    }
                }
            } while (error);
            break;
        }
        if (set.lines != 0&&set.cols != 0&&set.landboom != 0) {
            do {
                char decide;
                gotoxy(30,4);
                printf("有要調整的嗎?(Y/N) ");
                for (i = 0; i < 10; i++) {
                    putchar(' ');
                }
                for (i = 0; i < 10; i++) {
                    putchar('\b');
                }
                scanf("%c", &decide);
                fflush(stdin);
                switch (decide) {
                    case 'y':
                    case 'Y':
                        do {
                            error = false;
                            gotoxy(30,5);
                            printf("請問需要調整哪裡呢?(1:長度 2:寬度 3:地雷數) ");
                            for (i = 0; i < 10; i++) {
                                putchar(' ');
                            }
                            for (i = 0; i < 10; i++) {
                                putchar('\b');
                            }
                            scanf("%d", &change);
                            fflush(stdin);
                            if (change < 1||change > 3) {
                                error = true;
                                gotoxy(30,6);
                                color_set(252);
                                printf("請不要亂輸入喔!");
                                Sleep(3000);
                                color_set(7);
                                gotoxy(30,5);
                                for (i = 0; i < 50; i++) {
                                    putchar(' ');
                                }
                                gotoxy(30,6);
                                for (i = 0; i < 15; i++) {
                                    putchar(' ');
                                }
                            }
                            else if (change == 1) {
                                change = 0;
                                set.lines = 0;
                                choose_exit = true;
                            }
                            else if (change == 2) {
                                change = 0;
                                set.cols = 0;
                                choose_exit = true;
                            }
                            else if (change == 3) {
                                change = 0;
                                set.landboom = 0;
                                choose_exit = true;
                            }
                        } while (error);
                    break;
                    case 'n':
                    case 'N':
                        gotoxy(30,6);
                        printf("那就開始遊戲囉");
                        Sleep(1000);
                        choose_exit = true;
                        exit = true;
                    break;
                    default:
                        gotoxy(30,6);
                        color_set(252);
                        printf("請不要亂輸入喔!");
                        Sleep(3000);
                        color_set(7);
                        gotoxy(30,4);
                        for (i = 0; i < 25; i++) {
                            putchar(' ');
                        }
                        gotoxy(30,6);
                        for (i = 0; i < 15; i++) {
                            putchar(' ');
                        }
                    break;
                }
            } while (!choose_exit);
        }
    } while (!exit);
}

void game_dif_choose() {        //遊戲難度選則
    int choose, over, i;
    do {
        printf("請問要玩怎樣的難度呢\n"
               "    簡單:1\n"
               "    普通:2\n"
               "    困難:3\n"
               "    自定義:4\n"
               "  請輸入: ");
        scanf("%d", &choose);
        fflush(stdin);  //清理輸入緩衝區
        switch (choose) {
            case 1:
                set.lines = 9;
                set.cols = 9;
                set.landboom = 10;
                game_content();
                break;
            case 2:
                set.lines = 16;
                set.cols = 16;
                set.landboom = 40;
                game_content();
                break;
            case 3:
                set.lines = 30;
                set.cols = 16;
                set.landboom = 99;
                game_content();
                break;
            case 4:
                degree_of_difficulty();
                game_content();
                break;
            default:
                color_set(252);
                printf("請不要亂輸入喔!\n");
                color_set(7);
                Sleep(3000);    //延遲
                system("cls");  //刷新螢幕
                break;
        }
        while (game.over) {
            gotoxy(60,2);
            printf("你還想要繼續玩嗎(Y/N) ");
            for (i = 0; i < 10; i++) {
                putchar(' ');
            }
            for (i = 0; i < 10; i++) {
                putchar('\b');
            }
            scanf("%c", &game.continue_q);
            fflush(stdin);
            switch (game.continue_q) {
                case 'y':
                case 'Y':
                    game.continue_q = 1;
                    gotoxy(60,3);
                    printf("那就重新開始囉");
                    Sleep(1000);
                break;
                case 'n':
                case 'N':
                    over = 1;
                    gotoxy(60,3);
                    printf("那就掰掰囉");
                    Sleep(1000);
                break;
                default:
                    gotoxy(60,3);
                    color_set(252);
                    printf("請不要亂輸入喔");
                    Sleep(3000);
                    gotoxy(60,3);
                    color_set(7);
                    for (i = 0; i < 14; i++) {
                        putchar(' ');
                    }
                break;
            }
            if (over == 1||game.continue_q == 1) {
                break;
            }
        }
        if (game.continue_q == 1) {
            system("cls");
        }
    } while (over != 1);
}

void gotoxy(int x,int y) {  //指定座標(x和y從0開始算，所以減1，比較直觀)
    COORD c;
    c.X = x-1;
    c.Y = y-1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}
