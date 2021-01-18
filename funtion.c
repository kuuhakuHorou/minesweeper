#include "funtion.h"

int map[LINES+2][COLS+2];       //原始地圖
int map_guess[LINES+2][COLS+2]; //遊玩者使用的地圖
int map_mark[LINES][COLS]={0};  //標記地圖
int error_enter;    //錯誤輸入
int game_over;      //遊戲結束

void game() {   //遊戲內容
    int i, j, mod, mod_choose;
    game_over=0;
    game_start();
    print_game();
    while (1) {
        if (error_enter==0) {
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
                    printf("輸入錯誤!請重新輸入!!");
                    error_enter=1;
                    mod_choose=0;   //一開始沒輸入錯誤但後面有時，防止跳入上一個選則
                    Sleep(3000);
                    gotoxy(45,5);
                    for (i=0;i<21;i++)
                        putchar(' ');
                    for (i=0;i<21;i++)
                        putchar('\b');
                    break;
            }
            if (mod_choose==1) {
                mod_choose_1();
            }
            else if (mod_choose==2) {
                mod_choose_2();
            }
        }
        else if (error_enter==1) {
            error_enter=0;
            if (mod_choose==1)
                mod_choose_1();
            else if (mod_choose==2)
                mod_choose_2();
        }
        
        if (game_over==1) {
            gotoxy(45,11);
            break;
        }
    }
}

void game_start() {     //設定邊界
    int i, j;
    for (i=0;i<LINES+2;i++) {
        for (j=0;j<COLS+2;j++) {
            if (i==0||i==LINES+1||j==0||j==COLS+1)
                map[i][j]=99;   //99為邊界
            else 
                map[i][j]=0;    //0為空地
        }
    }
    landboom_generate();    //生成地雷
    for (i=0;i<LINES+2;i++) {
        for (j=0;j<COLS+2;j++) {
            map_guess[i][j]=map[i][j];
        }
    }
}

