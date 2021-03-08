#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
/*  #define LINES_EASY 9
    #define COLS_EASY 9
    #define LINES_NORMAL 16
    #define COLS_NORMAL 16
    #define LINES_HARD 30
    #define COLS_HARD 16 */

typedef enum {false, true} bool;

struct GAME{
    bool error;  //�C�����~(��J)
    bool over;   //�C������
    int flag;   //�C�����X�l�ƶq
    char continue_q;     //�C���~��߰�
} game;

struct SET{
    int lines;  //�a�ϴX��
    int cols;   //�a�ϴX�C
    int landboom;   //�a�p�ƶq
} set;

//���n�禡(important_funt.c)
void color_set(int);    //�����C��
void degree_of_difficulty(void);
void game_dif_choose(void); //�C���x���׿�h
void gotoxy(int,int);   //���w�y��

//�C�����e(game_content.c)
void game_content(void);    //�C�����e
void game_start(int *,int *);  //�]�w���
void landboom_generate(int,int,int *);   //�H���ͦ��a�p
void landboom_tester(int,int,int *);   //���ծ���K��a�p
void mod_choose_1(int *,int *);        //"��"
void mod_choose_2(int *,int *);        //�аO
void print_game(int *,int *);          //����
