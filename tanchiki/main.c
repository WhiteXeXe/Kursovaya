#define _CRT_SECURE_NO_WARNINGS

#include <gl/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define NUM_OF_MINI_BRICKS_BLOCKS 500
#define NUM_OF_MINI_CEMENT_BLOCKS 500
#define BOOM_ANIMATION_FRAMES 10

int TABLE_SIZE = 201;
int menuChoose = 0;
int moveSpeed = 2;

struct tankSettings
{
    int xPos;
    int yPos;
    int angle;
    int live;

    int saveXPos;
    int saveYPos;
    int flagAutoMove;
    int tickToShoot;
    int timerMove;
    int botSpeed;

    struct
    {
        int dx;
        int dy;
        int angle_bull;
        short live;
        short anim_bool;
        short time_anim;

        int an_x;
        int an_y;
    } bulletSet;
};
struct Node
{
    int key;
    int value[3];
    struct Node* next;
};
struct HashTable
{
    struct Node** table;
};

int bricksLoc[NUM_OF_MINI_BRICKS_BLOCKS][3] = { 0 };
int cementLoc[NUM_OF_MINI_CEMENT_BLOCKS][3] = { 0 };


struct tankSettings enemy[4];
struct tankSettings you;
struct HashTable* Hash_pos_x;
struct HashTable* Hash_pos_y;


struct Node* Create_Node(int key, int* arr)
{
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->key = key;
    new_node->value[0] = arr[0];
    new_node->value[1] = arr[1];
    new_node->value[2] = arr[2];
    new_node->next = NULL;
    return new_node;
}

void Create_Table_x() {
    Hash_pos_x = (struct HashTable*)malloc(sizeof(struct HashTable));
    Hash_pos_x->table = (struct Node**)malloc(sizeof(struct Node*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        Hash_pos_x->table[i] = NULL;
    }

}

void Create_Table_y() {
    Hash_pos_y = (struct HashTable*)malloc(sizeof(struct HashTable));
    Hash_pos_y->table = (struct Node**)malloc(sizeof(struct Node*) * TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; i++) {
        Hash_pos_y->table[i] = NULL;
    }

}

unsigned int Hash(int key)
{
    unsigned int hash = 0;
    if (key > 0) key += 100;
    if (key < 0) key *= (-1);
    hash = (unsigned int)key;



    return hash;
}

void Insert_x(int key, int* arr)
{
    unsigned int index = Hash(key);
    struct Node* new_node = Create_Node(key, arr);
    if (Hash_pos_x->table[index] == NULL) Hash_pos_x->table[index] = new_node;
    else
    {
        new_node->next = Hash_pos_x->table[index];
        Hash_pos_x->table[index] = new_node;
    }
}

void Insert_y(int key, int* arr)
{
    unsigned int index = Hash(key);
    struct Node* new_node = Create_Node(key, arr);
    if (Hash_pos_y->table[index] == NULL) Hash_pos_y->table[index] = new_node;
    else
    {
        new_node->next = Hash_pos_y->table[index];
        Hash_pos_y->table[index] = new_node;
    }
}

void Print_HashTable(struct HashTable* table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        struct Node* node = table->table[i];
        if (node != NULL) {
            printf("Index %d:\n", i);
            while (node != NULL) {
                printf("  Key: %d, Value: [%d, %d, %d]\n", node->key, node->value[0], node->value[1], node->value[2]);
                node = node->next;
            }
        }
    }
}

int Moving_Vertical(int key, int flag) {
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_y->table[index];
    int xPos_left = you.xPos - 4;
    int xPos_right = you.xPos + 4;
    int checkDistUP;


    while (node != NULL) {

        checkDistUP = fabs(node->value[2] - you.yPos + 5);

        if (node->value[2] <= key) {
            if (xPos_left >= node->value[1] && xPos_left <= node->value[1] + 4) {


                if (checkDistUP >= 12  && checkDistUP <= 12 && flag == 1) {
                    return 2;
                }

                return 1;
            }

            if (xPos_right >= node->value[1]  && xPos_right <= node->value[1] + 4) {

                if (checkDistUP >= 12 && checkDistUP <= 12 && flag == 1) {
                    return 2;
                }


                return 1;
            }

            if (you.xPos >= node->value[1] && you.xPos <= node->value[1] + 4) {

                if (checkDistUP >= 12  && checkDistUP <= 12  && flag == 1) {
                    return 2;
                }


                return 1;
            }
        }
        node = node->next;
    }

    return 0;
}

int Moving_Horizontal(int key)
{
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_x->table[index];


    int yPos_down = you.yPos - 4;
    int yPos_up = you.yPos + 4;
    while (node != NULL) {
        if (node->value[1] >= key - 5) {

            if (yPos_down >= node->value[2]  && yPos_down <= node->value[2] + 4) {
                return 1;
            }

            if (yPos_up >= node->value[2]  && yPos_up <= node->value[2] + 4) {
                return 1;
            }

            if (you.yPos >= node->value[2] && you.yPos <= node->value[2] + 4) {
                return 1;
            }
        }
        node = node->next;
    }

    return 0;
}

int Enemy_Moving_Vertical(int key, int enemy_num, int flag) {
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_y->table[index];
    int xPos_left = enemy[enemy_num].xPos - 4;
    int xPos_right = enemy[enemy_num].xPos + 4;
    int checkDistUP;


    while (node != NULL) {

        checkDistUP = fabs(node->value[2] - enemy[enemy_num].yPos + 5 );

        if (node->value[2] <= key ) {
            if (xPos_left >= node->value[1]  && xPos_left <= node->value[1] + 4) {


                if (checkDistUP >= 12  && checkDistUP <= 12 && flag == 1) {
                    return 2;
                }

                return 1;
            }

            if (xPos_right >= node->value[1]  && xPos_right <= node->value[1] + 4) {

                if (checkDistUP >= 12  && checkDistUP <= 12  && flag == 1) {
                    return 2;
                }


                return 1;
            }

            if (enemy[enemy_num].xPos >= node->value[1] && enemy[enemy_num].xPos <= node->value[1] + 4) {

                if (checkDistUP >= 12 && checkDistUP <= 12 && flag == 1) {
                    return 2;
                }


                return 1;
            }
        }
        node = node->next;
    }

    return 0;
}

int Enemy_Moving_Horizontal(int key, int enemy_num)
{
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_x->table[index];

    int yPos_down = enemy[enemy_num].yPos - 4;
    int yPos_up = enemy[enemy_num].yPos + 4;

    while (node != NULL) {
        if (node->value[1] >= key - 5) {

            if (yPos_down >= node->value[2]  && yPos_down <= node->value[2] + 4) {
                return 1;
            }

            if (yPos_up >= node->value[2]   && yPos_up <= node->value[2] + 4) {
                return 1;
            }

            if (enemy[enemy_num].yPos >= node->value[2]   && enemy[enemy_num].yPos <= node->value[2] + 4) {
                return 1;
            }
        }
        node = node->next;
    }

    return 0;
}

