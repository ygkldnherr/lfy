/*
（1）编程实现单链表的以下基本操作：建立单链表，查找单链表，插入单链表，删除单链表。
（2）采用单链表结构编程实现：两个有序单链表的归并运算。
*/

#include <iostream>
#define OK 1
#define ERROR 0
#define OVERFLOW -2
using namespace std;

typedef int Status;
typedef int ElemType;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode, *LinkList;

void menu();
Status InitList(LinkList &L);
LinkList LocationElem(LinkList L, ElemType e, int &count);
Status InsertList(LinkList &L, int i, ElemType e);
Status DeleteList(LinkList &L, int i);
void PrintList(LinkList L);
LinkList MergeList(LinkList &La, LinkList &Lb);

int main(){
    LinkList L, La, Lb, Lc;
    LinkList p;
    int op;
    ElemType e;
    int i;
    int count;
    menu();
    while(cin >> op && op > 0){
        switch(op){
            case 1:
                if(InitList(L)){
                    cout << "初始化成功" << endl;
                }
                break;
            case 2:
                cout << "请输入要查找的元素：";
                cin >> e;
                p = LocationElem(L, e, count);
                if(p){
                    cout << "元素存在，地址为：" << p << "，位置为第" << count + 1 << "个" << endl;
                }else{
                    cout << "元素不存在" << endl;
                }
                break;
            case 3:
                cout << "请输入要插入的位置和元素：";
                cin >> i >> e;
                if(InsertList(L, i, e)){
                    cout << "插入成功" << endl;
                }else{
                    cout << "插入失败" << endl;
                }
                break;
            case 4:
                cout << "请输入要删除的位置：";
                cin >> i;
                if(DeleteList(L, i)){
                    cout << "删除成功" << endl;
                }else{
                    cout << "删除失败" << endl;
                }
                break;
            case 5:
                PrintList(L);
                break;
            case 6:
                Lc = MergeList(La, Lb);
                if(Lc){
                    cout << "归并成功" << endl;
                    PrintList(Lc);
                }else{  
                    cout << "归并失败" << endl;
                }
                break;
            default:
                break;
        }
        menu();
    }
    return 0;
}

void menu(){
    cout << "1.建立单链表" << endl;
    cout << "2.查找单链表" << endl;
    cout << "3.插入单链表" << endl;
    cout << "4.删除单链表" << endl;
    cout << "5.打印单链表" << endl;
    cout << "6.两个有序单链表的归并运算" << endl;
    cout << "0.退出" << endl;
}

Status InitList(LinkList &L){
    L = new LNode;
    if(!L){
        exit(OVERFLOW);
    }
    cout << "请输入单链表的长度：";
    int n;
    cin >> n;
    LinkList p = L;
    for(int i = 0; i < n; i++){
        LinkList s = new LNode;
        s->next = NULL;
        cout << "请输入第" << i + 1 << "个元素：";
        cin >> s->data;
        p->next = s;
        p = s;
    }
    return OK;
}

LinkList LocationElem(LinkList L, ElemType e, int &count){
    LinkList p = L->next;
    count = 0;
    while(p && p->data != e){
        p = p->next;
        count++;
    }
    return p;
}

Status InsertList(LinkList &L, int i, ElemType e){
    LinkList p = L;
    int j = 0;
    while(p && j < i - 1){
        p = p->next;
        j++;
    }
    if(!p || j > i - 1){
        return ERROR;
    }
    LinkList s = new LNode;
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

Status DeleteList(LinkList &L, int i){
    LinkList p = L;
    int j = 0;
    while(p->next && j < i - 1){
        p = p->next;
        j++;
    }
    if(!p->next || j > i - 1){
        return ERROR;
    }
    LinkList q = p->next;
    p->next = q->next;
    delete q;
    return OK;
}

void PrintList(LinkList L){
    LinkList p = L->next;
    while(p){
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}

LinkList MergeList(LinkList &La, LinkList &Lb){
    InitList(La);
    InitList(Lb);
    LinkList Lc = new LNode;
    LinkList pa = La->next;
    LinkList pb = Lb->next;
    Lc = La;
    LinkList pc = Lc;
    while(pa && pb){
        if(pa->data <= pb->data){
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else{
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }
    pc->next = pa ? pa : pb;
    delete Lb;
    return Lc;
}