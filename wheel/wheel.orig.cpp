#include "GL\glut.h"
#include <math.h>
#include <windows.h>
#include <stdlib.h>

#define pi 3.1415926535

void *font = GLUT_STROKE_ROMAN;

int Style1[16] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},n=0,
	Style2[16] = {1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0},n2=0,
	Style3[16] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},n3=0,
	X = 200,Y = 330,R=30,Le=100,Ri=300,To=300,Bo=200,flag=0,angle=0,nP=0,nP2=0,init=0,
	X2 = 200,Y2 = 276,R2=24,begin2;
typedef double MT[3][3];

char buffer[5];

void StringPrint(char *s)
{
	if(s && strlen(s))
		while(*s)
		{
			glutStrokeCharacter(font,*s);
			s++;
		}
}

void StringOut(int x,int y,char *s)
{
	glPushMatrix();
	glTranslatef(x,y,0);
	glScalef(0.1,0.1,1);
	StringPrint(s);
	glPopMatrix();
}

struct Point
{
	int x,y;
};
Point P[500],P2[500];

void MT_DonVi(MT M)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			M[i][j] = (i==j);
}

void MT_TinhTien(double trX,double trY,MT M)
{
	MT_DonVi(M);
	M[2][0] = trX;
	M[2][1] = trY;
}

void MT_Quay(double sina,double cosa,MT M)
{
	MT_DonVi(M);
	M[0][0] = M[1][1] = cosa;
	M[0][1] = sina;
	M[1][0] = - M[0][1];
}

void Nhan_2MT(MT A,MT B,MT C)
{
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			C[i][j] = 0;
			for(int k=0;k<3;k++)
				C[i][j] += A[i][k] * B[k][j];
		}
}

void MT_Quay(Point V,int angle,MT M)
{
	double a = angle * pi / 180,sina,cosa;
	sina = sin(a); cosa = cos(a);
	MT M1,M2,M3,Tam;

	MT_TinhTien(-V.x,-V.y,M1);
	MT_Quay(sina,cosa,M2);
	MT_TinhTien(V.x,V.y,M3);

	Nhan_2MT(M1,M2,Tam);
	Nhan_2MT(Tam,M3,M);
}

Point Tim_Anh(Point P,MT M) //tim anh cua 1 diem qua mt M
{
	Point Q;
	Q.x = P.x * M[0][0] + P.y * M[1][0] + M[2][0];
	Q.y = P.x * M[0][1] + P.y * M[1][1] + M[2][1];
	return Q;
}

void Setpixel(int x,int y)
{
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void Bresenham1(int x1, int y1, int x2, int y2)
{
	int jump,n=0;
	if (x1 > x2)
	{
		Bresenham1(x2, y2, x1, y1);
		return;
	}
	int dx = x2 - x1, dy = y2 - y1;
	if (dy < 0)
	{
		jump = -1;
		dy = -dy;
	}
	else
	{
		jump = 1;
	}
	int c1 = 2 * dy, c2 = 2 * dy - 2 * dx;
	int p = 2 * dy - dx;
	int x = x1,y = y1;
	if(Style1[(n++)%16]) Setpixel(x,y);
	while (x<x2)
	{
		if (p >= 0)
		{
			p += c2;
			y += jump;
		}
		else
			p += c1;
		x++;
		if(Style1[(n++)%16]) Setpixel(x,y);
	}
}

void Bresenham2(int x1, int y1, int x2, int y2)
{
	int jump,n=0;
	if (y1 > y2)
	{
		Bresenham2(x2, y2, x1, y1);
		return;
	}
	int dx = x2 - x1, dy = y2 - y1;
	if (dx < 0)
	{
		jump = -1;
		dx = -dx;
	}
	else
	{
		jump = 1;
	}
	int c1 = 2 * dx, c2 = 2 * dx - 2 * dy;
	int p = 2 * dx - dy;
	int x = x1,y = y1;
	if(Style1[(n++)%16]) Setpixel(x,y);
	while (y<y2)
	{
		if (p >= 0)
		{
			p += c2;
			x += jump;
		}
		else
			p += c1;
		y++;
		if(Style1[(n++)%16]) Setpixel(x,y);
	}
}

void Bresenham(int xa,int ya,int xb,int yb)
{
	double m = (yb - ya) / double(xb - xa);
	if (m >= -1 && m <= 1) //lai
		Bresenham1(xa,ya,xb,yb);
	else //dung
		Bresenham2(xa,ya,xb,yb);
}

void Set8pixel(int x,int y)
{
	P[nP].x = x; P[nP++].y = y;
	P[nP].x = y; P[nP++].y = x;
	P[nP].x = y; P[nP++].y = -x;
	P[nP].x = x; P[nP++].y = -y;
	P[nP].x = -x; P[nP++].y = -y;
	P[nP].x = -y; P[nP++].y = -x;
	P[nP].x = -y; P[nP++].y = x;
	P[nP].x = -x; P[nP++].y = y;
}

void Circle(int r)
{
	int x = 0,y = r;
	int p = 1 - r;
	Set8pixel(x,y);
	while (x<y)
	{
		if (p<0)
			p += (x*2)+3;
		else
		{
			p += ((x-y)*2)+5;
			--y;
		}
		x++;
		Set8pixel(x,y);
	}
}

void Process()
{
	int count = nP / 8,i,j;

	i=0,j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i += 8;
		j++;
	}

	i=nP-7;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i -= 8;
		j++;
	}
	
	i=2;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i += 8;
		j++;
	}

	i=nP-5;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i -= 8;
		j++;
	}

	i=4;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i += 8;
		j++;
	}

	i=nP-3;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i -= 8;
		j++;
	}

	i=6;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i += 8;
		j++;
	}

	i=nP-1;j=0;
	while(j<count)
	{
		P2[nP2++] = P[i];
		i -= 8;
		j++;
	}
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Bresenham(Ri,Bo,Le,Bo);
	Bresenham(Ri,Bo,Ri,To);
	Bresenham(Le,To,Ri,To);
	Bresenham(Le,Bo,Le,To);
	nP=0;
	if(!init)
	{
		Circle(R);
		Process();
		begin2=nP;
		nP=0;
		Circle(R2);
		Process();
		init = 1;
	}
	
	for(int i=0;i<nP2;i++)
	{
		if(i<begin2)
		{
			if(Style2[(n2++)%16])
				Setpixel(P2[i].x+X,P2[i].y+Y);
		}
		else
		{
			if(Style3[(n3++)%16])
				Setpixel(P2[i].x+X2,P2[i].y+Y2);
		}
	}
	n2--;n3--;
	StringOut(0,480,"X = ");
	StringOut(40,480,itoa(X,buffer,10));
	StringOut(100,480,"Y = ");
	StringOut(140,480,itoa(Y,buffer,10));
	StringOut(180,480,"angle = ");
	StringOut(260,480,itoa(angle,buffer,10));
	glutSwapBuffers();
}

