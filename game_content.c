#include "funtion.h"

void game_content() {   //遊戲內容
    int i, j, mod, mod_choose;
    game.over=0;
    int *map=malloc((game.lines+2)*(game.cols+2)*sizeof(int));  //地圖
    int *map_mark=malloc(game.lines*game.cols*sizeof(int));     //標記地圖
    for (i=0;i<game.lines;i++) {
        for (j=0;j<game.cols;j++) {
            *(map_mark+game.cols*i+j)=0;
        }
    }
    game_start(map,map_mark);
    print_game(map,map_mark);
    while (1) {
        if (game.error==0) {
            gotoxy(45,1);
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
                    gotoxy(50,5);
                    color_set(12);
                    printf("輸入錯誤!請重新輸入!!");
                    color_set(7);
                    game.error=1;
                    mod_choose=0;   //一開始沒輸入錯誤但後面有時，防止跳入上一個選則
                    Sleep(3000);
                    gotoxy(50,5);
                    for (i=0;i<21;i++)
                        putchar(' ');
                    break;
            }
            if (mod_choose==1) {
                mod_choose_1(map,map_mark);
            }
            else if (mod_choose==2) {
                mod_choose_2(map,map_mark);
            }
        }
        else if (game.error==1) {
            game.error=0;
            if (mod_choose==1)
                mod_choose_1(map,map_mark);
            else if (mod_choose==2)
                mod_choose_2(map,map_mark);
        }
        
        if (game.over==1) {
            gotoxy(50,11);
            break;
        }
    }
    free(map);
    free(map_mark);
}

void game_start(int *map,int *map_mark) {     //遊戲開始設定
    int i, j, enter_x, enter_y, error;
    for (i=0;i<game.lines+2;i++) {
        for (j=0;j<game.cols+2;j++) {
            if (i==0||i==game.lines+1||j==0||j==game.cols+1)
                *(map+(game.cols+1)*i+j)=99;   //99為邊界
            else 
                *(map+(game.cols+1)*i+j)=0;    //0為空地
        }
    }
    while (1) {
        error=0;
        print_game(map,map_mark);
        gotoxy(45,1);
        printf("請先輸入一個座標(x y): ");
        scanf("%d%d", &enter_y, &enter_x);
        fflush(stdin);
        if (enter_x>game.cols||enter_x<1||enter_y>game.lines||enter_y<1) {
            error=1;
            gotoxy(50,5);
            color_set(252);
            printf("請不要亂輸入喔!");
            color_set(7);
            Sleep(3000);
        }
        if (error!=1)
            break;
    }
    *(map+(game.cols+1)*enter_y+enter_x)=10;
    landboom_generate(&enter_x,&enter_y,map);    //生成地雷
    landboom_tester(enter_x,enter_y,map);
}

void landboom_generate(int *enter_x, int *enter_y,int *map) {  //隨機生成地雷
    srand(time(0));
    int i, j, produce_x, produce_y, landboom_num=0;
    int *p_map;
    do {
        produce_x=rand()%game.cols+1;
        produce_y=rand()%game.lines+1;
        p_map=map+(game.cols+1)*produce_y+produce_x;
        if (*p_map!=20&&produce_x!=*enter_x&&produce_y!=*enter_y) {
            *p_map=20;   //20為地雷
            landboom_num++;
        }
    } while (landboom_num<game.landboom);
/*    for (int i=0;i<landboom_num;i++) {
 *       produce_x=rand()%COLS_EASY+1;
 *       produce_y=rand()%LINES_EASY+1;
 *       if (map_1[produce_y][produce_x]==20)  //已經為地雷的情況
 *           i--;
 *       map_1[produce_y][produce_x]=20;   //20為地雷
 *   }
 */
}

void landboom_tester(int x,int y,int *map) { //測試旁邊八格地雷
    int i, j, landboom;
    landboom=0; //重算地雷數
    
    for (i=y-1;i<=y+1;i++) {        //測試周邊地雷
        for (j=x-1;j<=x+1;j++) {
            if (i==y&&j==x)     //踩的那格不用算
                continue;
            else if (*(map+(game.cols+1)*i+j)==20)
                landboom++;
        }
    }
    
    if (landboom==0) {
        *(map+(game.cols+1)*y+x)=10;   //旁邊沒地雷
        for (i=y-1;i<=y+1;i++) {
            for (j=x-1;j<=x+1;j++) {
                if (*(map+(game.cols+1)*i+j)==0)     //還沒踩過的地方
                    landboom_tester(j,i,map);
            }
        }
    }
    else
        *(map+(game.cols+1)*y+x)=landboom;  //旁邊有地雷(輸出多少)
}

