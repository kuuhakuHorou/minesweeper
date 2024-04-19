#include "function.h"

struct COOR player;
extern struct GAME game;

void game_content(MAP *map) {   //遊戲內容
    player.x = 99;
    player.y = 99;
    map->map = (int*)calloc((game.set.lines + 2) * (game.set.cols + 2), sizeof(int));  //地圖
    map->mark = (int*)calloc(game.set.lines * game.set.cols, sizeof(int));     //標記地圖
    game_start(map);
    is_hide_cursor(true);
    print_game(map, true);
    is_win(map);
    while (!game.over) {
        interactive_mod(map);
    }
    is_hide_cursor(false);
    color_set(0x07);
    free(map->map);
    free(map->mark);
}

void game_start(MAP *map) {     //遊戲開始設定
    Coor loop;
    bool error = false;
    Coor enter = {0, 0};
    for (loop.y = 0; loop.y < game.set.lines + 2; loop.y++) {
        for (loop.x = 0; loop.x < game.set.cols + 2; loop.x++) {
            if (loop.y == 0 || loop.y == game.set.lines + 1 ||
                loop.x == 0 || loop.x == game.set.cols + 1)
                map->map[map_location(loop, Map)] = Border;   //99為邊界
            else
                map->map[map_location(loop, Map)] = NoSweep;    //0為空地
        }
    }
    do {
        error = false;
        print_game(map, true);
        gotoxy(60, 4);
        printf("請先輸入一個座標(x y): ");
        scanf("%d%d", &enter.y, &enter.x);
        fflush(stdin);
        if (over_range(enter)) {
            error = true;
            gotoxy(60, 5);
            color_set(0xfc);
            printf("請不要亂輸入喔!");
            color_set(0x07);
            Sleep(3000);
        }
    } while (error);
    player = enter;
    map->map[map_location(enter, Map)] = Space;
    landboom_generate(enter, map->map);    //生成地雷
    landboom_tester(enter, map);
}

void interactive_mod(MAP *map) {
    int keyboard;
    while (!kbhit());
    keyboard = getch();
    switch (keyboard) {
        case 'w':
        case 'W':
        case 72:
            player.y--;
            if (player.y == 0) {
                player.y = game.set.lines;
            }
            print_game(map, false);
            break;
        case 'a':
        case 'A':
        case 75:
            player.x--;
            if (player.x == 0) {
                player.x = game.set.cols;
            }
            print_game(map, false);
            break;
        case 's':
        case 'S':
        case 80:
            player.y++;
            if (player.y == game.set.lines + 1) {
                player.y = 1;
            }
            print_game(map, false);
            break;
        case 'd':
        case 'D':
        case 77:
            player.x++;
            if (player.x == game.set.cols + 1) {
                player.x = 1;
            }
            print_game(map, false);
            break;
        case 'f':
        case 'F':
        case '2':
            map_mark(map, false);
            break;
        case 'e':
        case 'E':
        case '1':
            sweep(map, false);
            break;
        case 'r':
        case 'R':
            game.over = true;
            game.restart = true;
            break;
        case 't':
        case 'T':
            text_mod(map);
            break;
        case 27:
            game.over = true;
            game.exit = true;
            break;
    }
}

void is_win(MAP *map) {  //測試是否贏了
    int untreated_places = 0;  //沒踩過的地
    Coor loop;
    for (loop.y = 0; loop.y < game.set.lines + 2; loop.y++) {
        for (loop.x = 0; loop.x < game.set.cols + 2; loop.x++) {
            if (location_value(map, loop, Map) == NoSweep) {
                untreated_places++;
            }
            else if (location_value(map, loop, Map) < Landboom) {
                continue;   //踩過且不算地雷處
            }
        }
    }
    if (untreated_places == 0) {
        for (loop.y = 0; loop.y < game.set.lines + 2; loop.y++) {
            for (loop.x = 0; loop.x < game.set.cols + 2; loop.x++) {
                if (location_value(map, loop, Map) == Landboom)
                    map->map[map_location(loop, Map)] = WinLandboom;  //顯現地雷處
            }
        }
        game.over = true;
        print_game(map, true);
        gotoxy(60, 4);
        color_set(0xe0);
        printf("你贏了");
    }
}

void landboom_generate(Coor test, int *map) {  //隨機生成地雷
    srand(time(NULL));
    Coor produce;
    int landboom_num = 0;
    int *p_map;
    while (landboom_num < game.set.landboom) {
        produce.x = rand() % game.set.cols + 1;
        produce.y = rand() % game.set.lines + 1;
        p_map = map + map_location(produce, Map);
        if (*p_map != Landboom && !compare_coor(produce, test)) {
            *p_map = Landboom;
            landboom_num++;
        }
    }
}

