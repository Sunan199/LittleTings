#include <stdio.h>
#include <windows.h>   

struct Record {
double price ; 		// 收支金额
char type[16] ; 	// 收支类型
char people[20] ; 	// 成员				
struct Record* next;//链式存储
};


struct  Record *recordList;//定义全局 收支记录 指针
int recordCount = 0;//收支记录 个数记录


void readRecord ();//收支记录
void writeRecord ();
    
void addRecord ();//收支记录 添加记录
    void addOneRecord (struct Record *record);
        
void delRecord ();//收支记录 删除记录
    
int delRecordByIndex (int index);//收支记录 根据下标删除
    
void modRecord ();//收支记录 修改记录
   
void findRecord (int all); //收支记录 查找记录
    
void exchangeRecord (int i ,int j);//交换

void menu();//菜单
    
int main(){
readRecord ();//读取收支记录
while(1){
    int choice;
    menu ();
    scanf("%d",&choice);
	switch(choice){
        case 1 :addRecord ();//添加收支记录
		break;
        case 2 :findRecord (1);//查找收支记录
                delRecord ();//删除收支记录
                break;
        case 3 :
                findRecord (1);//查找收支记录
                modRecord ();//修改收支记录
                break;
        case 4 :
                findRecord (0);//查找收支记录
                break;
        case 5 :
		        findRecord (1);//打印收支记录
		        break;
        case 0:
                printf("\n\n\n\t\t\t感谢\n\n\n");
				exit(0);
        default :
        		getchar();
                printf("\n\n\t输入有误，重新输入\n\n");            system("pause");
    }
                        }
        return 0;
    }



void readRecord (){
    FILE *fp = NULL;
    if (NULL == (fp = fopen("Record.txt", "r")))//读打开
    {

        return;
    }
    int i=0;//从文件读取数据
    recordList = NULL;
    struct Record *tail = recordList;
    while (1)//读取
    {
        //给节点分配空间
        struct Record *p=(struct Record*)malloc(sizeof(struct Record));
        //读取数据到p
        if(fscanf(fp, "%lf%s%s%s"
             ,&p->price  ,p->type  ,p->people  
        ) == EOF)  {
            break;
        }
        p->next=NULL;
        //指针为空则赋值。
        if (recordList  == NULL)
        {
            recordList=p;
            tail=p;
        }
        else {
            tail->next=p;
            tail=p;
        }
        i++;
    }
    //记录总数
    recordCount = i;
    fclose(fp);
}
//收支记录 写入文件
void writeRecord (){
    //打开文件
    FILE *fp = NULL;
    fp = fopen("Record.txt", "w+");
    //定义指针
    struct Record *p = recordList;
    //写入
    while(p){
        fprintf(fp,"%lf %s %s \n"
             ,p->price  ,p->type  ,p->people);
        p=p->next;
    }
    fclose(fp);
}
//收支记录 添加记录
void addRecord (){
    char selectYNFlag = 'y';
    while(selectYNFlag == 'y' || selectYNFlag == 'Y' ){
        printf("\n");
		struct Record *tempValue = (struct Record*)malloc(sizeof(struct Record));
		printf("收支类型为：");	char type[16];   scanf("%s",type);
        strcpy(tempValue->type,type);
		printf("金额为：");   double price;     scanf("%lf",&price);
		tempValue->price = price ;
        printf("成员为：");   char people[20];   scanf("%s",people);
        strcpy(tempValue->people,people);
 

        tempValue->next = NULL;
        addOneRecord (tempValue);
            printf("\n继续？(y/n)");    getchar();
            scanf("%c",&selectYNFlag);
        }
    writeRecord ( );
	    }
void addOneRecord (struct Record *temp){
    if(recordList == NULL){
        recordList = temp;
    }else{
        temp->next = recordList;
        recordList = temp;
    }
    recordCount ++;
}

