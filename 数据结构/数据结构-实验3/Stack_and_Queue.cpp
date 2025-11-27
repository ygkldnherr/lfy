/*
（1）编程实现栈的以下基本操作：建栈，取栈顶元素，入栈，出栈。
（2）编程实现队列的以下基本操作：建队列，取队头元素，入队，出队。
*/

#include <bits/stdc++.h>
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAXSIZE 100
using namespace std;

typedef int Status;
typedef int SElemType;
typedef int QElemType;

#pragma region SequenceStack
typedef struct {
    SElemType *base;
    SElemType *top;
    int stacksize;
}SqStack;

Status InitStack(SqStack &S) {
    S.base = new SElemType[100];
    if(!S.base) {
        exit(OVERFLOW);
    }
    S.top = S.base;
    S.stacksize = 100;
    return OK;
}

SElemType GetTop(SqStack S) {
    if (S.top != S.base) {
        return *(S.top - 1);
    } else {
        cerr << "Error: Stack is empty." << endl;
        exit(ERROR); // Exit with an error code
    }
}

Status Push(SqStack &S, SElemType e) {
    if (S.top - S.base >= S.stacksize) {
        return ERROR;
    }
    *S.top++ = e;
    return OK;
}

Status Pop(SqStack &S, SElemType &e) {
    if (S.top == S.base) {
        return ERROR;
    }
    e = *--S.top;
    return OK;
}
#pragma endregion SequenceStack

#pragma region LinkStack
typedef struct StackNode {
    SElemType data;
    struct StackNode *next;
}StackNode, *LinkStack;

Status InitStack(LinkStack &S) {
    S = NULL;
    return OK;
}

SElemType GetTop(LinkStack S) {
    if (S) {
        return S->data;
    }else {
        cerr << "Error: Stack is empty." << endl;
        exit(ERROR);
    }
}

Status Push(LinkStack &S, SElemType &e) {
    LinkStack p = new StackNode;
    p->data = e;
    p->next = S;
    S = p;
    return OK;
}

Status Pop(LinkStack &S, SElemType &e) {
    if (S == NULL) {
        return ERROR;
    }
    e = S->data;
    LinkStack p = S;
    S = S->next;
    delete p;
    return OK;
}
#pragma endregion LinkStack

#pragma region SequenceQueue
typedef struct {
    QElemType *base;
    int front;
    int rear;
}SqQueue;

Status InitSqQueue(SqQueue &Q) {
    Q.base = new QElemType[MAXSIZE];
    if (!Q.base) {
        exit(OVERFLOW);
    }
    Q.front = Q.rear = 0;
    return OK;
}

QElemType GetHead(SqQueue Q) {
    if (Q.front != Q.rear) {
        return Q.base[Q.front];
    }else {
        cerr << "Error: Queue is empty." << endl;
        exit(ERROR);
    }
}

Status EnQueue(SqQueue &Q, QElemType e) {
    if ((Q.rear + 1) %MAXSIZE == Q.front) {
        return ERROR;
    }
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MAXSIZE;
    return OK;
}

Status DeQueue(SqQueue &Q, QElemType &e) {
    if (Q.front == Q.rear) {
        return ERROR;
    }
    e = Q.base[Q.front];
    Q.front = (Q.front + 1) % MAXSIZE;
    return OK;
}
#pragma endregion SequenceQueue

#pragma region LinkQueue
typedef struct QNode {
    QElemType data;
    struct QNode *next;
}QNode, *QueuePtr;

typedef struct {
    QueuePtr front;
    QueuePtr rear;
}LinkQueue;

Status InitLinkQueue(LinkQueue &Q) {
    Q.front = Q.rear = new QNode;
    if (!Q.front) {
        exit(OVERFLOW);
    }
    Q.front->next = NULL;
    return OK;
}

QElemType GetHead(LinkQueue Q) {
    if (Q.front != Q.rear) {
        return Q.front->next->data;
    }else {
        cerr << "Error: Queue is empty." << endl;
        exit(ERROR);
    }
}

Status EnQueue(LinkQueue &Q, QElemType e) {
    QueuePtr p = new QNode;
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return OK;
}

Status DeQueue(LinkQueue &Q, QElemType &e) {
    if (Q.front == Q.rear) {
        return ERROR;
    }
    QueuePtr p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) {
        Q.rear = Q.front;
    }
    delete p;
    return OK;
}
#pragma endregion LinkQueue

#pragma region Union
union Stack {
    SqStack sqstack;
    LinkStack linkstack;
};

