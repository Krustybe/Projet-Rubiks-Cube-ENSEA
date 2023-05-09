#define _CRT_SECURE_NO_WARNINGS
#define MAX_LINE_LENGTH 256
#define NUM_LINES 6
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

char resultat[100];

#pragma pack(1)
typedef struct tagCUBE { // CUBE state node
    struct tagCUBE *parent; // pointer to parent node
    char f[3][3]; // front
    char b[3][3]; // back
    char u[3][3]; // up
    char d[3][3]; // down
    char l[3][3]; // left
    char r[3][3]; // right
    char op;
} CUBE;
#pragma pack()

typedef struct { // LINEITEM used to implement cube line rotation
    int   stride;
    char *buffer;
} LINEITEM;

static void line_rotate90(LINEITEM item[5]) // do line rotation 90 degree
{
    int i, j;
    for (i=0; i<5; i++) {
        int   dst_idx    = (i+0) % 5;
        int   src_idx    = (i+1) % 5;
        char *dst_buf    = item[dst_idx].buffer;
        char *src_buf    = item[src_idx].buffer;
        int   dst_stride = item[dst_idx].stride;
        int   src_stride = item[src_idx].stride;
        for (j=0; j<3; j++) {
            *dst_buf = *src_buf;
            dst_buf += dst_stride;
            src_buf += src_stride;
        }
    }
}

static void surface_rotate90(char buf[3][3]) // do surface rotation 90 degree
{
    char tmp[3][3];
    int  i, j;
    memcpy(tmp, buf, sizeof(tmp));
    for (i=0; i<3; i++) {
        for (j=0; j<3; j++) {
            buf[i][j] = tmp[3-j-1][i];
        }
    }
}

static void cube_init(CUBE *c)
{
    FILE *file = fopen("Reception.txt", "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    char ligne1[MAX_LINE_LENGTH], ligne2[MAX_LINE_LENGTH], ligne3[MAX_LINE_LENGTH], ligne4[MAX_LINE_LENGTH], ligne5[MAX_LINE_LENGTH], ligne6[MAX_LINE_LENGTH];
    char *tableau[] = { ligne1, ligne2, ligne3, ligne4, ligne5, ligne6 };
    char *lignes[NUM_LINES][MAX_LINE_LENGTH];
    int i, j, h;

    for (i = 0; i < NUM_LINES; i++) {
        if (fgets(tableau[i], MAX_LINE_LENGTH, file) == NULL) {
            printf("Erreur : Le fichier ne contient pas assez de lignes.\n");
            exit(1);
        }

        for (j = 0; j < MAX_LINE_LENGTH; j++) {
            lignes[i][j] = &(tableau[i][j]);
        }
    }

    //Initialise la face Up
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->u[i][j] = *(lignes[0][h]);
            h++;
        }
    }

    //Initialise la face Front
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->f[i][j] = *(lignes[1][h]);
            h++;
        }
    }

    //Initialise la face Down
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->d[i][j] = *(lignes[2][h]);
            h++;
        }
    }

    //Initialise la face Back
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->b[i][j] = *(lignes[3][h]);
            h++;
        }
    }

    //Initialise la face Right
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->l[i][j] = *(lignes[4][h]);
            h++;
        }
    }

    //Initialise la face Left
    h = 0;
    for (i = 0; i < 3; i++) 
    {
        for (j = 0; j < 3; j++) {
            c->r[i][j] = *(lignes[5][j + i * 3]);
            h++;
        }
    }

    fclose(file);
}

static void cube_f(CUBE *c) // f operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 3,  &(c->l[0][2]) },
        { 1,  &(c->d[0][0]) },
        {-3,  &(c->r[2][0]) },
        {-1,  &(c->u[2][2]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->f);
}

static void cube_b(CUBE *c) // b operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 3,  &(c->r[0][2]) },
        {-1,  &(c->d[2][2]) },
        {-3,  &(c->l[2][0]) },
        { 1,  &(c->u[0][0]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->b);
}

static void cube_u(CUBE *c) // u operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 1,  &(c->l[0][0]) },
        { 1,  &(c->f[0][0]) },
        { 1,  &(c->r[0][0]) },
        { 1,  &(c->b[0][0]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->u);
}

static void cube_d(CUBE *c) // d operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 1,  &(c->b[2][0]) },
        { 1,  &(c->r[2][0]) },
        { 1,  &(c->f[2][0]) },
        { 1,  &(c->l[2][0]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->d);
}