void gotoxy(int x,int y) {  //指定座標
    COORD c;
    c.X=x-1;
    c.Y=y-1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void landboom_generate() {  //隨機生成地雷
    srand(time(0));
    int produce_x, produce_y, landboom_num=10;
    for (int i=0;i<landboom_num;i++) {
        produce_x=rand()%COLS+1;
        produce_y=rand()%LINES+1;
        if (map[produce_y][produce_x]==20)  //已經為地雷的情況
            i--;
        map[produce_y][produce_x]=20;   //20為地雷
    }
}

void landboom_tester(int x,int y) { //測試旁邊八格地雷
    int i, j, landboom;
    landboom=0; //重算地雷數
    
    for (i=y-1;i<=y+1;i++) {        //測試周邊地雷
        for (j=x-1;j<=x+1;j++) {
            if (i==y&&j==x)     //踩的那格不用算
                continue;
            else if (map[i][j]==20)
                landboom++;
        }
    }
    
    if (landboom==0) {
        map_guess[y][x]=10;   //旁邊沒地雷
        for (i=y-1;i<=y+1;i++) {
            for (j=x-1;j<=x+1;j++) {
                if (map_guess[i][j]==0)     //還沒踩過的地方
                    landboom_tester(j,i);
            }
        }
    }
    else
        map_guess[y][x]=landboom;  //旁邊有地雷(輸出多少)
}

void mod_choose_1() {   //"踩"
    int i, j, guess_x, guess_y, untreated_items;
    gotoxy(40,2);
    printf("請輸入你想踩哪: ");
    for (i=0;i<10;i++)
        putchar(' ');
    for (i=0;i<10;i++)
        putchar('\b');
    scanf("%d%d", &guess_y, &guess_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (guess_x>COLS||guess_x<1||guess_y>LINES||guess_y<1) {    //超出地雷範圍
        gotoxy(45,5);
        printf("輸入錯誤!請重新輸入!!");
        error_enter=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        for (i=0;i<21;i++)
            putchar('\b');
        return;
    }
    else if (map_mark[guess_y-1][guess_x-1]==1) {
        gotoxy(45,5);
        printf("你不能踩標記的地喔!");
        error_enter=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        for (i=0;i<21;i++)
            putchar('\b');
        return;
    }
    else if (map_guess[guess_y][guess_x]==20) {
        for (i=0;i<LINES+2;i++) {
            for (j=0;j<COLS+2;j++) {
                if (map_guess[i][j]==20)
                    map_guess[i][j]++;  //輸了以顯示地雷處
                else if (map_mark[i-1][j-1]==1&&map_guess[i][j]<20)
                    map_mark[i-1][j-1]=2;   //標記錯誤，把錯誤顯現出來
            }
        }
        print_game();
        gotoxy(45,10);
        printf("你輸了");
        game_over=1;
        return;
    }
    landboom_tester(guess_x,guess_y);
    print_game();
    untreated_items=0;  //重算沒踩過的地
    for (i=0;i<LINES+2;i++) {
        for (j=0;j<COLS+2;j++) {
            if (map_guess[i][j]==0) {
                untreated_items++;
            }
            else if (map_guess[i][j]<20) {
                continue;   //踩過且不算地雷處
            }
        }
    }
    if (untreated_items==0) {
        for (i=0;i<LINES+2;i++) {
            for (j=0;j<COLS+2;j++) {
                if (map_guess[i][j]==20)
                    map_guess[i][j]=30; //顯現地雷處
            }
        }
        print_game();
        gotoxy(45,10);
        printf("你贏了");
        game_over=1;
        return;
    }
}

void mod_choose_2() {   //標記
    int i, mark_x, mark_y;
    gotoxy(40,2);
    printf("請輸入你想標記哪: ");
    scanf("%d%d", &mark_y, &mark_x);
    fflush(stdin);  //清除輸入緩衝區，以防亂輸入
    if (mark_x>COLS||mark_x<1||mark_y>LINES||mark_y<1) {//超出地雷範圍
        gotoxy(45,5);
        printf("輸入錯誤!請重新輸入!!");
        error_enter=1;
        Sleep(3000);
        gotoxy(45,5);
        for (i=0;i<21;i++)
            putchar(' ');
        for (i=0;i<21;i++)
            putchar('\b');
        return;
    }
    else if (map_mark[mark_y-1][mark_x-1]==0) {
        map_mark[mark_y-1][mark_x-1]=1; //標記
        print_game();
    }
    else if (map_mark[mark_y-1][mark_x-1]==1) {
        map_mark[mark_y-1][mark_x-1]=0; //清除標記
        print_game();
    }
}

void print_game() { //介面
    system("cls");
    int i, j;
    printf("　 　y→\n"
           "　　　 1 2 3 4 5 6 7 8 9\n"
           " x　\n"
           "↓ 1\n"
           "　 2\n"
           "　 3\n"
           "　 4\n"
           "　 5\n"
           "　 6\n"
           "　 7\n"
           "　 8\n"
           "　 9\n");
    for (i=0;i<LINES+2;i++) {
        gotoxy(5,i+3);
        for (j=0;j<COLS+2;j++) {
            if (map_guess[i][j]==99)
                printf("■");         //邊界
            else if (map_guess[i][j]==10)
                printf("　");    //踩過的空地
            else if (map_guess[i][j]==0||map_guess[i][j]==20) {
                if (map_mark[i-1][j-1]==0)
                    printf("□");     //還沒踩過的地面&地雷
                else if (map_mark[i-1][j-1]==1)
                    printf("◢");    //地雷標記顯現
                else if (map_mark[i-1][j-1]==2)
                    printf("Χ");     //標記錯誤顯現
            }
            else if (map_guess[i][j]<10)
                printf(" %d", map_guess[i][j]); //踩過且旁邊有地雷，顯現地雷數
            else if (map_guess[i][j]==21) {
                if (map_mark[i-1][j-1]==0)
                    printf("※");    //全場地雷處
                else if (map_mark[i-1][j-1]==1)
                    printf("◢");    //地雷標記顯現
            }
            else if (map_guess[i][j]==30) {
                if (map_mark[i-1][j-1]==0)
                    printf("⊕");    //過關顯示地雷
                else if (map_mark[i-1][j-1]==1)
                    printf("◢");    //地雷標記顯現
            }
        }
    }
    putchar('\n');
}