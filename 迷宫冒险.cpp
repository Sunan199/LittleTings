#include <iostream>
#include <fstream>  //读写文件，用于存档读档
#include <conio.h>  //键盘输入，kbhit() 函数
//测试代码
#include <Windows.h>
#include <ctime>

using namespace std;

const int M = 50;  //控制最大行列数，同时便于函数编写
const int N = 50;
const char savedFile[] = "saved_game.txt";

#define GO_TO_BEGIN goto begin  //无条件跳转至begin，同时限制了goto的功能
 
int row, col;   //人的初始位置
char key = '0';     //用户输入的按键
static int d2 = 0;     //敌人数量初始化
static int d3 = 0;     //宝藏数量初始化

//生成迷宫，m和n分别表示迷宫的行数和列数
//difficulty代表难易程度，可以采用不同级别表示


//人 
class People{
	private:
		int row,col;
		
	public:
		People(){
			row=1;
			col=1;
		}
		
		People(int r,int c){
			row=r;
			col=c;
		}
		
	    ~People(){
			row=1;
			col=1;
		}
		friend void generateMap(char matrix[][N], int m, int n, int difficulty);
		friend void displayMap(char matrix[][N], int m, int n);
		friend bool saveGame(char matrix[][N], int m, int n, int dif, int ach);
		friend bool loadGame(char matrix[][N], int& m, int& n, int& dif, int& ach);
		friend bool movePlayer(char matrix[][N], int m, int n, int dir);
		friend void moveEnemy(char matrix[][N], int m, int n);
};


//怪物 
class Monster{
	private:
		int id;		
		int row,col;	
	public:
		Monster(){
			row=1;
			col=1;
		}
			
		Monster(int r,int c){
			row=r;
			col=c;
		}
			
	    ~Monster(){
			row=1;
			col=1;
		}	
		friend void generateMap(char matrix[][N], int m, int n, int difficulty);
		friend void displayMap(char matrix[][N], int m, int n);
		friend bool saveGame(char matrix[][N], int m, int n, int dif, int ach);
		friend bool loadGame(char matrix[][N], int& m, int& n, int& dif, int& ach);
		friend void moveEnemy(char matrix[][N], int m, int n);
}Monster[N];


//宝藏 
class gold{
	private:
		int id;		
		int row,col;	
	public:
		gold(){
			row=1;
			col=1;
		}
			
		gold(int r,int c){
			row=r;
			col=c;
		}
			
	    ~gold(){
			row=1;
			col=1;
		}	
		
		friend void generateMap(char matrix[][N], int m, int n, int difficulty);
		friend void displayMap(char matrix[][N], int m, int n);
		friend bool saveGame(char matrix[][N], int m, int n, int dif, int ach);
		friend bool loadGame(char matrix[][N], int& m, int& n, int& dif, int& ach);
		friend void moveEnemy(char matrix[][N], int m, int n);	
}gold[N];





void generateMap(char matrix[][N], int m, int n, int difficulty) {
    int i, j;
    srand(time(NULL));  //设置随机数种子
    //初始化迷宫，用空格表示通道，用"#"表示边缘墙壁
    for (i = 0; i < M; i++) {
        for (j = 0; j < N; j++) {
            //边界位置设置为#，由于打印迷宫时不会打印超出的部分，所以超出部分不影响
            if (i == 0 || i == m - 1 || j == 0 || j == n - 1) {
                matrix[i][j] = '#';
            }
            else {
                matrix[i][j] = ' ';
            }
        }
    }
 
    //d用于控制难度系数，影响墙、宝藏、敌人的数量
    int d1 = 0;
    switch (difficulty) {
    case 1: {
        d1 = 9;
        break;
    }
    case 2: {
        d1 = 7;
        break;
    }
    case 3: {
        d1 = 5;
        break;
    }
    }
    //生成随机的迷宫内墙壁
    for (i = 0; i < m * n / d1; i++) {
        matrix[rand() % (m - 2) + 1][rand() % (n - 2) + 1] = '*';
    }
    //随机生成敌人
    switch (difficulty) {
    case 1: {
        d2 = 2;
        break;
    }
    case 2: {
        d2 = 4;
        break;
    }
    case 3: {
        d2 = 6;
        break;
    }
    }
    //随机生成敌人
    for (i = 0; i < d2; i++) {
        matrix[rand() % (m - 2) + 1][rand() % (n - 2) + 1] = '!';
    }
    //生成随机的宝藏
    switch (difficulty) {
    case 1: {
        d3 = 2;
        break;
    }
    case 2: {
        d3 = 4;
        break;
    }
    case 3: {
        d3 = 6;
        break;
    }
    }
    //随机生成宝藏
    for (i = 0; i < d3; i++) {
        matrix[rand() % (m - 2) + 1][rand() % (n - 2) + 1] = '$';
    }
}
 
