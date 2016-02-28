#include<stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <stdlib.h>

int i,j=1,n,number,count,rccount=0,count1=1,valid=0,i1;
float t,B[1000];

typedef struct									// Structure for storing points
{
	int x,y;
}Point;					

Point p[1000],temp1[1000],temp2[1000];

typedef struct									// Structure for storing points obtained by using 
{										// Bernstein Polynomial 	
	float x,y;
}BPoint;							
		
BPoint points[101],temp;

int factorial(int a)								// Method to calucate factorial of a number
{
	int product=1;
		
	if(a==0 || a==1)
	{
		return 1;	
	}
		
	else
	{
		int k=0;
		for(k=a;k>1;k--)
		{
			product = product * k;			
		}
		return product;
	}
}

int choose(int a,int b)								// Method to find ncr
{
	return factorial(a)/(factorial(a-b)*factorial(b));
}

void equate(Point p[],Point temp[],int n)					// Method to equate two point arrays
{
	for(i=0;i<n;i++)
	{
		temp[i].x = p[i].x ;
		temp[i].y = p[i].y ;
	}
}

void printBPoint(BPoint a)							// Method to print points with non-integer coordinates
{
	printf("(%.2f,%.2f)\n",a.x,a.y);
}

void printPoint(Point a)							// Method to print points with integer coordinates
{
	printf("(%d,%d)\n",a.x,a.y);
}


float calculateBernsteinCoefficient(int a,int b,float t)			// Metod to caluclate Bi (Bernstein coefficient)
{
	float z = (choose(b,a))*(pow((1-t),b-a))*(pow(t,a));
	return z;
}

void reset(Point p[],Point temp[],int n)					// Method to restore the original values in an array
{
	equate(temp,p,n);
	n = count;
}

void plotPoint(int a,int b)							// Method to plot points on screen
{
	glColor3f(1,0,0);
	glPointSize(5);
	glBegin(GL_POINTS);
     	glVertex2i(a,b);
    	glEnd();
    	glFlush();
}

void joinPoints(Point a, Point b)						// Method to join two points
{
	glBegin(GL_LINES);
	glLineWidth(2);
	glColor3f(0,0,1);
      	glVertex2i(a.x, a.y);
      	glVertex2i(b.x, b.y);
    	glEnd();
    	glFlush();
}

void drawCurve()								// Method to draw the curve
{
	glColor3f(0,0,0);
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<101;i++)
	{
		glVertex2f(points[i].x,points[i].y);
	}
	glEnd();
}

void draw()									// Draw function
{
	equate(p,temp1,n);

	points[0].x=p[0].x;
	points[0].y=p[0].y;
	points[100].x=p[n-1].x;
	points[100].y=p[n-1].y;

	t=0.01;
	j=1;
		
	while(t<=0.99)
	{
		temp.x = 0;
		temp.y = 0;
		for(i=0;i<n;i++)
		{
			B[i] = calculateBernsteinCoefficient(i,n-1,t);
			temp.x = temp.x + (B[i]*p[i].x);
			temp.y = temp.y + (B[i]*p[i].y);		
		}		

		points[j].x = temp.x;
		points[j].y = temp.y;
		j=j+1;
		t=t+0.01;
	}

	drawCurve();

	for(i=0;i<n;i++)
	{
		plotPoint(p[i].x,p[i].y);
	}

	for(i=0;i<n-1;i++)
	{
			joinPoints(p[i],p[i+1]);		
	}
}


void drawText(char *string, float x, float y, float z)					// Method to write text on screen
{
    	char *c;
    	glRasterPos3f(x, y, z);

    	for (c=string; *c != '\0'; c++)
    	{
        		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    	}
}

void myMouse(int button, int state, int a, int b) 
{
	int i=0;

  	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		p[number].x = a;
		p[number].y = 480-b; 
    		number++;
			
		if(number<=n)								// Plot n points 
		{
			plotPoint(a,480-b);
		}

		if(number == n)
		{
			equate(p,temp2,n);

			printf("\n The following are the control points choosen :\n\n");

			for(i=0;i<n;i++)						// Print the control points choosen
			{
				printf(" Point %d : ",i+1);
				printPoint(p[i]);
			}

			printf("\n");
				
			printf(" Right click on a control point and drag to move it\n\n");

			glClear(GL_COLOR_BUFFER_BIT);

			glColor3d(0,0,0);
			drawText("Right click on a control point and drag",170,460,0);

			draw();                               				// draw the curve for the control points given
					
			count = 0;		
		}

  	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 				// Right click on any of control points
	{
		if(count == 0) 
		{
			for(i=0;i<n;i++)
			{
				if(temp1[i].x-5 <= a && a <= temp1[i].x+5 && temp1[i].y-5 <= 480-b && 480-b <= temp1[i].y+5)
				{
					printf(" Selected point is control point %d\n",i+1);
					i1=i;
					rccount = 1;
					valid = state == GLUT_DOWN;						
					break;
				}				
			}
		}
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP && rccount == 1)		// Once the right click button is released move the
	{										// point and print it's location
		rccount = 0;
		count = 0;
		printf(" It has been moved to : (%d,%d)\n\n",a,480-b);
	}
}
   
void myMotion(int a, int b)								// Method to determine mouse motion
{
	if(rccount == 1)
	{
		temp2[i1].x = a;
		temp2[i1].y = 480-b; 
		
		equate(temp2,p,n);
		equate(p,temp1,n);

		points[0].x=p[0].x;
		points[0].y=p[0].y;
		points[100].x=p[n-1].x;
		points[100].y=p[n-1].y;

		t=0.01;
		j=1;
		
		while(t<=0.99)
		{
			temp.x = 0;
			temp.y = 0;
			for(i=0;i<n;i++)
			{
				B[i] = calculateBernsteinCoefficient(i,n-1,t);
				temp.x = temp.x + (B[i]*p[i].x);
				temp.y = temp.y + (B[i]*p[i].y);		
			}		

			points[j].x = temp.x;
			points[j].y = temp.y;
			j=j+1;
			t=t+0.01;
		}
		
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3d(0,0,0);
		drawText("Right click on a control point and drag",170,460,0);

		drawCurve();
	
		for(i=0;i<n;i++)
		{
			plotPoint(p[i].x,p[i].y);		
		}

		for(i=0;i<n-1;i++)
		{
			joinPoints(p[i],p[i+1]);		
		}
	}
}

void myInit() 											// Initializing the screen
{
    	glClearColor(1,1,1,1);
	glColor3f(0,0,0);
	glLoadIdentity();
	gluOrtho2D(0,640,0,480);
	glMatrixMode(GL_MODELVIEW);
}

void myDisplay() 										// Display function
{
    	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(0,0,0);
	drawText("Plot the points by clicking the left mouse button",130,460,0);
    	glFlush();
}

int main(int argc, char** argv)
{
	printf("\n Enter the no.of control points : ");
	scanf("%d",&n);

	while(n<=1)
	{
		printf("\n Number of control points should be greater than 1. Enter a new value : ");
		scanf("%d",&n);
	}

	printf("\n Plot the %d points on the screen \n",n);

	glutInit(&argc, argv);   
  	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);
  	glutInitWindowSize(640,480);
	glutInitWindowPosition(700,0);
	glutCreateWindow("Bezier Curve implementation using Bernstein Polynomial");  
	glutDisplayFunc(myDisplay);	
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	myInit(); 
	glutMainLoop(); 

	return 0;
}
