/*
（1）编程实现两种查找方法：折半查找和二叉排序树；
若查找成功，返回元素在有序数组中的位置和查找次数；
若查找失败，返回出错标志和查找次数。
（2）从课本中选择两种排序算法进行编程实现。
*/

#include <iostream>
using namespace std;

#define KeyType int

typedef struct {
    KeyType key;
} ElemType;

typedef struct {
    ElemType* R;
    int length;
} SSTable;

typedef struct BSTNode {
    ElemType data;
    struct BSTNode* lchild, *rchild;
} BSTNode, *BSTree;
// 折半查找
int Search_Bin(SSTable ST, KeyType key, int& count) {
    int low = 1, high = ST.length;
    count = 0;
    while (low <= high) {
        count++;
        int mid = (low + high) / 2;
        if (ST.R[mid].key == key) return mid;
        else if (key < ST.R[mid].key) high = mid - 1;
        else low = mid + 1;
    }
    return 0;
}

// 二叉排序树查找（递归）
BSTree SearchBST(BSTree T, KeyType key, int& count) {
    count++;
    if (!T || T->data.key == key) return T;
    else if (key < T->data.key) return SearchBST(T->lchild, key, count);
    else return SearchBST(T->rchild, key, count);
}

// 插入到二叉排序树
bool InsertBST(BSTree& T, KeyType key) {
    if (!T) {
        T = new BSTNode{ {key}, NULL, NULL};
        return true;
    }
    if (key == T->data.key) return false;
    if (key < T->data.key) return InsertBST(T->lchild, key);
    else return InsertBST(T->rchild, key);
}

// 冒泡排序
void BubbleSort(ElemType A[], int n) {
    for (int i = 1; i < n; i++) {
        for (int j = 1; j <= n - i; j++) {
            if (A[j].key > A[j + 1].key)
                swap(A[j], A[j + 1]);
        }
    }
}

// 快速排序
int Partition(ElemType A[], int low, int high) {
    ElemType pivot = A[low];
    while (low < high) {
        while (low < high && A[high].key >= pivot.key) high--;
        A[low] = A[high];
        while (low < high && A[low].key <= pivot.key) low++;
        A[high] = A[low];
    }
    A[low] = pivot;
    return low;
}

void QuickSort(ElemType A[], int low, int high) {
    if (low < high) {
        int pivotpos = Partition(A, low, high);
        QuickSort(A, low, pivotpos - 1);
        QuickSort(A, pivotpos + 1, high);
    }
}


void PrintArray(ElemType A[], int n) {
    for (int i = 1; i <= n; ++i)
        cout << A[i].key << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "请输入元素个数: ";
    cin >> n;

    ElemType* arr = new ElemType[n + 1]; // arr[0]为空
    ElemType* arr2 = new ElemType[n + 1]; // 用于快速排序测试
    cout << "请输入 " << n << " 个整数元素：" << endl;
    for (int i = 1; i <= n; ++i) {
        cin >> arr[i].key;
        arr2[i].key = arr[i].key;
    }
    // --------排序--------
    cout << "原始顺序：";
    PrintArray(arr, n);

    // 冒泡排序
    BubbleSort(arr, n);
    cout << "冒泡排序结果：";
    PrintArray(arr, n);

    // 快速排序
    QuickSort(arr2, 1, n);
    cout << "快速排序结果：";
    PrintArray(arr2, n);

    // 折半查找
    SSTable ST;
    ST.R = arr;
    ST.length = n;
    cout << "-----------------------------------------------" << endl;
    cout << "请输入要查找的元素：";
    KeyType key;
    cin >> key;
    int count = 0;
    int pos = Search_Bin(ST, key, count);
    if (pos){
        cout << "折半查找成功：位置 = " << pos << "，查找次数 = " << count << endl;
    } else {
        cout << "折半查找失败，查找次数 = " << count << endl;
    }
    // 二叉排序树
    BSTree T = NULL;
    for (int i = 1; i <= n; i++) {
        InsertBST(T, arr[i].key);
    }

    count = 0;
    BSTree result = SearchBST(T, key, count);
    if (result)
        cout << "二叉排序树查找成功：值 = " << result->data.key << "，查找次数 = " << count << endl;
    else
        cout << "二叉排序树查找失败，查找次数 = " << count << endl;

    return 0;
}
