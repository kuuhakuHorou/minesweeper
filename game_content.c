#include "function.h"

extern struct GAME game;
extern struct SET set;

void game_content(MAP *map) {   //遊戲內容
    int mod = 0, mod_choose = 0;
    game.over = false;
    game.error = false;
    map->map = (int*)calloc((set.lines + 2) * (set.cols + 2), sizeof(int));  //地圖
    map->mark = (int*)calloc(set.lines * set.cols, sizeof(int));     //標記地圖
/*  for (i=0;i<set.lines;i++) {
        for (j=0;j<set.cols;j++) {
            *(map_mark+set.cols*i+j)=0;
        }
    } */
    game_start(map);
    print_game(map);
    while (!game.over) {
        if (!game.error) {
            gotoxy(60, 1);
            printf("請輸入你想做什麼(1:\"踩\"，2:標記): ");
            print_space();
            scanf("%d", &mod);
            clean_stdin();
            switch (mod) {
                case 1:
                    mod = 0;
                    mod_choose = 1;
                    break;
                case 2:
                    mod = 0;
                    mod_choose = 2;
                    break;
                default:
                    gotoxy(60, 2);
                    color_set(252);
                    printf("輸入錯誤!請重新輸入!!");
                    color_set(7);
                    game.error = true;
                    mod_choose = 0;   //一開始沒輸入錯誤但後面有時，防止跳入上一個選則
                    Sleep(3000);
                    gotoxy(60, 2);
                    for (int i = 0; i < 21; i++)
                        putchar(' ');
                    break;
            }
            if (mod_choose == 1) {
                mod_choose_1(map);
            }
            else if (mod_choose == 2) {
                mod_choose_2(map);
            }
        }
        else if (game.error) {
            game.error = false;
            if (mod_choose == 1)
                mod_choose_1(map);
            else if (mod_choose == 2)
                mod_choose_2(map);
        }
    }
    color_set(7);
    gotoxy(60, 2);
    free(map->map);
    free(map->mark);
}

void game_start(MAP *map) {     //遊戲開始設定
    Coor loop;
    bool error = false;
    Coor enter = {0, 0};
    for (loop.y = 0; loop.y < set.lines + 2; loop.y++) {
        for (loop.x = 0; loop.x < set.cols + 2; loop.x++) {
            if (loop.y == 0 || loop.y == set.lines+1 || loop.x == 0 || loop.x == set.cols+1)
                map->map[map_location(loop, Map)] = 99;   //99為邊界
            else
                map->map[map_location(loop, Map)] = 0;    //0為空地
        }
    }
    do {
        error = false;
        print_game(map);
        gotoxy(60, 1);
        printf("請先輸入一個座標(x y): ");
        scanf("%d%d", &enter.y, &enter.x);
        fflush(stdin);
        if (over_range(enter)) {
            error = true;
            gotoxy(60,2);
            color_set(252);
            printf("請不要亂輸入喔!");
            color_set(7);
            Sleep(3000);
        }
    } while (error);
    map->map[map_location(enter, Map)] = 10;
    landboom_generate(enter, map->map);    //生成地雷
    landboom_tester(enter, map);
}

void landboom_generate(Coor test, int *map) {  //隨機生成地雷
    srand(time(NULL));
    Coor produce;
    int landboom_num = 0;
    int *p_map;
    while (landboom_num < set.landboom) {
        produce.x = rand() % set.cols + 1;
        produce.y = rand() % set.lines + 1;
        p_map = map + map_location(produce, Map);
        if (*p_map != 20) {
            if (produce.x == test.x && produce.y == test.y) {
                continue;
            }
            *p_map = 20;   //20為地雷
            landboom_num++;
        }
    }
/*    for (int i=0;i<landboom_num;i++) {
 *       produce_x=rand()%COLS_EASY+1;
 *       produce_y=rand()%LINES_EASY+1;
 *       if (map_1[produce_y][produce_x]==20)  //已經為地雷的情況
 *           i--;
 *       map_1[produce_y][produce_x]=20;   //20為地雷
 *   }
 */
}

void landboom_tester(Coor test, MAP *map) { //測試旁邊八格地雷
    Coor loop;
    int landboom = 0; //重算地雷數

    for (loop.y = test.y - 1; loop.y <= test.y + 1; loop.y++) {        //測試周邊地雷
        for (loop.x = test.x - 1; loop.x <= test.x + 1; loop.x++) {
            if (loop.x == test.x && loop.y == test.y)     //踩的那格不用算
                continue;
            else if (location_value(map, loop, Map) == 20)
                landboom++;
        }
    }

    if (landboom == 0) {
        map->map[map_location(test, Map)] = 10;   //旁邊沒地雷
        for (loop.y = test.y - 1; loop.y <= test.y + 1; loop.y++) {
            for (loop.x = test.x - 1; loop.x <= test.x + 1; loop.x++) {
                if (location_value(map, loop, Map) == 0 && location_value(map, loop, Mark) == 0)     //還沒踩過且沒標記的地方
                    landboom_tester(loop, map);
            }
        }
    }
    else
        map->map[map_location(test, Map)] = landboom;  //旁邊有地雷(輸出多少)
}

