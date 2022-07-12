/*
    Minesweeper program
    Author: Tomislav Beric
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 100
#define MINEFLAG -1
#define MINE -2
#define FLAG -3
#define EMPTY -4
#define ZERO 0


void init_board(int a[MAXN][MAXN], int n, int p)
{
    int x, y, k = p*n*n/100;

    for (x = 0; x < n; ++x)
        for (y = 0; y < n; ++y)
            a[x][y] = EMPTY;

    srand(time(NULL));
    while (k)
    {
        x = rand() % n;
        y = rand() % n;
        if (a[x][y] != EMPTY) continue;

        a[x][y] = MINE;
        k--;
    }
}

char znak(int x)
{
    switch(x)
    {
        case MINE : return '*';
        case EMPTY: return '_';
        case MINEFLAG:
        case FLAG : return '#';
        case ZERO : return ' ';
        default: return '0'+x;
    }
}

void print_board(int a[][MAXN], int n, int showMine)
{
    int i, j;
    printf("  ");
    for (i = 0; i < n; ++i)
        printf("%d", i%10);
    printf("\n");
    for (i = 0; i < n; ++i)
    {
        printf("%d:", i%10);
        for (j = 0; j < n; ++j)
            printf("%c", (a[i][j]==MINE) ? (showMine?'*':'_') : znak(a[i][j]) );
        printf("\n");
    }
}

int valid(int x, int y, int n)
{
    return (x >= 0 && x < n && y >= 0 && y < n);
}

void lose(int a[][MAXN], int n)
{
    print_board(a, n, 1);
    printf("You lose!\n");
}

int win(int a[][MAXN], int n)
{
    int i, j;

    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            if (a[i][j] == EMPTY) return 0;
            else if (a[i][j] == MINE) return 0;
            else if (a[i][j] == FLAG) return 0;

    return 1;
}

int mina(int a[][MAXN], int x, int y, int n)
{
    if (!valid(x,y,n)) return 0;
    return (a[x][y]==MINE || a[x][y]==MINEFLAG);
}

int count(int a[][MAXN], int x, int y, int n)
{
    return mina(a,x-1,y-1,n) + mina(a,x-1,y,n) + mina(a,x-1,y+1,n) +
           mina(a,x,y-1,n) +                     mina(a,x,y+1,n) +
           mina(a,x+1,y-1,n) + mina(a,x+1,y,n) + mina(a,x+1,y+1,n);
}

void open(int a[][MAXN], int x, int y, int n)
{
    if (!valid(x, y, n)) return ;
    if (a[x][y] != EMPTY) return ;

    a[x][y] = count(a, x, y, n);

    if (!a[x][y])
    {
        open(a, x-1, y-1, n);
        open(a, x-1, y, n);
        open(a, x-1, y+1, n);
        open(a, x, y-1, n);
        open(a, x, y+1, n);
        open(a, x+1, y-1, n);
        open(a, x+1, y, n);
        open(a, x+1, y+1, n);
    }
}

int main()
{
    int mines[MAXN][MAXN];
    int p, n;
    char c;
    int x, y;

    printf("Dimenzija kvadratne ploce (<= 100)? ");
    scanf("%d", &n);
    printf("Postotak mina na ploci? ");
    scanf("%d", &p);

    init_board(mines, n, p);

    char line[100];
    fgets(line, 100, stdin);

    while (!win(mines, n))
    {
        print_board(mines, n, 0);
        printf("Potez: [o]tvori/[z]astavica x y: ");

        fgets(line, 100, stdin);
        sscanf(line, "%c %d %d", &c, &x, &y);

        if (!valid(x, y, n)) continue;

        if (c == 'z' && (mines[x][y] < 0))
        {
            switch(mines[x][y])
            {
                case EMPTY : mines[x][y] = FLAG; break;
                case FLAG  : mines[x][y] = EMPTY; break;
                case MINE  : mines[x][y] = MINEFLAG; break;
                case MINEFLAG : mines[x][y] = MINE; break;
            }
        }
        else if (c == 'o')
        {
            if (mines[x][y] == MINE)
            {
                lose(mines, n);
                break;
            }
            else open(mines, x, y, n);
        }
        else
        {
            printf("Nepoznata naredba!\n");
        }
    }

    if (win(mines, n))
        printf("Pobjeda!\n");

    return 0;
}
