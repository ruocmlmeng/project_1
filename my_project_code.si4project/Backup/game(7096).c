#include "game.h"

int matrix_2048_1[ITEM_NUM][ITEM_NUM] = {0};
/*
	can_move: 判断是否还能移动
	返回值:
		1	还能移动
		0	不能移动,游戏结束
*/
int can_move()
{
    for (int i = 0; i < ITEM_NUM; i++)
    {
        for (int j = 0; j < ITEM_NUM; j++)
        {
            // 如果有空格，说明可以移动
            if (matrix_2048_1[i][j] == 0)
            {
                return 1;
            }

            // 向右合并
            if (j < ITEM_NUM - 1 && matrix_2048_1[i][j] == matrix_2048_1[i][j + 1])
            {
                return 1;
            }

            // 向下合并
            if (i < ITEM_NUM - 1 && matrix_2048_1[i][j] == matrix_2048_1[i + 1][j])
            {
                return 1;
            }

            // 向左合并
            if (j > 0 && matrix_2048_1[i][j] == matrix_2048_1[i][j - 1])
            {
                return 1;
            }

            // 向上合并
            if (i > 0 && matrix_2048_1[i][j] == matrix_2048_1[i - 1][j])
            {
                return 1;
            }
        }
    }
    return 0; // 无法再移动，游戏结束
}

/*
	place_random_number:生成新的随机数字（2或4）
	返回值:
			无
*/
void place_random_number()
{
    int empty_positions[ITEM_NUM * ITEM_NUM][2];
    int empty_count = 0;

    // 找到所有空位置
    for (int i = 0; i < ITEM_NUM; i++)
    {
        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (matrix_2048_1[i][j] == 0)
            {
                empty_positions[empty_count][0] = i;
                empty_positions[empty_count][1] = j;
                empty_count++;
            }
        }
    }

    if (empty_count > 0)
    {
        srand(time(NULL));
        int random_index = rand() % empty_count;
        int x = empty_positions[random_index][0];
        int y = empty_positions[random_index][1];
        int new_value = (rand() % 2 == 0) ? 2 : 4; // 随机生成2或4
        matrix_2048_1[x][y] = new_value;

        // 显示对应数字的图片
        display_number(x, y);
    }
}

/*
	check_game_over: 判断游戏是否结束
	返回值:
		1 游戏失败
		0 游戏继续
*/
int check_game_over()
{
    int grid_full = 1;

    // 检查是否有2048，游戏胜利
    for (int i = 0; i < ITEM_NUM; i++)
    {
        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (matrix_2048_1[i][j] == 2048)
            {
                printf("You win!\n");
                return 1; // 游戏胜利
            }
            // 如果还有空格，则网格未满
            if (matrix_2048_1[i][j] == 0)
            {
                grid_full = 0;
            }
        }
    }

    // 如果网格已满且没有2048，则检查是否还能移动
    if (grid_full && !can_move())
    {
        return 1; // 游戏失败
    }

    return 0; // 游戏未结束
}


/*
	Clear_BMP_Display : 清除指定位置的数字图片
	@x,y :图片的坐标
	返回值:
		无
*/
void Clear_BMP_Display(int x, int y)
{
    char pathname[32];
    sprintf(pathname, "/home/wangjun/pic/empty.bmp");
    BMP_Display(pathname, x, y);
}
 
/*
	can_merge :判断是否可以合并
	@old_x: 第一个位置的 x 坐标（行号）
	@old_y: 第一个位置的 y 坐标（列号）
	@new_x: 第二个位置的 x 坐标（行号）
	@new_y: 第二个位置的 y 坐标（列号）
	@horizontal:表示滑动方向。如果是水平滑动则为 1，否则为 0
	返回值:
	 1 可以合并
	 0 不可以合并
*/
int can_merge(int old_x, int old_y, int new_x, int new_y, int horizontal)
{
    // 计算滑动方向
    if (horizontal)
    {
        return (old_y < new_y) ? (old_y == new_y - 1) : (old_y == new_y + 1);
    }
    else
    {
        return (old_x < new_x) ? (old_x == new_x - 1) : (old_x == new_x + 1);
    }
}

/*
	display_number:显示指定位置的数字图片
	@row:表示要显示数字图片的行号(棋盘上的行位置)
	@col:表示要显示数字图片的列号(棋盘上的列位置)
	返回值:
		无

*/
void display_number(int row, int col)
{
    char pathname[32];
    sprintf(pathname, "/home/wangjun/pic/%d.bmp", matrix_2048_1[row][col]);
    printf("Loading image from: %s\n", pathname);
    int x0 = MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * col;
    int y0 = MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * row;
    BMP_Display(pathname, x0, y0);
}

// 
/*
	handle_swipe : 处理滑动方向的逻辑
	@dir:玩家在屏幕上滑动的方向(上下左右)
	返回值:
		无
*/
void handle_swipe(enum move_dir dir)
{
    int moved = 0;

    switch (dir)
    {
    case MOV_LEFT:
        printf("左滑\n");
        moved = move_left();
        break;
    case MOV_RIGHT:
        printf("右滑\n");
        moved = move_right();
        break;
    case MOV_UP:
        printf("上滑\n");
        moved = move_up();
        break;
    case MOV_DOWN:
        printf("下滑\n");
        moved = move_down();
        break;
    default:
        return;
    }

    if (moved)
    {
        place_random_number(); // 仅在有移动的情况下生成新的数字
        if (check_game_over())
        {
            printf("Game Over\n");
            // 这里可以添加游戏结束的处理逻辑
        }
    }
}

