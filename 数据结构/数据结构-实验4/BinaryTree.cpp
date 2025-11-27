#include <bits/stdc++.h>
using namespace std;

#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define ElementType char
typedef int Status;

typedef struct BiTNode {
    ElementType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

void CreateBiTree(BiTree &T) {
    ElementType x;
    cin >> x;
    if (x == '#') {
        T = NULL;
    } else {
        T = (BiTree)malloc(sizeof(BiTNode));
        if (!T) {
            exit(OVERFLOW);
        }
        T->data = x;
        CreateBiTree(T->lchild);
        CreateBiTree(T->rchild);
    }
}

void PreOrderTraverse(BiTree T) {
    if (T) {
        cout << T->data << " ";
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
}

void InOrderTraverse(BiTree T) {
    if (T) {
        InOrderTraverse(T->lchild);
        cout << T->data << " ";
        InOrderTraverse(T->rchild);
    }
}

void PostOrderTraverse(BiTree T) {
    if (T) {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        cout << T->data << " ";
    }
}

void InOrderTraverseNonRecursive(BiTree T) {
    stack<BiTree> s;
    BiTree p = T;
    while (p || !s.empty()) {
        while (p) {
            s.push(p);
            p = p->lchild;
        }
        if (!s.empty()) {
            p = s.top();
            s.pop();
            cout << p->data << " ";
            p = p->rchild;
        }
    }
}

void LevelOrderTraverse(BiTree T) {
    if (!T) {
        return;
    }
    queue<BiTree> q;
    q.push(T);
    while (!q.empty()) {
        BiTree p = q.front();
        q.pop();
        cout << p->data << " ";
        if (p->lchild) {
            q.push(p->lchild);
        }
        if (p->rchild) {
            q.push(p->rchild);
        }
    }
}

void Height(BiTree T, int &h) {
    if (T) {
        int hl = 0, hr = 0;
        Height(T->lchild, hl);
        Height(T->rchild, hr);
        h = max(hl, hr) + 1;
    } else {
        h = 0;
    }
}

void LeafCount(BiTree T, int &count) {
    if (T) {
        if (!T->lchild && !T->rchild) {
            count++;
        } else {
            LeafCount(T->lchild, count);
            LeafCount(T->rchild, count);
        }
    }
}

int main() {
    BiTree T;
    cout << "输入树的节点(用#表示空节点): ";
    CreateBiTree(T);
    cout << "先序遍历: ";
    PreOrderTraverse(T);
    cout << endl;
    cout << "中序遍历: ";
    InOrderTraverse(T);
    cout << endl;
    cout << "后序遍历: ";
    PostOrderTraverse(T);
    cout << endl;
    cout << "中序遍历(非递归): ";
    InOrderTraverseNonRecursive(T);
    cout << endl;
    cout << "层序遍历: ";
    LevelOrderTraverse(T);
    cout << endl;
    int height = 0;
    Height(T, height);
    cout << "深度: " << height << endl;
    int leafCount = 0;
    LeafCount(T, leafCount);
    cout << "叶子节点数目: " << leafCount << endl;
    return 0;
}