#include "function.h"

struct SET set;
struct GAME game;

void clean_stdin(void) {  //清除輸入緩衝區
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void color_set(int color_num) {     //https://baike.baidu.com/item/SetConsoleTextAttribute 顏色對應值
    HANDLE hOut;
    hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, color_num);
}

void degree_of_difficulty(void) {       //自訂難度
    int choose, change;
    char decide = '\0';
    bool exit = false, choose_exit = false, error = false;
    while (!exit) {
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
                    print_space();
                    scanf("%d", &set.lines);
                    clean_stdin();
                    if (set.lines < 9 || set.lines > 30) {
                        error = true;
                        print_warning(OverRange);
                    }
                } while (error);
                break;
            case 2:
                do {
                    error = false;
                    gotoxy(30,2);
                    printf("請輸入寬度y(最小9,最大24): ");
                    print_space();
                    scanf("%d", &set.cols);
                    clean_stdin();
                    if (set.cols < 9 || set.cols > 24) {
                        error = true;
                        print_warning(OverRange);
                    }
                } while (error);
                break;
            case 3:
                do {
                    error = false;
                    gotoxy(30,3);
                    printf("請輸入炸彈數(最小10,最大%-3d): ", (set.lines-1) * (set.cols-1));
                    print_space();
                    scanf("%d", &set.landboom);
                    clean_stdin();
                    if (set.landboom < 10 || set.landboom > (set.lines-1) * (set.cols-1)) {
                        error = true;
                        print_warning(OverRange);
                    }
                } while (error);
                break;
        }
        if (set.lines != 0 && set.cols != 0 && set.landboom != 0) {
            change = 0;
            while (!choose_exit) {
                gotoxy(30,4);
                printf("有要調整的嗎?(Y/N) ");
                print_space();
                scanf("%c", &decide);
                clean_stdin();
                switch (decide) {
                    case 'y':
                    case 'Y':
                        do {
                            error = false;
                            gotoxy(30,5);
                            printf("請問需要調整哪裡呢?(1:長度 2:寬度 3:地雷數) ");
                            print_space();
                            scanf("%d", &change);
                            clean_stdin();
                            switch (change) {
                                case 1:
                                    change = 0;
                                    set.lines = 0;
                                    set.landboom = 0;
                                    choose_exit = true;
                                    break;
                                case 2:
                                    change = 0;
                                    set.cols = 0;
                                    set.landboom = 0;
                                    choose_exit = true;
                                    break;
                                case 3:
                                    change = 0;
                                    set.landboom = 0;
                                    choose_exit = true;
                                    break;
                                default:
                                    error = true;
                                    print_warning(EnterError);
                                    break;
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
                        print_warning(EnterError);
                        break;
                }
            }
        }
    }
}

void gotoxy(int x,int y) {  //指定座標(x和y從0開始算，所以減1，比較直觀)
    COORD c;
    c.X = x-1;
    c.Y = y-1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void game_dif_choose(void) {        //遊戲難度選則
    int choose = 0,  i;
    bool over = false, continue_q = false;
    MAP map;
    while (!over) {
        reset();
        printf("請問要玩怎樣的難度呢\n"
               "    簡單:1\n"
               "    普通:2\n"
               "    困難:3\n"
               "    自定義:4\n"
               "  請輸入: ");
        scanf("%d", &choose);
        clean_stdin();
        switch (choose) {
            case 1:
                set.lines = 9;
                set.cols = 9;
                set.landboom = 10;
                game_content(&map);
                break;
            case 2:
                set.lines = 16;
                set.cols = 16;
                set.landboom = 40;
                game_content(&map);
                break;
            case 3:
                set.lines = 30;
                set.cols = 16;
                set.landboom = 99;
                game_content(&map);
                break;
            case 4:
                degree_of_difficulty();
                game_content(&map);
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
            print_space();
            scanf("%c", &game.continue_q);
            clean_stdin();
            switch (game.continue_q) {
                case 'y':
                case 'Y':
                    continue_q = true;
                    gotoxy(60,3);
                    printf("那就重新開始");
                    Sleep(1000);
                    break;
                case 'n':
                case 'N':
                    over = true;
                    gotoxy(60,3);
                    printf("下次再見");
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
            if (over || continue_q) {
                break;
            }
        }
        if (continue_q) {
            system("cls");
        }
    }
}

void print_space(void) {    //印空白(10個)
    for (int i = 0; i < 10; i++) {
        putchar(' ');
    }
    for (int i = 0; i < 10; i++) {
        putchar('\b');
    }
}

void print_warning(Warning warn) {  //輸入錯誤警告
    int i;
    switch(warn) {
        case OverRange:
            gotoxy(30,6);
            color_set(252);
            printf("請不要超過範圍喔!");
            Sleep(3000);
            color_set(7);
            gotoxy(30,6);
            for (i = 0; i < 17; i++) {
                putchar(' ');
            }
            break;
        case EnterError:
            gotoxy(30,6);
            color_set(252);
            printf("請不要亂輸入喔!");
            Sleep(3000);
            color_set(7);
            gotoxy(30,6);
            for (i = 0; i < 15; i++) {
                putchar(' ');
            }
            break;
        case OverCoordinate:
            gotoxy(60,3);
            color_set(252);
            printf("輸入錯誤!請重新輸入!!");
            color_set(7);
            game.error = true;
            Sleep(3000);
            gotoxy(60,3);
            for (i = 0;i < 21; i++)
                putchar(' ');
            break;
        case MarkCoordinate:
            gotoxy(60,3);
            color_set(252);
            printf("你不能踩標記的地喔!");
            color_set(7);
            game.error = true;
            Sleep(3000);
            gotoxy(60,3);
            for (i = 0; i < 21; i++)
                putchar(' ');
            break;
    }
}

int map_location(Coor test, Type type) {  //計算陣列位置
    int location;
    switch (type) {
        case Map:
            location = (set.cols + 1) * test.y + test.x;
            break;
        case Mark:
            location = set.cols * (test.y - 1) + (test.x - 1);
            break;
    }
    return location;
}

bool over_range(Coor c) {   //計算有無超出地圖範圍
    if (c.x < 1 || c.x > set.cols || c.y < 1 || c.y > set.lines) {
        return true;
    }
    else {
        return false;
    }
}

void reset(void) {  //重設遊戲參數
    set.cols = 0;
    set.lines = 0;
    set.landboom = 0;
    game.error = false;
    game.over = false;
    game.flag = 0;
    game.continue_q = '\0';
}

int location_value(MAP *map, Coor coor, Type type) { //計算地圖位置數值
    int value = 0;
    switch(type) {
        case Map:
            value = map->map[map_location(coor, type)];
            break;
        case Mark:
            value = map->mark[map_location(coor, type)];
            break;
    }
    return value;
}
