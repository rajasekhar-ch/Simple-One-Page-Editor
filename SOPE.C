#include<dos.h>
#include<string.h>
#include<conio.h>
#include<stdio.h>
int N=0,C=1,t[23],O=0;
char far *vm=(char far *)0xB8000000;

void highlight(int r1,int c1,int r2,int c2,int attr);
void dhighlight(int r1,int c1,int r2,int c2,int attr);
void getcharacter(int *sc,int *ac);
void writestring(char ch[],int attr,int r,int c);
void setcursepos(int pg,int r,int c);
void getname(char name[15]);


char *savemem(int r1,int c1,int r2,int c2,char *buff)
{
	  char far *v;
	  int i,j,k=0,size;

	  size=(r2-r1+1)*(c2-c1+1)*2;
	  buff=(char *)malloc(size);

	  for(i=r1;i<=r2;i++)
	  {
	  for(j=c1;j<=c2;j++)
	 {
	 v=(vm+i*160+j*2);
	   *(buff+k)=*v;
	   k++;
	   v++;
	   *(buff+k)=*v;
	   k++;
	 }
	  }
	  return(buff);
}
moveup(int r,int c)
{
char far *v;
int i,j;
char ch;
v=vm+r*160+2;
ch=*v;
v++;
if(t[r]>1)
{
if(78-t[r-1]+1>=t[r])
{
for(j=1;j<=t[r];j++)
{
		v=vm+r*160+j*2;
		ch=*v;
		*v=' ';
		v=vm+(r-1)*160+(t[r-1]+j)*2;
		*v=ch;
		v++;
		*v=31;
}
t[r-1]=t[r-1]+t[r];
t[r]=1;
moveup(r,c);
}
else
return 0;
}
else
{
for(i=1;r+i<=22;i++)
{
v=vm+(r+i)*160+2;
ch=*v;
v++;
if(ch== ' ' && *v==16)
		;
else
{
for(j=1;j<=t[r+i];j++)
{
		v=vm+(r+i)*160+j*2;
		ch=*v;
		*v=' ';
		v=vm+(r+i-1)*160+j*2;
		*v=ch;
		v++;
		*v=31;
}
}
t[r+i-1]=t[r+i];
}
t[22]=1;
}
}
movedown(int r,int c)
{
char far *v,ch;
int i,j;
for(i=21;i>r;i--)
{
v=vm+i*160+2;
ch=*v;
v++;
if(ch== ' ' && *v==16)
		;
else
{
for(j=1;j<=t[i];j++)
{
		v=vm+i*160+j*2;
		ch=*v;
		*v=' ';
		v=vm+(i+1)*160+j*2;
		*v=ch;
		v++;
		*v=31;
}
}
t[i+1]=t[i];
}
if(c<=t[r])
{
i=1;
for(j=c;j<=t[r];j++)
{
		v=vm+r*160+j*2;
		ch=*v;
		*v=' ';
		v=vm+(r+1)*160+i*2;
		*v=ch;
		v++;
		*v=31;
		i++;
}
t[r+1]=t[r]-c+1;
t[r]=c;
}
}

moveright(int r,int c)
{
char far *v;
char ch,tch;
int i=r,j=c;
for(j=c;j<=t[r];j++)
{
if(j==78)
{
v=vm+i*160+j*2;
*v=' ';
}
else
{
v=vm+i*160+(j+1)*2;
ch=*v;
v=vm+i*160+j*2;
*v=ch;
v++;
*v=31;
}
}
}
moveleft(int r,int c)
{
char far *v;
char ch,tch;
int i=r,j=c;
for(j=t[r];j>=c;j--)
{
if(j==78)
{
if(t[r+1]==1)
{
v=vm+i*160+j*2;
ch=*v;
v=vm+(i+1)*160+1*2;
*v=ch;
v++;
*v==31;
}
else
{
moveleft(r+1,1);
v=vm+i*160+j*2;
ch=*v;
v=vm+(i+1)*160+1*2;
*v=ch;
v++;
*v==31;
}
}
else
{
v=vm+i*160+j*2;
ch=*v;
v=vm+i*160+(j+1)*2;
*v=ch;
v++;
*v=31;
}
}
}

