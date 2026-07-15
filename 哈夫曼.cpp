#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// 定义哈夫曼树的节点结构体
typedef struct node
{
    int weight;         // 权值
    char character;     // 字符
    struct node *left;  // 左子树指针
    struct node *right; // 右子树指针
} Node;

// 定义哈夫曼树的节点类型
typedef Node *HuffmanTree;

// 定义哈夫曼树的节点最小堆
typedef struct heap
{
    int size;          // 堆的大小 
    int capacity;      // 堆的容量
    HuffmanTree *data; // 堆数据存储指针
} MinHeap;

// 初始化最小堆
MinHeap *initMinHeap(int capacity)
{
    // 动态分配最小堆
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->data = (HuffmanTree *)malloc(sizeof(HuffmanTree) * capacity);
    return heap;
}

// 最小堆中插入元素
void insert(MinHeap *heap, HuffmanTree node)
{
    if (heap->size >= heap->capacity)
    {
        return; // 如果堆已满，直接退出
    }
    heap->data[heap->size] = node;  // 将元素插入堆底
    int current = heap->size++;     // 更新堆大小
    int parent = (current - 1) / 2; // 父节点的下标
    // 自下往上调整堆，直到找到合适的位置插入新元素
    while (parent >= 0 && heap->data[current]->weight < heap->data[parent]->weight)
    {
        // 如果当前元素比父节点的权值小，则交换两个元素的位置
        HuffmanTree temp = heap->data[parent];
        heap->data[parent] = heap->data[current];
        heap->data[current] = temp;
        current = parent;
        parent = (current - 1) / 2;
    }
}

// 从最小堆中取出最小元素
HuffmanTree extractMin(MinHeap *heap)
{
    if (heap->size == 0) // 如果堆为空，直接返回空指针
    {
        return NULL;
    }
    HuffmanTree min = heap->data[0];          // 最小元素即为堆顶元素
    heap->data[0] = heap->data[--heap->size]; // 将堆底元素移到堆顶，并更新堆大小
    int current = 0;                          // 当前节点的下标
    int child = current * 2 + 1;              // 当前节点的左孩子的下标
    // 自上往下调整堆，直到找到合适的位置插入堆底元素
    while (child < heap->size) // 当前节点还有孩子节点
    {
        if (child + 1 < heap->size && heap->data[child + 1]->weight < heap->data[child]->weight)
        {
            child++; // 找到当前节点的左右孩子中较小的一个
        }
        if (heap->data[child]->weight < heap->data[current]->weight)
        {
            // 将当前节点和较小孩子节点交换位置
            HuffmanTree temp = heap->data[child];
            heap->data[child] = heap->data[current];
            heap->data[current] = temp;
            current = child;         // 更新当前节点的下标
            child = current * 2 + 1; // 更新当前节点的左孩子下标
        }
        else
        {
            break; // 如果已经满足最小堆的性质，则退出循环
        }
    }
    return min; // 返回被取出的最小元素
}

// 用最小堆构建哈夫曼树
HuffmanTree buildHuffmanTree(int *weights, char *characters, int n)
{
    // 初始化最小堆，将每个字符及其权重转换成节点，并插入堆中
    MinHeap *heap = initMinHeap(n);
    for (int i = 0; i < n; i++)
    {
        Node *node = (Node *)malloc(sizeof(Node));
        node->weight = weights[i];
        node->character = characters[i];
        node->left = NULL;
        node->right = NULL;
        insert(heap, node); // 将节点插入堆中
    }
    // 不断从最小堆中取出权重最小的两个节点，合并成一个新节点，再插入堆中
    while (heap->size > 1)
    {
        HuffmanTree left = extractMin(heap);  // 取出堆顶节点，即最小权重节点
        HuffmanTree right = extractMin(heap); // 再次取出最小权重节点
        Node *parent = (Node *)malloc(sizeof(Node));
        parent->weight = left->weight + right->weight; // 新节点的权重为左右节点的权重之和
        parent->left = left;                           // 将左节点作为新节点的左孩子
        parent->right = right;                         // 将右节点作为新节点的右孩子
        insert(heap, parent);                          // 将新节点插入堆中
    }
    HuffmanTree root = extractMin(heap); // 最后堆中只剩下根节点，即为哈夫曼树的根节点
    free(heap->data);                    // 释放堆数组占用的空间
    free(heap);                          // 释放最小堆结构体占用的空间
    return root;                         // 返回哈夫曼树的根节点指针
}
// 对单个字符进行编码模块
char *encodeChar(HuffmanTree root, char ch)
{
    static char code[100]; // 申请存储编码的空间
    static int index = 0;  // 记录编码位数

    if (root == NULL)
    {
        return NULL;
    }

    if (root->character == ch)
    {
        code[index] = '\0'; // 编码结尾
        index = 0;          // 编码位数归零
        return code;
    }

    code[index++] = '0';
    char *leftCode = encodeChar(root->left, ch);
    if (leftCode != NULL)
    {
        return leftCode;
    }

    index--; // 回溯
    code[index++] = '1';
    char *rightCode = encodeChar(root->right, ch);
    if (rightCode != NULL)
    {
        return rightCode;
    }

    index--; // 回溯
    return NULL;
}