//打印迷宫
void displayMap(char matrix[][N], int m, int n) {
    system("cls");  //清空屏幕
    int i, j;
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }
}
 
//游戏存档，将游戏的当前状态存储到saved_game.txt文件中
//如果游戏存档成功，则返回true；否则，返回false
bool saveGame(char matrix[][N], int m, int n, int dif, int ach) {
    ofstream outfile("d://saved_game.txt", ios::out);  // 打开文件流
    if (!outfile) {
        cerr << "存档文件打开失败！" << endl;
        return false;
    }
 
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            outfile << matrix[i][j];
        }
        outfile << '\n';
    }
 
    outfile << m << ' ' << n << ' ' << dif << ' ' << ach;
 
    outfile.close();  // 关闭文件流
 
    return true;
}
 
//读档游戏，读取saved_game.txt游戏存档文件，恢复游戏
//如果游戏读档成功，则返回true；否则，返回false
bool loadGame(char matrix[][N], int& m, int& n, int& dif, int& ach) {
    ifstream file("d://saved_game.txt"); // 打开文本文件
 
 
    if (file.is_open()) {
        // 逐行读取包含空格的字符，并存储到二维数组中
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                file.get(matrix[i][j]); // 使用get()函数读取字符，包括可能的空格
            }
            file.ignore(1); // 忽略每行末尾的换行符
        }
        //创建一个字符串变量用于存放读取的整型数据
        char tmp[20];
        file >> tmp;
        //一一读取
        m = atoi(tmp);
        file >> tmp;
        n = atoi(tmp);
        file >> tmp;
        dif = atoi(tmp);
        file >> tmp;
        ach = atoi(tmp);
        file.close(); // 关闭文件
    }
    return true;
}
 
//判断游戏当前状态：找到宝藏返回1；与敌人相撞返回2；继续游戏返回3
int statusGame(char matrix[][N], int m, int n) {
    switch (key) {
    case 'w': // 向上移动
        if (matrix[row - 1][col] == '$') {
            return 1;
        }
        else if (matrix[row - 1][col] == '!') {
            return 2;
        }
        else
            break;
    case 's': // 向下移动
        if (matrix[row + 1][col] == '$') {
            return 1;
        }
        else if (matrix[row + 1][col] == '!') {
            return 2;
        }
        else
            break;
    case 'a': // 向左移动
        if (matrix[row][col - 1] == '$') {
            return 1;
        }
        else if (matrix[row][col - 1] == '!') {
            return 2;
        }
        else
            break;
    case 'd': // 向右移动
        if (matrix[row][col + 1] == '$') {
            return 1;
        }
        else if (matrix[row][col + 1] == '!') {
            return 2;
        }
        else
            break;
    }
    return 3;
}
 
//判断位置(x, y)是否是墙，y表示行号，x表示列号
bool isWall(char matrix[][N], int x, int y) {
    if (matrix[x][y] == '*') {
        return true;
    }
    return false;
}
 
//判断位置(x, y)是否出界，y表示行号，x表示列号
bool isOutside(char matrix[][N], int x, int y) {
    if (matrix[x][y] == '#') {
        return true;
    }
    return false;
}
 