void shadow(int r1,int r2,int c1,int c2)
{
char far *v;
int r,c;
_setcursortype(_NOCURSOR);
for(r=r1;r<=r2;r++)
{
for(c=c1;c<=c2;c++)
{
v=vm+r*160+c*2;
v++;
*v=7;
}
}
}
void newfile()
{
int r,c;
border(1,23,0,79,31);
writestring("NONAME.TXT",31,1,34);
middle(2,22,1,78,16);
menu();
N=1;
C=0;
O=0;
}


void savefile()     //Save file function
{
int row,col;
char ch,name[15]={"saved"};
char far *v;
FILE *fp;
getname(name);
strcat(name,".txt");
fp=fopen( name , "w");
for(row=2;row<22;row++)
{
for(col=1;col<t[row];col++)
{
v = vm+row*160+col*2;
ch = *v;
fputc(ch,fp);
}
fputc('\n',fp);
}
fclose(fp);
border(1,23,0,79,31);
writestring(strupr(name),31,1,38-(strlen(name)/2));
}

void about()
{
int sc,ac;
char *bluff,m[10]={"[Esc]"},n1[30]={"R NIRANJAN KUMAR REDDY"},n2[30]={"C RAJASEKHAR REDDY"},n3[30]={"prof.R VENGATTRAMAN"},m1[15]={"#S #O #P #E "},m2[40]={"#Simple #One #Page #Editor"},m3[3]={"#&"},m4[40]={"&In the guidance of&"},m5[30]={"&Designed By&"};
bluff=savemem(5,20,19,60,bluff);
_setcursortype(_NOCURSOR);
shadow(6,19,22,60);
middle(5,18,20,58,112);
border(5,18,20,58,117);
writestring(m,121,5,22);
writestring(m1,112,6,40-strlen(m1)/2);
writestring(m2,112,7,40-strlen(m2)/2);
writestring(m5,112,9,40-strlen(m5)/2);
writestring(n1,118,11,40-strlen(n1)/2);
writestring(m3,112,12,40-strlen(m3)/2);
writestring(n2,118,13,40-strlen(n2)/2);
writestring(m4,112,15,40-strlen(m4)/2);
writestring(n3,118,17,40-strlen(n3)/2);
do{
getcharacter(&sc,&ac);
}while(ac!=27);
restoremem(5,20,19,60,bluff);
}

void getname(char name[])
{
int col,i,sc,ac,r,c;
char far *v;
char *buff,ch,save[20]={"Save file as"};
buff=savemem(7,15,23,50,buff);
shadow(7,15,23,50);
middle(7,14,22,48,112);
writestring(save,112,10,24);
middle(12,12,27,43,31);
border(7,14,22,48,119);
col=27;c=27;r=12;
setcursepos(0,12,27);
_setcursortype(_NORMALCURSOR);
while(ac!=13)
{
getcharacter(&sc,&ac);
if(ac==8)
{
if(c>27)
{
c--;
write2vdu(' ',16,r,c);
setcursepos(0,r,c);
}
}
else //if(33<=ac>=126)
{
if(c<43)
{
setcursepos(0,r,c);
write2vdu((char)ac,31,r,c);
c++;
}
}
}
c=27;r=12;
for(i=c;i<=strlen(name) || i<=c+7;i++)
{
v=vm+r*160+i*2;
ch=*v;
name[i-c] = ch;
}
restoremem(7,15,23,50,buff);
}
openfile()
{
int row,col,i,sc,ac;
char ch,*buff,name[20]="saved.txt",caution[15]="file not found";
char far *v;
FILE *fp;
//getname(name);
fp=fopen(name,"r");
if(fp==NULL)
{
buff=savemem(7,24,12,52,buff);
shadow(8,12,24,52);
middle(7,11,23,50,112);
border(7,11,23,50,112);
col=24;
writestring(caution,112,10,30);
getcharacter(&sc,&ac);
restoremem(7,24,12,52,buff);
}
else
{
O=1;
N=0;
C=0;
border(1,23,0,79,31);
writestring(name,31,1,34);
middle(2,22,1,78,16);
menu();
for(row=2;row<22;row++)
{
for(col=1;col<78;col++)
{
v=vm+row*160+col*2;
ch=fgetc(fp);
if(ch==EOF)
break;
if(ch=='\n')
break;
*v=ch;
v++;
*v=31;
}
t[row]=col;
if(ch==EOF)
break;
}
fclose(fp);
}
}

