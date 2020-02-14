#include<iostream>
#include<Windows.h>
#include<graphics.h>
#include<string.h>
#include<time.h>  //time(NULL)的头文件
#include<conio.h>

using namespace std;

#define WIDTH 550
#define HEIGHT 660

#define BLOCK_COUNT 5  //表示一共有几种类型的方块
#define BLOCK_WIDTH 5  //表示大方块宽度 (把整个二维数组看作一个方块)
#define BLOCK_HEIGHT 5  //表示大方块高度

#define UNIT_SIZE 20

#define FORM 4  //每种方块有4种形态

#define VISIT_ROW 30
#define VISIT_COL 15

#define START_X 130  //表示方块(整个5X5的二维数组)一开始落下的x坐标
#define START_Y 15   //表示方块(整个5X5的二维数组)一开始落下的y坐标

#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_DOWN 80

int score=0;
int grade=0;
int NextIndex=-1; //下一个方块的序号
int BlockIndex=-1;
int speed=500;   //降落速度

int color[BLOCK_COUNT]={
	GREEN,CYAN,MAGENTA,BROWN,BLUE
};

int visit[VISIT_ROW][VISIT_COL];
int visitColor[VISIT_ROW][VISIT_COL];  //设置游戏区颜色使用

int minX=30;  //30指的是游戏区左上角内框的坐标
int minY=30;

typedef enum{
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_DOWN,
	BLOCK_LEFT
}block_dir_t;