void Delete_Table_x(int key, int coord) {
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_x->table[index];
    struct Node* prev = NULL;

    while (node != NULL) {
        if ((key   <= node->key && node->key <= key  ) && (coord   <= node->value[2] && node->value[2] <= coord  ))
        {
            if (prev == NULL) {
                Hash_pos_x->table[index] = node->next;
            }
            else {
                prev->next = node->next;
            }
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }

}

void Delete_Table_y(int key, int coord) {
    unsigned int index = Hash(key);
    struct Node* node = Hash_pos_y->table[index];
    struct Node* prev = NULL;
    while (node != NULL) {
        if ((key   <= node->key && node->key <= key  ) && (coord   <= node->value[1] && node->value[1] <=  coord))
        {
            if (prev == NULL) {
                Hash_pos_y->table[index] = node->next;
            }
            else {
                prev->next = node->next;
            }

            free(node);
            return;
        }
        prev = node; 
        node = node->next; 
    }

} 

void startSettings()
{
    you.xPos = 0;
    you.yPos = -84;
    you.live = 1;
    you.bulletSet.time_anim = 0;

    enemy[0].live = 1;           
    enemy[0].xPos = -80;
    enemy[0].yPos = 12;
    enemy[0].angle = 270;
    enemy[0].saveXPos = 0;
    enemy[0].saveYPos = 0;
    enemy[0].flagAutoMove = 4;

    enemy[1].live = 1;          
    enemy[1].xPos = 70;
    enemy[1].yPos = 12;
    enemy[1].angle = 90;
    enemy[1].saveXPos = 0;
    enemy[1].saveYPos = 0;
    enemy[1].flagAutoMove = 1;

    enemy[2].live = 1;
    enemy[2].saveXPos = 0;
    enemy[2].saveYPos = 0;
    enemy[2].flagAutoMove = 2;

    enemy[3].live = 1;
    enemy[3].saveXPos = 0;
    enemy[3].saveYPos = 0;
    enemy[3].flagAutoMove = 2;
} 

void draw_mini_cement_block(int x, int y)
{
    int firstcol[9][2] = {
            {x, y},
            {x + 1, y},
            {x + 2, y},
            {x + 3, y},
            {x + 4, y},
            {x + 4, y + 1},
            {x + 4, y + 2},
            {x + 4, y + 3},
            {x + 4, y + 4}
    };

    int secondcol[7][2] = {
            {x, y + 1},
            {x, y + 2},
            {x, y + 3},
            {x, y + 4},
            {x + 1, y + 4},
            {x + 2, y + 4},
            {x + 3, y + 4},
    };

    int thirdcol[9][2] = {
            {x + 1, y + 1},
            {x + 1, y + 2},
            {x + 1, y + 3},
            {x + 2, y + 1},
            {x + 2, y + 2},
            {x + 2, y + 3},
            {x + 3, y + 1},
            {x + 3, y + 2},
            {x + 3, y + 3},
    };

    glBegin(GL_POINTS);

    glColor3f(1, 1, 1);
    for (int i = 0; i < 9; i++) {
        int x = thirdcol[i][0], y = thirdcol[i][1];
        glVertex2d(x, y);
    }

    glColor3f(0.78, 0.78, 0.78);
    for (int i = 0; i < 7; i++) {
        int x = secondcol[i][0], y = secondcol[i][1];
        glVertex2d(x, y);
    }

    glColor3f(0.48, 0.49, 0.48);
    for (int i = 0; i < 9; i++) {
        int x = firstcol[i][0], y = firstcol[i][1];
        glVertex2d(x, y);
    }

    glEnd();
}

void draw_mini_brick_block(int x, int y)
{
    int firstcol[13][2] = {
            {x, y},
            {x, y + 1},
            {x, y + 2},
            {x, y + 3},
            {x + 1, y + 3},
            {x + 2, y + 3},
            {x + 3, y + 3},
            {x + 4, y + 3},
            {x + 1, y},
            {x + 2, y},
            {x + 3, y},
            {x + 4, y},
            {x + 2, y + 4}
    };

    int secondcol[9][2] = {
            {x + 1, y + 1},
            {x + 1, y + 2},
            {x + 2, y + 2},
            {x + 3, y + 2},
            {x + 4, y + 2},
            {x, y + 4},
            {x + 1, y + 4},
            {x + 3, y + 4},
            {x + 4, y + 4}
    };

    int thirdcol[3][2] = {
            {x + 2, y + 1},
            {x + 3, y + 1},
            {x + 4, y + 1}
    };

    glBegin(GL_POINTS);

    glColor3f(0.48, 0.49, 0.48);
    for (int i = 0; i < 13; i++) {
        int x = firstcol[i][0], y = firstcol[i][1];
        glVertex2d(x, y);
    }

    glColor3f(0.65, 0.19, 0);
    for (int i = 0; i < 9; i++) {
        int x = secondcol[i][0], y = secondcol[i][1];
        glVertex2d(x, y);
    }

    glColor3f(0.77, 0.44, 0);
    for (int i = 0; i < 3; i++) {
        int x = thirdcol[i][0], y = thirdcol[i][1];
        glVertex2d(x, y);
    }
    glEnd();
}

void createCementWall()
{
    for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
    {
        draw_mini_cement_block(cementLoc[i][1], cementLoc[i][2]);
    }
}

void createBrickWall()
{
    for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
    {
        if (bricksLoc[i][0] == 1)
        {
            draw_mini_brick_block(bricksLoc[i][1], bricksLoc[i][2]);
        }
    }
}

void rotatePointyou(int* x, int* y, int angle) {
    if (angle == 0)
    {
        int xNew = *x * 1 - *y * 0;
        int yNew = *x * 0 + *y * 1;
        *x = xNew + you.xPos;
        *y = yNew + you.yPos;
    }
    if (angle == 90)
    {
        int xNew = *x * 0 - *y * 1;
        int yNew = *x * 1 + *y * 0;
        *x = xNew + you.xPos;
        *y = yNew + you.yPos;
    }
    if (angle == 180)
    {
        int xNew = *x * (-1) - *y * 0;
        int yNew = *x * 0 + *y * (-1);
        *x = xNew + you.xPos;
        *y = yNew + you.yPos;
    }
    if (angle == 270)
    {
        int xNew = *x * 0 - *y * (-1);
        int yNew = *x * (-1) + *y * 0;
        *x = xNew + you.xPos;
        *y = yNew + you.yPos;
    }
}

void rotatePointenemy_one(int* x, int* y, int angle)
{
    if (angle == 0)
    {
        int xNew = *x * 1 - *y * 0;
        int yNew = *x * 0 + *y * 1;
        *x = xNew + enemy[0].xPos;
        *y = yNew + enemy[0].yPos;
    }
    if (angle == 90)
    {
        int xNew = *x * 0 - *y * 1;
        int yNew = *x * 1 + *y * 0;
        *x = xNew + enemy[0].xPos;
        *y = yNew + enemy[0].yPos;
    }
    if (angle == 180)
    {
        int xNew = *x * (-1) - *y * 0;
        int yNew = *x * 0 + *y * (-1);
        *x = xNew + enemy[0].xPos;
        *y = yNew + enemy[0].yPos;
    }
    if (angle == 270)
    {
        int xNew = *x * 0 - *y * (-1);
        int yNew = *x * (-1) + *y * 0;
        *x = xNew + enemy[0].xPos;
        *y = yNew + enemy[0].yPos;
    }
}

void rotatePointenemy_two(int* x, int* y, int angle)
{
    if (angle == 0)
    {
        int xNew = *x * 1 - *y * 0;
        int yNew = *x * 0 + *y * 1;
        *x = xNew + enemy[1].xPos;
        *y = yNew + enemy[1].yPos;
    }
    if (angle == 90)
    {
        int xNew = *x * 0 - *y * 1;
        int yNew = *x * 1 + *y * 0;
        *x = xNew + enemy[1].xPos;
        *y = yNew + enemy[1].yPos;
    }
    if (angle == 180)
    {
        int xNew = *x * (-1) - *y * 0;
        int yNew = *x * 0 + *y * (-1);
        *x = xNew + enemy[1].xPos;
        *y = yNew + enemy[1].yPos;
    }
    if (angle == 270)
    {
        int xNew = *x * 0 - *y * (-1);
        int yNew = *x * (-1) + *y * 0;
        *x = xNew + enemy[1].xPos;
        *y = yNew + enemy[1].yPos;
    }
    
}

void rotatePointenemy_three(int* x, int* y, int angle)
{
    if (angle == 0)
    {
        int xNew = *x * 1 - *y * 0;
        int yNew = *x * 0 + *y * 1;
        *x = xNew + enemy[2].xPos;
        *y = yNew + enemy[2].yPos;
    }
    if (angle == 90)
    {
        int xNew = *x * 0 - *y * 1;
        int yNew = *x * 1 + *y * 0;
        *x = xNew + enemy[2].xPos;
        *y = yNew + enemy[2].yPos;
    }
    if (angle == 180)
    {
        int xNew = *x * (-1) - *y * 0;
        int yNew = *x * 0 + *y * (-1);
        *x = xNew + enemy[2].xPos;
        *y = yNew + enemy[2].yPos;
    }
    if (angle == 270)
    {
        int xNew = *x * 0 - *y * (-1);
        int yNew = *x * (-1) + *y * 0;
        *x = xNew + enemy[2].xPos;
        *y = yNew + enemy[2].yPos;
    }
}

void rotatePointenemy_four(int* x, int* y, int angle)
{
    if (angle == 0)
    {
        int xNew = *x * 1 - *y * 0;
        int yNew = *x * 0 + *y * 1;
        *x = xNew + enemy[3].xPos;
        *y = yNew + enemy[3].yPos;
    }
    if (angle == 90)
    {
        int xNew = *x * 0 - *y * 1;
        int yNew = *x * 1 + *y * 0;
        *x = xNew + enemy[3].xPos;
        *y = yNew + enemy[3].yPos;
    }
    if (angle == 180)
    {
        int xNew = *x * (-1) - *y * 0;
        int yNew = *x * 0 + *y * (-1);
        *x = xNew + enemy[3].xPos;
        *y = yNew + enemy[3].yPos;
    }
    if (angle == 270)
    {
        int xNew = *x * 0 - *y * (-1);
        int yNew = *x * (-1) + *y * 0;
        *x = xNew + enemy[3].xPos;
        *y = yNew + enemy[3].yPos;
    }
}

void draw_global_tank_up()
{

    glPointSize(5);
    glBegin(GL_POINTS);



    int coordFirstColor[35][2] = {
            {0, 0},
            {0, -3},
            {1, -3},
            {1, -2},
            {2, -3},
            {3, -3},
            {4, -3},
            {2, -1},
            {3, -1},
            {4, -1},
            {2, 1},
            {3, 1},
            {4, 1},
            {2, 3},
            {3, 3},
            {4, 3},
            {2, 0},
            {2, 2},
            {2, -2},
            {2, -4},
            {1, -5},
            {0, -5},
            {-1, -5},
            {-3, -5},
            {-4, -5},
            {3, -5},
            {4, -5},
            {-3, -3},
            {-4, -3},
            {-3, -1},
            {-4, -1},
            {-3, 1},
            {-4, 1},
            {-3, 3},
            {-4, 3},
    };


    int coordSecondColor[28][2] = {
            {0, 1},
            {1, 1},
            {1, 2},
            {-1, 2},
            {-1, 1},
            {1, 0},
            {1, -1},
            {0, -1},
            {0, -2},
            {-1, -3},
            {0, -4},
            {1, -4},
            {3, -4},
            {4, -4},
            {3, -2},
            {4, -2},
            {3, 0},
            {4, 0},
            {3, 2},
            {4, 2},
            {3, 4},
            {4, 4},
            {-2, 4},
            {-3, 4},
            {-3, 2},
            {-3, 0},
            {-3, -2},
            {-3, -4},

    };

    int coordThirdColor[22][2] = {
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {2, 4},
            {-1, -4},
            {-2, -4},
            {-1, -2},
            {-2, -2},
            {-1, -1},
            {-2, -1},
            {-1, 0},
            {-2, 0},
            {-2, -3},
            {-2, 1},
            {-2, 2},
            {-2, 3},
            {-4, 0},
            {-4, 2},
            {-4, 4},
            {-4, -2},
            {-4, -4},
    };


    glBegin(GL_POINTS);

    glColor3f(0.67, 0.48, 0);
    for (int i = 0; i < 35; i++) {
        int x = coordFirstColor[i][0], y = coordFirstColor[i][1];
        rotatePointyou(&x, &y, you.angle);
        glVertex2d(x, y);
    }

    glColor3f(1.0f, 0.63f, 0.27f);
    for (int i = 0; i < 28; i++) {
        int x = coordSecondColor[i][0], y = coordSecondColor[i][1];
        rotatePointyou(&x, &y, you.angle);
        glVertex2d(x, y);
    }

    glColor3f(0.97f, 0.84f, 0.47f);
    for (int i = 0; i < 22; i++) {
        int x = coordThirdColor[i][0], y = coordThirdColor[i][1];
        rotatePointyou(&x, &y, you.angle);
        glVertex2d(x, y);
    }

    glEnd();


}

void draw_enemy_tank_up_one()
{

    glPointSize(5);
    glBegin(GL_POINTS);



    int coordFirstColor[35][2] = {
            {0, 0},
            {0, -3},
            {1, -3},
            {1, -2},
            {2, -3},
            {3, -3},
            {4, -3},
            {2, -1},
            {3, -1},
            {4, -1},
            {2, 1},
            {3, 1},
            {4, 1},
            {2, 3},
            {3, 3},
            {4, 3},
            {2, 0},
            {2, 2},
            {2, -2},
            {2, -4},
            {1, -5},
            {0, -5},
            {-1, -5},
            {-3, -5},
            {-4, -5},
            {3, -5},
            {4, -5},
            {-3, -3},
            {-4, -3},
            {-3, -1},
            {-4, -1},
            {-3, 1},
            {-4, 1},
            {-3, 3},
            {-4, 3},
    };


    int coordSecondColor[28][2] = {
            {0, 1},
            {1, 1},
            {1, 2},
            {-1, 2},
            {-1, 1},
            {1, 0},
            {1, -1},
            {0, -1},
            {0, -2},
            {-1, -3},
            {0, -4},
            {1, -4},
            {3, -4},
            {4, -4},
            {3, -2},
            {4, -2},
            {3, 0},
            {4, 0},
            {3, 2},
            {4, 2},
            {3, 4},
            {4, 4},
            {-2, 4},
            {-3, 4},
            {-3, 2},
            {-3, 0},
            {-3, -2},
            {-3, -4},

    };

    int coordThirdColor[22][2] = {
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {2, 4},
            {-1, -4},
            {-2, -4},
            {-1, -2},
            {-2, -2},
            {-1, -1},
            {-2, -1},
            {-1, 0},
            {-2, 0},
            {-2, -3},
            {-2, 1},
            {-2, 2},
            {-2, 3},
            {-4, 0},
            {-4, 2},
            {-4, 4},
            {-4, -2},
            {-4, -4},
    };


    glBegin(GL_POINTS);

    glColor3f(0.28, 0.44, 0.56);
    for (int i = 0; i < 35; i++) {
        int x = coordFirstColor[i][0], y = coordFirstColor[i][1];
        rotatePointenemy_one(&x, &y, enemy[0].angle);
        glVertex2d(x, y);
    }

    glColor3f(0.73f, 0.74f, 0.73f);
    for (int i = 0; i < 28; i++) {
        int x = coordSecondColor[i][0], y = coordSecondColor[i][1];
        rotatePointenemy_one(&x, &y, enemy[0].angle);
        glVertex2d(x, y);
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 22; i++) {
        int x = coordThirdColor[i][0], y = coordThirdColor[i][1];
        rotatePointenemy_one(&x, &y, enemy[0].angle);
        glVertex2d(x, y);
    }

    glEnd();
}

void draw_enemy_tank_up_two()
{

    glPointSize(5);
    glBegin(GL_POINTS);



    int coordFirstColor[35][2] = {
            {0, 0},
            {0, -3},
            {1, -3},
            {1, -2},
            {2, -3},
            {3, -3},
            {4, -3},
            {2, -1},
            {3, -1},
            {4, -1},
            {2, 1},
            {3, 1},
            {4, 1},
            {2, 3},
            {3, 3},
            {4, 3},
            {2, 0},
            {2, 2},
            {2, -2},
            {2, -4},
            {1, -5},
            {0, -5},
            {-1, -5},
            {-3, -5},
            {-4, -5},
            {3, -5},
            {4, -5},
            {-3, -3},
            {-4, -3},
            {-3, -1},
            {-4, -1},
            {-3, 1},
            {-4, 1},
            {-3, 3},
            {-4, 3},
    };


    int coordSecondColor[28][2] = {
            {0, 1},
            {1, 1},
            {1, 2},
            {-1, 2},
            {-1, 1},
            {1, 0},
            {1, -1},
            {0, -1},
            {0, -2},
            {-1, -3},
            {0, -4},
            {1, -4},
            {3, -4},
            {4, -4},
            {3, -2},
            {4, -2},
            {3, 0},
            {4, 0},
            {3, 2},
            {4, 2},
            {3, 4},
            {4, 4},
            {-2, 4},
            {-3, 4},
            {-3, 2},
            {-3, 0},
            {-3, -2},
            {-3, -4},

    };

    int coordThirdColor[22][2] = {
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {2, 4},
            {-1, -4},
            {-2, -4},
            {-1, -2},
            {-2, -2},
            {-1, -1},
            {-2, -1},
            {-1, 0},
            {-2, 0},
            {-2, -3},
            {-2, 1},
            {-2, 2},
            {-2, 3},
            {-4, 0},
            {-4, 2},
            {-4, 4},
            {-4, -2},
            {-4, -4},
    };


    glBegin(GL_POINTS);

    glColor3f(0.28, 0.44, 0.56);
    for (int i = 0; i < 35; i++) {
        int x = coordFirstColor[i][0], y = coordFirstColor[i][1];
        rotatePointenemy_two(&x, &y, enemy[1].angle);
        glVertex2d(x, y);
    }

    glColor3f(0.73f, 0.74f, 0.73f);
    for (int i = 0; i < 28; i++) {
        int x = coordSecondColor[i][0], y = coordSecondColor[i][1];
        rotatePointenemy_two(&x, &y, enemy[1].angle);
        glVertex2d(x, y);
    }

    glColor3f(1.0f, 1.0f, 1.00f);
    for (int i = 0; i < 22; i++) {
        int x = coordThirdColor[i][0], y = coordThirdColor[i][1];
        rotatePointenemy_two(&x, &y, enemy[1].angle);
        glVertex2d(x, y);
    }

    glEnd();
}

void draw_enemy_tank_up_three()
{

    glPointSize(5);
    glBegin(GL_POINTS);



    int coordFirstColor[35][2] = {
            {0, 0},
            {0, -3},
            {1, -3},
            {1, -2},
            {2, -3},
            {3, -3},
            {4, -3},
            {2, -1},
            {3, -1},
            {4, -1},
            {2, 1},
            {3, 1},
            {4, 1},
            {2, 3},
            {3, 3},
            {4, 3},
            {2, 0},
            {2, 2},
            {2, -2},
            {2, -4},
            {1, -5},
            {0, -5},
            {-1, -5},
            {-3, -5},
            {-4, -5},
            {3, -5},
            {4, -5},
            {-3, -3},
            {-4, -3},
            {-3, -1},
            {-4, -1},
            {-3, 1},
            {-4, 1},
            {-3, 3},
            {-4, 3},
    };


    int coordSecondColor[28][2] = {
            {0, 1},
            {1, 1},
            {1, 2},
            {-1, 2},
            {-1, 1},
            {1, 0},
            {1, -1},
            {0, -1},
            {0, -2},
            {-1, -3},
            {0, -4},
            {1, -4},
            {3, -4},
            {4, -4},
            {3, -2},
            {4, -2},
            {3, 0},
            {4, 0},
            {3, 2},
            {4, 2},
            {3, 4},
            {4, 4},
            {-2, 4},
            {-3, 4},
            {-3, 2},
            {-3, 0},
            {-3, -2},
            {-3, -4},

    };

    int coordThirdColor[22][2] = {
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {2, 4},
            {-1, -4},
            {-2, -4},
            {-1, -2},
            {-2, -2},
            {-1, -1},
            {-2, -1},
            {-1, 0},
            {-2, 0},
            {-2, -3},
            {-2, 1},
            {-2, 2},
            {-2, 3},
            {-4, 0},
            {-4, 2},
            {-4, 4},
            {-4, -2},
            {-4, -4},
    };


    glBegin(GL_POINTS);

    glColor3f(0.28, 0.44, 0.56);
    for (int i = 0; i < 35; i++) {
        int x = coordFirstColor[i][0], y = coordFirstColor[i][1];
        rotatePointenemy_three(&x, &y, enemy[2].angle);
        glVertex2d(x, y);
    }

    glColor3f(0.73f, 0.74f, 0.73f);
    for (int i = 0; i < 28; i++) {
        int x = coordSecondColor[i][0], y = coordSecondColor[i][1];
        rotatePointenemy_three(&x, &y, enemy[2].angle);
        glVertex2d(x, y);
    }

    glColor3f(1.0f, 1.0f, 1.00f);
    for (int i = 0; i < 22; i++) {
        int x = coordThirdColor[i][0], y = coordThirdColor[i][1];
        rotatePointenemy_three(&x, &y, enemy[2].angle);
        glVertex2d(x, y);
    }

    glEnd();
}

void draw_enemy_tank_up_four()
{

    glPointSize(5);
    glBegin(GL_POINTS);



    int coordFirstColor[35][2] = {
            {0, 0},
            {0, -3},
            {1, -3},
            {1, -2},
            {2, -3},
            {3, -3},
            {4, -3},
            {2, -1},
            {3, -1},
            {4, -1},
            {2, 1},
            {3, 1},
            {4, 1},
            {2, 3},
            {3, 3},
            {4, 3},
            {2, 0},
            {2, 2},
            {2, -2},
            {2, -4},
            {1, -5},
            {0, -5},
            {-1, -5},
            {-3, -5},
            {-4, -5},
            {3, -5},
            {4, -5},
            {-3, -3},
            {-4, -3},
            {-3, -1},
            {-4, -1},
            {-3, 1},
            {-4, 1},
            {-3, 3},
            {-4, 3},
    };


    int coordSecondColor[28][2] = {
            {0, 1},
            {1, 1},
            {1, 2},
            {-1, 2},
            {-1, 1},
            {1, 0},
            {1, -1},
            {0, -1},
            {0, -2},
            {-1, -3},
            {0, -4},
            {1, -4},
            {3, -4},
            {4, -4},
            {3, -2},
            {4, -2},
            {3, 0},
            {4, 0},
            {3, 2},
            {4, 2},
            {3, 4},
            {4, 4},
            {-2, 4},
            {-3, 4},
            {-3, 2},
            {-3, 0},
            {-3, -2},
            {-3, -4},

    };

    int coordThirdColor[22][2] = {
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {2, 4},
            {-1, -4},
            {-2, -4},
            {-1, -2},
            {-2, -2},
            {-1, -1},
            {-2, -1},
            {-1, 0},
            {-2, 0},
            {-2, -3},
            {-2, 1},
            {-2, 2},
            {-2, 3},
            {-4, 0},
            {-4, 2},
            {-4, 4},
            {-4, -2},
            {-4, -4},
    };


    glBegin(GL_POINTS);

    glColor3f(0.28, 0.44, 0.56);
    for (int i = 0; i < 35; i++) {
        int x = coordFirstColor[i][0], y = coordFirstColor[i][1];
        rotatePointenemy_four(&x, &y, enemy[3].angle);
        glVertex2d(x, y);
    }

    glColor3f(0.73f, 0.74f, 0.73f);
    for (int i = 0; i < 28; i++) {
        int x = coordSecondColor[i][0], y = coordSecondColor[i][1];
        rotatePointenemy_four(&x, &y, enemy[3].angle);
        glVertex2d(x, y);
    }

    glColor3f(1.0f, 1.0f, 1.00f);
    for (int i = 0; i < 22; i++) {
        int x = coordThirdColor[i][0], y = coordThirdColor[i][1];
        rotatePointenemy_four(&x, &y, enemy[3].angle);
        glVertex2d(x, y);
    }

    glEnd();
}

void draw_block_zone()
{
    glColor3f(0.29f, 0.27f, 0.27f);

    glBegin(GL_QUADS);

    glVertex2d(-93, 110);
    glVertex2d(-110, 110);
    glVertex2d(-110, -110);
    glVertex2d(-93, -110);

    glEnd();


    glBegin(GL_QUADS);

    glVertex2d(-110, -93);
    glVertex2d(-110, -110);
    glVertex2d(110, -110);
    glVertex2d(110, -93);

    glEnd();


    glBegin(GL_QUADS);

    glVertex2d(93, 110);
    glVertex2d(110, 110);
    glVertex2d(110, -110);
    glVertex2d(93, -110);

    glEnd();


    glBegin(GL_QUADS);

    glVertex2d(-110, 93);
    glVertex2d(-110, 110);
    glVertex2d(110, 110);
    glVertex2d(110, 93);

    glEnd();

}

void check_block_entity(short flag)
{
    if (flag == 1)
    {
        int f = 0;
        for (int i = you.yPos; i < you.yPos + 8; i += 1) {


            f = Moving_Vertical(i, flag);

            if (f == 1) {
                f = 0;
                return;
            }

            if (f == 2) {
                if (you.yPos + 4 <= 92) you.yPos += 1;
                f = 0;

                return;
            }

        }
        if (you.yPos + 4 < 92) you.yPos += moveSpeed;
    }


    if (flag == 2)
    {

        int f = 0;
        for (int i = you.yPos; i > you.yPos - 11; i -= 1)
        {
            f = Moving_Vertical(i, flag);
            if (f == 1)
            {
                f = 0;
                return;
            }

            if (f == 2) {
                if (you.yPos - 4 >= -92) you.yPos -= 1;
                f = 0;
                return;
            }


        }
        if (you.yPos - 4 > -92) you.yPos -= moveSpeed;
    }


    if (flag == 3)
    {
        int f = 0;
        for (int i = you.xPos; i > you.xPos - 11; i -= 1)
        {
            f = Moving_Horizontal(i);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (you.xPos - 4 > -92) you.xPos -= moveSpeed;
    }

    if (flag == 4)
    {
        int f = 0;
        for (int i = you.xPos; i < you.xPos + 7; i += 1)
        {

            f = Moving_Horizontal(i);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (you.xPos + 4 < 92) you.xPos += moveSpeed;
    }
}

void check_block_entity_enemy_one(int flag)
{
    if (flag == 1)
    {
        int f = 0;
        for (int i = enemy[0].yPos; i < enemy[0].yPos + 8; i += 1) {


            f = Enemy_Moving_Vertical(i, 0, flag);

            if (f == 1) {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[0].yPos + 4 <= 92)  enemy[0].yPos += 1;
                f = 0;

                return;
            }

        }
        if (enemy[0].yPos + 4 <= 92)  enemy[0].yPos += moveSpeed;
    }


    if (flag == 2)
    {

        int f = 0;
        for (int i = enemy[0].yPos; i > enemy[0].yPos - 10; i -= 1)
        {
            f = Enemy_Moving_Vertical(i, 0, flag);
            if (f == 1)
            {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[0].yPos - 4 >= -92)  enemy[0].yPos -= 1;
                f = 0;
                return;
            }


        }
        if (enemy[0].yPos - 4 >= -92)  enemy[0].yPos -= moveSpeed;
    }


    if (flag == 3)
    {
        int f = 0;
        for (int i = enemy[0].xPos; i > enemy[0].xPos - 10; i -= 1)
        {
            f = Enemy_Moving_Horizontal(i, 0);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[0].xPos - 4 >= -92)  enemy[0].xPos -= moveSpeed;
    }

    if (flag == 4)
    {
        int f = 0;
        for (int i = enemy[0].xPos; i < enemy[0].xPos + 7; i += 1)
        {

            f = Enemy_Moving_Horizontal(i, 0);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[0].xPos + 4 <= 92)  enemy[0].xPos += moveSpeed;
    }
}

void check_block_entity_enemy_two(int flag)
{
    if (flag == 1)
    {
        int f = 0;
        for (int i = enemy[1].yPos; i < enemy[1].yPos + 8; i += 1) {


            f = Enemy_Moving_Vertical(i, 1, flag);

            if (f == 1) {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[1].yPos + 4 <= 92)  enemy[1].yPos += 1;
                f = 0;

                return;
            }

        }
        if (enemy[1].yPos + 4 <= 92)  enemy[1].yPos += moveSpeed;
    }


    if (flag == 2)
    {

        int f = 0;
        for (int i = enemy[1].yPos; i > enemy[1].yPos - 10; i -= 1)
        {
            f = Enemy_Moving_Vertical(i, 1, flag);
            if (f == 1)
            {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[1].yPos - 4 >= -92)  enemy[1].yPos -= 1;
                f = 0;
                return;
            }


        }
        if (enemy[1].yPos - 4 >= -92)  enemy[1].yPos -= moveSpeed;
    }


    if (flag == 3)
    {
        int f = 0;
        for (int i = enemy[1].xPos; i > enemy[1].xPos - 10; i -= 1)
        {
            f = Enemy_Moving_Horizontal(i, 1);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[1].xPos - 4 >= -92)  enemy[1].xPos -= moveSpeed;
    }

    if (flag == 4)
    {
        int f = 0;
        for (int i = enemy[1].xPos; i < enemy[1].xPos + 7; i += 1)
        {

            f = Enemy_Moving_Horizontal(i, 1);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[1].xPos + 4 <= 92)  enemy[1].xPos += moveSpeed;
    }
}