//判断位置(x, y)是否是敌人，y表示行号，x表示列号
bool isEnemy(char matrix[][N], int x, int y) {
    if (matrix[x][y] == '!') {
        return true;
    }
    return false;
}
 
//判断位置(x, y)是否是宝藏，y表示行号，x表示列号
bool isTreasure(char matrix[][N], int x, int y) {
    if (matrix[x][y] == '$') {
        return true;
    }
    return false;
}
 
//移动玩家的位置，如果移动成功，返回true；否则返回false
//dir表示输入的方向，（1,2,3,4）分别对应（上,下,左,右）
bool movePlayer(char matrix[][N], int m, int n, int dir) {
    if (statusGame(matrix, m, n) == 1) {
        //碰到宝藏时的实现方式
        switch (key) {
        case 'w': {// 向上移动
            matrix[row][col] = ' ';
            row--;
            matrix[row][col] = '@';
            break;
        }
        case 's': {// 向下移动
            matrix[row][col] = ' ';
            row++;
            matrix[row][col] = '@';
            break;
        }
        case 'a': {// 向左移动
            matrix[row][col] = ' ';
            col--;
            matrix[row][col] = '@';
            break;
        }
        case 'd': {// 向右移动
            matrix[row][col] = ' ';
            col++;
            matrix[row][col] = '@';
            break;
        }
        }
    }
    //正常移动的实现方式
    else {
        switch (key) {
        case 'w': // 向上移动
            if (matrix[row - 1][col] == ' ') {
                dir = 1;
                matrix[row][col] = ' ';
                row--;
                matrix[row][col] = '@';
            }
            break;
        case 's': // 向下移动
            if (matrix[row + 1][col] == ' ') {
                dir = 2;
                matrix[row][col] = ' ';
                row++;
                matrix[row][col] = '@';
            }
            break;
        case 'a': // 向左移动
            if (matrix[row][col - 1] == ' ') {
                dir = 3;
                matrix[row][col] = ' ';
                col--;
                matrix[row][col] = '@';
            }
            break;
        case 'd': // 向右移动
            if (matrix[row][col + 1] == ' ') {
                dir = 4;
                matrix[row][col] = ' ';
                col++;
                matrix[row][col] = '@';
            }
            break;
        }
    }
    if (matrix[row][col] != '@')
        return false;
    return true;
}
 
//随机移动所有敌人的位置
void moveEnemy(char matrix[][N], int m, int n) {
    int i = 0;
    int j = 0;
    int c = 0;  //用于修正迭代的问题
    int d = 0;
    int e = 0;
    while (1) {
        //迭代找到所有的敌人(!)
        for (i = 1; i < m - 1; i++) {
            for (j = 1; j < n - 1; j++) {
                if (i == c && j == d) {
                    c = 0;
                    d = 0;
                    continue;  //避免敌人向下移动时重新进入迭代
                }
                if (j == e) {
                    e = -1;
                    continue;  //避免敌人向右移动时重新进入迭代
                }
                //迭代至敌人时
                if (matrix[i][j] == '!') {
                    int a = 1;
                    int b = 0;
                    while (a) {
                        switch (rand() % 4 + 1) {  //生成1到4的随机数
                            //随机上下左右移动
                        case 1: // 向上移动
                            if (matrix[i - 1][j] == ' ') {
                                //交换敌人和通路，表示敌人移动一次
                                matrix[i][j] = ' ';
                                matrix[i - 1][j] = '!';
                                a = 0; //移动成功，跳出while循环
                                break;
                            }
                            //如果目标位置不是空格
                            else
                                //跳出switch循环并重新进入，继续随机选择，直到成功移动
                                break;
                        case 2: // 向下移动
                            if (matrix[i + 1][j] == ' ') {
                                matrix[i][j] = ' ';
                                //i--;
                                matrix[i + 1][j] = '!';
                                a = 0;
                                c = i + 1;  //记录位置，避免再次进入迭代重复移动
                                d = j;
                                break;
                            }
                            else
                                break;
                        case 3: // 向左移动
                            if (matrix[i][j - 1] == ' ') {
                                matrix[i][j] = ' ';
                                //i--;
                                matrix[i][j - 1] = '!';
                                a = 0;
 
                                break;
                            }
                            else
                                break;
                        case 4: // 向右移动
                            if (matrix[i][j + 1] == ' ') {
                                matrix[i][j] = ' ';
                                //i--;
                                matrix[i][j + 1] = '!';
                                a = 0;
                                e = j + 1;  //记录位置，避免再次进入迭代重复移动
                                break;
                            }
                            else
                                break;
                        }
                        b++;
                        if (b == 100)   //避免因为某一个敌人无处可去而陷入死循环
                            a = 0;  //此时跳出while循环
                    }
                }
            }
        }
        break;
    }
}
 
