#pragma once
#include<gl/glut.h>
#include<cmath>
#define MAX_CHAR       128

// ASCII字符总共只有0到127，一共128种字符
void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void Draw_Block(int x, int y, int size) {
	glPushMatrix();
	glTranslated(x*size, y*size,0);
	glColor4f(0.0f, 0.0f, 0.0f,1.0);
	glBegin(GL_QUADS);

	glVertex2f(x-1, y+size+1);
	glVertex2f(x+1+size, y+size+1);
	glVertex2f(x+1+size, y-1);
	glVertex2f(x-1, y-1);

	//glVertex2f(x , y + size);
	//glVertex2f(x + size, y + size);
	//glVertex2f(x + size, y );
	//glVertex2f(x, y);

	glEnd();
	glFlush();
	glPopMatrix();
}

void draw_solid_circle(float x, float y, float radius)
{
	int count;
	int sections = 200;

	GLfloat TWOPI = 2.0f * 3.14159f;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	for (count = 0; count <= sections; count++)
	{
		glVertex2f(x + radius * cos(count*TWOPI / sections), y + radius * sin(count*TWOPI / sections));
	}
	glEnd();
}