typedef enum{
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

int block[BLOCK_COUNT*FORM][BLOCK_HEIGHT][BLOCK_WIDTH]={  //定义一个三维数组,第一维表示有3种方块,每种方块4种形态,第二维,三维表示方块具体样式
	{   //I型方块
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	{
		0,0,0,0,0,   //省略大括号以后,编译器自动根据定义的行列数来给数组赋值
		0,0,0,0,0,
		0,1,1,1,0,
		0,0,0,0,0,
		0,0,0,0,0
	},
	{
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	{
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{  //L型
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,1,1,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,1,0,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,0,1,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,0,0,1,0},
		{0,1,1,1,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{  //T型
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,1,1,1,0},
		{0,0,0,0,0}
	},
	{  
		{0,0,0,0,0},
		{0,1,0,0,0},
		{0,1,1,0,0},
		{0,1,0,0,0},
		{0,0,0,0,0}
	},
	{  
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{  
		{0,0,0,0,0},
		{0,0,0,1,0},
		{0,0,1,1,0},
		{0,0,0,1,0},
		{0,0,0,0,0}
	},
	{  //Z型 
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,0,1,1,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,0,0,1,0},
		{0,0,1,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,0,1,1,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,1,1,0},
		{0,0,0,1,0},
		{0,0,0,0,0}
	},
	{   //田字型
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{  
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{  
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	},
	{ 
		{0,0,0,0,0},
		{0,1,1,0,0},
		{0,1,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0}
	}
};

void welcome();
void initGameBackground();
void nextBlock();
void clearBlock();
void drawBlock(int x,int y);
void newBlock();
void move();
void isGameOver();
void clearBlock(int x,int y,block_dir_t dir);  //函数重载 清除指定位置的方块
void drawBlock(int x,int y,int blockIndex,block_dir_t dir); //函数重载  在游戏主框绘制方块
void wait(int interval);
int moveable(int x,int y,move_dir_t moveDir,block_dir_t blockDir);
bool rotatable(int x,int y,block_dir_t dir);
void mark(int x,int y,int blockIndex,block_dir_t dir);
void check();
void clearDown(int i);
void addScore(int lines);
void updataGrade();

int main(){
	welcome();
	initGameBackground();

	//产生新方块
	nextBlock();

	//初始化visit数组
	memset(visit,0,sizeof(visit));

	Sleep(500);  //不要一产生新方块,就立刻让方块落下,要优化用户体验

	while(1){
		newBlock();
		//检查并且消除满行,更新分数,提高速度
		check();
	}

	system("pause");
	closegraph();
	return 0;
}

void welcome(){
	initgraph(WIDTH,HEIGHT);

	IMAGE bg_img;
	loadimage(&bg_img,_T("俄罗斯方块.jpg"),WIDTH,HEIGHT,true);
	putimage(0,0,&bg_img);

	//设置窗口标题  handle:(用手)触摸,句柄
	HWND window=GetHWnd();   //得到当前窗口句柄
	SetWindowText(window,_T("Mr.z's 俄罗斯方块")); //这一行的实参window,是指上边的HWND类型的变量
	
	//设置字体
	setfont(40,0,_T("微软雅黑"));
	settextcolor(WHITE);
	outtextxy(205,200,_T("俄罗斯方块"));
	
	setfont(22,0,_T("楷体"));
	setcolor(WHITE);
	outtextxy(175,300,_T("乐趣,从生活的点滴开始!"));

	Sleep(2000);
}

void initGameBackground(){
	char str[16];
	//清除之前的屏幕
	cleardevice();  //device: 设备
	rectangle(27,27,336,635);
	rectangle(29,29,334,633);
	rectangle(370,50,515,195);

	setfont(24,0,_T("楷体"));
	setcolor(LIGHTGRAY);
	outtextxy(405,215,_T("下一个:"));

	settextcolor(RED);
	outtextxy(405,280,_T("分数:"));
	sprintf(str,"%d",score);   //此行的str是指上边定义的字符数组变量str
	outtextxy(415,310,str);

	outtextxy(405,375,_T("等级:"));
	sprintf(str,"%d",grade);
	outtextxy(415,405,str);

	setcolor(LIGHTBLUE);
	outtextxy(390,475,_T("操作说明:"));
	outtextxy(390,500,"上箭头: 旋转");
	outtextxy(390,525,"下箭头: 下降");
	outtextxy(390,550,"左箭头: 左移");
	outtextxy(390,575,"右箭头: 右移");
	outtextxy(390,600,"  空格: 暂停");
}

void nextBlock(){
	clearBlock();  //先清除右上角区域
	/***************************************************************************************************
	 *随机选择一种方块(产生随机数) random:随机的  seed:种子 jingye
	 *每次随机种子不同 (随机种子由srand()函数设置,指的是srand()里的参数)
	 *如果随机种子是固定值,那么就是伪随机数(虽然每次产生的数不同但是有规律))
	 ***************************************************************************************************
	 *以下是对srand();函数的介绍
	 *定义于头文件 <cstdlib>
	 *函数功能:设置随机数种子
	 *void srand( unsigned seed );  seed:种子
	 *以值 seed 播种 std::rand() 所用的随机数生成器。 
	 *若在任何到 srand() 的调用前使用 rand() ，则 rand() 表现为如同它被以 srand(1) 播种。 
	 *每次以同一 seed 播种 rand() 时，它必须产生相同的值数列。 
	 *srand() 不保证为线程安全
	 *********************************************************************************************************************
	 *rand()函数产生的随机数严格意义讲不是真的随机数,它里面采用了线性同余法计算出随机数(这种方式产生的随机数在大范围有规律)
	 *该函数默认种子是1,而且rand函数没有参数,所以每次产生的随机数是相同的,即每次运行程序,产生的随机数是一样的
	 *这就失去了部分随机的意义,所以一般配合srand函数使用(即使用srand函数设置随机数种子)
	 */
	srand(time(NULL)); //设置随机数种子
	NextIndex=rand()%BLOCK_COUNT;  //将产生的随机数除以5(BLOCK_COUNT)并取余数,结果只会是0,1,2,3,4五种情况,对应5(BLOCK_COUNT)种方块
	drawBlock(398,78);
}

void clearBlock(){
	//■
	settextcolor(BLACK);
	setfont(UNIT_SIZE,0,"楷体");
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			outtextxy(391+j*UNIT_SIZE,71+i*UNIT_SIZE,"■");
		}
		cout<<endl;
	}
}

void drawBlock(int x,int y){
	clearBlock();  //先清除右上角区域
	setfont(UNIT_SIZE,0,"楷体");
	setcolor(color[NextIndex]);

	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[NextIndex*FORM][i][j]==1){
				outtextxy(x+j*UNIT_SIZE,y+i*UNIT_SIZE,"■");
			}
		}
	}
}

void newBlock(){
	//确定下一个使用的方块类型
	BlockIndex=NextIndex;
	//绘制准备下降的方块
	drawBlock(START_X,START_Y);
	//延迟一会
	Sleep(100);
	//绘制右上角下一个方块
	nextBlock();
	//system("pause");  //测试代码
	move();
}

void move(){
	int x=START_X;
	int y=START_Y;
	int k=0;  //清除方块时候,传参数用k,k表示方块距离游戏区顶部内线的垂直距离,一开始为0

	int curSpeed=speed;  //当前速度
	block_dir_t blockDir=BLOCK_UP;
	//检测游戏是否结束
	isGameOver();
	//方块持续下降
	while(1){
		//当不按向下键时,恢复下降速度
		curSpeed=speed;
		if(kbhit()){
			int key=getch();
			if(key==KEY_SPACE){
				system("pause");
			}
		}
		//清除当前方块
    	clearBlock(x,k,blockDir);  //x表示方块离最左侧界面的偏移量
		if(kbhit()){
			int key=getch();
			if(key==KEY_UP){
				block_dir_t nextDir=(block_dir_t)((blockDir+1)%4);  //强制类型转换
				if(rotatable(x,y+k,nextDir)){
					blockDir=nextDir;
				}
			}else if(key==KEY_RIGHT){
				if(moveable(x,y+k+UNIT_SIZE,MOVE_RIGHT,blockDir)){
					x+=UNIT_SIZE;
				}
			}else if(key==KEY_LEFT){
				if(moveable(x,y+k+UNIT_SIZE,MOVE_LEFT,blockDir)){
					x-=UNIT_SIZE;
				}
			}else if(key==KEY_DOWN){
				curSpeed=50;
			}
		}
		//绘制当前方块
		k+=UNIT_SIZE;   //这是修改后的代码,不要乱动
		drawBlock(x,y+k,BlockIndex,blockDir);
		wait(curSpeed);   //设置降落速度 让画出来的方块延迟被清除,否则用户看不到
		//方块的落地处理
		if(!moveable(x,y+k,MOVE_DOWN,blockDir)){
			mark(x,y+k,BlockIndex,blockDir);
			break;
		}
	}
}

void isGameOver(){
	if(!moveable(START_X,START_Y,MOVE_DOWN,BLOCK_UP)){
		settextcolor(WHITE);
		setfont(45,0,"微软雅黑");
		outtextxy(75,300,"Game Over!");
		Sleep(1000);  //强制画面显示1秒
		system("pause");
		closegraph();  //关闭画面,释放资源
		exit(0);
	}
}

//参数x指的是方块左上角(整个5x5二维数组)的x坐标 (距离整个左界面的x坐标)
//参数y指的是方块左上角(整个5x5二维数组)距离游戏区域顶部内线的垂直距离
void clearBlock(int x,int y,block_dir_t dir){
	setcolor(BLACK);
	setfont(UNIT_SIZE,0,"楷体");
	int id=BlockIndex*4+dir;

	//■
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1){
				outtextxy(x+j*UNIT_SIZE,y+i*UNIT_SIZE+START_Y,"■");
			}
		}
	}
}

