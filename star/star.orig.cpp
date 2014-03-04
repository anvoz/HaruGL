#include "GL\glut.h"
#include <math.h>

#define pi 3.1415926535

int n = 5, fill = 0;
double r = 100, xx = 250, yy = 200;

void Init()
{
	glClearColor(0,0,0,0);
}

struct Point
{
	double x,y;
};

Point Rotate(Point A,double sina, double cosa)
{
	Point B;
	B.x = A.x * cosa - A.y * sina;
	B.y = A.x * sina + A.y * cosa;
	return B;
}

double SubPoint(Point A,Point B,double y)
{
	return ((B.x - A.x) * y - B.x * A.y - A.x * B.y) / (B.y - A.y);
}

void Star(double X,double Y,double R,int n,int fill)
{
	int nn = n/2;
	Point *A, *a;
	A = new Point [n];
	a = new Point [n];
	double angle = 2 * pi / n;
	double sina = sin(angle), cosa = cos(angle);
	A[0].x = 0;
	A[0].y = R;
	for (int i=0;i<nn;i++)
		A[i+1] = Rotate(A[i],sina,cosa);
	for (int j=1,k=n-1;j<k;j++,k--)
	{
		A[k].x = -A[j].x;
		A[k].y = A[j].y;
	}
	a[0].x = SubPoint(A[0],A[n-2],A[1].y);
	a[0].y = A[1].y;
	a[1].x = -a[0].x;
	a[1].y = a[0].y;
	for (i=1;i<=nn;i++)
		a[i+1] = Rotate(a[i],sina,cosa);
	for (j=2,k=n-1;j<k;j++,k--)
	{
		a[k].x = -a[j].x;
		a[k].y = a[j].y;
	}
	if (fill%2!=0)
	{
		glBegin(GL_POLYGON);
			for (i = 0;i<n;i++)
			{
				glVertex2f(a[i].x+X,a[i].y+Y);
				glVertex2f(A[i].x+X,A[i].y+Y);
			}
		glEnd();
	}
	else
		glBegin(GL_LINE_STRIP);
			for (i = 0;i<n;i++)
			{
				glVertex2f(a[i].x+X,a[i].y+Y);
				glVertex2f(A[i].x+X,A[i].y+Y);
			}
			glVertex2f(a[0].x+X,a[0].y+Y);
		glEnd();
}

void Setpixel(double x,double y)
{
	glBegin(GL_POINTS);
		glVertex2f(x,y);
	glEnd();
}

void Line(double xa,double ya,double xb,double yb)
{
	glBegin(GL_LINES);
		glVertex2f(xa,ya);
		glVertex2f(xb,yb);
	glEnd();
}

void Set8pixel(double xc,double yc,double x,double y,int fill)
{
	if(fill%3==0)
	{
		Setpixel(xc+x,yc+y);
		Setpixel(xc+x,yc-y);
		Setpixel(xc-x,yc+y);
		Setpixel(xc-x,yc-y);
		Setpixel(xc+y,yc+x);
		Setpixel(xc+y,yc-x);
		Setpixel(xc-y,yc+x);
		Setpixel(xc-y,yc-x);
	}
	else
	{
		Line(xc+x,yc+y,xc-x,yc-y);
		Line(xc+y,yc+x,xc-y,yc-x);
		Line(xc+y,yc-x,xc-y,yc+x);
		Line(xc+x,yc-y,xc-x,yc+y);
	}
}

void Circle(double xc,double yc,double r,int fill)
{
	double x = 0,y = r;
	double p = 1 - r;
	glLineWidth(2);
	Set8pixel(xc,yc,x,y,fill);
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
		Set8pixel(xc,yc,x,y,fill);
	}
	glLineWidth(1);
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,0);
	Circle(xx,yy,r,fill);
	glColor3f(1,1,0);
	Star(xx,yy,r,n,fill);
	glutSwapBuffers();
}

void Reshape(int width,int height)
{
	glViewport(0,0,(GLsizei)width,(GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)width-1,0.0,(GLdouble)height-1);
}

void Keyboard(unsigned char key,int x,int y)
{
	if(key==100) xx++; //d
	if(key==97)  xx--; //a
	if(key==119) yy++; //w
	if(key==115) yy--; //s
	if(key==114) r++; //r
	if(key==102) r--; //f
	if(key==101) n++; //e
	if(key==113) n--; //q
	if(key==116) fill++; //t
	if(key==27) exit(0);
	glutPostRedisplay();
}

void main(int Argc,char** Argv)
{
	glutInit(&Argc,Argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500,400);
	glutInitWindowPosition(0,0);
	glutCreateWindow("An");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}