void delRecord (){		//收支记录 删除
                printf("选择您要删除的收支记录序号(输-1返回)：");        int index;	getchar();
                scanf("%d",&index);
    if(index >= 0){
        int res = delRecordByIndex (index);
        if(res){
                        printf("成功删除\n");                writeRecord ();
        }else{
                        printf("删除失败\n");            }
    }
}
//收支记录 根据下标删除
int delRecordByIndex (int index){
    if(index > recordCount || index < 1){
        return 0;//删除失败 ，数量没那么多
    }
    //开始删除！
    struct Record *p = (struct Record*)malloc(sizeof(struct Record));
    struct Record *q = recordList;
    p->next = recordList;
    if(index == 1){
        recordList = recordList ->next;
    }else{
        int i;
        for(i = 0;i < index-1 ; i++){
            p = p->next;
            q = q->next;
        }
        p->next = q->next;
    }
    recordCount = recordCount - 1;
    return 1;
}
//收支记录 修改
void modRecord (){
    printf("选择要修改的收支记录序号(输-1返回)：");        int indexOfMod = -1,i;
    scanf("%d",&indexOfMod);
    if(indexOfMod > 0 && indexOfMod <= recordCount ){
    	struct Record *temp = recordList ;
        for( i = 0;i < indexOfMod-1;i++){
            temp=temp->next;
        }
        printf("收支金额为：");
		double price;
        scanf("%lf",&price);
        printf("收支类型为：");
		char type[16];
        scanf("%s",type);
        printf("成员为：");
		char people[20];
        scanf("%s",people);
        //输入完毕，赋值
        temp-> price = price ;
        strcpy(temp-> type,type);
        strcpy(temp-> people,people);
        printf("成功修改\n");
		writeRecord ();
    }else{
        printf("输入有错误\n");        }
}
//收支记录 查找 如果all=1:查询全部 否则按条件查询
void findRecord (int all){
    int select = 0;
    char conCharArr[40];
    double conNum1,conNum2;
    while(1){
        printf("\n%-5s%-12s%-16s%-20s\n\n","序号" ,"收支金额","类型","成员");
        int i=1,count = 0;
        if( recordList ){//用于统计信息所用
            double priceSum=0;
            struct Record *t = recordList;
            while(t){
                int delFlag = 0;
                if(select == 0|| select == 1 && t->price >= conNum1 && t->price<=conNum2|| select == 2 && strstr(t->type,conCharArr)|| select == 3 && strstr(t->people,conCharArr)){
					printf("%-5d%-12lf%-16s%-20s%-30s \n\n",i,t->price,t->type,t->people);//用于统计信息所用
                    priceSum += t->price;
                    count++;}
				else{
                    delFlag = 1;
                }
                t=t->next;
                if(delFlag){
                    delRecordByIndex (i--);
                }
                i++;
            }
			printf("------------------------------------------------------------------\n");
            printf("\n");}
		else{
			printf("\n无记录\n\n");}
        if(all==1)
		break;
        printf("你可以进行以下条件筛选\n");
		printf("0：退出\n");
		printf("1：按金额筛选\n");
		printf("2：按类型筛选\n");
		printf("3：按成员筛选\n");
		printf("选择为：");
		scanf("%d",&select);
        if(select>0){
            if(0||select==2||select==3){
                printf("筛选条件为：");
				scanf("%s",conCharArr);}
			else if(0 ||select==1 ){
                printf("筛选范围为m,n用空格隔开(m<=X<=n)：");
				scanf("%lf",&conNum1);
                scanf("%lf",&conNum2);
                }}else{
            		break;}
    }
    readRecord();//从新读取数据。
}
                void menu (){        //菜单
                    printf("*------------------主菜单---------------------*\n");
					printf("*		 1 :添加收支记录  	      *\n");
					printf("*		 2 :删除收支记录  	      *\n");
					printf("*		 3 :修改收支记录  	      *\n");
					printf("*		 4 :查找收支记录 	      *\n");
					printf("*		 5 :打印收支记录 	      *\n");
					printf("*		 0 :退出	   	      *\n");
					printf("*---------------------------------------------*\n");
					printf("请输入选择：");        }