restoremem(int r1,int c1,int r2,int c2,char *buff)
{     char far *v;
	  int i,j,k=0;
	  for(i=r1;i<=r2;i++)
	  {
	  for(j=c1;j<=c2;j++)
	  {
	  v=(vm+i*160+j*2);
	*v=*(buff+k);
	k++;
	v++;
	*v=*(buff+k);
	k++;

	  }
	  }
	  _setcursortype(_NORMALCURSOR);
	  free(buff);
}
getcursepos(int *pg,int *r,int *c)
{
union REGS i,o;
i.h.ah=0x03;
*pg=i.h.bh;
*r=i.h.dh;
*c=i.h.dl;
}
write2vdu(char ch,int attr,int r,int c)
{
char far *v;
v=vm+r*160+c*2;
*v=ch;
v++;
*v=attr;
}
void writestring(char ch[],int attr,int r,int c)
{
int i;
char far *v;
char tempc=c;
for(i=0;ch[i]!='\0';i++)
{
v=vm+r*160+c*2;
if(ch[i]=='&')
{
i++;
while(ch[i]!='&')
{
v=vm+r*160+c*2;
*v=ch[i];
v++;
*v=115;
i++;
c++;
}
}
else if(ch[i]=='!')
{
r++;
c=tempc;
}
else if(ch[i]=='#')
{
*v=ch[i+1];
v++;
*v=116;
i++;
c++;
}
else
{
*v=ch[i];
v++;
*v=attr;
c++;
}

}
}
clrvdu(int attr,int r,int c)
{
char far *v;
v=vm+r*160+c*2;
*v=' ';
v++;
*v=attr;
}
border(int r1,int r2,int c1,int c2,int attr)
{
int c,r;
for(c=c1;c<=c2;c++)
{
write2vdu(205,attr,r1,c);
write2vdu(205,attr,r2,c);
}
for(r=r1;r<r2;r++)
{
write2vdu(186,attr,r,c1);
write2vdu(186,attr,r,c2);
}
write2vdu(201,attr,r1,c1);
write2vdu(187,attr,r1,c2);
write2vdu(200,attr,r2,c1);
write2vdu(188,attr,r2,c2);
}
singleborder(int r1,int r2,int c1,int c2,int attr)
{
int c,r;
for(c=c1;c<=c2;c++)
{
write2vdu(196,attr,r1,c);
write2vdu(196,attr,r2,c);
}
for(r=r1;r<r2;r++)
{
write2vdu(179,attr,r,c1);
write2vdu(179,attr,r,c2);
}
write2vdu(218,attr,r1,c1);
write2vdu(191,attr,r1,c2);
write2vdu(192,attr,r2,c1);
write2vdu(217,attr,r2,c2);
}
midline(int r,int c1,int c2,int attr)
{
int i;
write2vdu(195,attr,r,c1);
write2vdu(180,attr,r,c2);
for(i=c1+1;i<=c2-1;i++)
write2vdu(196,attr,r,i);
}
middle(int r1,int r2,int c1,int c2,int attr)
{
int r,c;
for(r=r1;r<=r2;r++)
{
for(c=c1;c<=c2;c++)
{
write2vdu(' ',attr,r,c);
}
}
}
menu()
{
char ch[32]={"  #File  #Edit  #Help  #Search"};
char bottom0[20]={"#F#1 Help"};
char bottom1[70]={"#A#l#t#+#F File   #C#t#r#l#+#N New   #F#2 Close   #F#4 Exit"};
middle(0,0,0,79,112);
middle(24,24,0,79,112);
writestring(ch,112,0,1);
writestring(bottom0,112,24,3);
write2vdu(179,112,24,11);
writestring(bottom1,112,24,13);
}
filemenu()
{
int flag=0,sc,tfalg,vflag,ac,i,j,c,r,tflag;
char *buff,items[100]={"#New    &Ctrl+N&!#Save   &Ctrl+S&!#Open   &Ctrl+O&!!#Print  &Ctrl+P&!!#Close  &F2&!#Exit   &F4&"};
_setcursortype(_NOCURSOR);
buff=savemem(1,1,11,21,buff);
highlight(0,2,0,7,32);
shadow(2,11,2,21);
middle(1,10,1,19,112);
singleborder(1,10,2,18,112);
midline(5,2,18,112);
midline(7,2,18,112);
writestring(items,112,2,3);
flag=0;vflag=0;tflag=0;
highlight(2,3,2,17,32);
while(1)
{
getcharacter(&sc,&ac);
if(ac==0 && (sc==72 || sc==80))
{
switch(sc)
{
case 72:if(flag==0)
		{
		tflag=vflag;
		vflag=7;
		flag=5;
		}
		else
		{
		if(flag==4)
		{
		tflag=vflag;
		vflag=4;
		}
		else if(flag==3)
		{
		tflag=vflag;
		vflag=2;
		}
		else
		{
		tflag=vflag;
		vflag--;
		}
		flag--;
		}
		dhighlight(2+tflag,3,2+tflag,17,32);
		highlight(2+vflag,3,2+vflag,17,32);
		break;
case 80:if(flag==5)
		{
		tflag=7;
		flag=0;
		vflag=0;
		}
		else
		{
		switch(flag)
		{
		case 2:tflag=vflag;
				vflag=4;
				break;
		case 3:	tflag=4;
				vflag=6;
				break;
		default:tflag=vflag;
				vflag++;
				break;
		}
		flag++;
		}
		dhighlight(2+tflag,3,2+tflag,17,32);
		highlight(2+vflag,3,2+vflag,17,32);
		break;
}
}
if(ac==13)
{
dhighlight(0,2,0,7,32);
dhighlight(2+flag,3,2+flag,17,32);
switch(flag)
{
case 0:newfile();
	   break;
case 1:restoremem(1,1,11,21,buff);
	   savefile();
	   break;
case 2:restoremem(1,1,11,21,buff);
	   openfile();
	   break;
case 3: restoremem(1,1,11,21,buff);
		//printf("repainted");
		break;
case 4: restoremem(1,1,11,21,buff);
		N=0;
		C=1;
		O=0;
		break;
case 5:exit();

}
break;
}
if(sc==75||sc==77)
{
dhighlight(2+vflag,3,2+vflag,17,32);
restoremem(1,1,11,21,buff);
dhighlight(0,2,0,7,32);
if(sc==75)
{
searchmenu();
break;
}
else if(sc==77)
{
editmenu();
break;
}
}
else if(ac==27)
{
dhighlight(2+vflag,3,2+vflag,17,32);
dhighlight(0,2,0,7,32);
restoremem(1,1,11,21,buff);
break;
}
}
}

