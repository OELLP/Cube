#include <glut.h>
#include <stdio.h>
#include <string.h>

// ħ��״̬��ʾ��6 ���棬ÿ������ 3x3 ��С����
int cube[6][3][3];

// �����������Ӧ����ɫ
float colors[6][3] = {
    {1.0f, 0.0f, 0.0f}, // ��ɫ
    {0.0f, 1.0f, 0.0f}, // ��ɫ
    {0.0f, 0.0f, 1.0f}, // ��ɫ
    {1.0f, 1.0f, 0.0f}, // ��ɫ
    {1.0f, 0.5f, 0.0f}, // ��ɫ
    {1.0f, 1.0f, 1.0f}  // ��ɫ
};

// ������ת�Ƕ�
float xAngle = 20.0f, yAngle = -30.0f;

// ��ʼ��ħ��״̬��ÿ�����ö�Ӧ��������䣩
void initializeCube() {
    for (int face = 0; face < 6; face++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cube[face][i][j] = face;
            }
        }
    }
}

// ����һ��С����
void drawCubeFacelet(float color[3]) {
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(0.5f, 0.5f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.0f);
    glEnd();
}

// ����ħ����һ����
void drawFace(int faceIndex, float xOffset, float yOffset, float zOffset, float rotationX, float rotationY, float rotationZ) {
    glPushMatrix();
    glTranslatef(xOffset, yOffset, zOffset);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);//��x��
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);//��y��
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);//��z��

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

// ��������ħ��
void drawCube() {
    // ǰ�� (Z+)
    drawFace(0, 0.0f, 0.0f, 1.5f, 0.0f, 0.0f, 0.0f);
    // ���� (Z-)
    drawFace(1, 0.0f, 0.0f, -1.5f, 0.0f, 180.0f, 0.0f);
    // ���� (X-)
    drawFace(2, -1.5f, 0.0f, 0.0f, 0.0f, -90.0f, 0.0f);
    // ���� (X+)
    drawFace(3, 1.5f, 0.0f, 0.0f, 0.0f, 90.0f, 0.0f);
    // ���� (Y+)
    drawFace(4, 0.0f, 1.5f, 0.0f, -90.0f, 0.0f, 0.0f);
    // ���� (Y-)
    drawFace(5, 0.0f, -1.5f, 0.0f, 90.0f, 0.0f, 0.0f);
}

// ˳ʱ����תһ���� (90 ��)
void rotateFaceClockwise(int face) {
    int temp[3][3];
    // ���Ƶ�ǰ��
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            temp[j][2 - i] = cube[face][i][j];
        }
    }
    // ������
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cube[face][i][j] = temp[i][j];
        }
    }
}

// ������ĳ�����ڵı߿�
void rotateEdgesClockwise(int adjacent[4][3]) {
    int temp[3];
    memcpy(temp, adjacent[3], sizeof(temp));
    for (int i = 3; i > 0; i--) {
        memcpy(adjacent[i], adjacent[i - 1], sizeof(temp));
    }
    memcpy(adjacent[0], temp, sizeof(temp));
}

