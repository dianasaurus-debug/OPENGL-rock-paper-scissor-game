/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#ifndef M_PI
#define PI 3.141592653589793
#endif
#include<iostream>
using namespace std;
class invoke_srand {
public:
     invoke_srand()
     {
         srand(time(0));
     }
};
static invoke_srand do_invoke_srand;
float xr = 0, yr = 0;
int acak = rand()%3 + 1;
float stick=0;
float ktick=0;
float sscale=4;
float kscale=4;
int over=0;
float sleep_time = 0.8;
bool stop = false;
bool isUserWin = false, isComWin = false, sama=false;
bool chooseRock = false, choosePaper = false, chooseScissor = false, udahMulai=false, disInstruction=false;
typedef struct
{
    float x;
    float y;
} point2D_t;
typedef struct {
	float v[3];
} vector2D_t;
typedef struct
{
    float r;
    float g;
    float b;
} color_t;
typedef struct {
    point2D_t p,p0;
    float d,s;
} obyek_t;
typedef struct {
	float m[3][3];
} matrix2D_t;

////////////// OpenGL drawShape Functions ver 1 /////////////////

void setColor(float red,float green,float blue)
{
    glColor3f(red, green, blue);
}

void setColorRGB(float red,float green,float blue)
{
    glColor3ub(red, green, blue);
}
void setColor(color_t col)
{
    glColor3f(col.r, col.g, col.b);
}

