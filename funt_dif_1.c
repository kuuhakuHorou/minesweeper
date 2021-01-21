#include "funtion.h"

int map_1[LINES_EASY+2][COLS_EASY+2];       //地圖
int map_mark_1[LINES_EASY][COLS_EASY]={0};  //標記地圖

void game_easy() {   //遊戲內容
    int i, j, mod, mod_choose;
    game.over=0;
    game_start_easy();
    print_game_easy();
    while (1) {
        if (game.error==0) {
            gotoxy(40,1);
            printf("請輸入你想做什麼(1:\"踩\"，2:標記): ");
            for (i=0;i<10;i++)
                putchar(' ');
            for (i=0;i<10;i++)
                putchar('\b');
            scanf("%d", &mod);
            fflush(stdin);  //清除輸入緩衝區，以防亂輸入
            switch (mod) {
                case 1:
                    mod_choose=1;
                    break;
                case 2:
                    mod_choose=2;
                    break;
                default:
                    gotoxy(45,5);
                    color_set(12);
                    printf("輸入錯誤!請重新輸入!!");
                    color_set(7);
                    game.error=1;
                    mod_choose=0;   //一開始沒輸入錯誤但後面有時，防止跳入上一個選則
                    Sleep(3000);
                    gotoxy(45,5);
                    for (i=0;i<21;i++)
                        putchar(' ');
                    break;
            }
            if (mod_choose==1) {
                mod_choose_1_easy();
            }
            else if (mod_choose==2) {
                mod_choose_2_easy();
            }
        }
        else if (game.error==1) {
            game.error=0;
            if (mod_choose==1)
                mod_choose_1_easy();
            else if (mod_choose==2)
                mod_choose_2_easy();
        }
        
        if (game.over==1) {
            gotoxy(45,11);
            break;
        }
    }
}

void game_start_easy() {     //遊戲開始設定
    int i, j, enter_x, enter_y, error;
    for (i=0;i<LINES_EASY+2;i++) {
        for (j=0;j<COLS_EASY+2;j++) {
            if (i==0||i==LINES_EASY+1||j==0||j==COLS_EASY+1)
                map_1[i][j]=99;   //99為邊界
            else 
                map_1[i][j]=0;    //0為空地
        }
    }
    while (1) {
        error=0;
        print_game_easy();
        gotoxy(40,1);
        printf("請先輸入一個座標(x y): ");
        scanf("%d%d", &enter_y, &enter_x);
        fflush(stdin);
        if (enter_x>COLS_EASY||enter_x<1||enter_y>LINES_EASY||enter_y<1) {
            error=1;
            gotoxy(45,5);
            color_set(252);
            printf("請不要亂輸入喔!");
            color_set(7);
            Sleep(3000);
        }
        if (error!=1)
            break;
    }
    map_1[enter_y][enter_x]=10;
    landboom_generate_easy(&enter_x,&enter_y);    //生成地雷
    landboom_tester_easy(enter_x,enter_y);
}

void landboom_generate_easy(int *enter_x, int *enter_y) {  //隨機生成地雷
    srand(time(0));
    int i, j, produce_x, produce_y, landboom_num=0;
    do {
        produce_x=rand()%COLS_EASY+1;
        produce_y=rand()%LINES_EASY+1;
        if (map_1[produce_y][produce_x]!=20&&produce_x!=*enter_x&&produce_y!=*enter_y) {
            map_1[produce_y][produce_x]=20;   //20為地雷
            landboom_num++;
        }
    } while (landboom_num<10);
/*    for (int i=0;i<landboom_num;i++) {
 *       produce_x=rand()%COLS_EASY+1;
 *       produce_y=rand()%LINES_EASY+1;
 *       if (map_1[produce_y][produce_x]==20)  //已經為地雷的情況
 *           i--;
 *       map_1[produce_y][produce_x]=20;   //20為地雷
 *   }
 */
}

void landboom_tester_easy(int x,int y) { //測試旁邊八格地雷
    int i, j, landboom;
    landboom=0; //重算地雷數
    
    for (i=y-1;i<=y+1;i++) {        //測試周邊地雷
        for (j=x-1;j<=x+1;j++) {
            if (i==y&&j==x)     //踩的那格不用算
                continue;
            else if (map_1[i][j]==20)
                landboom++;
        }
    }
    
    if (landboom==0) {
        map_1[y][x]=10;   //旁邊沒地雷
        for (i=y-1;i<=y+1;i++) {
            for (j=x-1;j<=x+1;j++) {
                if (map_1[i][j]==0)     //還沒踩過的地方
                    landboom_tester_easy(j,i);
            }
        }
    }
    else
        map_1[y][x]=landboom;  //旁邊有地雷(輸出多少)
}

