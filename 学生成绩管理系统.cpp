#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;
class Student
{
private:
    char name[10];
    char id[12];
    float math;
    float computer;
    float english;

public:
    Student()
    {
    }
    Student(char *n)
    {
        strcpy(name, n);
    }
    Student(char *n, char *i)
    {
        strcpy(name, n);
        strcpy(id, i);
    }
    void SetName(char *n)
    {
        strcpy(name, n);
    }
    void SetID(char *i)
    {
        strcpy(id, i);
    }
    void SetMath(float m)
    {
        math = m;
    }
    void SetComputer(float c)
    {
        computer = c;
    }
    void SetEnglish(float e)
    {
        english = e;
    }
    void SetAll(float m, float e, float c)
    {
        math = m;
        english = e;
        computer = c;
    }
    float Sum()
    {
        return math + english + computer;
    }
    string Name()
    {
        return name;
    }
    string Id()
    {
        return id;
    }
    float GetMath()
    {
        return math;
    }
    float GetEnglish()
    {
        return english;
    }
    float GetComputer()
    {
        return computer;
    }
    bool Match(char *str, short flag) //flag=1 name flag=2 id
    {
        if (flag == 1)
            return strcmp(name, str) == 0;
        else
            return strcmp(id, str) == 0;
    }
    void Print()
    {
        cout << name << '\t' << id << endl;
        cout << "高数:" << math << '\t' << "英语:" << english << '\t' << "C++:" << computer << endl;
    }
};

