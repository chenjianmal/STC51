/********************************************************************
* 文件名  ： 液晶1602显示.c
* 描述    :  该程序实现了对液晶1602的控制。
             两行字符从右向左每个0.5秒移动一位。
***********************************************************************/
#include<reg52.h>
#include<intrins.h>
#define uchar unsigned char
#define uint  unsigned int

//这三个引脚参考资料
sbit E=P2^7;		//1602使能引脚
sbit RW=P2^6;		//1602读写引脚	
sbit RS=P2^5;		//1602数据/命令选择引脚


/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间大概为5US。
* 输入 : 无
* 输出 : 无
***********************************************************************/

void delay()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
/********************************************************************
* 名称 : Delay_1ms()
* 功能 : 延时子程序，延时时间为 1ms * x
* 输入 : x (延时一毫秒的个数)
* 输出 : 无
***********************************************************************/
void Delay(uint i)
{
	uint x,j;
	for(j=0;j<i;j++)
	for(x=0;x<=148;x++);	
}
/********************************************************************
* 名称 : bit Busy(void)
* 功能 : 这个是一个读状态函数，读出函数是否处在忙状态
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/
bit Busy(void)
{
	bit busy_flag = 0;
	RS = 0;
	RW = 1;
	E = 1;
	delay();
	busy_flag = (bit)(P0 & 0x80);
	E = 0;
	return busy_flag;
}
/********************************************************************
* 名称 : wcmd(uchar del)
* 功能 : 1602命令函数
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/
void wcmd(uchar del)
{
	while(Busy());
	RS = 0;
	RW = 0;
	E = 0;
	delay();
	P0 = del;
	delay();
	E = 1;
	delay();
	E = 0;
}

/********************************************************************
* 名称 : wdata(uchar del)
* 功能 : 1602写数据函数
* 输入 : 需要写入1602的数据
* 输出 : 无
***********************************************************************/

void wdata(uchar del)
{
	while(Busy());
	RS = 1;
	RW = 0;
	E = 0;
	delay();
	P0 = del;
    delay();
	E = 1;
	delay();
	E = 0;
}

/********************************************************************
* 名称 : rdata(uchar del)
* 功能 : 1602读数据函数
* 输入 : 需要读出1602的数据
* 输出 : 无
***********************************************************************/

uchar rdata(void)
{
	RS = 1;
	RW = 1;
	E = 1;
	delay();
	return P0;
}

/********************************************************************
* 名称 : L1602_init()
* 功能 : 1602初始化，请参考1602的资料
* 输入 : 无
* 输出 : 无
***********************************************************************/
void L1602_init(void)
{
	wcmd(0x38);
	Delay(5);
	wcmd(0x38);
	Delay(5);
	wcmd(0x38);
	Delay(5);
	wcmd(0x38);
	wcmd(0x08);	
	wcmd(0x0c);
	wcmd(0x04);
	wcmd(0x01);
}

/********************************************************************
* 名称 : L1602_char(uchar hang,uchar lie,char sign)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，调用该函数如下
		 L1602_char(1,5,'b')
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_char(uchar hang,uchar lie,char sign)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	wcmd(a);
	wdata(sign);
}

/********************************************************************
* 名称 : L1602_string(uchar hang,uchar lie,uchar *p)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符开始显示"ab cd ef" ，调用该函数如下
	 	 L1602_string(1,5,"ab cd ef;")
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_string(uchar hang,uchar lie,uchar *p)
{
	uchar a,b=0;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	while(1)
	{
		wcmd(a++);	
		if((*p == '\0')||(b==16)) break;
		b++;
		wdata(*p);
		p++;
	}
}

/********************************************************************
* 名称 : Main()
* 功能 : 主函数
* 输入 : 无
* 输出 : 无
***********************************************************************/
void Main()
{
	uchar i=0,j,*p,*q;
	uchar table1[]={"  Welcome To   "};
	uchar table2[]={" The MCU World "};
	Delay(5);
	Delay(5);
	L1602_init();
	while(1)
	{
		p=table1;
		q=table2;
		//这里的算法，看起来比较麻烦，就是根据下面的注释掉的来找的规律。
		for(i=0;i<17;i++)
		{
			for(j=0;j<i;j++)
			{
				L1602_char(1,17-i+j,*(p+j));
				L1602_char(2,17-i+j,*(q+j));
			}
			Delay(500);
		}
		Delay(2000);
		wcmd(0x01);
	}
	/*		
	L1602_char(1,16,*p);		
	Delay(400);	
	L1602_char(1,15,*p);		
	L1602_char(1,16,*(p+1));
	Delay(400);	
	L1602_char(1,14,*p);		
	L1602_char(1,15,*(p+1));
	L1602_char(1,16,*(p+2));
	Delay(400);	
	*/
}