int main() {
    //标签，用于配合goto语句
begin:
    char matrix[M][N];  //组成迷宫的二维数组
    int m = 0, n = 0, difficulty = 0;   //迷宫的行列数和难度
    int ach = 0;  //宝藏计分
 
    int select; //存放用户的选择
    //打印交互页面
    cout << "**********************" << endl;
    cout << "***** 1.开始游戏 *****" << endl;
    cout << "***** 2.读取存档 *****" << endl;
    cout << "***** 3.结束游戏 *****" << endl;
    cout << "**********************" << endl;
    cout << "请选择：";
    cin >> select;
    //对游戏的运行进行判断选择
    if (select == 1) {
        cout << "请输入迷宫的行数和列数：";
        cin >> m >> n;
        cout << "迷宫的难易程度：" << endl <<
            "    1: 简单" << endl <<
            "    2: 中等" << endl <<
            "    3: 困难" << endl <<
            "请选择本次游戏的难度：";
        cin >> difficulty;
        //difficulty = 2;
 
        generateMap(matrix, m, n, difficulty);
 
        row = 1;            //人物出发点位于迷宫 第二行
        col = rand() % (n - 2) + 1;  // 第二列 到 倒数第二列
        matrix[row][col] = '@';  //初始化人物的位置
 
        cout << endl << "---------------按任意键开始游戏！---------------" << endl << endl;
 
        while (1) {
            while (!_kbhit()) {
                // 循环等待键盘输入
            }
            if (_kbhit()) { // 如果有键盘输入
                key = _getch();
                //输入Esc键则直接退出游戏
                if (key == 27) { // Esc键
                    return 0; // 退出游戏
                }
                //输入L键则存档
                else if (key == 'L' || key == 'l') { // L键
                    saveGame(matrix, m, n, difficulty, ach); // 存档
                    system("cls");  //清空屏幕
                    cout << "存档成功";
                    return 0;
                }
                //输入K键则重新开始
                else if (key == 'k' || key == 'K') {
                    system("cls");
                    GO_TO_BEGIN;
                }
                //输入其他键位
                int A = statusGame(matrix, m, n);
                movePlayer(matrix, m, n, 0);
                displayMap(matrix, m, n);
                switch (A) {
                case 1: {
                    ach++;
                    break;
                }
                case 2: {
                    system("cls");
                    cout << "触碰怪物，游戏结束!" << endl;
                    cout << "你的最终分数为 " << ach << " 分！" << endl;
                    cout << "--------------------------" << endl;
                    char YN = 0;
                    cout << "是否要重新开始游戏（Y/N)：";
                    cin >> YN;
                    if (YN == 'y' || YN == 'Y')
                        GO_TO_BEGIN;
                    else
                        return 0;
                }
                case 3:
                    break;
                }
                if (ach == d3) {
                    system("cls");
                    cout << "    **********    " << endl;
                    cout << " ***          *** " << endl;
                    cout << "*      WIN!      *" << endl;
                    cout << " ***          *** " << endl;
                    cout << "    **********    " << endl;
                    cout << "泰裤辣！" << "你在" << d2 << "个敌人的围追堵截下收集了全部" << d3 << "个宝藏！" << endl;
                    return 0;
                }
                moveEnemy(matrix, m, n);
            }
            cout << "请输入移动方向（WSAD控制方向）：" << endl;
            cout << "你目前的分数是：" << ach << endl;
            cout << "按下esc键退出游戏" << endl;
            cout << "按下L键存档游戏" << endl;
            cout << "按下K键重新开始" << endl;
 
        }
    }
    //即选择读取存档
    else if (select == 2) {
        bool loadSuccess = loadGame(matrix, m, n, difficulty, ach);
        if (loadSuccess) {
        //if (0) {
            cout << "存档载入成功！" << endl;//读档成功
            int i = 0;
            int j = 0;
            //迭代查找存档中人物的位置，并记录
            for (i = 0; i < M; i++) {
                for (j = 0; j < N; j++) {
                    if (matrix[i][j] == '@') {
                        row = i;
                        col = j;    //存档中人物的位置
                    }
                }
            }
            //根据读取到的难度得出敌人数和宝藏数的初始值
            switch (difficulty) {
            case 1: {
                d2 = 2;
                d3 = 2;
                break;
            }
            case 2: {
                d2 = 2;
                d3 = 4;
                break;
            }
            case 3: {
                d2 = 2;
                d3 = 6;
                break;
            }
            }
 
            cout << endl << "---------------按任意键开始游戏！---------------" << endl << endl;
 
            //以下代码同上
            while (1) {
                while (!_kbhit()) {
                    // 循环等待键盘输入
                }
                if (_kbhit()) { // 如果有键盘输入
                    key = _getch();
                    if (key == 27) { // Esc键
                        return 0; // 退出游戏
                    }
                    else if (key == 'L' || key == 'l') { // L键
                        saveGame(matrix, m, n, difficulty, ach); // 存档
                        system("cls");  //清空屏幕
                        cout << "存档成功";
                        return 0;
                    }
                    else if (key == 'k' || key == 'K') {
                        system("cls");
                        GO_TO_BEGIN;
                    }
                    int A = statusGame(matrix, m, n);
                    movePlayer(matrix, m, n, 0);
                    displayMap(matrix, m, n);
                    switch (A) {
                    case 1: {
                        ach++;
                        break;
                    }
                    case 2: {
                        system("cls");
                        cout << "触碰怪物，游戏结束!" << endl;
                        cout << "你的最终分数为 " << ach << " 分！" << endl;
                        cout << "---------------------------------" << endl;
                        char YN = 0;
                        cout << "是否要重新开始游戏（Y/N)：";
                        cin >> YN;
                        if (YN == 'y' || YN == 'Y') {
                            system("cls");
                            GO_TO_BEGIN;
                        }
                        else
                            return 0;
                    }
                    case 3:
                        break;
                    }
                    if (ach == d3) {
                        system("cls");
                        cout << "    **********    " << endl;
                        cout << " ***          *** " << endl;
                        cout << "*      WIN!      *" << endl;
                        cout << " ***          *** " << endl;
                        cout << "    **********    " << endl;
                        cout << "泰裤辣！" << "你在" << d2 << "个敌人的围追堵截下收集了全部" << d3 << "个宝藏！" << endl;
                        return 0;
                    }
                    moveEnemy(matrix, m, n);
                }
                cout << "请输入移动方向（WSAD控制方向）：" << endl;
                cout << "你目前的分数是：" << ach << endl;
                cout << "按下esc键退出游戏" << endl;
                cout << "按下L键存档游戏" << endl;
                cout << "按下K键重新开始" << endl;
            }
        }
        //读档失败的情况
        else {
            system("cls");
            cout << "存档载入失败！" << endl;//读档失败
            system("pause");
            system("cls");
            GO_TO_BEGIN;
        }
 
    }
    //选择退出游戏，直接return
    else if (select == 3) {
        return 0;
    }
    //输入其他按键，清屏并回到初始位置重新选择
    else {
        system("cls");
		cout<<"输入错误，请重新输入"<<endl;
        GO_TO_BEGIN;
         
    }
}