struct Node
{
    Student st;
    Node *next;
};
class Data //su zu
{
private:
    Node *head;

public:
    Data()
    {
        head = new Node;
        head->next = NULL;
    }
    ~Data()
    {
        Node *p;
        while (head != NULL)
        {
            p = head;
            head = head->next;
            delete p;
        }
        cout << "程序结束";
    }
    void Push(char *n, char *i, float m, float e, float c)
    {

        Student st(n, i);
        st.SetAll(m, e, c);
        Node *p = new Node;
        p->st = st;
        p->next = head->next;
        head->next = p;
    }
    void Push()
    {
        char n[10], id1[12];

        cout << "请输入添加学生姓名:";
        cin >> n;
        cout << endl;
        Node *p;
        do
        {
            cout << "请输入添加学生学号:";
            cin >> id1;
            for (p = head->next; p != NULL; p = p->next)
            {
                if (p->st.Match(id1, 2))
                {
                    cout << "该学号已存在请重新输入" << endl;
                    break;
                }
            }
        } while (p != NULL);
        Student stu1(n, id1);
        float m, e, c;
        cout << "请输入该生高数成绩:";
        cin >> m;
        cout << "请输入该生英语成绩:";
        cin >> e;
        cout << "请输入该生C++成绩:";
        cin >> c;
        stu1.SetAll(m, e, c);
        Node *s = new Node;
        s->st = stu1;
        s->next = head->next;
        head->next = s;
        cout << "添加成功\n";
        system("pause");
    }
    Node *Search(Node *start, char *str, short flag)
    {
        for (Node *p = start; p != NULL; p = p->next)
        {
            if (p->st.Match(str, flag))
            {

                p->st.Print();
                return p;
            }
        }
        return NULL;
    }
    Node *Search1()
    {
        short choice;
        char ch;
        do
        {
            cout << "1.按姓名搜索  2.按学号搜索";
            cin >> choice;
        } while (choice != 1 && choice != 2);
        char match[12];
        cout << "请输入你要查找的" << (choice == 1 ? "姓名" : "学号") << ":\n";
        cin >> match;
        Node *result = head->next;
        while (true)
        {
            result = Search(result, match, choice);
            if (result == NULL)
            {
                cout << "未找到该学生" << endl;
                system("pause");
                return NULL;
            }
            cout << "这是你要找的学生？(y/n)";
            cin >> ch;
            system("pause");
            if (ch == 'y' || ch == 'Y')
                return result;
            else
                result = result->next;
        }
    }
    bool Delete()
    {
        Node *result;
        result = Search1();
        if (result == NULL)
            return false;
        char choice;
        cout << "是否要删除这条信息？(y/n)";
        cin >> choice;
        Node *p;
        if (choice == 'y' || choice == 'Y')
        {
            p = result;
            for (p = head; p->next != result; p = p->next)
                ;
            p->next = result->next;
            result->next = NULL;
            delete result;
            cout << "删除成功";
            system("pause");
            return true;
        }
        cout << "信息未删除";
        system("pause");
        return false;
    }
    bool xiugai()
    {
        Node *result;
        result = Search1();
        if (result == NULL)
            return false;
        char choice;
        char choice1;
        char xinname[10];
        float xinmath, xinenglish, xincomputer;

        cout << "是否要修改 (y/n)";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            cout << "请输入要修改内容的序号    1.姓名   2.高数成绩   3.英语成绩   4.C++成绩\n";
            cout << "    (注: 学号无法修改)\n";
            cin >> choice1;
            switch (choice1)
            {
            case '1':
                cout << "请输入修改后姓名:";
                cin >> xinname;
                result->st.SetName(xinname);
                cout << "修改成功";
                system("pause");
                break;
            case '2':
                cout << "请输入修改后高数成绩:";
                cin >> xinmath;
                result->st.SetMath(xinmath);
                cout << "修改成功";
                system("pause");
                break;
            case '3':
                cout << "请输入修改后英语成绩:";
                cin >> xinenglish;
                result->st.SetEnglish(xinenglish);
                cout << "修改成功";
                system("pause");
                break;
            case '4':
                cout << "请输入修改后C++成绩:";
                cin >> xincomputer;
                result->st.SetComputer(xincomputer);
                cout << "修改成功";
                system("pause");
                break;
            default:
                cout << "信息未修改";
                system("pause");
                return false;
            }
            return true;
        }
        cout << "信息未修改";
        return false;
    }
    void Display()
    {
        cout << endl;
        cout << setw(12) << setiosflags(ios::left) << "姓名" << setw(12) << "学号" << setw(8) << "高数" << setw(8) << "英语" << setw(8) << "C++" << setw(8) << "总分" << endl;
        Node *p;
        for (p = head->next; p != NULL; p = p->next)
        {
            cout << setw(12) << p->st.Name() << setw(12) << p->st.Id() << setw(8) << p->st.GetMath() << setw(8) << p->st.GetEnglish() << setw(8) << p->st.GetComputer() << setw(8) << p->st.Sum() << endl;
        }
        cout << resetiosflags(ios::left);
        system("pause");
    }
    void Sort()
    {
        char choice;
        do
        {
            cout << "请输入排序方式:  1.学号升序     2.总成绩降序" << endl;
            cin >> choice;
        } while (choice != '1' && choice != '2');
        if (choice == '1')
        {
            Node *p = head->next;
            while (p != NULL)
            {
                Node *q = p->next;
                while (q != NULL)
                {
                    if (p->st.Id() > q->st.Id())
                    {
                        Node *s = new Node;
                        s->st = p->st;
                        p->st = q->st;
                        q->st = s->st;
                    }
                    q = q->next;
                }
                p = p->next;
            }
        }
        else
        {
            Node *p = head->next;
            while (p != NULL)
            {
                Node *q = p->next;
                while (q != NULL)
                {
                    if (p->st.Sum() < q->st.Sum())
                    {
                        Node *s = new Node;
                        s->st = p->st;
                        p->st = q->st;
                        q->st = s->st;
                    }
                    q = q->next;
                }
                p = p->next;
            }
        }
        cout << "排序完成";
        system("pause");
    }
    char MainMenu()
    {
        char choice;
        do
        {
            system("cls");
            cout << "    --------------欢迎使用学生成绩管理系统--------------\n\n\n";
            cout << "                      1.管理员入口\n";
            cout << "                      2.教师入口\n";
            cout << "                      3.学生入口\n";
            cout << "                      4.退出\n";
            cin >> choice;
        } while (choice < '1' || choice > '4');
        return choice;
    }
    char MMenu()
    {
        char choice;
        do
        {
            system("cls");
            cout << "    -------------------管理员模式--------------------\n\n\n";
            cout << "                      1.添加学生\n";
            cout << "                      2.查找学生\n";
            cout << "                      3.删除学生\n";
            cout << "                      4.重新排序\n";
            cout << "                      5.修改学生信息\n";
            cout << "                      6.返回上一级\n";
            cin >> choice;
        } while (choice < '1' || choice > '6');
        return choice;
    }
    char TMenu()
    {
        char choice;
        do
        {
            system("cls");
            cout << "    --------------------教师模式----------------------\n\n\n";
            cout << "                      1.添加学生\n";
            cout << "                      2.查找学生\n";
            cout << "                      3.重新排序\n";
            cout << "                      4.返回上一级\n";
            cin >> choice;
        } while (choice < '1' || choice > '4');
        return choice;
    }
    char SMenu()
    {
        char choice;
        do
        {
            system("cls");
            cout << "    --------------------学生模式-----------------------\n\n\n";
            cout << "                      1.查找学生\n";
            cout << "                      2.学生排序\n";
            cout << "                      3.返回上一级\n";
            cin >> choice;

        } while (choice < '1' || choice > '3');
        return choice;
    }
};
int main()
{
    Data s;
    bool flag = false;
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;
    char choice;
    s.Push("lxp", "1111", 60, 90, 80);
    s.Push("jjy", "1112", 80, 40, 100);
    s.Push("wzb", "1113", 30, 32, 100);
    s.Push("fhd", "1114", 100, 120, 200);
    while (!flag)
    {
        choice = s.MainMenu();
        switch (choice)
        {
        case '1':
            while (!flag1)
            {
                choice = s.MMenu();
                switch (choice)
                {
                case '1':
                    s.Push();
                    break;
                case '2':
                    s.Search1();
                    break;
                case '3':
                    s.Delete();
                    break;
                case '4':
                    s.Sort();
                    s.Display();
                    break;
                case '5':
                    s.xiugai();
                    break;
                case '6':
                    flag1 = true;
                    break;
                }
            }
            break;
        case '2':
            while (!flag2)
            {
                choice = s.TMenu();
                switch (choice)
                {
                case '1':
                    s.Push();
                    break;
                case '2':
                    s.Search1();
                    break;
                case '3':
                    s.Sort();
                    s.Display();
                    break;
                case '4':
                    flag2 = true;
                    break;
                }
            }
            break;
        case '3':
            while (!flag3)
            {
                choice = s.SMenu();
                switch (choice)
                {
                case '1':
                    s.Search1();
                    break;
                case '2':
                    s.Sort();
                    s.Display();
                    break;
                case '3':
                    flag3 = true;
                    break;
                }
            }
            break;
        case '4':
            flag = true;
            break;
        }
        flag1 = false;
        flag2 = false;
        flag3 = false;
    }
    return 0;
}