void drawDot(float x,float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void makeeyes(float x,float y)
{
    glPointSize(3);
    glColor3f(0,0,0);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawLine(point2D_t p1,point2D_t p2)
{
    drawLine(p1.x,p1.y,p2.x,p2.y);
}

//n: number of points
void drawPolyline(point2D_t pnt[],int n)
{
    int i;
    glBegin(GL_LINE_STRIP);
    for (i=0; i<n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}
void drawLines(point2D_t pnt[],int n)
{
    int i;
    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
    for (i=0; i<n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}

int i;
const double PI = 3.14;
void lingkaran(int jari2, int jumlah_titik, int x_tengah, int y_tengah) {
    glBegin(GL_POLYGON);
    for (i=0;i<=360;i++){
    float sudut=i*(2*PI/jumlah_titik);
    float x=x_tengah+jari2*cos(sudut);
    float y=y_tengah+jari2*sin(sudut);
    glVertex2f(x,y);
    }
    glEnd();
 }

//n: number of vertices
void drawPolygon(point2D_t pnt[],int n)
{
    int i;
    glBegin(GL_LINE_LOOP);
    for (i=0; i<n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}

void drawBitmapText(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);

    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}
void drawBitmapTextSmall(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);

    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}
void drawRect(point2D_t pnt[])
{
    int i;
    int n=4;
    glBegin(GL_LINE_LOOP);
    for (i=0; i<n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
}
// The function fillPolygon can fills only convex polygons
//n: number of vertices
void fillPolygon(point2D_t pnt[],int n,color_t color)
{
    int i;
    setColor(color);
    glBegin(GL_POLYGON);
    for (i=0; i<n; i++)
    {
        glVertex2f(pnt[i].x, pnt[i].y);
    }
    glEnd();
};
// The function gradatePolygon can fills only convex polygons
// The vertices will be painted with corresponding given colors.
// The points inside the polygon will be painted with the mixed color.
//n: number of vertices
void gradatePolygon(point2D_t p[], int n,color_t col[])
{
    glBegin(GL_POLYGON);
    for(int i=0; i<n; i++)
    {
        setColor(col[i].r,col[i].g,col[i].b);
        glVertex2f(p[i].x,p[i].y);
    }
    glEnd();
}
point2D_t interpolate(point2D_t a, point2D_t b, float r)
{
    // 0.0<=r<=1.0  AC:CB=r:(1-r)
    if (r<0.0) r=0.0;
    if (1.0<r) r=1.0;
    point2D_t c;
    float r1=1.0-r;
    c.x=r1*a.x+r*b.x;
    c.y=r1*a.y+r*b.y;
    return c;
}
matrix2D_t createIdentity(void)
{
	matrix2D_t u;
	int i,j;
	for (i=0;i<3;i++) {
		for(j=0;j<3;j++) u.m[i][j]=0.;
		u.m[i][i]=1.;
	}
	return u;
}

matrix2D_t translationMTX(float dx,float dy)
{
	matrix2D_t trans=createIdentity();
	trans.m[0][2]=dx;
	trans.m[1][2]=dy;
	return trans;
}

matrix2D_t rotationMTX(float theta)
{
	matrix2D_t rotate=createIdentity();
	float cs=cos(theta);
	float sn=sin(theta);
	rotate.m[0][0]=cs; rotate.m[0][1]=-sn;
	rotate.m[1][0]=sn; rotate.m[1][1]=cs;
	return rotate;
}

matrix2D_t scalingMTX(float factorx,float factory)
{
	matrix2D_t scale=createIdentity();
	scale.m[0][0]=factorx;
	scale.m[1][1]=factory;
	return scale;
}

point2D_t Vector2Point(vector2D_t vec)
{
	point2D_t pnt;
	pnt.x=vec.v[0];
	pnt.y=vec.v[1];
	return pnt;
}


matrix2D_t operator * (matrix2D_t a, matrix2D_t b)
{
	matrix2D_t c;//c=a*b
	int i,j,k;
	for (i=0;i<3;i++) for (j=0;j<3;j++) {
		c.m[i][j]=0;
		for (k=0;k<3;k++) c.m[i][j]+=a.m[i][k]*b.m[k][j];
	}
	return c;
}

vector2D_t operator * (matrix2D_t a, vector2D_t b)
{
	vector2D_t c;//c=a*b
	int i,j;
	for (i=0;i<3;i++) {
		c.v[i]=0;
		for (j=0;j<3;j++) c.v[i]+=a.m[i][j]*b.v[j];
	}
	return c;
}

vector2D_t Point2Vector(point2D_t pnt)
{
	vector2D_t vec;
	vec.v[0]=pnt.x;
	vec.v[1]=pnt.y;
	vec.v[2]=1.;
	return vec;
}


void rock(void)
{
    static int tick=0;
    int disp=(tick/700)%7;
    float rtick=(tick%700)/80.;
    point2D_t tangan1[255] =
    {
        {0, 237.46}, {32.25, 246.84},{54.97, 253.76}, {74.73, 259.19},{90.54, 270.06}, {105.36, 272.04},{118.2, 266.11}, {135.99, 265.61},
        {145.87, 262.65}, {148.34, 254.25},{143.89, 246.35}, {151.79, 241.9},{151.79, 231.53}, {144.88, 226.59},{145.87, 214.73}, {138.95, 208.8},
        {130.06, 208.8}, {129.56, 200.9},{119.68, 193.99}, {79.67, 204.85},{55.96, 199.42}, {34.22, 194.48},{0, 186.58},
    };
    point2D_t tangan2[255] =
    {
        {0, 245.85}, {24.34, 257.71},{48.06, 269.07}, {54.97, 272.04},{66.83, 284.88}, {79.49, 286.36},{110.79, 289.33}, {120.67, 288.83},
        {120.67, 288.83}, {127.09, 282.9},{124.13, 273.02}, {132.03, 269.57},{135, 260.18}, {129.07, 252.28},{133.02, 243.38}, {128.08, 234.99},
        {116.72, 232.02}, {118.2, 224.61},{112.27, 217.7}, {92.02, 216.71},{69.79, 219.67}, {45.59, 207.82},{0,188.55}
    };
    point2D_t tangan3[255] =
    {
        {0, 250.79}, {63.86, 243.38},{127.5, 236.96}, {142.9, 241.9},{156.73, 242.89}, {175.01, 230.54},{188.84, 224.61}, {205.15, 215.23},
        {206.13, 203.37}, {197.24, 196.46},{204.16, 186.08}, {198.23, 176.7},{188.84, 172.74}, {188.84, 161.88},{178.47, 157.43}, {166.12, 161.38},
        {163.16, 152.98}, {149.32, 150.02},{122.65, 166.82}, {108.82, 177.19},{62.38, 183.61}, {15.95, 190.53},{0, 192.5}
    };
    point2D_t shape[255];
    color_t color = {0.922, 0.89, 0.75};
    int i;
    cout << disp;
    switch(disp)
    {
    case 0:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 1:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 2:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 3:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 4:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 5:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan3[i],rtick);
        }
        fillPolygon(shape, 23, color);
        usleep(sleep_time);
        break;
    case 6:
        stop=true;
        break;
    default:
        break;
    }
    tick++;
}
void scissor(void)
{
    static int tick=0;
    int disp=(tick/700)%7;
    float rtick=(tick%700)/80.;
    point2D_t tangan1[255] =
    {
        {0, 237.46}, {32.25, 246.84},{54.97, 253.76}, {74.73, 259.19},{90.54, 270.06}, {105.36, 272.04},{118.2, 266.11}, {135.99, 265.61},
        {145.87, 262.65}, {148.34, 254.25},{143.89, 246.35}, {151.79, 241.9},{151.79, 231.53}, {144.88, 226.59},{145.87, 214.73}, {138.95, 208.8},
        {130.06, 208.8}, {129.56, 200.9},{119.68, 193.99}, {79.67, 204.85},{55.96, 199.42}, {34.22, 194.48},{0, 186.58},
    };
    point2D_t tangan2[255] =
    {
        {0, 245.85}, {24.34, 257.71},{48.06, 269.07}, {54.97, 272.04},{66.83, 284.88}, {79.49, 286.36},{110.79, 289.33}, {120.67, 288.83},
        {120.67, 288.83}, {127.09, 282.9},{124.13, 273.02}, {132.03, 269.57},{135, 260.18}, {129.07, 252.28},{133.02, 243.38}, {128.08, 234.99},
        {116.72, 232.02}, {118.2, 224.61},{112.27, 217.7}, {92.02, 216.71},{69.79, 219.67}, {45.59, 207.82},{0,188.55}
    };
    point2D_t tangan3[255] =
    {
        {0, 261.85}, {102.6, 290.85}, {125.35, 305.07}, {136.72, 307.35}, {157.19, 300.52}, {229.99, 308.48}, {241.36, 309.62}, {250, 300},
        {248.75, 286.3}, {171.98, 273.79}, {222.02, 274.93}, {236.81, 271.52}, {240.79, 256.16}, {229.42, 245.93}, {191.88, 245.93}, {193.02, 234.55},
        {183.92, 226.59}, {171.98, 226.59}, {170.27, 216.35}, {159.47, 210.1}, {106.58, 220.33}, {67.34, 212.37}, {0, 193.61}
    };
    point2D_t shape[255];
    color_t color = {0.922, 0.89, 0.75};
    int i;
    switch(disp)
    {
    case 0:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 1:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 2:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 3:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 4:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 5:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan3[i],rtick);
        }
        fillPolygon(shape, 23, color);
        usleep(sleep_time);
        break;
    case 6:
        stop=true;
        break;
    default:
        break;
    }
    tick++;
    //fillPolygon(tangan,37, color);
}
void paper(void)
{
    static int tick=0;
    int disp=(tick/700)%7;
    float rtick=(tick%700)/80.;
    point2D_t tangan1[255] =
    {
        {0, 237.46}, {32.25, 246.84},{54.97, 253.76}, {74.73, 259.19},{90.54, 270.06}, {105.36, 272.04},{118.2, 266.11}, {135.99, 265.61},
        {145.87, 262.65}, {148.34, 254.25},{143.89, 246.35}, {151.79, 241.9},{151.79, 231.53}, {144.88, 226.59},{145.87, 214.73}, {138.95, 208.8},
        {130.06, 208.8}, {129.56, 200.9},{119.68, 193.99}, {79.67, 204.85},{55.96, 199.42}, {34.22, 194.48},{0, 186.58},
    };
    point2D_t tangan2[255] =
    {
        {0, 245.85}, {24.34, 257.71},{48.06, 269.07}, {54.97, 272.04},{66.83, 284.88}, {79.49, 286.36},{110.79, 289.33}, {120.67, 288.83},
        {120.67, 288.83}, {127.09, 282.9},{124.13, 273.02}, {132.03, 269.57},{135, 260.18}, {129.07, 252.28},{133.02, 243.38}, {128.08, 234.99},
        {116.72, 232.02}, {118.2, 224.61},{112.27, 217.7}, {92.02, 216.71},{69.79, 219.67}, {45.59, 207.82},{0,188.55}
    };
    point2D_t tangan3[255] =
    {
        {0, 272.7}, {73.45, 275.53}, {156.63, 278.92}, {211.52, 281.19}, {250, 282.88}, {285.08, 282.32}, {293.01, 272.7}, {292.44, 260.82},
        {281.69, 254.03}, {206.43, 252.89}, {195.11, 241.01}, {171.91, 228.56}, {148.71, 224.04}, {163.99, 208.76}, {168.52, 195.18}, {161.73, 184.99},
        {149.28, 183.86}, {127.78, 201.97}, {96.65, 221.21}, {72.89, 218.38}, {41.77, 215.55}, {12.91, 212.15}, {0, 211.02}
    };
    point2D_t shape[255];
    color_t color = {0.922, 0.89, 0.75};
    int i;
    switch(disp)
    {
    case 0:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 1:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 2:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 3:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan1[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 4:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan1[i],tangan2[i],rtick);
        }
        fillPolygon(shape, 23, color);
        break;
    case 5:
        for(i=0; i<23; i++)
        {
            shape[i]=interpolate(tangan2[i],tangan3[i],rtick);
        }
        fillPolygon(shape, 23, color);
        usleep(sleep_time);
        break;
    case 6:
        stop=true;
        break;
    default:
        break;
    }
    tick++;
    //fillPolygon(tangan,37, color);
}