void check_block_entity_enemy_three(int flag)
{
    if (flag == 1)
    {
        int f = 0;
        for (int i = enemy[2].yPos; i < enemy[2].yPos + 8; i += 1) {


            f = Enemy_Moving_Vertical(i, 2, flag);

            if (f == 1) {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[2].yPos + 4 <= 92)  enemy[2].yPos += 1;
                f = 0;

                return;
            }

        }
        if (enemy[2].yPos + 4 <= 92)  enemy[2].yPos += moveSpeed;
    }


    if (flag == 2)
    {

        int f = 0;
        for (int i = enemy[2].yPos; i > enemy[2].yPos - 10; i -= 1)
        {
            f = Enemy_Moving_Vertical(i, 2, flag);
            if (f == 1)
            {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[2].yPos - 4 >= -92)  enemy[2].yPos -= 1;
                f = 0;
                return;
            }


        }
        if (enemy[2].yPos - 4 >= -92)  enemy[2].yPos -= moveSpeed;
    }


    if (flag == 3)
    {
        int f = 0;
        for (int i = enemy[2].xPos; i > enemy[2].xPos - 10; i -= 1)
        {
            f = Enemy_Moving_Horizontal(i, 2);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[2].xPos - 4 >= -92)  enemy[2].xPos -= moveSpeed;
    }

    if (flag == 4)
    {
        int f = 0;
        for (int i = enemy[2].xPos; i < enemy[2].xPos + 7; i += 1)
        {

            f = Enemy_Moving_Horizontal(i, 2);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[2].xPos + 4 <= 92)  enemy[2].xPos += moveSpeed;
    }
}

