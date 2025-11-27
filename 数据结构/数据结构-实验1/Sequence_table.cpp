/*
编程实现顺序表的以下基本操作：建立顺序表，修改顺序表，插入顺序表，删除顺序表。
采用顺序表结构编程实现：两个集合的运算：交集/并集/差集。
*/

#include <iostream>
#define MAXSIZE 100
#define OK 1
#define ERROR 0
#define OVERFLOW -2
using namespace std;

typedef int Status;
typedef int ElemType;
typedef struct
{
    ElemType *elem;
    int length;
}SqList;

void menu();
Status InitList(SqList &L);
Status ClearList(SqList &L);
void IsClear(SqList L);
int ListLength(SqList L);
ElemType GetElem(SqList L, int i);
ElemType FrontDrive(SqList L, ElemType e);
ElemType NextDrive(SqList L, ElemType e);
Status InsertList(SqList &L, int i, ElemType e);
Status DeleteList(SqList &L, int i);
Status ReviseList(SqList &L, int i, ElemType e);
Status LocateElem(SqList L, ElemType e, int* index);
void PrintList(SqList L);

SqList IntersectionSet(const SqList& A, const SqList& B);
SqList UnionSet(const SqList& A, const SqList& B);
SqList DifferenceSet(const SqList& A, const SqList& B);

int main ()
{
   
    SqList L, A, B;
    int i;
    ElemType e;
    int op;
    int index[MAXSIZE];
    string location_and_elem;
    InitList(L);
    InitList(A);
    InitList(B);
    menu();
    while(cin >> op && op > 0)
    {
        
        switch (op)
        {
        case 1: 
            if(ClearList(L))
            {
                cout << "清空成功" << endl;
            }
            else
            {
                cout << "清空失败" << endl;
            } 
            break;
        case 2: 
            IsClear(L); 
            break;
        case 3:
            ListLength(L);
            break;
        case 4:
            cout << "请输入要获取元素的位置：";
            cin >> i;
            if(GetElem(L, i) == ERROR)
            {
                cout << "位置不合法" << endl;
            }
            else
            {
                cout << GetElem(L, i) << endl;
            }
            break;
        case 5:
            cout << "请输入要获取前驱的元素：";
            cin >> e;
            FrontDrive(L, e);
            break;
        case 6:
            cout << "请输入要获取后继的元素：";
            cin >> e;
            NextDrive(L, e);
            break;
        case 7:
            cout << "请输入要插入的位置和元素：";
            cin >> location_and_elem;
            i = location_and_elem.find(",");
            if(InsertList(L, stoi(location_and_elem.substr(1, i)), stoi(location_and_elem.substr(i + 1, location_and_elem.length() - 2))))
            {
                cout << "插入成功" << endl;
            }
            else
            {
                cout << "插入失败" << endl;
            }
            break;
        case 8:
            cout << "请输入要删除的位置：";
            cin >> i;
            DeleteList(L, i);
            break;
        case 9:
            cout << "请输入要修改的位置和元素：";
            cin >> location_and_elem;
            i = location_and_elem.find(",");
            if(ReviseList(L, stoi(location_and_elem.substr(1, i)), stoi(location_and_elem.substr(i + 1, location_and_elem.length() - 2))))
            {
                cout << "修改成功" << endl;
            }
            else
            {
                cout << "修改失败" << endl;
            }
            break;
        case 10:
            cout << "请输入要查找的元素：";
            cin >> e;
            if(LocateElem(L, e, index))
            {
                cout << "位置为：";
                for (int j = 1; j < index[0] + 1; j++)
                {
                    cout << index[j] << " ";
                }
                cout << endl;
            }
            else
            {
                cout << "不存在" << endl;
            }
            break;
        case 11:
            PrintList(L);
            break;
        case 12:
            cout << "请输入集合A的元素个数：";
            cin >> A.length;
            cout << "请输入集合A的元素：";
            for (int j = 0; j < A.length; j++)
            {
                cin >> A.elem[j];
            }
            cout << "请输入集合B的元素个数：";
            cin >> B.length;
            cout << "请输入集合B的元素：";
            for (int j = 0; j < B.length; j++)
            {
                cin >> B.elem[j];
            }
            cout << "A ∩ B = ";
            PrintList(IntersectionSet(A, B));
            break;
        case 13:
            cout << "请输入集合A的元素个数：";
            cin >> A.length;
            cout << "请输入集合A的元素：";
            for (int j = 0; j < A.length; j++)
            {
                cin >> A.elem[j];
            }
            cout << "请输入集合B的元素个数：";
            cin >> B.length;
            cout << "请输入集合B的元素：";
            for (int j = 0; j < B.length; j++)
            {
                cin >> B.elem[j];
            }
            cout << "A ∪ B = ";
            PrintList(UnionSet(A, B));
            break;
        case 14:
            cout << "请输入集合A的元素个数：";
            cin >> A.length;
            cout << "请输入集合A的元素：";
            for (int j = 0; j < A.length; j++)
            {
                cin >> A.elem[j];
            }
            cout << "请输入集合B的元素个数：";
            cin >> B.length;
            cout << "请输入集合B的元素：";
            for (int j = 0; j < B.length; j++)
            {
                cin >> B.elem[j];
            }
            cout << "A - B = ";
            PrintList(DifferenceSet(A, B));
            break;
        default:
            break;
        }
        menu();
        //cout << "请输入操作代码：";
    }
    return 0;
}