editmenu()
{
int flag,sc,ac,i,tflag,j,c,r;
char *buff,items[70]={"#Copy     &Ctrl+C&!#Paste    &Ctrl+V&!#Cut      &Ctrl+X&"};
_setcursortype(_NOCURSOR);
buff=savemem(1,7,6,29,buff);
highlight(0,8,0,13,32);
shadow(2,6,8,29);
middle(1,5,7,27,112);
singleborder(1,5,8,26,112);
writestring(items,112,2,10);
flag=0;
highlight(2,9,2,25,32);
while(1)
{
getcharacter(&sc,&ac);
if(ac==0 && (sc==72 || sc==80))
{
switch(sc)
{
case 72:if(flag==0)
		{
		tflag=flag;
		flag=2;
		}
		else
		{
		tflag=flag;
		flag--;
		}
		dhighlight(2+tflag,9,2+tflag,25,32);
		highlight(2+flag,9,2+flag,25,32);
		break;
case 80:if(flag==2)
		{
		tflag=flag;
		flag=0;
		}
		else
		{
		tflag=flag;
		flag++;
		}
		dhighlight(2+tflag,9,2+tflag,25,32);
		highlight(2+flag,9,2+flag,25,32);
		break;
}
}
if(ac==13)
{
dhighlight(0,8,0,13,32);
restoremem(1,7,6,29,buff);
switch(flag)
{
//case 0:copy();
//		break;
//case 1: paste();
//		break;
//case 2:cut();
//		 break;

}
break;
}
if(sc==75||sc==77)
{
dhighlight(0,8,0,13,32);
restoremem(1,7,6,29,buff);
if(sc==75)
{
filemenu();
break;
}
if(sc==77)
{
helpmenu();
break;
}
}
else if(ac==27)
{
dhighlight(0,8,0,13,32);
restoremem(1,7,6,29,buff);
break;
}
}
}
helpmenu()
{
int i,j,c,r,sc,ac,flag,tflag;
char *buff,items[70]={"#Help     &F1&!#About"};
highlight(0,14,0,19,32);
_setcursortype(_NOCURSOR);
buff=savemem(1,13,5,32,buff);
shadow(2,5,14,32);
middle(1,4,13,30,112);
singleborder(1,4,14,29,112);
writestring(items,112,2,16);
flag=0;
highlight(2,15,2,28,32);
while(1)
{
getcharacter(&sc,&ac);
if(ac==0 && (sc==72 || sc==80))
{
switch(sc)
{
case 72:if(flag==0)
		{
		tflag=flag;
		flag=1;
		}
		else
		{
		tflag=flag;
		flag--;
		}
		dhighlight(2+tflag,15,2+tflag,28,32);
		highlight(2+flag,15,2+flag,28,32);
		break;
case 80:if(flag==1)
		{
		tflag=flag;
		flag=0;
		}
		else
		{
		tflag=flag;
		flag++;
		}
		dhighlight(2+tflag,15,2+tflag,28,32);
		highlight(2+flag,15,2+flag,28,32);
		break;
}
}
if(ac==13)
{
restoremem(1,13,5,32,buff);
dhighlight(0,14,0,19,32);
switch(flag)
{
//case 1:help();
//		break;
case 1:about();
		break;
}
break;
}
if(sc==75||sc==77)
{
dhighlight(2+flag,15,2+flag,28,32);
restoremem(1,13,5,32,buff);
dhighlight(0,14,0,19,32);

if(sc==75)
{
editmenu();
break;
}
if(sc==77)
{
searchmenu();
break;
}
}
else if(ac==27)
{
dhighlight(0,14,0,19,32);
dhighlight(2+flag,15,2+flag,28,32);
restoremem(1,13,5,32,buff);
break;
}
}
}
searchmenu()
{
int i,j,c,r,sc,ac,flag,tflag;
char *buff,items[50]={"#Find  &Ctrl+F&!#Replace"};
buff=savemem(1,19,5,36,buff);
highlight(0,20,0,26,32);
shadow(2,5,20,36);
middle(1,4,19,34,112);
singleborder(1,4,20,34,112);
writestring(items,112,2,21);
flag=0;
highlight(2,21,2,33,32);
while(1)
{
getcharacter(&sc,&ac);
if(ac==0 && (sc==72 || sc==80))
{
switch(sc)
{
case 72:if(flag==0)
		{
		tflag=flag;
		flag=1;
		}
		else
		{
		tflag=flag;
		flag--;
		}
		dhighlight(2+tflag,21,2+tflag,33,32);
		highlight(2+flag,21,2+flag,33,32);
		break;
case 80:if(flag==1)
		{
		tflag=flag;
		flag=0;
		}
		else
		{
		tflag=flag;
		flag++;
		}
		dhighlight(2+tflag,21,2+tflag,33,32);
		highlight(2+flag,21,2+flag,33,32);
		break;
}
}
if(ac==13)
{
dhighlight(0,20,0,26,32);
restoremem(1,19,5,36,buff);
switch(flag)
{
//case 1://find();
  //		break;
//case 2://replace();
	//	break;
}
break;
}
if(sc==75||sc==77)
{
dhighlight(0,20,0,26,32);
dhighlight(2+flag,21,2+flag,33,32);
restoremem(1,19,5,36,buff);
if(sc==75)
{
helpmenu();
break;
}
if(sc==77)
{
filemenu();
break;
}
}
else if(ac==27)
{
dhighlight(0,20,0,26,32);
dhighlight(2+flag,21,2+flag,33,32);
restoremem(1,19,5,36,buff);
break;
}
}
}