void mod_choose_1(int *map,int *map_mark) {   //"踩"
    int i, j, guess_x, guess_y, untreated_items;
    gotoxy(45,2);
    printf("請輸入你想踩哪(x y): ");
    for (i=0;i<10;i++)
        putchar(' ');
    for (i=0;i<10;i++)
        putchar('\b');
    scanf("%d%d", &guess_y, &guess_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (guess_x>game.cols||guess_x<1||guess_y>game.lines||guess_y<1) {    //超出地雷範圍
        gotoxy(50,5);
        color_set(12);
        printf("輸入錯誤!請重新輸入!!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(50,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (*(map_mark+game.cols*(guess_y-1)+(guess_x-1))==1) {
        gotoxy(50,5);
        color_set(12);
        printf("你不能踩標記的地喔!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(50,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (*(map+(game.cols+1)*guess_y+guess_x)==20) {
        for (i=0;i<game.lines+2;i++) {
            for (j=0;j<game.cols+2;j++) {
                if (*(map+(game.cols+1)*i+j)==20)
                    *(map+(game.cols+1)*i+j)+=1;  //輸了以顯示地雷處
                else if (*(map_mark+game.cols*(i-1)+(j-1))==1&&*(map+game.cols*i+j)<20)
                    *(map_mark+game.cols*(i-1)+(j-1))=2;   //標記錯誤，把錯誤顯現出來
            }
        }
        print_game(map,map_mark);
        gotoxy(50,10);
        printf("你輸了");
        game.over=1;
        return;
    }
    landboom_tester(guess_x,guess_y,map);
    print_game(map,map_mark);
    untreated_items=0;  //重算沒踩過的地
    for (i=0;i<game.lines+2;i++) {
        for (j=0;j<game.cols+2;j++) {
            if (*(map+(game.cols+1)*i+j)==0) {
                untreated_items++;
            }
            else if (*(map+(game.cols+1)*i+j)<20) {
                continue;   //踩過且不算地雷處
            }
        }
    }
    if (untreated_items==0) {
        for (i=0;i<game.lines+2;i++) {
            for (j=0;j<game.cols+2;j++) {
                if (*(map+(game.cols+1)*i+j)==20)
                    *(map+(game.cols+1)*i+j)=30; //顯現地雷處
            }
        }
        print_game(map,map_mark);
        gotoxy(50,10);
        printf("你贏了");
        game.over=1;
        return;
    }
}

void mod_choose_2(int *map,int *map_mark) {   //標記
    int i, mark_x, mark_y;
    int *p_mark;
    gotoxy(45,2);
    printf("請輸入你想標記哪(x y): ");
    for (i=0;i<10;i++)
        putchar(' ');
    for (i=0;i<10;i++)
        putchar('\b');
    scanf("%d%d", &mark_y, &mark_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (mark_x>game.cols||mark_x<1||mark_y>game.lines||mark_y<1) {//超出地雷範圍
        gotoxy(50,5);
        color_set(12);
        printf("輸入錯誤!請重新輸入!!");
        color_set(7);
        game.error=1;
        Sleep(3000);
        gotoxy(50,5);
        for (i=0;i<21;i++)
            putchar(' ');
        return;
    }
    else if (*(map+(game.cols+1)*mark_y+mark_x)==20||*(map+(game.cols+1)*mark_y+mark_x)==0) {
        p_mark=map_mark+game.cols*(mark_y-1)+(mark_x-1);
        if (*p_mark==0) {
            *p_mark=1; //標記
            game.flag++;
            print_game(map,map_mark);
        }
        else if (*p_mark==1) {
            *p_mark=0; //清除標記
            game.flag--;
            print_game(map,map_mark);
        }
    }
    else
        print_game(map,map_mark);
}

void print_game(int *map,int *map_mark) { //介面
    system("cls");
    int i, j;
    int *p_map, *p_mark;
    printf("　 　y→\n　　　");
    for (i=1;i<game.cols+1;i++)
        printf("%2d", i);
    printf("\n x\n↓");  //下為原設計
/*  gotoxy(2,3);
 *  printf("x");
 *  gotoxy(1,4);
 *  printf("↓");
 */
    for (i=1;i<game.lines+1;i++) {
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
    printf("地雷數:%d", game.landboom);
    gotoxy(25,1);
    printf("旗子數:%2d", game.flag);
    for (i=0;i<game.lines+2;i++) {
        gotoxy(5,i+3);
        for (j=0;j<game.cols+2;j++) {
            p_map=map+(game.cols+1)*i+j;
            p_mark=map_mark+game.cols*(i-1)+(j-1);
            if (*(p_map)==99)
                printf("■");         //邊界
            else if (*(p_map)==10)
                printf("　");    //踩過的空地
            else if (*(p_map)==0||*(p_map)==20) {
                if (*(p_mark)==0)
                    printf("□");     //還沒踩過的地面&地雷
                else if (*(p_mark)==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
                else if (*(p_mark)==2) {
                    color_set(12);
                    printf("Χ");     //標記錯誤顯現
                }
            }
            else if (*(p_map)<10)
                printf(" %d", *(p_map)); //踩過且旁邊有地雷，顯現地雷數
            else if (*(p_map)==21) {
                if (*(p_mark)==0) {
                    color_set(14);
                    printf("※");    //全場地雷處
                }
                else if (*(p_mark)==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            else if (*(p_map)==30) {
                if (*(p_mark)==0) {
                    color_set(11);
                    printf("⊕");    //過關顯示地雷
                }
                else if (*(p_mark)==1) {
                    color_set(12);
                    printf("◢");    //地雷標記顯現
                }
            }
            color_set(7);
        }
    }
    putchar('\n');
}