void Reshape(int w,int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,(GLdouble)w-1,0,(GLdouble)h-1);
}

void Keyboard(unsigned char key,int x,int y)
{
	if(key) exit(0);
	glutPostRedisplay();
}

void Idle()
{
	Sleep(30);
	MT M;
	Point P,Q;

	if(X==Ri && Y==To+R) flag=1;
	if(X==Ri+R && Y==Bo) flag=2;
	if(X==Le && Y==Bo-R) flag=3;
	if(X==Le-R && Y==To) flag=4;
	if(flag==1)
	{
		P.x = Ri; P.y = To;
		MT_Quay(P,angle,M);
		Q.x = Ri; Q.y = To+R;
		Q = Tim_Anh(Q,M);
		X = Q.x; Y = Q.y;
		angle-=2;
		if(angle<-90) {flag=0;angle=0;}
	}
	if(flag==2)
	{
		P.x = Ri; P.y = Bo;
		MT_Quay(P,angle,M);
		Q.x = Ri+R; Q.y = Bo;
		Q = Tim_Anh(Q,M);
		X = Q.x; Y = Q.y;
		angle-=2;
		if(angle<-90) {flag=0;angle=0;}
	}
	if(flag==3)
	{
		P.x = Le; P.y = Bo;
		MT_Quay(P,angle,M);
		Q.x = Le; Q.y = Bo-R;
		Q = Tim_Anh(Q,M);
		X = Q.x; Y = Q.y;
		angle-=2;
		if(angle<-90) {flag=0;angle=0;}
	}
	if(flag==4)
	{
		P.x = Le; P.y = To;
		MT_Quay(P,angle,M);
		Q.x = Le-R; Q.y = To;
		Q = Tim_Anh(Q,M);
		X = Q.x; Y = Q.y;
		angle-=2;
		if(angle<-90) {flag=0;angle=0;}
	}
	if(!flag)
	{
		if(Y==To+R && X<Ri+R)
			X++;
		if(X==Ri+R && Y>Bo-R)
			Y--;
		if(Y==Bo-R && X>Le-R)
			X--;
		if(X==Le-R && Y<To+R)
			Y++;
	}
	if(Y2==To-R2 && X2>Le+R2)
		X2--;
	if(X2==Le+R2 && Y2>Bo+R2)
		Y2--;
	if(Y2==Bo+R2 && X2<Ri-R2)
		X2++;
	if(X2==Ri-R2 && Y2<To-R2)
		Y2++;
	glutPostRedisplay();
}

void main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("An");
	glClearColor(0,0,0,0);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutIdleFunc(Idle);
	glutMainLoop();
}