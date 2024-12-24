#include <glut.h>
#include <stdio.h>
#include <string.h>

// 魔方状态表示：6 个面，每个面是 3x3 的小方块
int cube[6][3][3];

// 定义六个面对应的颜色
float colors[6][3] = {
    {1.0f, 0.0f, 0.0f}, // 红色
    {0.0f, 1.0f, 0.0f}, // 绿色
    {0.0f, 0.0f, 1.0f}, // 蓝色
    {1.0f, 1.0f, 0.0f}, // 黄色
    {1.0f, 0.5f, 0.0f}, // 橙色
    {1.0f, 1.0f, 1.0f}  // 白色
};

// 定义旋转角度
float xAngle = 20.0f, yAngle = -30.0f;

// 初始化魔方状态（每个面用对应的数字填充）
void initializeCube() {
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cube[face][i][j] = face;
            }
        }
    }
}

// 绘制一个小方块
void drawCubeFacelet(float color[3]) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();
}

// 绘制魔方的一个面
void drawFace(int faceIndex, float xOffset, float yOffset, float zOffset, float rotationX, float rotationY, float rotationZ) {
    glPushMatrix();
    glTranslatef(xOffset, yOffset, zOffset);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);//绕x轴
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);//绕y轴
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);//绕z轴

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glPushMatrix();
            glTranslatef(-1.0f + j * 1.0f, 1.0f - i * 1.0f, 0.0f);
            drawCubeFacelet(colors[cube[faceIndex][i][j]]);
            glPopMatrix();
        }
    }
    glPopMatrix();
}

// 绘制整个魔方
void drawCube() {
    // 前面 (Z+)
    drawFace(0, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f);
    // 背面 (Z-)
    drawFace(1, 0.0f, 0.0f, -1.5f, 0.0f, 180.0f, 0.0f);
    // 左面 (X-)
    drawFace(2, -1.5f, 0.0f, 0.0f, 0.0f, -90.0f, 0.0f);
    // 右面 (X+)
    drawFace(3, 1.5f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f);
    // 上面 (Y+)
    drawFace(4, 0.0f, 1.5f, 0.0f, -90.0f, 0.0f, 0.0f);
    // 下面 (Y-)
    drawFace(5, 0.0f, -1.5f, 0.0f, 90.0f, 0.0f, 0.0f);
}

// 顺时针旋转一个面 (90 度)
void rotateFaceClockwise(int face) {
    int temp[3][3];
    // 复制当前面
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[j][2 - i] = cube[face][i][j];
        }
    }
    // 更新面
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cube[face][i][j] = temp[i][j];
        }
    }
}

// 更新与某面相邻的边块
void rotateEdgesClockwise(int adjacent[4][3]) {
    int temp[3];
    memcpy(temp, adjacent[3], sizeof(temp));
    for (int i = 3; i > 0; i--) {
        memcpy(adjacent[i], adjacent[i - 1], sizeof(temp));
    }
    memcpy(adjacent[0], temp, sizeof(temp));
}