void drawBlock(int x,int y,int blockIndex,block_dir_t dir){
	setcolor(color[blockIndex]);
	setfont(UNIT_SIZE,0,"楷体");  
	int id=blockIndex*4+dir;
	//■
	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1){
				outtextxy(x+j*UNIT_SIZE,y+i*UNIT_SIZE-2,"■");  //为了解决压线问题,画方块的时候往上两个像素
			}
		}
	}
}

void wait(int interval){   //interval:间隔
	int count=interval/10;
	for(int i=0;i<count;i++){
		Sleep(10);
		if(kbhit()){
			return;
		}
	}
}

//如果在指定位置可以向指定方向移动,就返回1,否则就返回0
int moveable(int x,int y,move_dir_t moveDir,block_dir_t blockDir){
	//计算当前方块的左上角在30X15的游戏区中的位置(行列坐标)
	int currentX=(y-minY)/UNIT_SIZE; 
	int currentY=(x-minX)/UNIT_SIZE;
	
	int id=BlockIndex*4+blockDir;

	int ret=1;

	if(moveDir==MOVE_DOWN){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){
				if(block[id][i][j]==1 &&  //有方块的时候才需要判断
					//具体分析参考下边左移和右移
					(currentX+i+1>(VISIT_ROW-1)      //判断方块到底
					||(visit[currentX+i+1][currentY+j]==1))){  //方块下已经有方块
					//满足则不能下移
					ret=0;
				}
			}
		}
	}else if(moveDir==MOVE_RIGHT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){
				if(block[id][i][j]==1 &&  //有方块的时候才需要判断
					//currentY+j+1是为了求5x5数组中元素值为1的元素在30x15的大数
					//组中向右移动一步后的列下标 具体分析参考下边对向左移动的解释
					//14是因为数组下标从0开始,第15列的列下标就是14
					//currentY+j是求原本方块在大数组中的列下表,加1就是向右一步,如果大于14就不能右移,可以等于14
					(currentY+j+1>(VISIT_COL-1)      //判断方块是否是最右边
					||(visit[currentX+i][currentY+j+1]==1))){  //判断方块在游戏区但是方块右侧已经有方块
					//满足则不能右移
					ret=0;
				}
			}
		}
	}else if(moveDir==MOVE_LEFT){
		for(int i=0;i<BLOCK_HEIGHT;i++){
			for(int j=0;j<BLOCK_WIDTH;j++){
				if(block[id][i][j]==1&& //有方块才判断
					//currentY是指整个5x5的二维数组在30x15大二维数组中的左上角的列下标,
					//即使两个二维数组,下标都是从0开始的,求数组元素值是1的列下标
					//的时候,也只需要加j,不需要额外加1 
					//传进来的x像素坐标是方块原本位置的坐标
					//这里是判断如果左移后的列小于0就不能左移,列数可以等于0
					(currentY+j-1<0   
					||(visit[currentX+i][currentY+j-1]==1))){  //左有方块
					ret=0;
				}
			}
		}
	}

	return ret;
}