// 对文本进行编码模块
char *encode(HuffmanTree root, char *text)
{
    char *result = (char *)malloc(strlen(text) * 100 * sizeof(char)); // 申请存储编码结果的空间
    result[0] = '\0';                                                 // 初始化
    for (int i = 0; i < strlen(text); i++)
    {
        char *code = encodeChar(root, text[i]); // 对单个字符编码
        if (code)
        {
            strcat(result, code); // 将编码拼接到结果中
        }
    }
    return result;
}

// 初始化函数
HuffmanTree initialization()
{
    int n=0,temp=1; // 字符集大小
    while(temp){
    	//temp=1;
		printf("请输入字符集大小：\n");
		scanf("%d", &n);
		if(n>0)
			temp=0;
		else{
			printf("输入错误，请输入一个大于0的数字\n");
			temp=1;}
		while(getchar()!='\n');
	}
    
    int *weights = (int *)malloc(sizeof(int) * n);       // 动态分配n个权重值
    char *characters = (char *)malloc(sizeof(char) * n); // 动态分配n个字符
    printf("请输入字符和权值：(用空格隔开)\n");
    for (int i = 0; i < n; i++)
    {
        scanf(" %c %d", &characters[i], &weights[i]);
    }
    HuffmanTree root = buildHuffmanTree(weights, characters, n);
    FILE *fp = fopen("hfmtree.txt", "w");
    fprintf(fp, "%d\n", n);
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%c%s", characters[i], i == n - 1 ? "\n" : " ");
    }
    for (int i = 0; i < n; i++)
    {
        fprintf(fp, "%d%s", weights[i], i == n - 1 ? "\n" : " ");
    }
    fclose(fp);
    return root;
}