void check_block_entity_enemy_four(int flag)
{
    if (flag == 1)
    {
        int f = 0;
        for (int i = enemy[3].yPos; i < enemy[3].yPos + 8; i += 1) {


            f = Enemy_Moving_Vertical(i, 3, flag);

            if (f == 1) {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[3].yPos + 4 <= 92)  enemy[3].yPos += 1;
                f = 0;

                return;
            }

        }
        if (enemy[3].yPos + 4 <= 92)  enemy[3].yPos += moveSpeed;
    }


    if (flag == 2)
    {

        int f = 0;
        for (int i = enemy[3].yPos; i > enemy[3].yPos - 10; i -= 1)
        {
            f = Enemy_Moving_Vertical(i, 3, flag);
            if (f == 1)
            {
                f = 0;
                return;
            }

            if (f == 2) {
                if (enemy[3].yPos - 4 >= -92)  enemy[3].yPos -= 1;
                f = 0;
                return;
            }


        }
        if (enemy[3].yPos - 4 >= -92)  enemy[3].yPos -= moveSpeed;
    }


    if (flag == 3)
    {
        int f = 0;
        for (int i = enemy[3].xPos; i > enemy[3].xPos - 10; i -= 1)
        {
            f = Enemy_Moving_Horizontal(i, 3);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[3].xPos - 4 >= -92)  enemy[3].xPos -= moveSpeed;
    }

    if (flag == 4)
    {
        int f = 0;
        for (int i = enemy[3].xPos; i < enemy[3].xPos + 7; i += 1)
        {

            f = Enemy_Moving_Horizontal(i, 3);
            if (f == 1)
            {
                f = 0;
                return;
            }
        }
        if (enemy[3].xPos + 4 <= 92)  enemy[3].xPos += moveSpeed;
    }
}