void mod_choose_1_easy() {   //"踩"
    int i, j, guess_x, guess_y, untreated_items;
    gotoxy(40,2);
    printf("請輸入你想踩哪(x y): ");
    for (i=0;i<10;i++)
        putchar(' ');
    for (i=0;i<10;i++)
        putchar('\b');
    scanf("%d%d", &guess_y, &guess_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (guess_x>COLS_EASY||guess_x<1||guess_y>LINES_EASY||guess_y<1) {    //超出地雷範圍
        gotoxy(45,5);
        color_set(12);
        printf("輸入錯誤!請重新輸入!!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (map_mark_1[guess_y-1][guess_x-1]==1) {
        gotoxy(45,5);
        color_set(12);
        printf("你不能踩標記的地喔!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (map_1[guess_y][guess_x]==20) {
        for (i=0;i<LINES_EASY+2;i++) {
            for (j=0;j<COLS_EASY+2;j++) {
                if (map_1[i][j]==20)
                    map_1[i][j]++;  //輸了以顯示地雷處
                else if (map_mark_1[i-1][j-1]==1&&map_1[i][j]<20)
                    map_mark_1[i-1][j-1]=2;   //標記錯誤，把錯誤顯現出來
            }
        }
        print_game_easy();
        gotoxy(45,10);
        printf("你輸了");
        game.over=1;
        return;
    }
    landboom_tester_easy(guess_x,guess_y);
    print_game_easy();
    untreated_items=0;  //重算沒踩過的地
    for (i=0;i<LINES_EASY+2;i++) {
        for (j=0;j<COLS_EASY+2;j++) {
            if (map_1[i][j]==0) {
                untreated_items++;
            }
            else if (map_1[i][j]<20) {
                continue;   //踩過且不算地雷處
            }
        }
    }
    if (untreated_items==0) {
        for (i=0;i<LINES_EASY+2;i++) {
            for (j=0;j<COLS_EASY+2;j++) {
                if (map_1[i][j]==20)
                    map_1[i][j]=30; //顯現地雷處
            }
        }
        print_game_easy();
        gotoxy(45,10);
        printf("你贏了");
        game.over=1;
        return;
    }
}

void mod_choose_2_easy() {   //標記
    int i, mark_x, mark_y;
    gotoxy(40,2);
    printf("請輸入你想標記哪(x y): ");
    for (i=0;i<10;i++)
        putchar(' ');
    for (i=0;i<10;i++)
        putchar('\b');
    scanf("%d%d", &mark_y, &mark_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (mark_x>COLS_EASY||mark_x<1||mark_y>LINES_EASY||mark_y<1) {//超出地雷範圍
        gotoxy(45,5);
        color_set(12);
        printf("輸入錯誤!請重新輸入!!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (map_1[mark_y][mark_x]==20||map_1[mark_y][mark_x]==0) {
        if (map_mark_1[mark_y-1][mark_x-1]==0) {
            map_mark_1[mark_y-1][mark_x-1]=1; //標記
            game.flag++;
            print_game_easy();
        }
        else if (map_mark_1[mark_y-1][mark_x-1]==1) {
            map_mark_1[mark_y-1][mark_x-1]=0; //清除標記
            game.flag--;
            print_game_easy();
        }
    }
    else
        print_game_easy();
}

void print_game_easy() { //介面
    system("cls");
    int i, j;
    printf("　 　y→\n　　　");
    for (i=1;i<10;i++)
        printf("%2d", i);
    printf("\n x\n↓");  //下為原設計
/*  gotoxy(2,3);
 *  printf("x");
 *  gotoxy(1,4);
 *  printf("↓");
 */
    for (i=1;i<10;i++) {
        gotoxy(3,i+3);
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
    gotoxy(14,1);
    printf("地雷數:10");
    gotoxy(25,1);
    printf("旗子數:%2d", game.flag);
    for (i=0;i<LINES_EASY+2;i++) {
        gotoxy(5,i+3);
        for (j=0;j<COLS_EASY+2;j++) {
            if (map_1[i][j]==99)
                printf("■");         //邊界
            else if (map_1[i][j]==10)
                printf("　");    //踩過的空地
            else if (map_1[i][j]==0||map_1[i][j]==20) {
                if (map_mark_1[i-1][j-1]==0)
                    printf("□");     //還沒踩過的地面&地雷
                else if (map_mark_1[i-1][j-1]==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
                else if (map_mark_1[i-1][j-1]==2) {
                    color_set(12);
                    printf("Χ");     //標記錯誤顯現
                }
            }
            else if (map_1[i][j]<10)
                printf(" %d", map_1[i][j]); //踩過且旁邊有地雷，顯現地雷數
            else if (map_1[i][j]==21) {
                if (map_mark_1[i-1][j-1]==0) {
                    color_set(14);
                    printf("※");    //全場地雷處
                }
                else if (map_mark_1[i-1][j-1]==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            else if (map_1[i][j]==30) {
                if (map_mark_1[i-1][j-1]==0) {
                    color_set(11);
                    printf("⊕");    //過關顯示地雷
                }
                else if (map_mark_1[i-1][j-1]==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            color_set(7);
        }
    }
    putchar('\n');
}