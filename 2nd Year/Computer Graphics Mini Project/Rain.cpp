 #include<stdio.h>
#include<graphics.h>
#include<windows.h>


#define ScreenWidth getmaxx()
#define ScreenHeight getmaxy()
#define GroundY ScreenHeight*0.85

void DrawManAndUmbrella(int x,int ldisp)
{
//head
circle(x,GroundY-90,10);
line(x,GroundY-80,x,GroundY-30);
//hand
line(x,GroundY-70,x+10,GroundY-60);
line(x,GroundY-65,x+10,GroundY-55);
line(x+10,GroundY-60,x+20,GroundY-70);
line(x+10,GroundY-55,x+20,GroundY-70);
//legs
line(x,GroundY-30,x+ldisp,GroundY);
line(x,GroundY-30,x-ldisp,GroundY);
//umbrella
pieslice(x+20,GroundY-140,0,180,65);
line(x+20,GroundY-140,x+20,GroundY-70);
}

void DrawManAndUmbrella1(int x1,int ldisp)
{
//head
circle(x1,GroundY-70,10);
line(x1,GroundY-60,x1,GroundY-10);
//hand
line(x1,GroundY-50,x1+00,GroundY-40);
line(x1,GroundY-45,x1+00,GroundY-35);
line(x1+10,GroundY-40,x1+00,GroundY-50);
line(x1+10,GroundY-35,x1+00,GroundY-50);
//legs
line(x1,GroundY-20,x1+ldisp,GroundY);
line(x1,GroundY-20,x1-ldisp,GroundY);
/*//umbrella
pieslice(x1+20,GroundY-100,0,180,40);
line(x1+20,GroundY-100,x1+20,GroundY-50);
*/
}
int ldisp=0;


void Rain(int x)
{
int i,rx,ry;
for(i=0;i<400;i++)
{
 rx=rand() % ScreenWidth;
 ry=rand() % ScreenHeight;
 if(ry<GroundY-4)
 {
  if(ry<GroundY-120 || (ry>GroundY-120 && (rx<x-20 || rx>x+60)))
  line(rx,ry,rx+0.5,ry+4);
 }
}
}

void drawCloud(int z,int y)
{
        int r=55;

		arc(z,y,45,135,r);
		arc(z+50,y,45,135,r);
		arc(z+100,y,45,135,r);
		arc(z,y,135,225,r);
		arc(z+50,y,135+90,225+90,r);
		arc(z,y,135+90,225+90,r);
		arc(z+100,y,135+90,225+90,r);
		arc(z+100,y,315,45,r);
}



int main()
{
    

  int gd=DETECT,gm,x=0,x1=30;
  int z;
  initgraph(&gd,&gm,NULL);

  while(!kbhit())
  {
     drawCloud(z,70);

     drawCloud(250,30);
     drawCloud(450,70);
     drawCloud(700,20);
     drawCloud(800,70);
     drawCloud(1100,90);
     drawCloud(1400,40);
   //Draw Ground
   line(0,GroundY,ScreenWidth,GroundY);

   Rain(x);


   ldisp=(ldisp+2)%20;
   DrawManAndUmbrella(x,ldisp);
   DrawManAndUmbrella1(x1,ldisp);

   delay(75);
   cleardevice();
   x=(x+2)%ScreenWidth;
   x1=(x1+2)%ScreenWidth;

}

  getch();
  return 0;
}