/*
	move_left : 实现左滑操作
	返回值:
		1 在此次左滑操作中，数字确实发生了移动或合并。
		0 此次左滑操作中，矩阵的状态没有发生变化，即没有数字被移动或合并。
*/
int move_left()
{
    int moved = 0;
    for (int i = 0; i < ITEM_NUM; i++)
    {
        int temp[ITEM_NUM] = {0};
        int idx = 0;
        int merged = 0;

        // First pass: merge numbers
        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (matrix_2048_1[i][j] != 0)
            {
                if (idx > 0 && temp[idx - 1] == matrix_2048_1[i][j] && !merged)
                {
                    temp[idx - 1] *= 2;
                    merged = 1;
                    moved = 1;
                }
                else
                {
                    if (temp[idx] != matrix_2048_1[i][j])
                    {
                        moved = 1;
                    }
                    temp[idx] = matrix_2048_1[i][j];
                    idx++;
                    merged = 0;
                }
            }
        }

        // Clear old images and update the matrix
        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (matrix_2048_1[i][j] != temp[j])
            {
                matrix_2048_1[i][j] = temp[j];

                // Display new image
                if (temp[j] != 0)
                {
                    display_number(i, j);
                }
                else
                {
                    Clear_BMP_Display(MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * j, MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * i);
                }
            }
        }
    }
    return moved;
}

// 向右滑动
int move_right()
{

    int moved = 0;
    for (int i = 0; i < ITEM_NUM; i++)
    {
        int temp[ITEM_NUM] = {0};
        int idx = ITEM_NUM - 1;
        int merged = 0;

        // First pass: merge numbers
        for (int j = ITEM_NUM - 1; j >= 0; j--)
        {
            if (matrix_2048_1[i][j] != 0)
            {
                if (idx < ITEM_NUM - 1 && temp[idx + 1] == matrix_2048_1[i][j] && !merged)
                {
                    temp[idx + 1] *= 2;
                    merged = 1;
                    moved = 1;
                }
                else
                {
                    if (temp[idx] != matrix_2048_1[i][j])
                    {
                        moved = 1;
                    }
                    temp[idx] = matrix_2048_1[i][j];
                    idx--;
                    merged = 0;
                }
            }
        }

        // Clear old images and update the matrix
        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (matrix_2048_1[i][j] != temp[j])
            {
                matrix_2048_1[i][j] = temp[j];

                // Display new image
                if (temp[j] != 0)
                {
                    display_number(i, j);
                }
                else
                {
                    Clear_BMP_Display(MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * j, MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * i);
                }
            }
        }
    }
    return moved;
}

// 向上滑动
int move_up()
{
    int moved = 0;
    for (int j = 0; j < ITEM_NUM; j++)
    {
        int temp[ITEM_NUM] = {0};
        int idx = 0;
        int merged = 0;

        // First pass: merge numbers
        for (int i = 0; i < ITEM_NUM; i++)
        {
            if (matrix_2048_1[i][j] != 0)
            {
                if (idx > 0 && temp[idx - 1] == matrix_2048_1[i][j] && !merged)
                {
                    temp[idx - 1] *= 2;
                    merged = 1;
                    moved = 1;
                }
                else
                {
                    if (temp[idx] != matrix_2048_1[i][j])
                    {
                        moved = 1;
                    }
                    temp[idx] = matrix_2048_1[i][j];
                    idx++;
                    merged = 0;
                }
            }
        }

        // Clear old images and update the matrix
        for (int i = 0; i < ITEM_NUM; i++)
        {
            if (matrix_2048_1[i][j] != temp[i])
            {
                matrix_2048_1[i][j] = temp[i];

                // Display new image
                if (temp[i] != 0)
                {
                    display_number(i, j);
                }
                else
                {
                    Clear_BMP_Display(MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * j, MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * i);
                }
            }
        }
    }
    return moved;
}

// 向下滑动
int move_down()
{
    int moved = 0;
    for (int j = 0; j < ITEM_NUM; j++)
    {
        int temp[ITEM_NUM] = {0};
        int idx = ITEM_NUM - 1;
        int merged = 0;

        // First pass: merge numbers
        for (int i = ITEM_NUM - 1; i >= 0; i--)
        {
            if (matrix_2048_1[i][j] != 0)
            {
                if (idx < ITEM_NUM - 1 && temp[idx + 1] == matrix_2048_1[i][j] && !merged)
                {
                    temp[idx + 1] *= 2;
                    merged = 1;
                    moved = 1;
                }
                else
                {
                    if (temp[idx] != matrix_2048_1[i][j])
                    {
                        moved = 1;
                    }
                    temp[idx] = matrix_2048_1[i][j];
                    idx--;
                    merged = 0;
                }
            }
        }

        // Clear old images and update the matrix
        for (int i = 0; i < ITEM_NUM; i++)
        {
            if (matrix_2048_1[i][j] != temp[i])
            {
                matrix_2048_1[i][j] = temp[i];

                // Display new image
                if (temp[i] != 0)
                {
                    display_number(i, j);
                }
                else
                {
                    Clear_BMP_Display(MATRIX_X0 + (ITEM_WIDTH + BLACK_LINE) * j, MATRIX_Y0 + (ITEM_HEIGHT + BLACK_LINE) * i);
                }
            }
        }
    }
    return moved;
}