void landboom_tester(Coor test, MAP *map) { //測試旁邊八格地雷
    if (over_range(test))
        return;
    Coor loop;
    int landboom = 0; //重算地雷數
    int val_map = location_value(map, test, Map);
    int val_mark = location_value(map, test, Mark);
    if (val_mark != NoFlag || (val_map > NoSweep && val_map < Space))
        //不看旗子和周邊有地雷的地方
        return;
    if (location_value(map, test, Map) == Landboom) {
        map->map[map_location(test, Map)] = SweepLandboom;
        game.sweep_landboom = true;
        return;
    }
    for (loop.y = test.y - 1; loop.y <= test.y + 1; loop.y++) {        //測試周邊地雷
        for (loop.x = test.x - 1; loop.x <= test.x + 1; loop.x++) {
            if (compare_coor(test, loop))     //踩的那格不用算
                continue;
            else if (location_value(map, loop, Map) == Landboom ||
                     location_value(map, loop, Map) == SweepLandboom)
                landboom++;
        }
    }
    if (landboom == 0) {
        map->map[map_location(test, Map)] = Space;   //旁邊沒地雷
        for (loop.y = test.y - 1; loop.y <= test.y + 1; loop.y++) {
            for (loop.x = test.x - 1; loop.x <= test.x + 1; loop.x++) {
                //還沒踩過且沒標記的地方
                if (location_value(map, loop, Map) == NoSweep &&
                    location_value(map, loop, Mark) == NoFlag)
                    landboom_tester(loop, map);
            }
        }
    }
    else
        map->map[map_location(test, Map)] = landboom;  //旁邊有地雷(輸出多少)
}

bool mod_choose(int mod, MAP *map, bool isTextmod) {
    if (mod == 1) {
        return sweep(map, isTextmod);
    }
    return map_mark(map, isTextmod);
}

bool sweep(MAP *map, bool isTextmod) {   //"踩"
    Coor guess = {0, 0}, loop;
    if (isTextmod) {
        gotoxy(60, 6);
        printf("請輸入你想踩哪(x y): ");
        print_space();
        scanf("%d%d", &guess.y, &guess.x);
        clean_stdin();
    }
    else {
        guess = player;
    }
    if (over_range(guess)) {    //超出地雷範圍
        print_warning(OverCoordinate);
        return false;
    }
    else if (location_value(map, guess, Mark) == Flag) {   //踩標記
        print_warning(MarkCoordinate);
        return false;
    }
    else if(location_value(map, guess, Map) < Space &&
            location_value(map, guess, Map) > NoSweep) {
        //踩數字(九宮格地雷數)
        int flags = 0;
        for (loop.y = guess.y - 1; loop.y <= guess.y + 1; loop.y++) {
            for (loop.x = guess.x - 1; loop.x <= guess.x + 1; loop.x++) {
                if (over_range(loop))
                    continue;
                if (location_value(map, loop, Mark) == Flag) {
                    flags++;
                }
            }
        }
        if (flags == location_value(map, guess, Map)) {
            for (loop.y = guess.y - 1; loop.y <= guess.y + 1; loop.y++) {
                for (loop.x = guess.x - 1; loop.x <= guess.x + 1; loop.x++) {
                    landboom_tester(loop, map);
                }
            }
        }
    }
    //踩到地雷處
    if (location_value(map, guess, Map) == Landboom || game.sweep_landboom) {
        if (!game.sweep_landboom) {
            //明確顯示踩到的地雷
            map->map[map_location(guess, Map)] = SweepLandboom;
        }
        for (loop.y = 0; loop.y < game.set.lines + 2; loop.y++) {
            for (loop.x = 0; loop.x < game.set.cols + 2; loop.x++) {
                if (location_value(map, loop, Map) == Landboom) {
                    //輸了以顯示地雷處
                    map->map[map_location(loop, Map)] = LoseLandboom;
                }
                else if (location_value(map, loop, Mark) == Flag &&
                         location_value(map, loop, Map) < Landboom) {
                    //標記錯誤，把錯誤顯現出來
                    map->mark[map_location(loop, Mark)] = WrongFlag;
                }
            }
        }
        game.over = true;
        print_game(map, isTextmod);
        gotoxy(60, 4);
        color_set(0xc0);
        printf("你輸了");
        return true;
    }
    player = guess;
    landboom_tester(guess, map);
    print_game(map, isTextmod);
    is_win(map);
    return true;
}

bool map_mark(MAP *map, bool isTextmod) {   //標記
    Coor mark = {0, 0};
    int *p_mark;
    int val_map;
    if (isTextmod) {
        gotoxy(60, 6);
        printf("請輸入你想標記哪(x y): ");
        print_space();
        scanf("%d%d", &mark.y, &mark.x);
        clean_stdin();
    }
    else {
        mark = player;
    }
    if (over_range(mark)) { //超出地雷範圍
        print_warning(OverCoordinate);
        return false;
    }
    val_map = location_value(map, mark, Map);
    if (val_map == Landboom || val_map == NoSweep) {
        p_mark = &map->mark[map_location(mark, Mark)];
        if (*p_mark == NoFlag) {
            *p_mark = Flag;    //標記
            game.flag++;
        }
        else if (*p_mark == Flag) {
            *p_mark = NoFlag;    //清除標記
            game.flag--;
        }
    }
    player = mark;
    print_game(map, isTextmod);
    return true;
}