// ��תǰ�� (F ��)
void rotateFrontClockwise() {
    rotateFaceClockwise(0);

    int adjacent[4][3] = {
        {cube[4][2][0], cube[4][2][1], cube[4][2][2]}, // ���� (U) �ĵױ�
        {cube[3][0][0], cube[3][1][0], cube[3][2][0]}, // ���� (R) �����
        {cube[5][0][2], cube[5][0][1], cube[5][0][0]}, // ���� (D) �Ķ���
        {cube[2][2][2], cube[2][1][2], cube[2][0][2]}  // ���� (L) ���ұ�
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    memcpy(cube[4][2], adjacent[0], sizeof(adjacent[0]));
    for (int i = 0; i < 3; i++) {
        cube[3][i][0] = adjacent[1][i];
        cube[5][0][2 - i] = adjacent[2][i];
        cube[2][2 - i][2] = adjacent[3][i];
    }
}

// ��ת���� (U ��)
void rotateUpClockwise() {
    rotateFaceClockwise(4);

    int adjacent[4][3] = {
        {cube[0][0][0], cube[0][0][1], cube[0][0][2]}, // ǰ�� (F) �Ķ���
        {cube[3][0][0], cube[3][0][1], cube[3][0][2]}, // ���� (R) �Ķ���
        {cube[1][0][0], cube[1][0][1], cube[1][0][2]}, // ���� (B) �Ķ���
        {cube[2][0][0], cube[2][0][1], cube[2][0][2]}  // ���� (L) �Ķ���
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    memcpy(cube[0][0], adjacent[0], sizeof(adjacent[0]));
    memcpy(cube[3][0], adjacent[1], sizeof(adjacent[1]));
    memcpy(cube[1][0], adjacent[2], sizeof(adjacent[2]));
    memcpy(cube[2][0], adjacent[3], sizeof(adjacent[3]));
}

// ��ת���� (D ��)
void rotateDownClockwise() {
    rotateFaceClockwise(5);

    int adjacent[4][3] = {
        {cube[0][2][0], cube[0][2][1], cube[0][2][2]}, // ǰ�� (F) �ĵױ�
        {cube[2][2][0], cube[2][2][1], cube[2][2][2]}, // ���� (L) �ĵױ�
        {cube[1][2][0], cube[1][2][1], cube[1][2][2]}, // ���� (B) �ĵױ�
        {cube[3][2][0], cube[3][2][1], cube[3][2][2]}  // ���� (R) �ĵױ�
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    memcpy(cube[0][2], adjacent[0], sizeof(adjacent[0]));
    memcpy(cube[2][2], adjacent[1], sizeof(adjacent[1]));
    memcpy(cube[1][2], adjacent[2], sizeof(adjacent[2]));
    memcpy(cube[3][2], adjacent[3], sizeof(adjacent[3]));
}

// ��ת���� (L ��)
void rotateLeftClockwise() {
    rotateFaceClockwise(2);

    int adjacent[4][3] = {
        {cube[0][0][0], cube[0][1][0], cube[0][2][0]}, // ǰ�� (F) �����
        {cube[4][0][0], cube[4][1][0], cube[4][2][0]}, // ���� (U) �����
        {cube[1][2][2], cube[1][1][2], cube[1][0][2]}, // ���� (B) ���ұ�
        {cube[5][0][0], cube[5][1][0], cube[5][2][0]}  // ���� (D) �����
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    for (int i = 0; i < 3; i++) {
        cube[0][i][0] = adjacent[0][i];
        cube[4][i][0] = adjacent[1][i];
        cube[1][2 - i][2] = adjacent[2][i];
        cube[5][i][0] = adjacent[3][i];
    }
}

// ��ת���� (R ��)
void rotateRightClockwise() {
    rotateFaceClockwise(3);

    int adjacent[4][3] = {
        {cube[0][0][2], cube[0][1][2], cube[0][2][2]}, // ǰ�� (F) ���ұ�
        {cube[5][0][2], cube[5][1][2], cube[5][2][2]}, // ���� (D) ���ұ�
        {cube[1][2][0], cube[1][1][0], cube[1][0][0]}, // ���� (B) �����
        {cube[4][0][2], cube[4][1][2], cube[4][2][2]}  // ���� (U) ���ұ�
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    for (int i = 0; i < 3; i++) {
        cube[0][i][2] = adjacent[0][i];
        cube[5][i][2] = adjacent[1][i];
        cube[1][2 - i][0] = adjacent[2][i];
        cube[4][i][2] = adjacent[3][i];
    }
}

// ��ת���� (B ��)
void rotateBackClockwise() {
    rotateFaceClockwise(1);

    int adjacent[4][3] = {
        {cube[4][0][2], cube[4][0][1], cube[4][0][0]}, // ���� (U) �Ķ���
        {cube[2][0][0], cube[2][1][0], cube[2][2][0]}, // ���� (L) �����
        {cube[5][2][0], cube[5][2][1], cube[5][2][2]}, // ���� (D) �ĵױ�
        {cube[3][2][2], cube[3][1][2], cube[3][0][2]}  // ���� (R) ���ұ�
    };

    rotateEdgesClockwise(adjacent);

    // д�����ڱ߿�
    memcpy(cube[4][0], adjacent[0], sizeof(adjacent[0]));
    for (int i = 0; i < 3; i++) {
        cube[2][i][0] = adjacent[1][i];
        cube[5][2][i] = adjacent[2][i];
        cube[3][2 - i][2] = adjacent[3][i];
    }
}

// ���̽�����֧�����������ת
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'f': // ǰ�� (F)
            rotateFrontClockwise();
            break;
        case 'b': // ���� (B)
            rotateBackClockwise();
            break;
        case 'u': // ���� (U)
            rotateUpClockwise();
            break;
        case 'd': // ���� (D)
            rotateDownClockwise();
            break;
        case 'l': // ���� (L)
            rotateLeftClockwise();
            break;
        case 'r': // ���� (R)
            rotateRightClockwise();
            break;
        case 's': // �����ӽ�
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
        case 27: // ESC ��
            exit(0);
    }
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // �����ӽ�
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
    glRotatef(yAngle, 0.0f, 1.0f, 0.0f);

    // ����ħ��
    drawCube();

    glutSwapBuffers();
}

// ��ʼ�� OpenGL ����
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
