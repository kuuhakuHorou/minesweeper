#include "funtion.h"

void color_set(int color_num) {     //https://baike.baidu.com/item/SetConsoleTextAttribute 顏色對應值
    HANDLE hOut;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut,color_num);
}

void degree_of_difficulty() {
    int choose, i, change, exit, choose_exit;
    while (1) {
        if (game.lines==0) {
            choose=1;
        }
        else if (game.cols==0) {
            choose=2;
        }
        else if (game.landboom==0) {
            choose=3;
        }
        switch (choose) {
            case 1:
            while (1) {
                gotoxy(30,1);
                printf("請輸入長度x(最小9,最大30): ");
                for (i=0;i<10;i++) {
                    putchar(' ');
                }
                for (i=0;i<10;i++) {
                    putchar('\b');
                }
                scanf("%d", &game.lines);
                fflush(stdin);
                if (game.lines<9||game.lines>30) {
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,1);
                    for (i=0;i<35;i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i=0;i<17;i++) {
                        putchar(' ');
                    }
                }
                else
                    break;
            }
            break;
            case 2:
            while (1) {
                gotoxy(30,2);
                printf("請輸入寬度y(最小9,最大24): ");
                for (i=0;i<10;i++) {
                    putchar(' ');
                }
                for (i=0;i<10;i++) {
                    putchar('\b');
                }
                scanf("%d", &game.cols);
                fflush(stdin);
                if (game.cols<9||game.cols>24) {
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,2);
                    for (i=0;i<35;i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i=0;i<17;i++) {
                        putchar(' ');
                    }
                }
                else
                    break;
            }
            break;
            case 3:
            while (1) {
                gotoxy(30,3);
                printf("請輸入炸彈數(最小10,最大%-3d): ", (game.lines-1)*(game.cols-1));
                for (i=0;i<10;i++) {
                    putchar(' ');
                }
                for (i=0;i<10;i++) {
                    putchar('\b');
                }
                scanf("%d", &game.landboom);
                fflush(stdin);
                if (game.landboom<10||game.landboom>(game.lines-1)*(game.cols-1)) {
                    gotoxy(30,6);
                    color_set(252);
                    printf("請不要超過範圍喔!");
                    Sleep(3000);
                    color_set(7);
                    gotoxy(30,3);
                    for (i=0;i<40;i++) {
                        putchar(' ');
                    }
                    gotoxy(30,6);
                    for (i=0;i<17;i++) {
                        putchar(' ');
                    }
                }
                else
                    break;
            }
            break;
        }
        if (game.lines!=0&&game.cols!=0&&game.landboom!=0) {
            while (1) {
                char decide;
                gotoxy(30,4);
                printf("有要調整的嗎?(Y/N) ");
                for (i=0;i<10;i++) {
                    putchar(' ');
                }
                for (i=0;i<10;i++) {
                    putchar('\b');
                }
                scanf("%c", &decide);
                fflush(stdin);
                switch (decide) {
                    case 'y':
                    case 'Y':
                        while (1) {
                            gotoxy(30,5);
                            printf("請問需要調整哪裡呢?(1:長度 2:寬度 3:地雷數) ");
                            for (i=0;i<10;i++) {
                                putchar(' ');
                            }
                            for (i=0;i<10;i++) {
                                putchar('\b');
                            }
                            scanf("%d", &change);
                            fflush(stdin);
                            if (change<1||change>3) {
                                gotoxy(30,6);
                                color_set(252);
                                printf("請不要亂輸入喔!");
                                Sleep(3000);
                                color_set(7);
                                gotoxy(30,5);
                                for (i=0;i<50;i++) {
                                    putchar(' ');
                                }
                                gotoxy(30,6);
                                for (i=0;i<15;i++) {
                                    putchar(' ');
                                }
                            }
                            else if (change==1) {
                                change=0;
                                game.lines=0;
                                choose_exit=1;
                                break;
                            }
                            else if (change==2) {
                                change=0;
                                game.cols=0;
                                choose_exit=1;
                                break;
                            }
                            else if (change==3) {
                                change=0;
                                game.landboom=0;
                                choose_exit=1;
                                break;
                            }
                        }
                    break;
                    case 'n':
                    case 'N':
                        gotoxy(30,6);
                        printf("那就開始遊戲囉");
                        Sleep(1000);
                        choose_exit=1;
                        exit=1;
                    break;
                    default:
                        gotoxy(30,6);
                        color_set(252);
                        printf("請不要亂輸入喔!");
                        Sleep(3000);
                        color_set(7);
                        gotoxy(30,4);
                        for (i=0;i<25;i++) {
                            putchar(' ');
                        }
                        gotoxy(30,6);
                        for (i=0;i<15;i++) {
                            putchar(' ');
                        }
                    break;
                }
                if (choose_exit==1) {
                    break;
                }
            }
        }
        if (exit==1) {
            break;
        }
    }
}

void game_dif_choose() {
    int choose, error_in=0;
    while (1) {
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
                game.lines=9;
                game.cols=9;
                game.landboom=10;
                game_content();
                break;
            case 2:
                game.lines=16;
                game.cols=16;
                game.landboom=40;
                game_content();
                break;
            case 3:
                game.lines=30;
                game.cols=16;
                game.landboom=99;
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
        if (game.over==1) {
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