void tombolInstruksi(void){
    point2D_t enterRects[4]= {{200,200}, {200, 170}, {250, 170}, {250, 200}};
    point2D_t enterRects2[4]= {{200,200}, {200, 170}, {250, 170}, {250, 200}};
    point2D_t cancelRects[4]= {{200,160}, {200, 130}, {250, 130}, {250, 160}};
    point2D_t cancelRects2[4]= {{200,165}, {200, 135}, {250, 135}, {250, 165}};
    point2D_t otherRects[4]= {{200,240}, {200, 210}, {250, 210}, {250, 240}};
    point2D_t otherRects2[4]= {{200,245}, {200, 215}, {250, 215}, {250, 245}};
    color_t color = {0,0,0};
    color_t color2 = {1,1,1};
    color_t ccolor = {1,1,1};
    color_t ccolor2 = {1,0,0};
    color_t lcolor = {0,1,0};
    color_t lcolor2 = {1,0,0};
    glPushMatrix();
    glTranslatef(3, 0, 0);
    drawPolygon(otherRects, 4);
    fillPolygon(otherRects2, 4, ccolor);
    glPopMatrix();
    drawPolygon(otherRects, 4);
    fillPolygon(otherRects, 4, ccolor2);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Right",210,217,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Tekan ",150,217,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Untuk Memilih Scissor! ",260,217,0);

    glPushMatrix();
    glTranslatef(3, -3, 0);
    drawPolygon(enterRects2, 4);
    fillPolygon(enterRects2, 4, color);
    glPopMatrix();
    drawPolygon(enterRects, 4);
    fillPolygon(enterRects, 4, color2);
    glColor3ub(0,0,0);
    drawBitmapTextSmall("Up",206,177,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Tekan ",150,177,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Untuk Memilih Rock! ",260,177,0);
    glPushMatrix();
    glTranslatef(3, -7, 0);
    drawPolygon(cancelRects2, 4);
    fillPolygon(cancelRects2, 4, ccolor);
    glPopMatrix();
    drawPolygon(cancelRects, 4);
    fillPolygon(cancelRects, 4, ccolor2);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Down",210,140,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Tekan ",150,140,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Untuk Memilih Paper! ",260,140,0);

}
void tombol(void){
    point2D_t enterRects[4]= {{200,100}, {200, 70}, {250, 70}, {250, 100}};
    point2D_t enterRects2[4]= {{200,100}, {200, 70}, {250, 70}, {250, 100}};
    point2D_t cancelRects[4]= {{200,60}, {200, 30}, {250, 30}, {250, 60}};
    point2D_t cancelRects2[4]= {{200,65}, {200, 35}, {250, 35}, {250, 65}};
    color_t color = {0,0,0};
    color_t color2 = {1,1,1};
    color_t ccolor = {1,1,1};
    color_t ccolor2 = {1,0,0};

    glPushMatrix();
    glTranslatef(3, -3, 0);
    drawPolygon(enterRects2, 4);
    fillPolygon(enterRects2, 4, color);
    glPopMatrix();
    drawPolygon(enterRects, 4);
    fillPolygon(enterRects, 4, color2);
    glColor3ub(0,0,0);
    drawBitmapTextSmall("Enter",206,77,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Tekan ",150,77,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Untuk Memulai! ",260,77,0);
    glPushMatrix();
    glTranslatef(3, -7, 0);
    drawPolygon(cancelRects2, 4);
    fillPolygon(cancelRects2, 4, ccolor);
    glPopMatrix();
    drawPolygon(cancelRects, 4);
    fillPolygon(cancelRects, 4, ccolor2);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Esc",210,40,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Tekan ",150,40,0);
    glColor3ub(255,255,255);
    drawBitmapTextSmall("Untuk Keluar! ",260,40,0);
}
void menu(void){
    glPushMatrix();
    glTranslatef(-45, 0, 0);
    tombol();
    glPopMatrix();

}

