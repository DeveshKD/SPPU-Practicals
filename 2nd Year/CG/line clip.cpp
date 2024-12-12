#include<iostream>
#include<graphics.h>
#include<stdlib.h>
#include<math.h>
using namespace std;
class coordinate
{
	public:
		int x,y;
		char code[4];
};
class Lineclip
{
	public:
		coordinate PT;
		void drawwindow();
		void drawline(coordinate p1,coordinate p2);
		coordinate setcode(coordinate p);
		int visibility(coordinate p1,coordinate p2);
		coordinate resetendpt(coordinate p1,coordinate p2);
};
int main()
{
	Lineclip lc;
	int gd=DETECT,gm,v;
	coordinate p1,p2,p3,p4,ptemp;
	
	cout<<"\nEnter x1 and y1:";
	cin>>p1.x>>p1.y;
	cout<<"\nEnter x2 and y2:";
	cin>>p2.x>>p2.y;
	
	initgraph(&gd,&gm,NULL);
	lc.drawwindow();
	delay(2000);
	
	lc.drawline(p1,p2);
	delay(2000);
	cleardevice();
	
	p1=lc.setcode(p1);
	p2=lc.setcode(p2);
	v=lc.visibility(p1,p2);
	delay(2000);
	
	switch(v) {
    case 0:  // Completely visible
        lc.drawwindow();
        lc.drawline(p1, p2);
        break;
    
    case 1:  // Completely outside
        lc.drawwindow();  // Just show the window, no line drawn
        break;
    
    case 2:  // Partially visible, needs clipping
        p3 = lc.resetendpt(p1, p2);  // Clip first point
        p4 = lc.resetendpt(p2, p1);  // Clip second point
        lc.drawwindow();
        lc.drawline(p3, p4);  // Draw the clipped line
        break;
}
	getch();
	closegraph();
}
void Lineclip::drawwindow()
{
	line(100,100,450,100);
	line(450,100,450,450);
	line(450,450,100,450);
	line(100,450,100,100);
}
void Lineclip::drawline(coordinate p1,coordinate p2)
{
	line(p1.x,p1.y,p2.x,p2.y);
}
coordinate Lineclip::setcode(coordinate p)
{
	coordinate ptemp;
	
	if(p.y<100)
	ptemp.code[0]='1';
	else
	ptemp.code[0]='0';
	if(p.y>450)
	ptemp.code[1]='1';
	else
	ptemp.code[1]='0';
	if(p.x>450)
	ptemp.code[2]='1';
	else
	ptemp.code[2]='0';
	if(p.x<100)
	ptemp.code[3]='1';
	else
	ptemp.code[3]='0';
	
	ptemp.x=p.x;
	ptemp.y=p.y;
	return ptemp;
}
int Lineclip::visibility(coordinate p1, coordinate p2) {
    int i, flag = 0;

    // Check if both points are completely inside the window
    for (i = 0; i < 4; i++) {
        if (p1.code[i] != '0' || p2.code[i] != '0') {
            flag = 1;  // Partially visible
        }
    }
    if (flag == 0) return 0;  // Completely visible

    // Check if both points share an outside region (completely outside)
    for (i = 0; i < 4; i++) {
        if (p1.code[i] == '1' && p2.code[i] == '1') {
            return 1;  // Completely outside
        }
    }

    return 2;  // Partially visible, needs clipping
}


coordinate Lineclip::resetendpt(coordinate p1, coordinate p2) {
    coordinate temp;
    float m;
    int x, y;

    // Compute slope
    if (p1.x != p2.x) {
        m = (float)(p2.y - p1.y) / (p2.x - p1.x);
    }

    // Clip against top boundary (y = 100)
    if (p1.code[0] == '1') {
        y = 100;
        x = p1.x + (y - p1.y) / m;
        temp.x = x;
        temp.y = y;
        return temp;
    }

    // Clip against bottom boundary (y = 450)
    if (p1.code[1] == '1') {
        y = 450;
        x = p1.x + (y - p1.y) / m;
        temp.x = x;
        temp.y = y;
        return temp;
    }

    // Clip against right boundary (x = 450)
    if (p1.code[2] == '1') {
        x = 450;
        y = p1.y + m * (x - p1.x);
        temp.x = x;
        temp.y = y;
        return temp;
    }

    // Clip against left boundary (x = 100)
    if (p1.code[3] == '1') {
        x = 100;
        y = p1.y + m * (x - p1.x);
        temp.x = x;
        temp.y = y;
        return temp;
    }

    // Return unchanged point if no clipping is needed
    return p1;
}