void menu()
{
    cout << "1----清空线性表" << endl;
    cout << "2----判断线性表是否为空" << endl;
    cout << "3----求线性表长度" << endl;
    cout << "4----获取线性表指定位置元素" << endl;
    cout << "5----求前驱" << endl;
    cout << "6----求后继" << endl;
    cout << "7----在线性表指定位置插入元素" << endl;
    cout << "8----删除线性表指定位置元素" << endl;
    cout << "9----修改线性表指定位置元素" << endl;
    cout << "10---查找线性表指定元素位置" << endl;
    cout << "11---显示线性表" << endl;
    cout << "12---求两个集合的交集" << endl;
    cout << "13---求两个集合的并集" << endl;
    cout << "14---求两个集合的差集" << endl;
    cout << "     退出，输入一个负数！" << endl;
    cout << "请输入操作代码：";
}

Status InitList(SqList &L)
{
    L.elem = new ElemType[MAXSIZE];
    if (!L.elem)
    {
        exit(OVERFLOW);
    }
    L.length = 0;
    return OK;
}

Status ClearList(SqList &L)
{
    L.length = 0;
    return OK;
}

void IsClear(SqList L)
{
    if (L.length == 0)
    {
        cout << "顺序表为空" << endl;
    }
    else
    {
        cout << "顺序表非空" << endl;
    }
}

int ListLength(SqList L)
{
    cout << L.length << endl;
    return L.length;
}

ElemType GetElem(SqList L, int i)
{
    if (i < 1 || i > L.length)
    {
        return ERROR;
    }
    return L.elem[i - 1];
}

ElemType FrontDrive(SqList L, ElemType e)
{
    if (L.elem[0] == e)
    {
        cout << "第一个元素无直接前驱" << endl;
    }
    else if (!LocateElem(L, e, NULL))
    {
        cout << "顺序表中无此元素" << endl;
    }
    else
    {
        for (int i = 1; i < L.length; i++)
        {
            if (L.elem[i] == e)
            {
                cout << L.elem[i - 1] << endl;
                return L.elem[i - 1];
            }
        }
    }
    return -1;
}

ElemType NextDrive(SqList L, ElemType e)
{
    if (L.elem[L.length - 1] == e)
    {
        cout << "最后一个元素无直接后继" << endl;
    }
    else if (!LocateElem(L, e, NULL))
    {
        cout << "顺序表中无此元素" << endl;
    }
    else
    {
        for (int i = 0; i < L.length - 1; i++)
        {
            if (L.elem[i] == e)
            {
                cout << L.elem[i + 1] << endl;
                return L.elem[i + 1];
            }
        }
    }
    return -1;
}

Status InsertList(SqList &L, int i, ElemType e)
{
    if (i < 1 || i > L.length +1)
    {
        return ERROR;
    }
    if (L.length >= MAXSIZE)
    {
        return ERROR;
    }
    for (int j = L.length; j >= i; j--)
    {
        L.elem[j] = L.elem[j - 1];
    }
    L.elem[i - 1] = e;
    L.length++;
    return OK;
}

Status DeleteList(SqList &L, int i)
{
    if (i < 1 || i > L.length)
    {
        return ERROR;
    }
    for (int j = i; j < L.length; j++)
    {
        L.elem[j -1] = L.elem[j];
    }
    L.length--;
    return OK;
}

Status ReviseList(SqList &L, int i, ElemType e)
{
    if (i < 1 || i > L.length)
    {
        exit(ERROR);
    }
    L.elem[i - 1] = e;
    return OK;
}

Status LocateElem(SqList L, ElemType e, int* index)
{
    int count = 0;
    bool flag = false;
    for (int i = 0; i < L.length; i++)
    {
        if (L.elem[i] == e)
        {
            if(index != NULL)
            {
                index[count + 1] = i + 1;
                count++;
            }
            flag = true;
            continue;
        }
    }
    if (flag)
    {
        if(index != NULL)
            index[0] = count;
        return OK;
    }
    else
    {
        return ERROR;
    }
}

void PrintList(SqList L)
{
    for (int i = 0; i < L.length; i++)
    {
        cout << L.elem[i] << " ";
    }
    cout << endl;
}

SqList IntersectionSet(const SqList& A, const SqList& B)
{
    SqList C;
    InitList(C);
    for (int i = 0; i < A.length; i++)
    {
        for (int j = 0; j < B.length; j++)
        {
            if (A.elem[i] == B.elem[j])
            {
                InsertList(C, C.length + 1, A.elem[i]);
            }
        }
    }
    return C;
}

SqList UnionSet(const SqList& A, const SqList& B)
{
    SqList C;
    InitList(C);
    for (int i = 0; i < A.length; i++)
    {
        InsertList(C, C.length + 1, A.elem[i]);
    }
    for (int i = 0; i < B.length; i++)
    {
        if (!LocateElem(C, B.elem[i], NULL))
        {
            InsertList(C, C.length + 1, B.elem[i]);
        }
    }
    return C;
}
SqList DifferenceSet(const SqList& A, const SqList& B)
{
    SqList C;
    InitList(C);
    for (int i = 0; i < A.length; i++)
    {
        if (!LocateElem(B, A.elem[i], NULL))
        {
            InsertList(C, C.length + 1, A.elem[i]);
        }
    }
    return C;
}