void awan(void){
    glColor3f(1,1,1);
           lingkaran(20, 200, 40, 218);
           glColor3f(1,1,1);
           lingkaran(25, 200, 82, 234);
           glColor3f(1,1,1);
           lingkaran(19, 200, 120, 220);
           glColor3f(1,1,1);
           lingkaran(20, 200, 82, 209);
           glColor3f(1,1,1);
           lingkaran(15,200, 60, 233);
           glColor3f(1,1,1);
           lingkaran(13, 200, 55, 209);
           glColor3f(1,1,1);
           lingkaran(14, 200, 107, 228);
           glColor3f(1,1,1);
           lingkaran(14, 200, 107, 210);
}


void lingkaran2(int jari2, int jumlah_titik, int x_tengah, int y_tengah, bool base) {
     const double PI = 3.141592653589793;
     static float tick=0;
     glBegin(GL_POLYGON);
     for (int i=0;i<=360;i++){
         float sudut=i*(2*PI/jumlah_titik);
         float x=x_tengah+jari2*cos(sudut)+tick;
         float y=y_tengah+jari2*sin(sudut)+tick;
         if(base==true){
             if(i%2==0){
                glVertex2f(x,y);
             }
             else if(i%2!=0){
                glVertex2f(x+10, y+10);
             }
         }
         else {
            glVertex2f(x,y);
         }
     }
     tick+=0.05;
     if(tick>=2200){
        tick=0;
     }
     glEnd();
}

void flying(void) {
    glColor3f(0, 0, 0);
    lingkaran2(100, 100, 30, 30, true);
    glColor3f(1, 1, 1);
    lingkaran2(30, 30, 0, 50, false);
    glColor3f(1, 1, 1);
    lingkaran2(30, 30, 70, 50, false);
    glColor3f(0, 0, 0);
    lingkaran2(10, 10, 10, 50, false);
    glColor3f(0, 0, 0);
    lingkaran2(10, 10, 60, 50, false);
}