void print_game(MAP *map, bool isClean) { //介面
    gotoxy(1, 1);
    if (isClean) {
        system("cls");
    }
    Coor loop;
    int val_map, val_mark;
    printf("　 　y→\n　　　");
    for (int i = 1; i < game.set.cols + 1; i++)
        printf("%2d", i);
    printf("\n x\n↓");
    for (int i = 1; i < game.set.lines + 1; i++) {
        gotoxy(3, i + 3);
        printf("%2d", i);
    }
    gotoxy(14, 1);
    printf("地雷數:%d", game.set.landboom);
    gotoxy(25, 1);
    printf("旗子數:%2d", game.flag);
    for (loop.y = 0; loop.y < game.set.lines + 2; loop.y++) {
        gotoxy(5, loop.y + 3);
        for (loop.x = 0; loop.x < game.set.cols + 2; loop.x++) {
            val_map = location_value(map, loop, Map);
            val_mark = location_value(map, loop, Mark);
            if (val_map == Border) {
                color_set(0xaa);
                printf("  ");         //邊界
            }
            else if (compare_coor(loop, player) && !game.over) { //玩家位置處
                if (val_map == NoSweep || val_map == Landboom) {
                    if (val_mark == NoFlag) {
                        color_set(0x86);    //還沒踩過的地面&地雷
                        printf("■");
                    }
                    else if (val_mark == Flag) {
                        color_set(0x64);
                        printf("◢");    //地雷標記顯現
                    }
                }
                else if (val_map == Space) {
                    color_set(0x68);
                    printf("■");    //踩過的空地
                }
                else if (val_map < Space) {
                    color_set(0x61);
                    printf(" %d", val_map); //踩過且旁邊有地雷，顯現地雷數
                }
            }
            else if (val_mark == Flag) {
                color_set(0x8c);
                printf("◢");    //地雷標記顯現
            }
            else if (val_mark == WrongFlag) {
                color_set(0x8c);
                printf("Χ");     //標記錯誤顯現
            }
            else if (val_map == Space) {
                color_set(0x88);
                printf("  ");    //踩過的空地
            }
            else if (val_map == NoSweep || val_map == Landboom) {
                color_set(0x87);
                printf("■");     //還沒踩過的地面&地雷
            }
            else if (val_map < Space) {
                color_set(0x8b);
                printf(" %d", val_map); //踩過且旁邊有地雷，顯現地雷數
            }
            else if (val_map == LoseLandboom) {
                color_set(0x8e);
                printf("※");    //全場地雷處
            }
            else if (val_map == SweepLandboom) {
                color_set(0xce);
                printf("※");    //踩到地雷處
            }
            else if (val_map == WinLandboom) {
                color_set(0x8e);
                printf("◎");    //過關顯示地雷
            }
        }
    }
    color_set(0x07);
    for (int i = 1; i <= 36; i++) {
        gotoxy(58, i);
        printf("|");
    }
    gotoxy(60, 1);
    printf("w,a,s,d & 方向鍵: 移動方向\te & 1: \"踩\"  f & 2: 標記");
    gotoxy(60, 2);
    printf("t: 文字模式  r: 重新開始\tesc: 離開");
    gotoxy(60, 3);
    for (int i = 0; i < 60; i++) {
        putchar('-');
    }
    gotoxy(60, 4);
}

void text_mod(MAP *map) {
    int mod = 0, choose = 0;
    bool over = false;
    is_hide_cursor(false);
    while(!over || game.error) {
        if (!game.error) {
            mod = 0;
            choose = 0;
            gotoxy(60, 4);
            printf("1:\"踩\", 2:標記, 3:重新開始, 4:退出文字模式");
            gotoxy(60, 5);
            printf("請輸入你想做什麼: ");
            print_space();
            scanf("%d", &mod);
            clean_stdin();
            switch (mod) {
                case 1:
                    mod = 0;
                    choose = 1;
                    break;
                case 2:
                    mod = 0;
                    choose = 2;
                    break;
                case 3:
                    mod = 0;
                    choose = 0;
                    over = true;
                    game.over = true;
                    game.restart = true;
                    break;
                case 4:
                    mod = 0;
                    choose = 0;
                    over = true;
                    print_game(map, true);
                    break;
                default:
                    gotoxy(60, 6);
                    color_set(0xfc);
                    printf("輸入錯誤!請重新輸入!!");
                    color_set(0x07);
                    game.error = true;
                    choose = 0;   //一開始沒輸入錯誤但後面有時，防止跳入上一個選則
                    Sleep(3000);
                    gotoxy(60, 6);
                    for (int i = 0; i < 30; i++)
                        putchar(' ');
                    break;
            }
            if (choose != 0) {
                over = mod_choose(choose, map, true);
            }
        }
        else if (game.error) {
            game.error = false;
            if (choose != 0) {
                over = mod_choose(choose, map, true);
            }
        }
    }
    is_hide_cursor(true);
}