static void cube_l(CUBE *c) // l operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 3,  &(c->d[0][0]) },
        { 3,  &(c->f[0][0]) },
        { 3,  &(c->u[0][0]) },
        {-3,  &(c->b[2][2]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->l);
}

static void cube_r(CUBE *c) // r operation
{
    char temp[3];
    LINEITEM lines[5] = {
        { 1,  temp          },
        { 3,  &(c->u[0][2]) },
        { 3,  &(c->f[0][2]) },
        { 3,  &(c->d[0][2]) },
        {-3,  &(c->b[2][0]) },
    };
    line_rotate90(lines);
    surface_rotate90(c->r);
}

enum {
    CUBE_OP_F,
    CUBE_OP_B,
    CUBE_OP_U,
    CUBE_OP_D,
    CUBE_OP_L,
    CUBE_OP_R,
};

static void (*g_op_tab[])(CUBE *c) = { cube_f, cube_b, cube_u, cube_d, cube_l, cube_r }; // define g_op_tab, an operation functions table
static void cube_op  (CUBE *c, int op) { (g_op_tab[op])(c); } // cube_op is operation function, parameter op indicate the operation

static void cube_render(CUBE *c) // draw cube on screen, now only working on win32 console
{
    char buffer[9][12] = {0};
    int  i, j;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD wOldColorAttrs;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

    // save the current color
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    wOldColorAttrs = csbiInfo.wAttributes;

    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[3+i][3+j] = c->f[i][j];
    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[3+i][9+j] = c->b[i][j];
    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[0+i][3+j] = c->u[i][j];
    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[6+i][3+j] = c->d[i][j];
    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[3+i][0+j] = c->l[i][j];
    for (i=0; i<3; i++) for (j=0; j<3; j++) buffer[3+i][6+j] = c->r[i][j];

    for (i=0; i<9; i++) {
        for (j=0; j<12; j++) {
            switch (buffer[i][j]) {
            case 'w': case 'W': SetConsoleTextAttribute(h, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); break;
            case 'y': case 'Y': SetConsoleTextAttribute(h, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN); break;
            case 'b': case 'B': SetConsoleTextAttribute(h, FOREGROUND_INTENSITY|FOREGROUND_BLUE ); break;
            case 'g': case 'G': SetConsoleTextAttribute(h, FOREGROUND_INTENSITY|FOREGROUND_GREEN); break;
            case 'o': case 'O': SetConsoleTextAttribute(h, FOREGROUND_RED|FOREGROUND_BLUE       ); break;
            case 'r': case 'R': SetConsoleTextAttribute(h, FOREGROUND_INTENSITY|FOREGROUND_RED  ); break;
            }
            printf(buffer[i][j] ? "# " : "  ");
        }
        printf("\n");
    }

    // restore the original color
    SetConsoleTextAttribute(h, wOldColorAttrs);
}

static int cube_check_fcross0(CUBE *cube) // check front cross state, in total 4 squares need be solved, if solved score should be 4
{
    int checklist[][2] = {
        { cube->f[1][1], cube->f[0][1] },
        { cube->f[1][1], cube->f[1][0] },
        { cube->f[1][1], cube->f[1][2] },
        { cube->f[1][1], cube->f[2][1] },
    };
    int value = 0, i;
    for (i=0; i<4; i++) {
        value += (checklist[i][0] == checklist[i][1]);
    }
    return value;
}

static int cube_check_fcross1(CUBE *cube) // check first layer cross, in total 4 squares need be solved, if solved score should be 4
{
    int checklist[][2] = {
        { cube->l[1][1], cube->l[1][2] },
        { cube->u[1][1], cube->u[2][1] },
        { cube->r[1][1], cube->r[1][0] },
        { cube->d[1][1], cube->d[0][1] },
    };
    int value = 0, i;
    for (i=0; i<4; i++) {
        value += (checklist[i][0] == checklist[i][1]);
    }
    return value;
}

static int cube_check_fcorners(CUBE *cube) // check front corners
{
    int checklist[][3][2] = {
        {
            { cube->f[1][1], cube->f[0][0] },
            { cube->l[1][1], cube->l[0][2] },
            { cube->u[1][1], cube->u[2][0] },
        },
        {
            { cube->f[1][1], cube->f[0][2] },
            { cube->u[1][1], cube->u[2][2] },
            { cube->r[1][1], cube->r[0][0] },
        },
        {
            { cube->f[1][1], cube->f[2][0] },
            { cube->l[1][1], cube->l[2][2] },
            { cube->d[1][1], cube->d[0][0] },
        },
        {
            { cube->f[1][1], cube->f[2][2] },
            { cube->r[1][1], cube->r[2][0] },
            { cube->d[1][1], cube->d[0][2] },
        },
    };
    int value = 0, i, j;
    for (j=0; j<4; j++) {
        value++;
        for (i=0; i<3; i++) {
            if (checklist[j][i][0] != checklist[j][i][1]) { value--; break; }
        }
    }
    return value;
}

static int cube_check_medges(CUBE *cube) // check middle edges
{
    int checklist[][2][2] = {
        {
            { cube->l[1][1], cube->l[0][1] },
            { cube->u[1][1], cube->u[1][0] },
        },
        {
            { cube->u[1][1], cube->u[1][2] },
            { cube->r[1][1], cube->r[0][1] },
        },
        {
            { cube->l[1][1], cube->l[2][1] },
            { cube->d[1][1], cube->d[1][0] },
        },
        {
            { cube->r[1][1], cube->r[2][1] },
            { cube->d[1][1], cube->d[1][2] },
        },
    };
    int value = 0, i, j;
    for (j=0; j<4; j++) {
        value++;
        for (i=0; i<2; i++) {
            if (checklist[j][i][0] != checklist[j][i][1]) { value--; break; }
        }
    }
    return value;
}

static int cube_check_bcross(CUBE *cube) // check back cross
{
    int checklist[][2][2] = {
        {
            { cube->b[1][1], cube->b[0][1] },
            { cube->b[1][1], cube->b[2][1] },
        },
        {
            { cube->b[1][1], cube->b[1][0] },
            { cube->b[1][1], cube->b[1][2] },
        },
    };
    int value = 0, i, j;
    for (j=0; j<2; j++) {
        value += 2;
        for (i=0; i<2; i++) {
            if (checklist[j][i][0] != checklist[j][i][1]) { value -= 2; break; }
        }
    }
    return value;
}

static int cube_check_bsurface(CUBE *cube) // check back surface
{
    int checklist[][2] = {
        { cube->b[1][1], cube->b[0][0] },
        { cube->b[1][1], cube->b[0][2] },
        { cube->b[1][1], cube->b[2][0] },
        { cube->b[1][1], cube->b[2][2] },
    };
    int value = 0, i;
    for (i=0; i<4; i++) {
        value += (checklist[i][0] == checklist[i][1]);
    }
    if (value == 2) value = 0;
    return value;
}

static int cube_check_bcorners(CUBE *cube) // check back corners
{
    int checklist[][3][2] = {
        {
            { cube->b[1][1], cube->b[0][2] },
            { cube->l[1][1], cube->l[0][0] },
            { cube->u[1][1], cube->u[0][0] },
        },
        {
            { cube->b[1][1], cube->b[0][0] },
            { cube->u[1][1], cube->u[0][2] },
            { cube->r[1][1], cube->r[0][2] },
        },
        {
            { cube->b[1][1], cube->b[2][0] },
            { cube->r[1][1], cube->r[2][2] },
            { cube->d[1][1], cube->d[2][2] },
        },
        {
            { cube->b[1][1], cube->b[2][2] },
            { cube->l[1][1], cube->l[2][0] },
            { cube->d[1][1], cube->d[2][0] },
        },
    };
    int returned[4] = { 1, 1, 1, 1 };
    int value = 0, flag = 0, i, j;
    for (j=0; j<4; j++) {
        value++;
        for (i=0; i<3; i++) {
            if (checklist[j][i][0] != checklist[j][i][1]) { value--; returned[j] = 0; break; }
        }
    }
    for (i=0; i<4; i++) {
        if (returned[i] && returned[(i+1)%4]) flag = 1;
    }
    return flag ? value : 0;
}

static int cube_check_bedges(CUBE *cube) // check back edges
{
    int checklist[][2][2] = {
        {
            { cube->u[1][1], cube->u[0][1] },
            { cube->b[1][1], cube->b[0][1] },
        },
        {
            { cube->d[1][1], cube->d[2][1] },
            { cube->b[1][1], cube->b[2][1] },
        },
        {
            { cube->l[1][1], cube->l[1][0] },
            { cube->b[1][1], cube->b[1][2] },
        },
        {
            { cube->r[1][1], cube->r[1][2] },
            { cube->b[1][1], cube->b[1][0] },
        },
    };
    int value = 0, i, j;
    for (j=0; j<4; j++) {
        value++;
        for (i=0; i<2; i++) {
            if (checklist[j][i][0] != checklist[j][i][1]) { value--; break; }
        }
    }
    return value;
}

static int cube_check_state(CUBE *cube, int flag) // flag: 0 - calculate score stop when score of one step is not 4, >0 - calulate score stop when cur >= flag
{
    int (*pfn_check_tab[])(CUBE* cube) = {
        cube_check_fcross0 ,
        cube_check_fcross1 ,
        cube_check_fcorners,
        cube_check_medges  ,
        cube_check_bcross  ,
        cube_check_bsurface,
        cube_check_bcorners,
        cube_check_bedges  ,
        NULL,
    };
    int value = 0, cur = 0, i;
    for (i=0; pfn_check_tab[i]; i++) {
        if (flag) {
            value += pfn_check_tab[i](cube);
            cur   += 4;
            if (cur >= flag) break;
        } else {
            cur    = pfn_check_tab[i](cube);
            value += cur;
            if (cur != 4) break;
        }
    }
    return value;
}

typedef struct {
    int   open ;
    int   close;
    int   size ;
    CUBE *cubes;
} TABLE;

static int search_table_create(TABLE *table, int size)
{
    table->size  = size;
    table->cubes = malloc(size * sizeof(CUBE));
    return table->cubes ? 0 : -1;
}

static void search_table_destroy(TABLE *table)
{
    free(table->cubes);
    table->cubes = NULL;
}

static int is_4same_ops(CUBE *cube) // check whether parent path is 4 same operations path
{
    int curop = cube->op;
    int n     = 0;
    while (cube) {
        if (cube->op == curop) {
            if (++n == 4) return 1;
            cube = cube->parent;
        } else break;
    }
    return 0;
}

static int cut_branch(int newval, int cutval)
{
    return newval < cutval;
}

static CUBE* search(TABLE *table, CUBE *start, int state, char *oplist, int opnum, int cutval)
{
    CUBE *curcube ,*newcube;
    int   newstate, newvalue, i;

    start->parent = NULL;
    start->op     = -1;
    if (cube_check_state(start, 0) >= state) return start;

    // init search table
    table->open = table->close = 0;

    // put original cube into open table
    table->cubes[table->open] = *start;
    table->open++;

    while (table->close < table->open) {
        // check memory usage
        if (table->open + 6 >= table->size - 1) { printf("all table memory have been used !\n"); break; }

        // dequeue a cube from open table
        curcube = &(table->cubes[table->close++]);

        // extend cubes check state and put new cubes into open table
        for (i=0; i<opnum; i++) {
            newcube = &(table->cubes[table->open]);
            memcpy (newcube, curcube, sizeof(CUBE));
            cube_op(newcube, oplist[i]);
            newcube->op     = oplist[i];
            newcube->parent = curcube;
            newstate = cube_check_state(newcube, 0    );
            newvalue = cube_check_state(newcube, state);
            if (newstate >= state) return newcube; // found
            if (cut_branch(newvalue, cutval) || is_4same_ops(newcube)) continue; // cut branch
            table->open++;
        }
    }
    return NULL;
}

static void print_solve_oplist(CUBE *cube)
{
    
    static char* optab[] = {
        "F,H" , "B,H" , "U,H" , "D,H" , "L,H" , "R,H" ,
        "F,2", "B,2", "U,2", "D,2", "L,2", "R,2",
        "F,T", "B,T", "U,T", "D,T", "L,T", "R,T",
    };
    char *oplist[256];
    int   last = -1, times = 0, i = 0, n = 0;
    while (cube) {
        if (cube->op >= 0) {
            if (last != cube->op) {
                if (last != -1) {
                    oplist[i++] = optab[last + times * 6];
                }
                last = cube->op;
                times= 0;
            } else {
                times++;
            }
        } else {
            if (last != -1) oplist[i++] = optab[last + times * 6];
        }
        cube = cube->parent;
    }
    
    //printf("\noperation list:\n");
    while (--i >= 0) {
        sprintf(resultat + strlen(resultat), "%s%s%s,", oplist[i], i == 0 ? "" : "", ++n % 12 == 0 ? "" : "");
    }

    writeFILE(resultat);
    
}

static void cube_solve(CUBE *c)
{
    TABLE t;

    if (search_table_create(&t, 1024*1024*16) != 0) {
        printf("failed to create cube search table !\n");
        return;
    }

    if (1) {
        static char oplisttab[][6] = {
            { CUBE_OP_F, CUBE_OP_U, CUBE_OP_D, CUBE_OP_L, CUBE_OP_R },
            { CUBE_OP_U, CUBE_OP_D, CUBE_OP_L, CUBE_OP_R, CUBE_OP_B },
            { CUBE_OP_B, CUBE_OP_R, CUBE_OP_U },
        };
        static int stepparams[][4] = {
            { 2 , 0, 5, 0 }, //+ fcross0
            { 4 , 0, 5, 2 }, //- fcross0
            { 8 , 0, 5, 2 }, //+-fcross1
            { 9 , 0, 5, 3 }, //+ fcorners
            { 10, 0, 5, 4 }, //..
            { 11, 0, 5, 5 }, //..
            { 12, 0, 5, 5 }, //- fcorners
            { 13, 1, 5, 6 }, //+ medges
            { 14, 1, 5, 6 }, //..
            { 15, 1, 5, 7 }, //..
            { 16, 1, 5, 8 }, //- medges
            { 18, 1, 5, 10}, //+ bcross
            { 20, 1, 5, 11}, //- bcross
            { 21, 1, 5, 11}, //+ bsurface
            { 24, 1, 5, 11}, //- bsurface
            { 26, 2, 3, 12}, //+ bcorners
            { 28, 2, 3, 12}, //- bcorners
            { 32, 2, 2, 12}, //+-bedges
            { 0 , 0, 0, 0 },
        };
        CUBE *start = c;
        CUBE *find  = NULL;
        int   i;
        for (i=0; stepparams[i][0]; i++) {
            find = search(&t, start, stepparams[i][0], oplisttab[stepparams[i][1]], stepparams[i][2], stepparams[i][3]);
            if (find) {
                if (find != start) {
                    start = find;
                    *c    =*find;
                    print_solve_oplist(find);
                }
            } else {
                printf("can't solve !\n");
                goto done;
            }
        }
    }

done:
    search_table_destroy(&t);
}

void writeFILE(Solution)
{
    FILE *fichier;
    fichier = fopen("Solution.txt", "w"); // ouverture du fichier en mode écriture ("w")

    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier.");
        return 1;
    }

    // Effacer le contenu du fichier
    ftruncate(fileno(fichier), 0); // Efface le contenu du fichier en le réduisant à zéro octet

    // Écrire dans le fichier
    fprintf(fichier, Solution);

    fclose(fichier); // Fermeture du fichier

    return 0;
}

void ReadFILE(RUBIKS_CUBE)
{
    FILE *file = fopen(RUBIKS_CUBE, "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    char ligne1[MAX_LINE_LENGTH], ligne2[MAX_LINE_LENGTH], ligne3[MAX_LINE_LENGTH], ligne4[MAX_LINE_LENGTH], ligne5[MAX_LINE_LENGTH], ligne6[MAX_LINE_LENGTH];
    char *tableau[] = { ligne1, ligne2, ligne3, ligne4, ligne5, ligne6 };
    char *lignes[NUM_LINES][MAX_LINE_LENGTH];
    int i, j;

    for (i = 0; i < NUM_LINES; i++) {
        if (fgets(tableau[i], MAX_LINE_LENGTH, file) == NULL) {
            printf("Erreur : Le fichier ne contient pas assez de lignes.\n");
            exit(1);
        }

        for (j = 0; j < MAX_LINE_LENGTH; j++) {
            lignes[i][j] = &(tableau[i][j]);
        }
    }

    fclose(file);
    return 0;
}


int main(void)
{
    char cmd[128];
    char str[256];
    char Sol;
    char TabSol[256];
    int i = 0;
    int sizeof_TabSol;
    CUBE c;

    // init cube
    cube_init(&c);
    cube_render(&c);
    cube_solve(&c);
    ReadFILE("Reception.txt");

    printf("\noperation list: ");
    FILE *file = fopen("Solution.txt", "r");
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    while ((Sol = fgetc(file)) != EOF) {
        TabSol[i] = Sol;
        i++;
    }
    
    sizeof_TabSol = i;

    i = 0;

    for (i = 0; i < sizeof_TabSol; i++)
    {
        printf("%c",TabSol[i]);
        if (TabSol[i+1] == 'U' || TabSol[i+1] == 'F' || TabSol[i+1] == 'R' || TabSol[i+1] == 'L' || TabSol[i+1] == 'D' || TabSol[i+1] == 'B')
        {
            printf(" ");
        }
    }
    

    fclose(file);
    
    printf("\n\n");

    return 0;
}