union Queue {
    SqQueue sqqueue;
    LinkQueue linkqueue;
};
#pragma endregion Union

void menu();
void StackFunctionChoose(bool flag);
void QueueFunctionChoose(bool flag);

int main () {
    int op;
    menu();
    cin >> op;
    while(op != 0) {
        switch (op)
        {
        case 1:
            StackFunctionChoose(0);
            break;
        case 2:
            StackFunctionChoose(1);
            break;
        case 3:
            QueueFunctionChoose(0);
            break;
        case 4:
            QueueFunctionChoose(1);
            break;
        default:
            break;
        }
        menu();
        cin >> op;
    }
    return 0;
}

#pragma region Menu
void menu() {
    cout << "1. Sequence Stack" << endl;
    cout << "2. Link Stack" << endl;
    cout << "3. Sequence Queue" << endl;
    cout << "4. Link Queue" << endl;
    cout << "0. Exit" << endl;
    cout << "请输入相应编号" << endl;
}

void StackFunctionChoose(bool flag) {
    if(!flag) {
        cout << "---------------Sequence Stack---------------" << endl;
    }else {
        cout << "---------------Link Stack---------------" << endl;
    }
    cout << "1. InitStack" << endl;
    cout << "2. GetTop" << endl;
    cout << "3. Push" << endl;
    cout << "4. Pop" << endl;
    cout << "0. Exit" << endl;
    cout << "请输入相应编号";
    if(!flag) {
        cout << "(Sequence Stack):" << endl;
    }else {
        cout << "(Link Stack):" << endl;
    }
    int op;
    cin >> op;
    Stack S;
    SElemType e;
    while(op != 0) {
        switch (op)
        {
        case 1:
            if (!flag) {
                InitStack(S.sqstack);
            }else {
                InitStack(S.linkstack);
            }
            break;
        case 2:
            if(!flag) {
                e = GetTop(S.sqstack);
            }else {
                e = GetTop(S.linkstack);
            }
            cout << "栈顶元素为" << e << endl;
            break;
        case 3:
            cout << "请输入元素" << endl;
            cin >> e;
            if(!flag) {
                Push(S.sqstack, e);
            }else {
                Push(S.linkstack, e);
            }
            break;
        case 4:
            if(!flag) {
                if(Pop(S.sqstack, e)) {
                    cout << "弹出元素为" << e << endl;
                }
            }else {
                if(Pop(S.linkstack, e)) {
                    cout << "弹出元素为" << e << endl;
                }
            }
            break;
        default:
            break;
        }
        cout << "请输入相应编号" << endl;
        cin >> op;
    }
}

void QueueFunctionChoose(bool flag) {
    if(!flag) {
        cout << "---------------Sequence Queue---------------" << endl;
    }else {
        cout << "---------------Link Queue---------------" << endl;
    }
    cout << "1. InitQueue" << endl;
    cout << "2. GetHead" << endl;
    cout << "3. EnQueue" << endl;
    cout << "4. DeQueue" << endl;
    cout << "0. Exit" << endl;
    cout << "请输入相应编号";
    if(!flag) {
        cout << "(Sequence Queue):" << endl;
    }else {
        cout << "(Link Queue):" << endl;
    }
    int op;
    cin >> op;
    // if (!flag) {
    //     SqQueue Q;
    // } else {
    //     LinkQueue Q;
    // }
    Queue Q;
    QElemType e;
    while(op != 0) {
        switch (op)
        {
        case 1:
            if(!flag) {
                InitSqQueue(Q.sqqueue);
            }else {
                InitLinkQueue(Q.linkqueue);
            }
            break;
        case 2:
            if(!flag) {
                e = GetHead(Q.sqqueue);
            }else {
                e = GetHead(Q.linkqueue);
            }
            cout << "队头元素为" << e << endl;
            break;
        case 3:
            cout << "请输入元素" << endl;
            cin >> e;
            if(!flag) {
                EnQueue(Q.sqqueue, e);
            }else {
                EnQueue(Q.linkqueue, e);
            }
            break;
        case 4:
            if(!flag) {
                if(DeQueue(Q.sqqueue, e)) {
                    cout << "弹出元素为" << e << endl;
                }
            }else { 
                if(DeQueue(Q.linkqueue, e)) {
                    cout << "弹出元素为" << e << endl;
                }
            }
            break;
        default:
            break;
        }
        cout << "请输入相应编号" << endl;
        cin >> op;
    }
}
#pragma endregion Menu