// 旋转前面 (F 面)
void rotateFrontClockwise() {
    rotateFaceClockwise(0);

    int adjacent[4][3] = {
        {cube[4][2][0], cube[4][2][1], cube[4][2][2]}, // 上面 (U) 的底边
        {cube[3][0][0], cube[3][1][0], cube[3][2][0]}, // 右面 (R) 的左边
        {cube[5][0][2], cube[5][0][1], cube[5][0][0]}, // 下面 (D) 的顶边
        {cube[2][2][2], cube[2][1][2], cube[2][0][2]}  // 左面 (L) 的右边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    memcpy(cube[4][2], adjacent[0], sizeof(adjacent[0]));
    for (int i = 0; i < 3; i++) {
        cube[3][i][0] = adjacent[1][i];
        cube[5][0][2 - i] = adjacent[2][i];
        cube[2][2 - i][2] = adjacent[3][i];
    }
}

// 旋转上面 (U 面)
void rotateUpClockwise() {
    rotateFaceClockwise(4);

    int adjacent[4][3] = {
        {cube[0][0][0], cube[0][0][1], cube[0][0][2]}, // 前面 (F) 的顶边
        {cube[3][0][0], cube[3][0][1], cube[3][0][2]}, // 右面 (R) 的顶边
        {cube[1][0][0], cube[1][0][1], cube[1][0][2]}, // 背面 (B) 的顶边
        {cube[2][0][0], cube[2][0][1], cube[2][0][2]}  // 左面 (L) 的顶边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    memcpy(cube[0][0], adjacent[0], sizeof(adjacent[0]));
    memcpy(cube[3][0], adjacent[1], sizeof(adjacent[1]));
    memcpy(cube[1][0], adjacent[2], sizeof(adjacent[2]));
    memcpy(cube[2][0], adjacent[3], sizeof(adjacent[3]));
}

// 旋转下面 (D 面)
void rotateDownClockwise() {
    rotateFaceClockwise(5);

    int adjacent[4][3] = {
        {cube[0][2][0], cube[0][2][1], cube[0][2][2]}, // 前面 (F) 的底边
        {cube[2][2][0], cube[2][2][1], cube[2][2][2]}, // 左面 (L) 的底边
        {cube[1][2][0], cube[1][2][1], cube[1][2][2]}, // 背面 (B) 的底边
        {cube[3][2][0], cube[3][2][1], cube[3][2][2]}  // 右面 (R) 的底边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    memcpy(cube[0][2], adjacent[0], sizeof(adjacent[0]));
    memcpy(cube[2][2], adjacent[1], sizeof(adjacent[1]));
    memcpy(cube[1][2], adjacent[2], sizeof(adjacent[2]));
    memcpy(cube[3][2], adjacent[3], sizeof(adjacent[3]));
}

// 旋转左面 (L 面)
void rotateLeftClockwise() {
    rotateFaceClockwise(2);

    int adjacent[4][3] = {
        {cube[0][0][0], cube[0][1][0], cube[0][2][0]}, // 前面 (F) 的左边
        {cube[4][0][0], cube[4][1][0], cube[4][2][0]}, // 上面 (U) 的左边
        {cube[1][2][2], cube[1][1][2], cube[1][0][2]}, // 背面 (B) 的右边
        {cube[5][0][0], cube[5][1][0], cube[5][2][0]}  // 下面 (D) 的左边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    for (int i = 0; i < 3; i++) {
        cube[0][i][0] = adjacent[0][i];
        cube[4][i][0] = adjacent[1][i];
        cube[1][2 - i][2] = adjacent[2][i];
        cube[5][i][0] = adjacent[3][i];
    }
}

// 旋转右面 (R 面)
void rotateRightClockwise() {
    rotateFaceClockwise(3);

    int adjacent[4][3] = {
        {cube[0][0][2], cube[0][1][2], cube[0][2][2]}, // 前面 (F) 的右边
        {cube[5][0][2], cube[5][1][2], cube[5][2][2]}, // 下面 (D) 的右边
        {cube[1][2][0], cube[1][1][0], cube[1][0][0]}, // 背面 (B) 的左边
        {cube[4][0][2], cube[4][1][2], cube[4][2][2]}  // 上面 (U) 的右边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    for (int i = 0; i < 3; i++) {
        cube[0][i][2] = adjacent[0][i];
        cube[5][i][2] = adjacent[1][i];
        cube[1][2 - i][0] = adjacent[2][i];
        cube[4][i][2] = adjacent[3][i];
    }
}

// 旋转背面 (B 面)
void rotateBackClockwise() {
    rotateFaceClockwise(1);

    int adjacent[4][3] = {
        {cube[4][0][2], cube[4][0][1], cube[4][0][0]}, // 上面 (U) 的顶边
        {cube[2][0][0], cube[2][1][0], cube[2][2][0]}, // 左面 (L) 的左边
        {cube[5][2][0], cube[5][2][1], cube[5][2][2]}, // 下面 (D) 的底边
        {cube[3][2][2], cube[3][1][2], cube[3][0][2]}  // 右面 (R) 的右边
    };

    rotateEdgesClockwise(adjacent);

    // 写回相邻边块
    memcpy(cube[4][0], adjacent[0], sizeof(adjacent[0]));
    for (int i = 0; i < 3; i++) {
        cube[2][i][0] = adjacent[1][i];
        cube[5][2][i] = adjacent[2][i];
        cube[3][2 - i][2] = adjacent[3][i];
    }
}

// 键盘交互：支持所有面的旋转
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'f': // 前面 (F)
            rotateFrontClockwise();
            break;
        case 'b': // 背面 (B)
            rotateBackClockwise();
            break;
        case 'u': // 上面 (U)
            rotateUpClockwise();
            break;
        case 'd': // 下面 (D)
            rotateDownClockwise();
            break;
        case 'l': // 左面 (L)
            rotateLeftClockwise();
            break;
        case 'r': // 右面 (R)
            rotateRightClockwise();
            break;
        case 's': // 调整视角
            xAngle -= 5.0f;
            break;
        case 'x':
            xAngle += 5.0f;
            break;
        case 'z':
            yAngle -= 5.0f;
            break;
        case 'c':
            yAngle += 5.0f;
            break;
        case 27: // ESC 键
            exit(0);
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 设置视角
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
    glRotatef(yAngle, 0.0f, 1.0f, 0.0f);

    // 绘制魔方
    drawCube();

    glutSwapBuffers();
}

// 初始化 OpenGL 设置
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Cube");

    initOpenGL();
    initializeCube();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