void mod_choose_1(MAP *map) {   //"踩"
    Coor guess = {0, 0}, loop;
    int val_map, val_mark, untreated_places;
    gotoxy(60, 2);
    printf("請輸入你想踩哪(x y): ");
    print_space();
    scanf("%d%d", &guess.y, &guess.x);
    clean_stdin();
    val_map = location_value(map, guess, Map);
    val_mark = location_value(map, guess, Mark);
    if (over_range(guess)) {    //超出地雷範圍
        print_warning(OverCoordinate);
        return;
    }
    else if (val_mark == 1) {
        print_warning(MarkCoordinate);
        return;
    }
    else if(val_map < 10 && val_map > 0) {
        int flags = 0;
        for (loop.y = guess.y - 1; loop.y <= guess.y + 1; loop.y++) {
            for (loop.x = guess.x - 1; loop.x <= guess.x + 1; loop.x++) {
                if (location_value(map, loop, Mark) == 1) {
                    flags++;
                }
            }
        }
        if (flags == val_map) {
            for (loop.y = guess.y - 1; loop.y <= guess.y + 1; loop.y++) {
                for (loop.x = guess.x - 1; loop.x <= guess.x + 1; loop.x++) {
                    if (location_value(map, loop, Map) == 0) {
                        landboom_tester(loop, map);
                    }
                }
            }
        }
    }
    else if (val_map == 20) {
        for (loop.y = 0; loop.y < set.lines + 2; loop.y++) {
            for (loop.x = 0; loop.x < set.cols + 2; loop.x++) {
                if (location_value(map, loop, Map) == 20)
                    map->map[map_location(loop, Map)] += 1;  //輸了以顯示地雷處
                else if (location_value(map, loop, Map) == 1 && location_value(map, loop, Map) < 20)
                    map->mark[map_location(loop, Mark)] = 2;   //標記錯誤，把錯誤顯現出來
            }
        }
        print_game(map);
        gotoxy(60,1);
        color_set(192);
        printf("你輸了");
        game.over = true;
        return;
    }
    landboom_tester(guess, map);
    print_game(map);
    untreated_places = 0;  //重算沒踩過的地
    for (loop.y = 0; loop.y < set.lines + 2; loop.y++) {
        for (loop.x = 0; loop.x < set.cols + 2; loop.x++) {
            if (location_value(map, loop, Map) == 0) {
                untreated_places++;
            }
            else if (location_value(map, loop, Map) < 20) {
                continue;   //踩過且不算地雷處
            }
        }
    }
    if (untreated_places == 0) {
        for (loop.y = 0; loop.y < set.lines + 2; loop.y++) {
            for (loop.x = 0; loop.x < set.cols + 2; loop.x++) {
                if (location_value(map, loop, Map) == 20)
                    map->map[map_location(loop, Map)] = 30; //顯現地雷處
            }
        }
        print_game(map);
        gotoxy(60,1);
        color_set(224);
        printf("你贏了");
        game.over = true;
        return;
    }
}

void mod_choose_2(MAP *map) {   //標記
    Coor mark = {0, 0};
    int *p_mark;
    int val_map;
    gotoxy(60, 2);
    printf("請輸入你想標記哪(x y): ");
    print_space();
    scanf("%d%d", &mark.y, &mark.x);
    clean_stdin();
    if (over_range(mark)) {//超出地雷範圍
        print_warning(OverCoordinate);
        return;
    }
    val_map = location_value(map, mark, Map);
    if (val_map == 20 || val_map == 0) {
        p_mark = &map->mark[map_location(mark, Mark)];
        if (*p_mark == 0) {
            *p_mark = 1; //標記
            game.flag++;
        }
        else if (*p_mark == 1) {
            *p_mark = 0; //清除標記
            game.flag--;
        }
    }
    if (val_map > 0 ) {

    }
    print_game(map);
}

void print_game(MAP *map) { //介面
    system("cls");
    Coor loop;
    int val_map, val_mark;
    printf("　 　y→\n　　　");
    for (int i = 1; i < set.cols + 1; i++)
        printf("%2d", i);
    printf("\n x\n↓");  //下為原設計
/*  gotoxy(2,3);
 *  printf("x");
 *  gotoxy(1,4);
 *  printf("↓");
 */
    for (int i = 1; i < set.lines + 1; i++) {
        gotoxy(3, i + 3);
        printf("%2d", i);
    }
/*  printf("　　 y→\n"        原輸出
 *         "　　　 1 2 3 4 5 6 7 8 9\n"
 *         " x　\n"
 *         "↓ 1\n"
 *         "　 2\n"
 *         "　 3\n"
 *         "　 4\n"
 *         "　 5\n"
 *         "　 6\n"
 *         "　 7\n"
 *         "　 8\n"
 *         "　 9\n");
 */
    gotoxy(14, 1);
    printf("地雷數:%d", set.landboom);
    gotoxy(25, 1);
    printf("旗子數:%2d", game.flag);
    for (loop.y = 0; loop.y < set.lines + 2; loop.y++) {
        gotoxy(5, loop.y + 3);
        for (loop.x = 0; loop.x < set.cols + 2; loop.x++) {
            val_map = location_value(map, loop, Map);
            val_mark = location_value(map, loop, Mark);
            if (val_map == 99) {
                color_set(51);
                printf("■");         //邊界
            }
            else if (val_map == 10) {
                color_set(119);
                printf("■");    //踩過的空地
            }
            else if (val_map == 0 || val_map == 20) {
                if (val_mark == 0) {
                    color_set(135);
                    printf("■");     //還沒踩過的地面&地雷
                }
                else if (val_mark == 1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
                else if (val_mark == 2) {
                    color_set(12);
                    printf("Χ");     //標記錯誤顯現
                }
            }
            else if (val_map < 10) {
                color_set(23);
                printf(" %d", val_map); //踩過且旁邊有地雷，顯現地雷數
            }
            else if (val_map == 21) {
                if (val_mark == 0) {
                    color_set(14);
                    printf("※");    //全場地雷處
                }
                else if (val_mark == 1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            else if (val_map == 30) {
                if (val_mark == 0) {
                    color_set(11);
                    printf("⊕");    //過關顯示地雷
                }
                else if (val_mark == 1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            color_set(7);
        }
    }
    for (int i = 1; i <= 36; i++) {
        gotoxy(58, i);
        printf("|");
    }
}