void draw_bullet_effects_for_global_tank()
{
    int anim_x;
    int anim_y;

    if (you.bulletSet.time_anim == 0)
    {
        you.bulletSet.an_x = you.bulletSet.dx;
        you.bulletSet.an_y = you.bulletSet.dy;
    }

    anim_x = you.bulletSet.an_x;
    anim_y = you.bulletSet.an_y;

    GLfloat colors[10][3] = {
            {1.0f, 1.0f, 1.0f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
    };

    if (you.bulletSet.anim_bool == 1 && you.bulletSet.time_anim < BOOM_ANIMATION_FRAMES) {

        int randomIndex = rand() % 3;

        glColor3fv(colors[randomIndex]);

        glBegin(GL_POINTS);
        for (int i = 0; i < 30; ++i) {
            float angle = 2 * 3.14159516 * rand() / RAND_MAX;
            int radius = 5 * rand() / RAND_MAX;
            glVertex2f(anim_x + radius * cos(angle), anim_y + radius * sin(angle));
        }
        you.bulletSet.time_anim += 1;
        glEnd();
    }
    else if (you.bulletSet.time_anim == BOOM_ANIMATION_FRAMES)
    {
        you.bulletSet.anim_bool = 0;
        you.bulletSet.time_anim = 0;

    }
}

void draw_bullet_global()
{
    if (you.bulletSet.live == 1)
    {
        if (you.bulletSet.dx < 92 && you.bulletSet.dx > -92 && you.bulletSet.dy < 92 && you.bulletSet.dy > -92)
        {
            for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
            {
                if (bricksLoc[i][0] == 1)
                {
                    if (
                        (bricksLoc[i][2] <= you.bulletSet.dy && bricksLoc[i][2] + 4 >= you.bulletSet.dy)
                        &&
                        (bricksLoc[i][1] <= you.bulletSet.dx && bricksLoc[i][1] + 4 >= you.bulletSet.dx)
                        )
                    {
                        you.bulletSet.anim_bool = 1;
                        bricksLoc[i][0] = 0;

                        Delete_Table_x(bricksLoc[i][1], bricksLoc[i][2]);
                        Delete_Table_y(bricksLoc[i][2], bricksLoc[i][1]);
                        you.bulletSet.live = 0;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
            {
                if (cementLoc[i][0] == 0)
                {
                    if (
                        (cementLoc[i][2] <= you.bulletSet.dy && cementLoc[i][2] + 4 >= you.bulletSet.dy)
                        &&
                        (cementLoc[i][1] <= you.bulletSet.dx && cementLoc[i][1] + 4 >= you.bulletSet.dx)
                        )
                    {
                        you.bulletSet.anim_bool = 1;
                        cementLoc[i][0] = 0;
                        you.bulletSet.live = 0;
                        break;
                    }
                }
            }

            glColor3f(0.502f, 0.502f, 0.502f);
            if (you.bulletSet.angle_bull == 0.0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(you.bulletSet.dx - 0.4, you.bulletSet.dy);
                glVertex2d(you.bulletSet.dx + 0.4, you.bulletSet.dy);

                glVertex2d(you.bulletSet.dx - 0.4, you.bulletSet.dy + 1);
                glVertex2d(you.bulletSet.dx + 0.4, you.bulletSet.dy + 1);

                glVertex2d(you.bulletSet.dx, you.bulletSet.dy + 1.5);
                glVertex2d(you.bulletSet.dx, you.bulletSet.dy + 1.5);



                glEnd();
                you.bulletSet.dy += 1;
            }
            if (you.bulletSet.angle_bull == 180.0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(you.bulletSet.dx - 0.4, you.bulletSet.dy);
                glVertex2d(you.bulletSet.dx + 0.4, you.bulletSet.dy);

                glVertex2d(you.bulletSet.dx - 0.4, you.bulletSet.dy - 1);
                glVertex2d(you.bulletSet.dx + 0.4, you.bulletSet.dy - 1);

                glVertex2d(you.bulletSet.dx, you.bulletSet.dy - 1.5);
                glVertex2d(you.bulletSet.dx, you.bulletSet.dy - 1.5);
                glEnd();
                you.bulletSet.dy -= 1;
            }
            if (you.bulletSet.angle_bull == 270.0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(you.bulletSet.dx, you.bulletSet.dy - 0.4);
                glVertex2d(you.bulletSet.dx, you.bulletSet.dy + 0.4);

                glVertex2d(you.bulletSet.dx + 1, you.bulletSet.dy - 0.4);
                glVertex2d(you.bulletSet.dx + 1, you.bulletSet.dy + 0.4);

                glVertex2d(you.bulletSet.dx + 1.5, you.bulletSet.dy);
                glVertex2d(you.bulletSet.dx + 1.5, you.bulletSet.dy);
                glEnd();

                you.bulletSet.dx += 1;
            }
            if (you.bulletSet.angle_bull == 90.0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(you.bulletSet.dx, you.bulletSet.dy - 0.4);
                glVertex2d(you.bulletSet.dx, you.bulletSet.dy + 0.4);

                glVertex2d(you.bulletSet.dx - 1, you.bulletSet.dy - 0.4);
                glVertex2d(you.bulletSet.dx - 1, you.bulletSet.dy + 0.4);

                glVertex2d(you.bulletSet.dx - 1.5, you.bulletSet.dy);
                glVertex2d(you.bulletSet.dx - 1.5, you.bulletSet.dy);
                glEnd();
                you.bulletSet.dx -= 1;

            }
            if (you.bulletSet.dx <= enemy[0].xPos + 5 && you.bulletSet.dx >= enemy[0].xPos - 5 && enemy[0].live == 1)
                if (you.bulletSet.dy <= enemy[0].yPos + 5 && you.bulletSet.dy >= enemy[0].yPos - 5 && enemy[0].live == 1)
                {
                    enemy[0].live = 0;
                    you.bulletSet.anim_bool = 1;
                    you.bulletSet.live = 0;
                }

            if (you.bulletSet.dx <= enemy[1].xPos + 5 && you.bulletSet.dx >= enemy[1].xPos - 5 && enemy[1].live == 1)
                if (you.bulletSet.dy <= enemy[1].yPos + 5 && you.bulletSet.dy >= enemy[1].yPos - 5 && enemy[1].live == 1)
                {
                    enemy[1].live = 0;
                    you.bulletSet.anim_bool = 1;
                    you.bulletSet.live = 0;
                }

            if (you.bulletSet.dx <= enemy[2].xPos + 5 && you.bulletSet.dx >= enemy[2].xPos - 5 && enemy[2].live == 1)
                if (you.bulletSet.dy <= enemy[2].yPos + 5 && you.bulletSet.dy >= enemy[2].yPos - 5 && enemy[2].live == 1)
                {
                    enemy[2].live = 0;
                    you.bulletSet.anim_bool = 1;
                    you.bulletSet.live = 0;
                }

            if (you.bulletSet.dx <= enemy[3].xPos + 5 && you.bulletSet.dx >= enemy[3].xPos - 5 && enemy[3].live == 1)
                if (you.bulletSet.dy <= enemy[3].yPos + 5 && you.bulletSet.dy >= enemy[3].yPos - 5 && enemy[3].live == 1)
                {
                    enemy[3].live = 0;
                    you.bulletSet.anim_bool = 1;
                    you.bulletSet.live = 0;
                }

        }
        else
        {
            you.bulletSet.anim_bool = 1;
            you.bulletSet.live = 0;
        }
    }
    else
    {
        you.bulletSet.dx = you.xPos;
        you.bulletSet.dy = you.yPos;
        you.bulletSet.angle_bull = you.angle;
    }
}

void draw_bullet_enemy_one()
{
    if (enemy[0].bulletSet.live == 1)
    {
        if (enemy[0].bulletSet.dx < 92 && enemy[0].bulletSet.dx > -92 && enemy[0].bulletSet.dy < 92 && enemy[0].bulletSet.dy > -92)
        {
            for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
            {
                if (bricksLoc[i][0] == 1)
                {
                    if (
                        (bricksLoc[i][2] <= enemy[0].bulletSet.dy && bricksLoc[i][2] + 4 >= enemy[0].bulletSet.dy)
                        &&
                        (bricksLoc[i][1] <= enemy[0].bulletSet.dx && bricksLoc[i][1] + 4 >= enemy[0].bulletSet.dx)
                        )
                    {
                        enemy[0].bulletSet.anim_bool = 1;
                        bricksLoc[i][0] = 0;
                        Delete_Table_x(bricksLoc[i][1], bricksLoc[i][2]);
                        Delete_Table_y(bricksLoc[i][2], bricksLoc[i][1]);
                        enemy[0].bulletSet.live = 0;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
            {
                if (cementLoc[i][0] == 0)
                {
                    if (
                        (cementLoc[i][2] <= enemy[0].bulletSet.dy && cementLoc[i][2] + 4 >= enemy[0].bulletSet.dy)
                        &&
                        (cementLoc[i][1]<= enemy[0].bulletSet.dx && cementLoc[i][1] + 4 >= enemy[0].bulletSet.dx)
                        )
                    {
                        enemy[0].bulletSet.anim_bool = 1;
                        cementLoc[i][0] = 0;
                        enemy[0].bulletSet.live = 0;
                        break;
                    }
                }
            }

            glColor3f(0.502f, 0.502f, 0.502f);
            if (enemy[0].bulletSet.angle_bull == 0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(enemy[0].bulletSet.dx - 0.4, enemy[0].bulletSet.dy);
                glVertex2d(enemy[0].bulletSet.dx + 0.4, enemy[0].bulletSet.dy);

                glVertex2d(enemy[0].bulletSet.dx - 0.4, enemy[0].bulletSet.dy + 1);
                glVertex2d(enemy[0].bulletSet.dx + 0.4, enemy[0].bulletSet.dy +1);

                glVertex2d(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy + 1.5);
                glVertex2d(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy + 1.5);



                glEnd();
                enemy[0].bulletSet.dy += 1;
            }
            if (enemy[0].bulletSet.angle_bull == 180)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[0].bulletSet.dx - 0.4, enemy[0].bulletSet.dy);
                glVertex2f(enemy[0].bulletSet.dx + 0.4, enemy[0].bulletSet.dy);

                glVertex2f(enemy[0].bulletSet.dx - 0.4, enemy[0].bulletSet.dy - 1);
                glVertex2f(enemy[0].bulletSet.dx + 0.4, enemy[0].bulletSet.dy - 1);

                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy - 1.5);
                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy - 1.5);
                glEnd();
                enemy[0].bulletSet.dy -= 1;
            }
            if (enemy[0].bulletSet.angle_bull == 270)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy - 0.4);
                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy + 0.4);

                glVertex2f(enemy[0].bulletSet.dx + 1, enemy[0].bulletSet.dy - 0.4);
                glVertex2f(enemy[0].bulletSet.dx + 1, enemy[0].bulletSet.dy + 0.4);

                glVertex2f(enemy[0].bulletSet.dx + 1.5, enemy[0].bulletSet.dy);
                glVertex2f(enemy[0].bulletSet.dx + 1.5, enemy[0].bulletSet.dy);
                glEnd();

                enemy[0].bulletSet.dx += 1;
            }
            if (enemy[0].bulletSet.angle_bull == 90)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy - 0.4);
                glVertex2f(enemy[0].bulletSet.dx, enemy[0].bulletSet.dy + 0.4);

                glVertex2f(enemy[0].bulletSet.dx - 1, enemy[0].bulletSet.dy - 0.4);
                glVertex2f(enemy[0].bulletSet.dx - 1, enemy[0].bulletSet.dy + 0.4);

                glVertex2f(enemy[0].bulletSet.dx - 1.5, enemy[0].bulletSet.dy);
                glVertex2f(enemy[0].bulletSet.dx - 1.5, enemy[0].bulletSet.dy);
                glEnd();
                enemy[0].bulletSet.dx -= 1;

            }
            if (enemy[0].bulletSet.dx <= you.xPos + 5 && enemy[0].bulletSet.dx >= you.xPos - 5 && you.live == 1 && enemy[0].live == 1)
                if (enemy[0].bulletSet.dy <= you.yPos + 5 && enemy[0].bulletSet.dy >= you.yPos - 5 && you.live == 1 && enemy[0].live == 1)
                {
                    you.live = 0;
                    enemy[0].bulletSet.anim_bool = 1;
                    enemy[0].bulletSet.live = 0;
                }

        }
        else
        {
            enemy[0].bulletSet.anim_bool = 1;
            enemy[0].bulletSet.live = 0;
        }
    }
    else
    {
        enemy[0].bulletSet.dx = enemy[0].xPos;
        enemy[0].bulletSet.dy = enemy[0].yPos;
        enemy[0].bulletSet.angle_bull = you.angle;
    }
}

void draw_bullet_effects_for_enemy_one()
{
    int anim_x;
    int anim_y;

    if (enemy[0].bulletSet.time_anim == 0)
    {
        enemy[0].bulletSet.an_x = enemy[0].bulletSet.dx;
        enemy[0].bulletSet.an_y = enemy[0].bulletSet.dy;
    }

    anim_x = enemy[0].bulletSet.an_x;
    anim_y = enemy[0].bulletSet.an_y;

    GLfloat colors[10][3] = {
            {1.0f, 1.0f, 1.0f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
    };

    if (enemy[0].bulletSet.anim_bool == 1 && enemy[0].bulletSet.time_anim < BOOM_ANIMATION_FRAMES) {

        int randomIndex = rand() % 3;

        glColor3fv(colors[randomIndex]);

        glBegin(GL_POINTS);
        for (int i = 0; i < 30; ++i) {
            float angle = 2 * 3.14159516 * rand() / RAND_MAX;
            int radius = 5 * rand() / RAND_MAX;
            glVertex2f(anim_x + radius * cos(angle), anim_y + radius * sin(angle));
        }
        enemy[0].bulletSet.time_anim += 1;
        glEnd();
    }
    else if (enemy[0].bulletSet.time_anim == BOOM_ANIMATION_FRAMES)
    {
        enemy[0].bulletSet.anim_bool = 0;
        enemy[0].bulletSet.time_anim = 0;

    }
}

void move_and_shoot_enemy_one()
{
    if (enemy[0].live == 1) {
        if (enemy[0].timerMove < enemy[0].botSpeed)
            enemy[0].timerMove++;
        else {
            if (enemy[0].yPos == enemy[0].saveYPos && enemy[0].xPos == enemy[0].saveXPos) {
                enemy[0].flagAutoMove = 1 + (rand() % 4 - 1 + 1);
            }

            enemy[0].saveXPos = enemy[0].xPos;
            enemy[0].saveYPos = enemy[0].yPos;

            if (enemy[0].flagAutoMove == 1) {
                enemy[0].angle = 0;
            }
            else if (enemy[0].flagAutoMove == 2) {
                enemy[0].angle = 180;
            }
            else if (enemy[0].flagAutoMove == 3) {
                enemy[0].angle = 90;
            }
            else if (enemy[0].flagAutoMove == 4) {
                enemy[0].angle = 270;
            }

            check_block_entity_enemy_one(enemy[0].flagAutoMove);
            enemy[0].timerMove = 0;
        }

        if (enemy[0].tickToShoot < 100)
            enemy[0].tickToShoot++;
        else {

            if (enemy[0].flagAutoMove == 1) {
                enemy[0].bulletSet.angle_bull = 0;
                enemy[0].bulletSet.live = 1;
            }
            else if (enemy[0].flagAutoMove == 2) {
                enemy[0].bulletSet.angle_bull = 180;
                enemy[0].bulletSet.live = 1;
            }
            else if (enemy[0].flagAutoMove == 3) {
                enemy[0].bulletSet.angle_bull = 90;
                enemy[0].bulletSet.live = 1;
            }
            else if (enemy[0].flagAutoMove == 4) {
                enemy[0].bulletSet.angle_bull = 270;
                enemy[0].bulletSet.live = 1;
            }


            enemy[0].tickToShoot = 0;
        }


    }
}

void draw_bullet_enemy_two()
{
    if (enemy[1].bulletSet.live == 1)
    {
        if (enemy[1].bulletSet.dx < 92 && enemy[1].bulletSet.dx > -92 && enemy[1].bulletSet.dy < 92 && enemy[1].bulletSet.dy > -92)
        {
            for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
            {
                if (bricksLoc[i][0] == 1)
                {
                    if (
                        (bricksLoc[i][2]  <= enemy[1].bulletSet.dy && bricksLoc[i][2] + 4 >= enemy[1].bulletSet.dy)
                        &&
                        (bricksLoc[i][1] <= enemy[1].bulletSet.dx && bricksLoc[i][1] + 4 >= enemy[1].bulletSet.dx)
                        )
                    {
                        enemy[1].bulletSet.anim_bool = 1;
                        Delete_Table_x(bricksLoc[i][1], bricksLoc[i][2]);
                        Delete_Table_y(bricksLoc[i][2], bricksLoc[i][1]);
                        bricksLoc[i][0] = 0;
                        enemy[1].bulletSet.live = 0;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
            {
                if (cementLoc[i][0] == 0)
                {
                    if (
                        (cementLoc[i][2]  <= enemy[1].bulletSet.dy && cementLoc[i][2] + 4 >= enemy[1].bulletSet.dy)
                        &&
                        (cementLoc[i][1]  <= enemy[1].bulletSet.dx && cementLoc[i][1] + 4 >= enemy[1].bulletSet.dx)
                        )
                    {
                        enemy[1].bulletSet.anim_bool = 1;
                        cementLoc[i][0] = 0;
                        enemy[1].bulletSet.live = 0;
                        break;
                    }
                }
            }

            glColor3f(0.502f, 0.502f, 0.502f);
            if (enemy[1].bulletSet.angle_bull == 0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[1].bulletSet.dx - 0.4, enemy[1].bulletSet.dy);
                glVertex2f(enemy[1].bulletSet.dx + 0.4, enemy[1].bulletSet.dy);

                glVertex2f(enemy[1].bulletSet.dx - 0.4, enemy[1].bulletSet.dy + 1);
                glVertex2f(enemy[1].bulletSet.dx + 0.4, enemy[1].bulletSet.dy + 1);

                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy + 1.5);
                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy + 1.5);



                glEnd();
                enemy[1].bulletSet.dy += 1;
            }
            if (enemy[1].bulletSet.angle_bull == 180)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[1].bulletSet.dx - 0.4, enemy[1].bulletSet.dy);
                glVertex2f(enemy[1].bulletSet.dx + 0.4, enemy[1].bulletSet.dy);

                glVertex2f(enemy[1].bulletSet.dx - 0.4, enemy[1].bulletSet.dy - 1);
                glVertex2f(enemy[1].bulletSet.dx + 0.4, enemy[1].bulletSet.dy - 1);

                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy - 1.5);
                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy - 1.5);
                glEnd();
                enemy[1].bulletSet.dy -= 1;
            }
            if (enemy[1].bulletSet.angle_bull == 270)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy - 0.4);
                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy + 0.4);

                glVertex2f(enemy[1].bulletSet.dx + 1, enemy[1].bulletSet.dy - 0.4);
                glVertex2f(enemy[1].bulletSet.dx + 1, enemy[1].bulletSet.dy + 0.4);

                glVertex2f(enemy[1].bulletSet.dx + 1.5, enemy[1].bulletSet.dy);
                glVertex2f(enemy[1].bulletSet.dx + 1.5, enemy[1].bulletSet.dy);
                glEnd();

                enemy[1].bulletSet.dx += 1;
            }
            if (enemy[1].bulletSet.angle_bull == 90)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy - 0.4);
                glVertex2f(enemy[1].bulletSet.dx, enemy[1].bulletSet.dy + 0.4);

                glVertex2f(enemy[1].bulletSet.dx - 1, enemy[1].bulletSet.dy - 0.4);
                glVertex2f(enemy[1].bulletSet.dx - 1, enemy[1].bulletSet.dy + 0.4);

                glVertex2f(enemy[1].bulletSet.dx - 1.5, enemy[1].bulletSet.dy);
                glVertex2f(enemy[1].bulletSet.dx - 1.5, enemy[1].bulletSet.dy);
                glEnd();
                enemy[1].bulletSet.dx -= 1;

            }
            if (enemy[1].bulletSet.dx <= you.xPos + 5 && enemy[1].bulletSet.dx >= you.xPos - 5 && you.live == 1 && enemy[1].live == 1)
                if (enemy[1].bulletSet.dy <= you.yPos + 5 && enemy[1].bulletSet.dy >= you.yPos - 5 && you.live == 1 && enemy[1].live == 1)
                {
                    you.live = 0;
                    enemy[1].bulletSet.anim_bool = 1;
                    enemy[1].bulletSet.live = 0;
                }

        }
        else
        {
            enemy[1].bulletSet.anim_bool = 1;
            enemy[1].bulletSet.live = 0;
        }
    }
    else
    {
        enemy[1].bulletSet.dx = enemy[1].xPos;
        enemy[1].bulletSet.dy = enemy[1].yPos;
        enemy[1].bulletSet.angle_bull = you.angle;
    }
}

void draw_bullet_effects_for_enemy_two()
{
    int anim_x;
    int anim_y;

    if (enemy[1].bulletSet.time_anim == 0)
    {
        enemy[1].bulletSet.an_x = enemy[1].bulletSet.dx;
        enemy[1].bulletSet.an_y = enemy[1].bulletSet.dy;
    }

    anim_x = enemy[1].bulletSet.an_x;
    anim_y = enemy[1].bulletSet.an_y;

    GLfloat colors[10][3] = {
            {1.0f, 1.0f, 1.0f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
    };

    if (enemy[1].bulletSet.anim_bool == 1 && enemy[1].bulletSet.time_anim < BOOM_ANIMATION_FRAMES) {

        int randomIndex = rand() % 3;

        glColor3fv(colors[randomIndex]);

        glBegin(GL_POINTS);
        for (int i = 0; i < 30; ++i) {
            float angle = 2 * 3.14159516 * rand() / RAND_MAX;
            int radius = 5 * rand() / RAND_MAX;
            glVertex2f(anim_x + radius * cos(angle), anim_y + radius * sin(angle));
        }
        enemy[1].bulletSet.time_anim += 1;
        glEnd();
    }
    else if (enemy[1].bulletSet.time_anim == BOOM_ANIMATION_FRAMES)
    {
        enemy[1].bulletSet.anim_bool = 0;
        enemy[1].bulletSet.time_anim = 0;

    }
}

void move_and_shoot_enemy_two()
{
    if (enemy[1].live == 1) {
        if (enemy[1].timerMove < enemy[1].botSpeed)
            enemy[1].timerMove++;
        else {
            if (enemy[1].yPos == enemy[1].saveYPos && enemy[1].xPos == enemy[1].saveXPos) {
                enemy[1].flagAutoMove = 1 + (rand() % 4 - 1 + 1);
            }

            enemy[1].saveXPos = enemy[1].xPos;
            enemy[1].saveYPos = enemy[1].yPos;

            if (enemy[1].flagAutoMove == 1) {
                enemy[1].angle = 0;
            }
            else if (enemy[1].flagAutoMove == 2) {
                enemy[1].angle = 180;
            }
            else if (enemy[1].flagAutoMove == 3) {
                enemy[1].angle = 90;
            }
            else if (enemy[1].flagAutoMove == 4) {
                enemy[1].angle = 270;
            }

            check_block_entity_enemy_two(enemy[1].flagAutoMove);
            enemy[1].timerMove = 0;
        }

        if (enemy[1].tickToShoot < 100)
            enemy[1].tickToShoot++;
        else {

            if (enemy[1].flagAutoMove == 1) {
                enemy[1].bulletSet.angle_bull = 0;
                enemy[1].bulletSet.live = 1;
            }
            else if (enemy[1].flagAutoMove == 2) {
                enemy[1].bulletSet.angle_bull = 180;
                enemy[1].bulletSet.live = 1;
            }
            else if (enemy[1].flagAutoMove == 3) {
                enemy[1].bulletSet.angle_bull = 90;
                enemy[1].bulletSet.live = 1;
            }
            else if (enemy[1].flagAutoMove == 4) {
                enemy[1].bulletSet.angle_bull = 270;
                enemy[1].bulletSet.live = 1;
            }


            enemy[1].tickToShoot = 0;
        }


    }
}

void draw_bullet_enemy_three()
{
    if (enemy[2].bulletSet.live == 1)
    {
        if (enemy[2].bulletSet.dx < 92 && enemy[2].bulletSet.dx > -92 && enemy[2].bulletSet.dy < 92 && enemy[2].bulletSet.dy > -92)
        {
            for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
            {
                if (bricksLoc[i][0] == 1)
                {
                    if (
                        (bricksLoc[i][2]  <= enemy[2].bulletSet.dy && bricksLoc[i][2] + 4 >= enemy[2].bulletSet.dy)
                        &&
                        (bricksLoc[i][1]  <= enemy[2].bulletSet.dx && bricksLoc[i][1] + 4 >= enemy[2].bulletSet.dx)
                        )
                    {
                        enemy[2].bulletSet.anim_bool = 1;
                        Delete_Table_x(bricksLoc[i][1], bricksLoc[i][2]);
                        Delete_Table_y(bricksLoc[i][2], bricksLoc[i][1]);
                        bricksLoc[i][0] = 0;
                        enemy[2].bulletSet.live = 0;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
            {
                if (cementLoc[i][0] == 0)
                {
                    if (
                        (cementLoc[i][2]  <= enemy[2].bulletSet.dy && cementLoc[i][2] + 4 >= enemy[2].bulletSet.dy)
                        &&
                        (cementLoc[i][1]  <= enemy[2].bulletSet.dx && cementLoc[i][1] + 4 >= enemy[2].bulletSet.dx)
                        )
                    {
                        enemy[2].bulletSet.anim_bool = 1;
                        cementLoc[i][0] = 0;
                        enemy[2].bulletSet.live = 0;
                        break;
                    }
                }
            }

            glColor3f(0.502f, 0.502f, 0.502f);
            if (enemy[2].bulletSet.angle_bull == 0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(enemy[2].bulletSet.dx - 0.4, enemy[2].bulletSet.dy);
                glVertex2d(enemy[2].bulletSet.dx + 0.4, enemy[2].bulletSet.dy);

                glVertex2d(enemy[2].bulletSet.dx - 0.4, enemy[2].bulletSet.dy + 1);
                glVertex2d(enemy[2].bulletSet.dx + 0.4, enemy[2].bulletSet.dy + 1);

                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy + 1.5);
                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy + 1.5);



                glEnd();
                enemy[2].bulletSet.dy += 1;
            }
            if (enemy[2].bulletSet.angle_bull == 180)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(enemy[2].bulletSet.dx - 0.4, enemy[2].bulletSet.dy);
                glVertex2d(enemy[2].bulletSet.dx + 0.4, enemy[2].bulletSet.dy);

                glVertex2d(enemy[2].bulletSet.dx - 0.4, enemy[2].bulletSet.dy - 1);
                glVertex2d(enemy[2].bulletSet.dx + 0.4, enemy[2].bulletSet.dy - 1);

                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy - 1.5);
                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy - 1.5);
                glEnd();
                enemy[2].bulletSet.dy -= 1;
            }
            if (enemy[2].bulletSet.angle_bull == 270)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy - 0.4);
                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy + 0.4);

                glVertex2d(enemy[2].bulletSet.dx + 1, enemy[2].bulletSet.dy - 0.4);
                glVertex2d(enemy[2].bulletSet.dx + 1, enemy[2].bulletSet.dy + 0.4);

                glVertex2d(enemy[2].bulletSet.dx + 1.5, enemy[2].bulletSet.dy);
                glVertex2d(enemy[2].bulletSet.dx + 1.5, enemy[2].bulletSet.dy);
                glEnd();

                enemy[2].bulletSet.dx += 1;
            }
            if (enemy[2].bulletSet.angle_bull == 90)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy - 0.4);
                glVertex2d(enemy[2].bulletSet.dx, enemy[2].bulletSet.dy + 0.4);

                glVertex2d(enemy[2].bulletSet.dx - 1, enemy[2].bulletSet.dy - 0.4);
                glVertex2d(enemy[2].bulletSet.dx - 1, enemy[2].bulletSet.dy + 0.4);

                glVertex2d(enemy[2].bulletSet.dx - 1.5, enemy[2].bulletSet.dy);
                glVertex2d(enemy[2].bulletSet.dx - 1.5, enemy[2].bulletSet.dy);
                glEnd();
                enemy[2].bulletSet.dx -= 1;

            }
            if (enemy[2].bulletSet.dx <= you.xPos + 5 && enemy[2].bulletSet.dx >= you.xPos - 5 && you.live == 1 && enemy[2].live == 1)
                if (enemy[2].bulletSet.dy <= you.yPos + 5 && enemy[2].bulletSet.dy >= you.yPos - 5 && you.live == 1 && enemy[2].live == 1)
                {
                    you.live = 0;
                    enemy[2].bulletSet.anim_bool = 1;
                    enemy[2].bulletSet.live = 0;
                }

        }
        else
        {
            enemy[2].bulletSet.anim_bool = 1;
            enemy[2].bulletSet.live = 0;
        }
    }
    else
    {
        enemy[2].bulletSet.dx = enemy[2].xPos;
        enemy[2].bulletSet.dy = enemy[2].yPos;
        enemy[2].bulletSet.angle_bull = you.angle;
    }
}

void draw_bullet_effects_for_enemy_three()
{
    int anim_x;
    int anim_y;

    if (enemy[2].bulletSet.time_anim == 0)
    {
        enemy[2].bulletSet.an_x = enemy[2].bulletSet.dx;
        enemy[2].bulletSet.an_y = enemy[2].bulletSet.dy;
    }

    anim_x = enemy[2].bulletSet.an_x;
    anim_y = enemy[2].bulletSet.an_y;

    GLfloat colors[10][3] = {
            {1.0f, 1.0f, 1.0f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
    };

    if (enemy[2].bulletSet.anim_bool == 1 && enemy[2].bulletSet.time_anim < BOOM_ANIMATION_FRAMES) {

        int randomIndex = rand() % 3;

        glColor3fv(colors[randomIndex]);

        glBegin(GL_POINTS);
        for (int i = 0; i < 30; ++i) {
            float angle = 2 * 3.14159516 * rand() / RAND_MAX;
            int radius = 5 * rand() / RAND_MAX;
            glVertex2f(anim_x + radius * cos(angle), anim_y + radius * sin(angle));
        }
        enemy[2].bulletSet.time_anim += 1;
        glEnd();
    }
    else if (enemy[2].bulletSet.time_anim == BOOM_ANIMATION_FRAMES)
    {
        enemy[2].bulletSet.anim_bool = 0;
        enemy[2].bulletSet.time_anim = 0;

    }
}

void move_and_shoot_enemy_three()
{
    if (enemy[2].live == 1) {
        if (enemy[2].timerMove < enemy[2].botSpeed)
            enemy[2].timerMove++;
        else {
            if (enemy[2].yPos == enemy[2].saveYPos && enemy[2].xPos == enemy[2].saveXPos) {
                enemy[2].flagAutoMove = 1 + (rand() % 4 - 1 + 1);
            }

            enemy[2].saveXPos = enemy[2].xPos;
            enemy[2].saveYPos = enemy[2].yPos;

            if (enemy[2].flagAutoMove == 1) {
                enemy[2].angle = 0;
            }
            else if (enemy[2].flagAutoMove == 2) {
                enemy[2].angle = 180;
            }
            else if (enemy[2].flagAutoMove == 3) {
                enemy[2].angle = 90;
            }
            else if (enemy[2].flagAutoMove == 4) {
                enemy[2].angle = 270;
            }

            check_block_entity_enemy_three(enemy[2].flagAutoMove);
            enemy[2].timerMove = 0;
        }

        if (enemy[2].tickToShoot < 100)
            enemy[2].tickToShoot++;
        else {

            if (enemy[2].flagAutoMove == 1) {
                enemy[2].bulletSet.angle_bull = 0;
                enemy[2].bulletSet.live = 1;
            }
            else if (enemy[2].flagAutoMove == 2) {
                enemy[2].bulletSet.angle_bull = 180;
                enemy[2].bulletSet.live = 1;
            }
            else if (enemy[2].flagAutoMove == 3) {
                enemy[2].bulletSet.angle_bull = 90;
                enemy[2].bulletSet.live = 1;
            }
            else if (enemy[2].flagAutoMove == 4) {
                enemy[2].bulletSet.angle_bull = 270;
                enemy[2].bulletSet.live = 1;
            }
            enemy[2].tickToShoot = 0;
        }
    }
}

void draw_bullet_enemy_four()
{
    if (enemy[3].bulletSet.live == 1)
    {
        if (enemy[3].bulletSet.dx < 92 && enemy[3].bulletSet.dx > -92 && enemy[3].bulletSet.dy < 92 && enemy[3].bulletSet.dy > -92)
        {
            for (int i = 0; i < NUM_OF_MINI_BRICKS_BLOCKS; i++)
            {
                if (bricksLoc[i][0] == 1)
                {
                    if (
                        (bricksLoc[i][2]  <= enemy[3].bulletSet.dy && bricksLoc[i][2] + 4 >= enemy[3].bulletSet.dy)
                        &&
                        (bricksLoc[i][1]  <= enemy[3].bulletSet.dx && bricksLoc[i][1] + 4 >= enemy[3].bulletSet.dx)
                        )
                    {
                        enemy[3].bulletSet.anim_bool = 1;
                        Delete_Table_x(bricksLoc[i][1], bricksLoc[i][2]);
                        Delete_Table_y(bricksLoc[i][2], bricksLoc[i][1]);
                        bricksLoc[i][0] = 0;
                        enemy[3].bulletSet.live = 0;
                        break;
                    }
                }
            }

            for (int i = 0; i < NUM_OF_MINI_CEMENT_BLOCKS; i++)
            {
                if (cementLoc[i][0] == 0)
                {
                    if (
                        (cementLoc[i][2]  <= enemy[3].bulletSet.dy && cementLoc[i][2] + 4 >= enemy[3].bulletSet.dy)
                        &&
                        (cementLoc[i][1]  <= enemy[3].bulletSet.dx && cementLoc[i][1] + 4 >= enemy[3].bulletSet.dx)
                        )
                    {
                        enemy[3].bulletSet.anim_bool = 1;
                        cementLoc[i][0] = 0;
                        enemy[3].bulletSet.live = 0;
                        break;
                    }
                }
            }

            glColor3f(0.502f, 0.502f, 0.502f);
            if (enemy[3].bulletSet.angle_bull == 0)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[3].bulletSet.dx - 0.4, enemy[3].bulletSet.dy);
                glVertex2f(enemy[3].bulletSet.dx + 0.4, enemy[3].bulletSet.dy);

                glVertex2f(enemy[3].bulletSet.dx - 0.4, enemy[3].bulletSet.dy + 1);
                glVertex2f(enemy[3].bulletSet.dx + 0.4, enemy[3].bulletSet.dy + 1);

                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy + 1.5);
                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy + 1.5);



                glEnd();
                enemy[3].bulletSet.dy += 1;
            }
            if (enemy[3].bulletSet.angle_bull == 180)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[3].bulletSet.dx - 0.4, enemy[3].bulletSet.dy);
                glVertex2f(enemy[3].bulletSet.dx + 0.4, enemy[3].bulletSet.dy);

                glVertex2f(enemy[3].bulletSet.dx - 0.4, enemy[3].bulletSet.dy - 1);
                glVertex2f(enemy[3].bulletSet.dx + 0.4, enemy[3].bulletSet.dy - 1);

                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy - 1.5);
                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy - 1.5);
                glEnd();
                enemy[3].bulletSet.dy -= 1;
            }
            if (enemy[3].bulletSet.angle_bull == 270)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy - 0.4);
                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy + 0.4);

                glVertex2f(enemy[3].bulletSet.dx + 1, enemy[3].bulletSet.dy - 0.4);
                glVertex2f(enemy[3].bulletSet.dx + 1, enemy[3].bulletSet.dy + 0.4);

                glVertex2f(enemy[3].bulletSet.dx + 1.5, enemy[3].bulletSet.dy);
                glVertex2f(enemy[3].bulletSet.dx + 1.5, enemy[3].bulletSet.dy);
                glEnd();

                enemy[3].bulletSet.dx += 1;
            }
            if (enemy[3].bulletSet.angle_bull == 90)
            {
                glPointSize(5.0f);
                glBegin(GL_POINTS);

                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy - 0.4);
                glVertex2f(enemy[3].bulletSet.dx, enemy[3].bulletSet.dy + 0.4);

                glVertex2f(enemy[3].bulletSet.dx - 1, enemy[3].bulletSet.dy - 0.4);
                glVertex2f(enemy[3].bulletSet.dx - 1, enemy[3].bulletSet.dy + 0.4);

                glVertex2f(enemy[3].bulletSet.dx - 1.5, enemy[3].bulletSet.dy);
                glVertex2f(enemy[3].bulletSet.dx - 1.5, enemy[3].bulletSet.dy);
                glEnd();
                enemy[3].bulletSet.dx -= 1;

            }
            if (enemy[3].bulletSet.dx <= you.xPos + 5 && enemy[3].bulletSet.dx >= you.xPos - 5 && you.live == 1 && enemy[3].live == 1)
                if (enemy[3].bulletSet.dy <= you.yPos + 5 && enemy[3].bulletSet.dy >= you.yPos - 5 && you.live == 1 && enemy[3].live == 1)
                {
                    you.live = 0;
                    enemy[3].bulletSet.anim_bool = 1;
                    enemy[3].bulletSet.live = 0;
                }

        }
        else
        {
            enemy[3].bulletSet.anim_bool = 1;
            enemy[3].bulletSet.live = 0;
        }
    }
    else
    {
        enemy[3].bulletSet.dx = enemy[3].xPos;
        enemy[3].bulletSet.dy = enemy[3].yPos;
        enemy[3].bulletSet.angle_bull = you.angle;
    }
}

void draw_bullet_effects_for_enemy_four()
{
    int anim_x;
    int anim_y;

    if (enemy[3].bulletSet.time_anim == 0)
    {
        enemy[3].bulletSet.an_x = enemy[3].bulletSet.dx;
        enemy[3].bulletSet.an_y = enemy[3].bulletSet.dy;
    }

    anim_x = enemy[3].bulletSet.an_x;
    anim_y = enemy[3].bulletSet.an_y;

    GLfloat colors[10][3] = {
            {1.0f, 1.0f, 1.0f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.525f, 0.125f, 0.437f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
            {0.878f, 0.227f, 0.020f},
    };

    if (enemy[3].bulletSet.anim_bool == 1 && enemy[3].bulletSet.time_anim < BOOM_ANIMATION_FRAMES) {

        int randomIndex = rand() % 3;

        glColor3fv(colors[randomIndex]);

        glBegin(GL_POINTS);
        for (int i = 0; i < 30; ++i) {
            float angle = 2 * 3.14159516 * rand() / RAND_MAX;
            int radius = 5 * rand() / RAND_MAX;
            glVertex2f(anim_x + radius * cos(angle), anim_y + radius * sin(angle));
        }
        enemy[3].bulletSet.time_anim += 1;
        glEnd();
    }
    else if (enemy[3].bulletSet.time_anim == BOOM_ANIMATION_FRAMES)
    {
        enemy[3].bulletSet.anim_bool = 0;
        enemy[3].bulletSet.time_anim = 0;

    }
}

void move_and_shoot_enemy_four()
{
    if (enemy[3].live == 1) {
        if (enemy[3].timerMove < enemy[3].botSpeed)
            enemy[3].timerMove++;
        else {
            if (enemy[3].yPos == enemy[3].saveYPos && enemy[3].xPos == enemy[3].saveXPos) {
                enemy[3].flagAutoMove = 1 + (rand() % 4 - 1 + 1);
            }

            enemy[3].saveXPos = enemy[3].xPos;
            enemy[3].saveYPos = enemy[3].yPos;

            if (enemy[3].flagAutoMove == 1) {
                enemy[3].angle = 0;
            }
            else if (enemy[3].flagAutoMove == 2) {
                enemy[3].angle = 180;
            }
            else if (enemy[3].flagAutoMove == 3) {
                enemy[3].angle = 90;
            }
            else if (enemy[3].flagAutoMove == 4) {
                enemy[3].angle = 270;
            }

            check_block_entity_enemy_four(enemy[3].flagAutoMove);
            enemy[3].timerMove = 0;
        }

        if (enemy[3].tickToShoot < 100)
            enemy[3].tickToShoot++;
        else {

            if (enemy[3].flagAutoMove == 1) {
                enemy[3].bulletSet.angle_bull = 0;
                enemy[3].bulletSet.live = 1;
            }
            else if (enemy[3].flagAutoMove == 2) {
                enemy[3].bulletSet.angle_bull = 180;
                enemy[3].bulletSet.live = 1;
            }
            else if (enemy[3].flagAutoMove == 3) {
                enemy[3].bulletSet.angle_bull = 90;
                enemy[3].bulletSet.live = 1;
            }
            else if (enemy[3].flagAutoMove == 4) {
                enemy[3].bulletSet.angle_bull = 270;
                enemy[3].bulletSet.live = 1;
            }


            enemy[3].tickToShoot = 0;
        }


    }
}

void lose_window() {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 300.0);

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(165, 150, 0);
    glScalef(0.1, 0.1, 0.1);


    char* text = "YOU LOSE";
    int len = strlen(text);
    for (int i = 0; i < len; ++i) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }

    glPopMatrix();
    glFlush();
}

void win_window() {

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 400.0, 0.0, 300.0);

    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(165, 150, 0);
    glScalef(0.1, 0.1, 0.1);


    char* text = "YOU WIN";
    int len = strlen(text);
    for (int i = 0; i < len; ++i) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }

    glPopMatrix();
    glFlush();
}

void update(int value) {

    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void drawText(int x, int y, char* text) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.1, 0.1, 0.1);
    int len = strlen(text);
    for (int i = 0; i < len; ++i) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }
    glPopMatrix();
}

void menu() {
    if (menuChoose == 0) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);


        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-100, 100, -100, 100, -1, 1);


        glViewport(0, 0, 1000, 1000);


        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(1.0, 1.0, 1.0);


        drawText(-40, 30, "1 EASY");        
        drawText(-40, 10, "2 HARD");        
        drawText(-40, -10, "3 ULTRA HARD");  

        glFlush();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (menuChoose == 0) {
        menu();
    }


    if (menuChoose == 1) {

        glClearColor(0.1, 0.1, 0.1, 0);

        createBrickWall();
        createCementWall();
        draw_block_zone();
        if (you.live > 0) {
            draw_global_tank_up();
            draw_bullet_global();
            draw_bullet_effects_for_global_tank();
        }
        if (enemy[0].live == 1) {
            move_and_shoot_enemy_one();
            draw_enemy_tank_up_one();
            draw_bullet_enemy_one();
            draw_bullet_effects_for_enemy_one();
        }
        if (enemy[1].live == 1) {
            move_and_shoot_enemy_two();
            draw_enemy_tank_up_two();
            draw_bullet_enemy_two();
            draw_bullet_effects_for_enemy_two();
        }
        if (you.live == 0) {
            glClearColor(1, 0, 0, 0);
            lose_window();
        }
        if (enemy[0].live + enemy[1].live == 1) {       // ИЗМЕНЕНО 
            glClearColor(0.0, 0.2, 0.13, 1.0);
            win_window();
        }
    }

    if (menuChoose == 2) {

        glClearColor(0.1, 0.1, 0.1, 0);

        createBrickWall();
        createCementWall();
        draw_block_zone();
        if (you.live > 0) {
            draw_global_tank_up();
            draw_bullet_global();
            draw_bullet_effects_for_global_tank();
        }
        if (enemy[0].live == 1) {
            move_and_shoot_enemy_one();
            draw_enemy_tank_up_one();
            draw_bullet_enemy_one();
            draw_bullet_effects_for_enemy_one();
        }
        if (enemy[1].live == 1) {
            move_and_shoot_enemy_two();
            draw_enemy_tank_up_two();
            draw_bullet_enemy_two();
            draw_bullet_effects_for_enemy_two();
        }

        if (enemy[2].live == 1) {
            move_and_shoot_enemy_three();
            draw_enemy_tank_up_three();
            draw_bullet_enemy_three();
            draw_bullet_effects_for_enemy_three();
        }

        if (you.live == 0) {
            glClearColor(1, 0, 0, 0);
            lose_window();
        }
        if (enemy[0].live + enemy[1].live + enemy[2].live == 0) {
            glClearColor(0.0, 0.2, 0.13, 1.0);
            win_window();
        }
    }

    if (menuChoose == 3) {

        glClearColor(0.1, 0.1, 0.1, 0);

        createBrickWall();
        createCementWall();
        draw_block_zone();
        if (you.live > 0) {
            draw_global_tank_up();
            draw_bullet_global();
            draw_bullet_effects_for_global_tank();
        }
        if (enemy[0].live == 1) {
            move_and_shoot_enemy_one();
            draw_enemy_tank_up_one();
            draw_bullet_enemy_one();
            draw_bullet_effects_for_enemy_one();
        }
        if (enemy[1].live == 1) {
            move_and_shoot_enemy_two();
            draw_enemy_tank_up_two();
            draw_bullet_enemy_two();
            draw_bullet_effects_for_enemy_two();
        }

        if (enemy[2].live == 1) {
            move_and_shoot_enemy_three();
            draw_enemy_tank_up_three();
            draw_bullet_enemy_three();
            draw_bullet_effects_for_enemy_three();
        }

        if (enemy[3].live == 1) {
            move_and_shoot_enemy_four();
            draw_enemy_tank_up_four();
            draw_bullet_enemy_four();
            draw_bullet_effects_for_enemy_four();
        }

        if (you.live == 0) {
            glClearColor(1, 0, 0, 0);
            lose_window();
        }
        if (enemy[0].live + enemy[1].live + enemy[2].live + enemy[3].live == 0) {
            glClearColor(0.0, 0.2, 0.13, 1.0);
            win_window();
        }
    }

    glutSwapBuffers();
}

void keyboard(char key) {

    if (menuChoose == 0) {
        Create_Table_x(); Create_Table_y();
        switch (key) {
        case '1':
            menuChoose = 1;
            enemy[0].botSpeed = 10;
            enemy[1].botSpeed = 10;
            FILE* fileCem = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_cementLocFirst.txt", "r");
            int N;
            fscanf(fileCem, "%d\n", &N);
            int a, b, c;
            int arr1[3] = { 0 };
            for (int i = 0; i < N; i++) {
                fscanf(fileCem, "%d %d %d\n", &a, &b, &c);
                cementLoc[i][0] = a;
                cementLoc[i][1] = b;
                cementLoc[i][2] = c;
                arr1[0] = a; arr1[1] = b; arr1[2] = c;
                Insert_x(arr1[1], arr1);
                Insert_y(arr1[2], arr1);
            }
            fclose(fileCem);

            FILE* fileBri = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_bricksLocFirst.txt", "r");
            fscanf(fileBri, "%d\n", &N);
            for (int i = 0; i < N; i++) {
                fscanf(fileBri, "%d %d %d\n", &a, &b, &c);
                bricksLoc[i][0] = a;
                bricksLoc[i][1] = b;
                bricksLoc[i][2] = c;
                arr1[0] = a; arr1[1] = b; arr1[2] = c;
                Insert_x(arr1[1], arr1);
                Insert_y(arr1[2], arr1);
            }
            fclose(fileBri);
            break;
        case '2':

            menuChoose = 2;
            enemy[0].botSpeed = 5;
            enemy[1].botSpeed = 5;
            enemy[2].botSpeed = 5;
            fileCem = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_cementLocSecond.txt", "r");
            fscanf(fileCem, "%d\n", &N);
            int arr2[3] = { 0 };
            for (int i = 0; i < N; i++) {
                fscanf(fileCem, "%d %d %d\n", &a, &b, &c);
                cementLoc[i][0] = a;
                cementLoc[i][1] = b;
                cementLoc[i][2] = c;
                arr2[0] = a; arr2[1] = b; arr2[2] = c;
                Insert_x(arr2[1], arr2);
                Insert_y(arr2[2], arr2);
            }
            fclose(fileCem);

            fileBri = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_bricksLocSecond.txt", "r");
            fscanf(fileBri, "%d\n", &N);
            for (int i = 0; i < N; i++) {
                fscanf(fileBri, "%d %d %d\n", &a, &b, &c);
                bricksLoc[i][0] = a;
                bricksLoc[i][1] = b;
                bricksLoc[i][2] = c;
                arr2[0] = a; arr2[1] = b; arr2[2] = c;
                Insert_x(arr2[1], arr2);
                Insert_y(arr2[2], arr2);
            }
            fclose(fileBri);

            enemy[2].xPos = 0.025;
            enemy[2].yPos = 0.8;
            enemy[2].angle = 180;

            break;
        case '3':
            menuChoose = 3;
            enemy[0].botSpeed = 3;
            enemy[1].botSpeed = 3;
            enemy[2].botSpeed = 3;
            enemy[3].botSpeed = 3;
            fileCem = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_cementLocThird.txt", "r");
            fscanf(fileCem, "%d\n", &N);
            int arr3[3] = { 0 };
            for (int i = 0; i < N; i++) {
                fscanf(fileCem, "%d %d %d\n", &a, &b, &c);
                cementLoc[i][0] = a;
                cementLoc[i][1] = b;
                cementLoc[i][2] = c;
                arr3[0] = a; arr3[1] = b; arr3[2] = c;
                Insert_x(arr3[1], arr3);
                Insert_y(arr3[2], arr3);
            }
            fclose(fileCem);

            fileBri = fopen("C:\\Users\\dimak\\Desktop\\Kursovaya\\map\\processed_bricksLocThird.txt", "r");
            fscanf(fileBri, "%d\n", &N);
            for (int i = 0; i < N; i++) {
                fscanf(fileBri, "%d %d %d\n", &a, &b, &c);
                bricksLoc[i][0] = a;
                bricksLoc[i][1] = b;
                bricksLoc[i][2] = c;
                arr3[0] = a; arr3[1] = b; arr3[2] = c;
                Insert_x(arr3[1], arr3);
                Insert_y(arr3[2], arr3);
            }
            fclose(fileBri);

            enemy[2].xPos = 25;
            enemy[2].yPos = 5;
            enemy[2].angle = 180;

            enemy[3].xPos = -25;
            enemy[3].yPos = 80;
            enemy[3].angle = 180;
            break;
        }
    }
    else {
        switch (key) {
        case 'w':
        case 'W':

            you.angle = 0;

            check_block_entity(1);

            printf("x = %d, y = %d\n", you.xPos, you.yPos);
            break;


        case 's':
        case 'S':

            you.angle = 180;


            check_block_entity(2);

            printf("x = %d, y = %d\n", you.xPos, you.yPos);
            break;


        case 'A':
        case 'a':


            you.angle = 90;

            check_block_entity(3);

            printf("x = %d, y = %d\n", you.xPos, you.yPos);

            break;
        case 'd':
        case 'D':


            you.angle = 270;


            check_block_entity(4);


            printf("x = %d, y = %d\n", you.xPos, you.yPos);

            break;
        case ' ':
            you.bulletSet.live = 1;
            break;
        }
    }
}


int main(int argc, char** argv) {
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);


    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(500, 0);

    glutCreateWindow("Tanchik");
    glClearColor(0.1, 0.1, 0.1, 0);

    startSettings();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100, 100, -100, 100, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glutTimerFunc(10, update, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc((void*)keyboard);

    glutMainLoop();
    return 0;
}