void EnCodeChar(HuffmanTree root)
{
    FILE *fp;
    char *characters;
    char buffer[100];
    int n;
    // 打开文件
    fp = fopen("hfmtree.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    // 读取第一行，获取字符集大小
    fscanf(fp, "%d", &n);
    // 分配空间
    characters = (char *)malloc(n * sizeof(char));
    // 读取第二行数据
    fgets(buffer, sizeof(characters) * 2, fp);
    fgets(buffer, sizeof(characters) * 2, fp);
    int i = 0;
    int j = 0;
    while (buffer[i] != NULL)
    {
        if (buffer[i] != ' ')
        {
            characters[j] = buffer[i];
            j++;
        }
        i++;
    }
    fclose(fp);
    for (int i = 0; i < n; i++)
    {
        int index = 0;
        char *res = encodeChar(root, characters[i]);
        printf("%c: %s\n", characters[i], res);
    }
}

void EnCode(HuffmanTree root)
{
    char tobetrans[100];
    char *result;
    printf("请输入一个字符串：\n");
    scanf("%s", tobetrans); // 读取输入的字符串，存储到字符数组中
    result = encode(root, tobetrans);
    printf("%s\n", result);
    FILE *fp = fopen("codefile.txt", "w");
    for (int i = 0; i < strlen(result); i++)
    {
        fprintf(fp, "%c", result[i]);
    }
    fclose(fp);
}

char DeCodeChar(HuffmanTree root, char *code)
{
    HuffmanTree p = root;
    while (*code != '\0')
    {
        if (*code == '0')
        {
            p = p->left;
        }
        else if (*code == '1')
        {
            p = p->right;
        }
        if (p->left == NULL && p->right == NULL)
        {
            return p->character;
        }
        code++;
    }
    return '\0';
}

void DeCode(HuffmanTree root)
{
    // 读取译码文件
    FILE *fp_code = fopen("codefile.txt", "r");
    char *code = (char *)malloc(1000 * sizeof(char)); // 申请存储代码的空间
    fscanf(fp_code, "%s", code);                      // 读取代码
    fclose(fp_code);

    // 译码
    char *text = (char *)malloc(1000 * sizeof(char)); // 申请存储译文的空间
    int i = 0, j = 0;
    while (code[i] != '\0')
    {
        char *tmp = (char *)malloc(100 * sizeof(char)); // 申请临时空间存储单个字符的编码
        int k = 0;
        while (DeCodeChar(root, tmp) == '\0')
        {
            tmp[k++] = code[i++];
        }
        text[j++] = DeCodeChar(root, tmp); // 译码并存储译文
        free(tmp);                         // 释放临时空间
    }
    text[j] = '\0';
    // 存储译文到文件中
    FILE *fp_text = fopen("textfile.txt", "w");
    fprintf(fp_text, "%s", text);
    fclose(fp_text);
    // 释放申请的空间
    free(code);
    free(text);
}

void Print()
{
    FILE *fp;
    char buffer[100];
    fp = fopen("codefile.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    // 读取数据
    fgets(buffer, 100, fp);
    printf("%s", buffer);
    fclose(fp);
    fp = fopen("codeprint.txt", "w");
    for (int i = 0; i < strlen(buffer); i++)
    {
        fprintf(fp, "%c", buffer[i]);
        printf("\n");
    }
    fclose(fp);
}

// 打印哈夫曼树的函数
void PrintHfmTree(FILE *file, HuffmanTree root, int level, char *prefix, int is_last)
{
    // 如果根节点为空，则返回
    if (root == NULL)
    {
        return;
    }
    // 打印当前节点的值
    printf("%s", prefix);
    printf(is_last ? "└── " : "├── ");
    printf("(%c:%d)\n", root->character, root->weight);
    fprintf(file, "%s", prefix);
    fprintf(file, is_last ? "└── " : "├── ");
    fprintf(file, "(%c:%d)\n", root->character, root->weight);

    // 更新前缀
    char new_prefix[128];
    sprintf(new_prefix, "%s%s", prefix, is_last ? "    " : "│   ");

    // 递归打印左右子树
    PrintHfmTree(file, root->left, level + 1, new_prefix, (root->right == NULL));
    PrintHfmTree(file, root->right, level + 1, new_prefix, 1);
}

void Tree(HuffmanTree root)
{
    // 打开文件treeprint
    FILE *file = fopen("treeprint.txt", "w");
    // 调用打印函数打印哈夫曼树并写入文件
    PrintHfmTree(file, root, 0, "", 1);
    // 关闭文件
    fclose(file);
}

HuffmanTree root = nullptr; // 将huffman树根设置为全局变量

void Window()
{
    char choice;
    char exit_choice;
    while (1)
    {
        printf("请选择您的操作：\n");
        printf("1. 初始化\n");
        printf("2. 编码\n");
        printf("3. 译码\n");
        printf("4. 印代码文件\n");
        printf("5. 印哈夫曼树\n");
        printf("0. 退出\n");
        scanf(" %c", &choice); // 加上空格忽略换行符

        switch (choice)
        {
        case '1':
            printf("您选择了初始化操作。\n");
            root = initialization();
            break;
        case '2':
            if (root == NULL)
            {
                printf("请先进行初始化操作！\n");
                break;
            }
            printf("您选择了编码操作。\n");
            
            EnCode(root);
            break;
        case '3':
            if (root == NULL)
            {
                printf("请先进行初始化操作！\n");
                break;
            }
            printf("您选择了译码操作。\n");
            DeCode(root);
            break;
        case '4':
            printf("您选择了打印操作。\n");
            Print();
            break;
        case '5':
            printf("您选择了打印操作。\n");
            Tree(root);
            break;
        case '0':
            printf("您确定要退出吗？按0键确认退出，按其他键返回上级菜单。\n");
            scanf(" %c", &exit_choice); // 加上空格忽略换行符
            if (exit_choice == '0')
            {
                printf("谢谢使用，再见！\n");
                return;
            }
            break;
        default:
            printf("无效的选择，请重新选择。\n");
            break;
        }
    }
}

int main()
{
    Window();
    return 0;
}