void getcharacter(int *sc,int *ac)
{
union REGS i,o;
i.h.ah=0;
int86(0x16,&i,&o);
*sc=o.h.ah;
*ac=o.h.al;
}
void setcursepos(int pg,int r,int c)
{
union REGS i,o;
int x=r,y=c,page=pg;
i.h.ah=2;
i.h.bh=page;
i.h.dh=x;
i.h.dl=y;
int86(0x10,&i,&o);
}
void highlight(int r1,int c1,int r2,int c2,int attr)
{
int i,j;
char far *v;
for(i=r1;i<=r2;i++)
{
for(j=c1;j<=c2;j++)
{
v=vm+i*160+j*2;
v++;
*v=*v-112+attr;
}
}
}
void dhighlight(int r1,int c1,int r2,int c2,int attr)
{
int i,j;
char far *v;
for(i=r1;i<=r2;i++)
{
for(j=c1;j<=c2;j++)
{
v=vm+i*160+j*2;
v++;
*v=*v+112-attr;
}
}
}

void charanal(int sc,int ac)
{
static int i,pg,r=2,c=1;
char far *v;
char ch;
setcursepos(0,r,c);
_setcursortype(_NORMALCURSOR);
if(N==1)
{
for(i=0;i<=23;i++)
t[i]=1;
r=2;
c=1;
N=0;
}
if(O==1)
{
r=2;
c=1;
O=0;
}
if(ac==0)
{
switch(sc)
{
case 75:if(r==2 && c==1)
		;
		else if(c==1 && r!=2)
		{
		r--;
		c=t[r];
		}
		else if(c>1)
		{
		c--;
		}
		setcursepos(0,r,c);
		break;
case 77:if(c==78)
		{
		r++;
		c=1;
		}
		else
		c++;
		setcursepos(0,r,c);
		break;
case 72:if(r==2)
		;
		else
		r--;
		setcursepos(0,r,c);
		break;
case 80:if(r==22)
		;
		else
		r++;
		setcursepos(0,r,c);
		break;
case 33:filemenu();
		break;
case 18:editmenu();
		break;
case 35:helpmenu();
		break;
case 31:searchmenu();
		break;
case 60:N=0;
		O=0;
		C=1;
		break;
case 62:exit();
		break;
case 83: if(t[r]>c)
		 {
		 moveright(r,c);
		 t[r]--;
		 }
		 else
		 moveup(r+1,c);
		 setcursepos(0,r,c);
		 break;

}
}
else
{
switch(ac)
{
case 19:savefile();
		break;
case 14:newfile();
		break;
case 13:if(r==22)
		;
		else
		{
		if(t[22]<=1)
		movedown(r,c);
		r++;
		c=1;
		}
		setcursepos(0,r,c);
		break;
case 8:if(c>1)
		{
		if(t[r]>c)
		{
		c--;
		moveright(r,c);
		t[r]--;
		}
		else
		{
		c--;
		t[r]--;
		write2vdu(' ',16,r,c);
		}
		setcursepos(0,r,c);
		}
		else if(c==1 && r!=2)
		{
		c=t[r-1];
		setcursepos(0,r-1,t[r-1]);
		moveup(r,1);
		r--;
		write2vdu(' ',16,r,c);
		}
		break;
default://if(33<= ac >=126)
		//{
		if(t[r]>c)
		{
		moveleft(r,c);
		write2vdu((char)ac,31,r,c);
		if(t[r]==78)
		{
		for(i=1;r+i<=22;i++)
		{
		if(t[r+i]!=78)
		break;
		else
		;
		}
		t[r+i]++;
		}
		else
		t[r]++;
		c++;
		}
		else
		{
		if(r>=22&&c==78)
		break;
		else if(c==78)
		{
		write2vdu((char)ac,31,r,c);
		t[r]=c;
		c=1;
		r++;
		}
		else
		{
		write2vdu((char)ac,31,r,c);
		c++;
		if(t[r]<c)
		t[r]=c;
		}
		}
		setcursepos(0,r,c);
		//}
		break;
}
}
}
void bcharanal(int sc,int ac)
{
_setcursortype(_NOCURSOR);
if(ac==0)
{
switch(sc)
{

case 33:filemenu();
		break;
case 18:editmenu();
		break;
case 35:helpmenu();
		break;
case 31:searchmenu();
		break;
case 62:exit();
		break;
}
}
else
{
switch(ac)
{
case 14:newfile();
		break;
}
}
}

blank()
{
int r,c;
_setcursortype(_NOCURSOR);
for(r=1;r<=23;r++)
{
for(c=0;c<=79;c++)
{
write2vdu(177,31,r,c);
}
}
}
void main()
{
int pg=0,r=0,c=0;
int button,i=0,sc,ac;
char *buff,key;
clrscr();
A:
menu();
blank();
while(1)
{
if(N==1 || O==1)
break;
_setcursortype(_NOCURSOR);
getcharacter(&sc,&ac);
bcharanal(sc,ac);
}
if(N==1 || O==1 )
{
while(1)
{
getcharacter(&sc,&ac);
charanal(sc,ac);
if(C==1)
goto A;
}
}
}