void grass(void){
    static obyek_t rumput[300];
    static int tick=0;
    int i;
    if(tick==0){
    for(i=0;i<300;i++){
        rumput[i].p.x=rand()%500;
        rumput[i].p.y=40+rand()%10;
        rumput[i].d=rand()%8+2;
        rumput[i].s=float(rand()%100+1)/1000;
        rumput[i].p0=rumput[i].p;
    }
    }
    tick++;
    float xa,xb;
    for(i=0;i<300;i++){
        rumput[i].p.x+=rumput[i].s;
        xa=rumput[i].p0.x-0.5*rumput[i].d;
        xb=rumput[i].p0.x+0.5*rumput[i].d;
        if((rumput[i].p.x<xa) || (rumput[i].p.x>xb))
        rumput[i].s=-rumput[i].s;
    }
    point2D_t p[3];
    color_t cp[3]={{0.1,1.0,0.3},{0.,0.2,0.},{0.,0.2,0.}};
    for(i=0;i<300;i++){
        p[0].x=rumput[i].p.x;
        p[0].y=rumput[i].p.y;
        p[1].x=rumput[i].p0.x-rumput[i].d;
        p[1].y=0;
        p[2].x=rumput[i].p0.x+rumput[i].d;
        p[2].y=0;
        gradatePolygon(p,3,cp);
    }
}
void bg(void){
    point2D_t langit[4]= {{0,0},{600,0},{600,400},{0,400}};
    color_t wLangit= {0.552, 0.713, 0.968};
    fillPolygon(langit,4,wLangit);
}
void belumMulai(void)
{
    glClear( GL_COLOR_BUFFER_BIT);
    point2D_t langit[4]= {{0,0},{600,0},{600,400},{0,400}};
    color_t wLangit= {0.74901960784,0.61176470588,0.30588235294};
    color_t wDinding= {1,1,1};
    fillPolygon(langit,4,wLangit);
        glPushMatrix();
        glTranslatef(50, 130, 0);
        awan();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(200, 80, 0);
        awan();
        glPopMatrix();
    point2D_t rectPoints[4]= {{200,200}, {200, 180}, {260, 180}, {260, 200}};
    point2D_t rectPoints2[4]= {{210,200}, {210, 180}, {270, 180}, {270, 200}};

    color_t color = {0, 0.8, 0};
    //make brick
    for (int x = -70; x <= 130; x += 60)
    {
        for (int y = 120; y >= 0; y -= 20)
        {
            if(y%40==0){
                glPushMatrix();
                glTranslatef(x, y, 0);
                glColor3f(1,0,0);
                drawRect(rectPoints);
                glPopMatrix();
            }
            else {
                if(x!=180){
                    glPushMatrix();
                    glTranslatef(x, y, 0);
                    glColor3f(1,0,0);
                    drawRect(rectPoints2);
                    glPopMatrix();
                }

            }

        }
    }
    glColor3ub(255,255,255);
    drawBitmapText("Rock, Paper Scissors Game",140,250,0);
    glPushMatrix();
    glTranslatef(50, 70, 0);
    menu();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 0);
    grass();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(50, 50,0);
    glScalef(0.4,0.4,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(90, 90,0);
    glScalef(0.3,0.3,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(100, 100,0);
    glScalef(0.2,0.2,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(150, -30,0);
    glScalef(0.6,0.6,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(130, -10,0);
    glScalef(0.4,0.4,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-20, -50,0);
    glScalef(0.4,0.4,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-40, -60,0);
    glScalef(0.3,0.3,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-200, 50,0);
    glScalef(0.3,0.3,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-250, 90,0);
    glScalef(0.2,0.2,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-180, -50,0);
    glScalef(0.2,0.2,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-160, -40,0);
    glScalef(0.3,0.3,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-230, -80,0);
    glScalef(0.1,0.1,0);
    flying();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-220, -50,0);
    glScalef(0.3,0.3,0);
    flying();
    glPopMatrix();
}
void rock2(){
    glPushMatrix();
    glTranslatef(500, 400, 0);
    glRotatef(180, 0, 0, 1);
    rock();
    glPopMatrix();
}
void paper2(){
    glPushMatrix();
    glTranslatef(500, 400, 0);
    glRotatef(180, 0, 0, 1);
    paper();
    glPopMatrix();
}
void scissor2(){
    glPushMatrix();
    glTranslatef(500, 400, 0);
    glRotatef(180, 0, 0, 1);
    scissor();
    glPopMatrix();
}
void instruction(){
    point2D_t langit[4]= {{0,0},{600,0},{600,400},{0,400}};
    color_t wLangit= {0.83529411764,0.71764705882,0.54509803921};
    fillPolygon(langit,4,wLangit);
    tombolInstruksi();

}

void bubblechat(void){
    point2D_t bubble[255] =
    {
	{187.23, 132.57}, {196.25, 140.69}, {180, 140}, {170.99, 145.65},
	{162.87, 149.71}, {163.32, 166.86}, {175.95, 174.53}, {257.61, 175.88},
	{272.04, 166.86}, {271.14, 148.81}, {259.41, 144.3}, {204.37, 142.49},
	{201.21, 135.73}, {187.23, 132.57}
};
glColor3f(0,0,0);
drawPolygon(bubble, 13);
}


void losechar(void){
    point2D_t bearwinbody[255] =
    {
        {33.89, 69.62}, {31.63, 88.58}, {34.34, 104.38}, {40.21, 124.24}, {50.14, 146.37}, {100.82, 158.1}, {40.21, 168.94}, {45.18, 180.23},
        {59.17, 183.39}, {67.3, 195.58}, {84.45, 198.74}, {97.1, 194.67}, {104.32, 187.45}, {113.35, 166.23}, {123.73, 134.63}, {130.5, 101.22},
        {131.41, 79.55}, {127.8, 63.29}, {121.02, 48.4}, {112.45, 40.27}, {112.45, 26.27}, {114.25, 14.54}, {107.03, 8.67}, {88.07, 8.21},
        {78.13, 9.12}, {75.88, 15.89}, {76.78, 28.08}, {75.43, 39.37}, {65.49, 37.11}, {53.3, 42.53},
        {46.08, 47.49}, {40.21, 54.72}, {33.89, 69.62}
    };
    point2D_t bearwinears[255] =
    {
	{73.61, 196.94}, {73.91, 200.44}, {76.22, 203.04}, {79.92, 203.34}, {82.72, 201.24},
	{82.92, 199.54}, {82.12, 197.44}, {75.31, 196.04}
    };
    point2D_t bearwinears2[255] =
    {
	{97.63, 193.08}, {99.03, 196.94}, {103.43, 198.02}, {106.33, 195.65}, {106.22, 191.25},
	{102.89, 189}
};
    point2D_t bearwinfoot[255] =
    {
	{46.73, 46.82}, {46.84, 42.52}, {48.02, 37.37}, {51.56, 29.1}, {53.07, 24.7}, {50.49, 21.91},
	{49.52, 17.83}, {53.28, 15.03}, {65.09, 14.39}, {76.15, 15.14}, {78.52, 18.25}, {77.01, 26.63},
	{75.4, 33.93}, {75.4, 36.62}, {69.17, 37.37}, {58.01, 41.02}, {51.35, 44.67}
};
    point2D_t bearwinekor[255] =
        {
        {129.2, 68.19}, {131.78, 64.43}, {130.81, 58.2}, {127.91, 55.84},
        {124.48, 55.84}
    };
    point2D_t bearwintangan[255] =
    {
	{90.71, 125.69}, {93.24, 111.9}, {94.63, 97.98}, {95.89, 83.67}, {97.03, 76.46},
	{99.82, 68.74}, {103.99, 66.59}, {108.42, 68.23}, {114.63, 75.57}, {118.17, 84.31},
	{119.56, 93.04}, {119.94, 105.44}, {119.31, 114.3}, {118.42, 121.14}, {116.78, 131.51},
};
    point2D_t tangandetail1[255] =
    {
	 {102.29, 74.32}, {102.69, 71.12}, {101.65, 68.08}
};
    point2D_t tangandetail2[255] =
    {
	 {108.69, 75.82}, {108.21, 72.16}, {107.09, 68.08}
};
point2D_t hidung[255] =
    {
	 {50.45, 176.47}, {50.45, 173.63}, {55.7, 170.51}, {60, 170}, {60.53, 173.92}, {56.41, 176.33},
	 {53.29, 177.32}
    };
point2D_t mewek[255] ={
	{45.96, 163.01}, {51.83, 163.76}, {55.29, 162.26}, {57.39, 159.25}
};
point2D_t bodylines1[255] =
    {
	{74.73, 36.29}, {62.58, 38.16}, {46.39, 47.2}, {36.11, 61.84},{32.06, 86.44}, {36.11, 112.61},
	{40, 145.94}, {43.89, 157.15}, {40.78, 168.68}, {43.84, 176}, {49.52, 181.69}, {59.16, 183.94}, {67.27, 194.17},
	{76.92, 198.5}, {87.56, 198.83}, {97.54, 194.17}, {100.71, 197.81}, {104.43, 197.45}, {107.1, 194.26}, {105.32, 190.36},
	{102.84, 189.11}, {106.03, 184.68}, {115.79, 160.36}, {127.71, 117.64}, {130.98, 86.56}, {127.71, 59.85},
	{117.89, 44.58}, {110.81, 38.04},
};
point2D_t bodylinesfoot[255] =
    {
	{112.83, 40.5}, {112.34, 34.18}, {112.5, 26.87}, {113.67, 19.22},
	{114.16, 14.24}, {111.34, 11.91}, {105.69, 9.25}, {96.55, 8.26}, {84.08, 8.42}, {77.6, 9.92},
	{76.27, 13.57}, {80,20}, {77.77, 26.21}, {76.27, 32.19}, {76.1, 40.83}
};
point2D_t footpart1[255] =
    {
	{78.44, 9.98}, {78.56, 12.06}, {80.41, 14.49}
};
point2D_t footpart2[255] =
    {
	{82.49, 8.48}, {82.25, 10.67}, {84.45, 13.45}
};
point2D_t footpartbody2[255] =
    {
	{76.25, 15.53}, {72.66, 14.72}, {65.61, 14.49}, {56.25, 15.18},
	{50.48, 16.34}, {49.32, 18.07}, {49.9, 21.65}, {53.13, 24.42},
	{52.67, 26.97}, {50.13, 32.4}, {48.63, 37.02}, {47.13, 42.22},
	{47.13, 47.07}
};
point2D_t footpart4[255] =
    {
	{51.53, 16.18}, {51.58, 17.82}, {52.56, 20.11}
};
point2D_t footpart3[255] =
    {
	{55.78, 15.37}, {55.18, 17.11}, {56, 19.62}
};
    color_t warnabear = {0.521, 0.337, 0.184};
    color_t colrhidung = {0,0,0};
    color_t gigi = {1,1,1};
    color_t mulut = {0.431, 0.219, 0.203};
    fillPolygon(bearwinears, 8, warnabear);
    fillPolygon(bearwinfoot, 17, warnabear);
    fillPolygon(bearwinears2, 6, warnabear);
    fillPolygon(bearwinekor, 5, warnabear);
    fillPolygon(bearwinbody, 35, warnabear);
    drawLines(mewek, 4);
    drawLines(bearwintangan, 15);
    drawLines(tangandetail1, 3);
    drawLines(tangandetail2, 3);
    fillPolygon(hidung, 7, colrhidung);
    makeeyes(63.79, 182.9);
    makeeyes(75.35, 176.65);
    drawLines(bodylines1, 28);
    drawLines(bodylinesfoot, 15);
    drawLines(footpart1, 3);
    drawLines(footpart2, 3);
    drawLines(footpart3, 3);
    drawLines(footpart4, 3);
    drawLines(footpartbody2, 13);

}

void winchar(void){
    point2D_t bearwinbody[255] =
    {
        {33.89, 69.62}, {31.63, 88.58}, {34.34, 104.38}, {40.21, 124.24}, {50.14, 146.37}, {100.82, 158.1}, {40.21, 168.94}, {45.18, 180.23},
        {59.17, 183.39}, {67.3, 195.58}, {84.45, 198.74}, {97.1, 194.67}, {104.32, 187.45}, {113.35, 166.23}, {123.73, 134.63}, {130.5, 101.22},
        {131.41, 79.55}, {127.8, 63.29}, {121.02, 48.4}, {112.45, 40.27}, {112.45, 26.27}, {114.25, 14.54}, {107.03, 8.67}, {88.07, 8.21},
        {78.13, 9.12}, {75.88, 15.89}, {76.78, 28.08}, {75.43, 39.37}, {65.49, 37.11}, {53.3, 42.53},
        {46.08, 47.49}, {40.21, 54.72}, {33.89, 69.62}
    };
    point2D_t bearwinears[255] =
    {
	{73.61, 196.94}, {73.91, 200.44}, {76.22, 203.04}, {79.92, 203.34}, {82.72, 201.24},
	{82.92, 199.54}, {82.12, 197.44}, {75.31, 196.04}
    };
    point2D_t bearwinears2[255] =
    {
	{97.63, 193.08}, {99.03, 196.94}, {103.43, 198.02}, {106.33, 195.65}, {106.22, 191.25},
	{102.89, 189}
};
    point2D_t bearwinfoot[255] =
    {
	{46.73, 46.82}, {46.84, 42.52}, {48.02, 37.37}, {51.56, 29.1}, {53.07, 24.7}, {50.49, 21.91},
	{49.52, 17.83}, {53.28, 15.03}, {65.09, 14.39}, {76.15, 15.14}, {78.52, 18.25}, {77.01, 26.63},
	{75.4, 33.93}, {75.4, 36.62}, {69.17, 37.37}, {58.01, 41.02}, {51.35, 44.67}
};
    point2D_t bearwinekor[255] =
        {
        {129.2, 68.19}, {131.78, 64.43}, {130.81, 58.2}, {127.91, 55.84},
        {124.48, 55.84}
    };
    point2D_t bearwintangan[255] =
    {
	{90.71, 125.69}, {93.24, 111.9}, {94.63, 97.98}, {95.89, 83.67}, {97.03, 76.46},
	{99.82, 68.74}, {103.99, 66.59}, {108.42, 68.23}, {114.63, 75.57}, {118.17, 84.31},
	{119.56, 93.04}, {119.94, 105.44}, {119.31, 114.3}, {118.42, 121.14}, {116.78, 131.51},
};
    point2D_t tangandetail1[255] =
    {
	 {102.29, 74.32}, {102.69, 71.12}, {101.65, 68.08}
};
    point2D_t tangandetail2[255] =
    {
	 {108.69, 75.82}, {108.21, 72.16}, {107.09, 68.08}
};
point2D_t hidung[255] =
    {
	 {50.45, 176.47}, {50.45, 173.63}, {55.7, 170.51}, {60, 170}, {60.53, 173.92}, {56.41, 176.33},
	 {53.29, 177.32}
};
point2D_t smile[255] =
    {
	{55.39, 160.65}, {55.23, 158.06}, {57.37, 155.17},{63.01, 152.12}, {69.56, 151.97}, {77.33, 154.4},
	{82.21, 159.59}, {82.51, 164.46}, {78.25, 165.99},{72.67, 164.01}, {67.12, 162.18}, {60, 160}
};
point2D_t tooth1[255] =
    {
	{60.73, 160.76}, {61.62, 159.19}, {63.73, 158.3}, {65.64, 159.26}, {66.12, 161.71}, {63.53, 161.17},
};
point2D_t tooth2[255] =
    {
	{66.1, 161.73}, {66.52, 160.52}, {68.09, 159.56}, {70, 160}, {71.7, 161.37}, {71.1, 163.66},
};
point2D_t tooth3[255] =
    {
	{71.84, 163.51}, {72.95, 161.6}, {75.17, 161.04}, {76.95, 162.28}, {77.26, 165.67}
};
point2D_t bodylines1[255] =
    {
	{74.73, 36.29}, {62.58, 38.16}, {46.39, 47.2}, {36.11, 61.84},{32.06, 86.44}, {36.11, 112.61},
	{40, 145.94}, {43.89, 157.15}, {40.78, 168.68}, {43.84, 176}, {49.52, 181.69}, {59.16, 183.94}, {67.27, 194.17},
	{76.92, 198.5}, {87.56, 198.83}, {97.54, 194.17}, {100.71, 197.81}, {104.43, 197.45}, {107.1, 194.26}, {105.32, 190.36},
	{102.84, 189.11}, {106.03, 184.68}, {115.79, 160.36}, {127.71, 117.64}, {130.98, 86.56}, {127.71, 59.85},
	{117.89, 44.58}, {110.81, 38.04},
};
point2D_t bodylinesfoot[255] =
    {
	{112.83, 40.5}, {112.34, 34.18}, {112.5, 26.87}, {113.67, 19.22},
	{114.16, 14.24}, {111.34, 11.91}, {105.69, 9.25}, {96.55, 8.26}, {84.08, 8.42}, {77.6, 9.92},
	{76.27, 13.57}, {80,20}, {77.77, 26.21}, {76.27, 32.19}, {76.1, 40.83}
};
point2D_t footpart1[255] =
    {
	{78.44, 9.98}, {78.56, 12.06}, {80.41, 14.49}
};
point2D_t footpart2[255] =
    {
	{82.49, 8.48}, {82.25, 10.67}, {84.45, 13.45}
};
point2D_t footpartbody2[255] =
    {
	{76.25, 15.53}, {72.66, 14.72}, {65.61, 14.49}, {56.25, 15.18},
	{50.48, 16.34}, {49.32, 18.07}, {49.9, 21.65}, {53.13, 24.42},
	{52.67, 26.97}, {50.13, 32.4}, {48.63, 37.02}, {47.13, 42.22},
	{47.13, 47.07}
};
point2D_t footpart4[255] =
    {
	{51.53, 16.18}, {51.58, 17.82}, {52.56, 20.11}
};
point2D_t footpart3[255] =
    {
	{55.78, 15.37}, {55.18, 17.11}, {56, 19.62}
};
    color_t warnabear = {0.521, 0.337, 0.184};
    color_t colrhidung = {0,0,0};
    color_t gigi = {1,1,1};
    color_t mulut = {0.431, 0.219, 0.203};
    fillPolygon(bearwinears, 8, warnabear);
    fillPolygon(bearwinfoot, 17, warnabear);
    fillPolygon(bearwinears2, 6, warnabear);
    fillPolygon(bearwinekor, 5, warnabear);
    fillPolygon(bearwinbody, 35, warnabear);
    fillPolygon(smile, 12, mulut);
    fillPolygon(tooth1, 6, gigi);
    fillPolygon(tooth2, 6, gigi);
    fillPolygon(tooth3, 5, gigi);
    drawLines(smile, 12);
    drawLines(tooth1, 6);
    drawLines(tooth2, 6);
    drawLines(tooth3, 5);
    drawLines(bearwintangan, 15);
    drawLines(tangandetail1, 3);
    drawLines(tangandetail2, 3);
    fillPolygon(hidung, 7, colrhidung);
    makeeyes(63.79, 182.9);
    makeeyes(75.35, 176.65);
    drawLines(bodylines1, 28);
    drawLines(bodylinesfoot, 15);
    drawLines(footpart1, 3);
    drawLines(footpart2, 3);
    drawLines(footpart3, 3);
    drawLines(footpart4, 3);
    drawLines(footpartbody2, 13);

}

void menang(){
    glClearColor(1, 1, 1, 1 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(isUserWin==true&&isComWin==false){
        glColor3ub(255,0,0);
        drawBitmapTextSmall("User Win!",270,300,0);
        glPushMatrix();
        glTranslatef(130, 70,0);
        winchar();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(90,150,0);
        bubblechat();
        glPopMatrix();
    } else if(isComWin==true&&isUserWin==false){
        glColor3ub(255,0,0);
        drawBitmapTextSmall("Computer Win",260,300,0);
        glPushMatrix();
        glTranslatef(130, 70,0);
        losechar();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(90,150,0);
        bubblechat();
        glPopMatrix();
    } else if(sama){
        glColor3ub(255,0,0);
        drawBitmapTextSmall("Seri!",270,300,0);
        glPushMatrix();
        glTranslatef(130, 70,0);
        winchar();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(90,150,0);
        bubblechat();
        glPopMatrix();
    }
}
void pilihanComputer(int acak){
    if(acak==1)
        rock2();
    else if(acak==2)
        paper2();
    else if(acak==3)
        scissor2();
    if(acak==1&&chooseRock==true||acak==2&&choosePaper==true||acak==3&&chooseScissor==true){
            sama=true;
    }
    else if(acak==1&&choosePaper==true||acak==2&&chooseScissor==true||acak==3&&chooseRock==true){
            isUserWin=true;
    }
    else if(acak==1&&chooseScissor==true||acak==2&&chooseRock==true||acak==1&&chooseScissor==true||acak==3&&choosePaper==true){
            isComWin=true;
    }
}
void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    if(stop==false){
        if(udahMulai!=true)
        {
            belumMulai();
        }
        else if(disInstruction){
            instruction();
        }

            if(chooseRock)
            {
                disInstruction=false;
                bg();
                rock();
                pilihanComputer(acak);
            }
            if(choosePaper)
            {
                disInstruction=false;
                bg();
                paper();
                pilihanComputer(acak);
            }
            if(chooseScissor)
            {
                disInstruction=false;
                bg();
                scissor();
                pilihanComputer(acak);
            }
        }
    else if(stop==true) {
        menang();
    }
    glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:     // ESC key
         exit(0);
         break;
      case 13:
        udahMulai=true;
        disInstruction=true;
        break;
   }
}
void specialkey(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP :
        chooseRock=true;
        choosePaper=false;
        chooseScissor=false;
        break;
    case GLUT_KEY_DOWN :
        chooseRock=false;
        choosePaper=true;
        chooseScissor=false;
        break;
    case GLUT_KEY_RIGHT :
        chooseRock=false;
        choosePaper=false;
        chooseScissor=true;
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition(300,200);
    glutInitWindowSize(600,400);
    glutCreateWindow ("figures");
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0., 500., 0., 400.0);
    // Define the dimensions of the Orthographic Viewing Volume
    glutIdleFunc(display); // idle event call back
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkey);
    glutMainLoop();
    return 0;
}