//参数dir是即将旋转的方向
bool rotatable(int x,int y,block_dir_t dir){
	int id=BlockIndex*4+dir;
	//将xy转换成行列坐标
	int currentX=(y-minY)/20;
	int currentY=(x-minX)/20;

	//moveable的参数直接传像素xy坐标就可以,因为moveable函数内有转换行列坐标
	if(!moveable(x,y,MOVE_DOWN,dir)){  //首先判断能不能向下移动,如果不能就不能旋转
		return false;
	}

	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1&&(currentY+j<0||currentY+j>14||visit[currentX+i][currentY+j]==1)){
				return false;
			}
		}
	}

	return true;
}

void mark(int x,int y,int blockIndex,block_dir_t dir){
	int id=blockIndex*4+dir;
	int x1=(y-minY)/UNIT_SIZE;   //传进来的参数xy指的是像素坐标  定义的x1y1是行列坐标
	int y1=(x-minY)/UNIT_SIZE;

	for(int i=0;i<BLOCK_HEIGHT;i++){
		for(int j=0;j<BLOCK_WIDTH;j++){
			if(block[id][i][j]==1){
				visit[x1+i][y1+j]=1;
				visitColor[x1+i][y1+j]=color[blockIndex];
			}
		}
	}
}

void check(){
	for(int i=(VISIT_ROW-1);i>=0;i--){
		//for循环的判断条件只要首次不满足(即只要判断条件一次不满足),就会结束for循环
		//当循环次数确定时,推荐使用for循环(从for循环的定义概念来看,继续执行没有意义)
		for(int j=(VISIT_COL-1);j>=0&&(visit[i][j]==1);j--){
			//这里for循环结束有两种可能:1.j<0 2.visit[i][j]不为1
			//只有当j==0并且这一行都是1的时候才会执行if里的语句
			if(j==0){
				//此时,这一行已经满了,应该消除
				clearDown(i);  //清除第i行,并且下降上面的方块
				//当清除掉满的一行后,上边的方块会下降到这一行,此时应该再次检查这一行,所以i++
				i++;
			}
		}
	}
}

void clearDown(int x){
	int clearLines=0;
	int j=0;
	//■
	for(int i=x;i>0;i--){
		for(j=0;j<VISIT_COL;j++){
			visit[i][j]=visit[i-1][j]; //把上边的直接下移
			visitColor[i][j]=visitColor[i-1][j];
			if(visit[i][j]){
				setfont(UNIT_SIZE,0,"楷体");
				setcolor(visitColor[i][j]);
				outtextxy(minX+UNIT_SIZE*j,minY+UNIT_SIZE*i+2,"■");//为了解决压线问题,清除一行的时候,往上2像素
			}else{
				setfont(UNIT_SIZE,0,"楷体");
				setcolor(BLACK);
				outtextxy(minX+UNIT_SIZE*j,minY+UNIT_SIZE*i+2,"■");
			}
		}
	}
	clearLines++;
	//清除顶行
	setcolor(BLACK);
	for(int j=0;j<VISIT_COL;j++){
		setfont(UNIT_SIZE,0,"楷体");
		visit[0][j]=0;
		outtextxy(20*j+minX,minY+3,"■");
	}

	//更新分数
	addScore(clearLines);
	//更新等级
	updataGrade();
}

void addScore(int lines){
	char str[10];
	setfont(24,0,_T("楷体"));

	setcolor(RED);
	score+=lines*10;
	sprintf(str,"%d",score);
	outtextxy(415,310,str);
}

void updataGrade(){
	setfont(24,0,_T("楷体"));
	//100分一级
	grade=score/100+1;
	char str[16];
	sprintf(str,"%d",grade);
	outtextxy(425,405,str);

	//更新速度
	speed=500-grade*50;
	//最快是100毫秒
	if(speed<=100){
		speed=100;
	}
}