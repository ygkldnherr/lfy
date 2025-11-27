#include <bits/stdc++.h>
using namespace std;

#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define ElementType char
typedef int Status;
typedef char **HuffmanCode;

int weights[27] = {64, 13, 22, 32, 103, 21, 15, 47, 57, 1, 5, 32, 20, 57, 63, 15, 1, 48, 51, 80, 23, 8, 18, 1, 16, 1, 168}; // 26个字母和空格

typedef struct {
    int weight;
    int parent, lchild, rchild;
}HTNode, *HuffmanTree;

void menu();
void input_args();
void init_args(HuffmanTree &HT, int n, int weight[] = weights);
void select(HuffmanTree &HT, int n, int &s1, int &s2); 
void create_HuffmanTree_and_HuffmanCode(HuffmanTree &HT, HuffmanCode &HC, int n);
void print_HuffmanCode(HuffmanCode &HC, int n);
void translate(HuffmanCode &HC, int n);
void translate_to_code(HuffmanCode &HC, int n);

int main() {
    HuffmanTree HT;
    HuffmanCode HC;
    int n = 27; // 26个字母和空格
    int choice = 0;
    while (true) {
        menu();
        cout << "请输入你的选择: ";
        cin >> choice;
        switch (choice) {
            case 1:
                input_args();
                break;
            case 2:
                init_args(HT, n, weights);
                cout << "初始化完成!" << endl;
                break;
            case 3:
                create_HuffmanTree_and_HuffmanCode(HT, HC, n);
                break;
            case 4:
                print_HuffmanCode(HC, n);
                break;
            case 5:
                translate(HC, n);
                break;
            case 6:
                translate_to_code(HC, n);
                break;
            case 7:
                cout << "退出程序!" << endl;
                for (int i = 1; i <= n; i++) {
                    delete[] HC[i];
                }
                delete[] HC;
                delete[] HT;
                return 0;
            default:
                cout << "输入错误，请重新输入!" << endl;
        }
    }
    return 0;
}

void menu() {
    cout << "***********************************************************************" << endl;
    cout << "*                           Huffman Tree Menu                        *" << endl;
    cout << "*****************************************************************************" << endl;
    cout << "******* 1. 输入HuffmanTree的参数.                                   *******" << endl;
    cout << "******* 2. 初始化HuffmanTree的参数.（含有26个字母及空格）          *******" << endl;
    cout << "******* 3. 创建HuffmanTree和编码表.                                 *******" << endl;
    cout << "******* 4. 输出编码表.                                             *******" << endl;
    cout << "******* 5. 输入编码，并翻译为字符.                                 *******" << endl;
    cout << "******* 6. 输入字符，并实现转码.                                   *******" << endl;
    cout << "******* 7. 退出.                                                  *******" << endl;
    cout << "*****************************************************************************" << endl;
}

void input_args() {
    cout << "请输入26个字母和空格的权值（用空格隔开）:" << endl;
    for (int i = 0; i < 27; i++) {
        cin >> weights[i];
    }
    cout << "输入完成!" << endl;
}

void init_args(HuffmanTree &HT, int n, int weights[]) {
    if (n <= 1) {
        return;
    }
    int m = 2*n - 1;
    HT = new HTNode[m + 1];
    for (int i = 1; i <= m; i++) {
        HT[i].parent = 0;
        HT[i].lchild = 0;
        HT[i].rchild = 0;
    }
    for (int i = 1; i <= n; i++) {
        HT[i].weight = weights[i - 1];
    }
}

void select(HuffmanTree &HT, int n, int &s1, int &s2) {
    int min1 = INT_MAX, min2 = INT_MAX;
    s1 = s2 = 0;
    for (int i = 1; i <= n; i++) {
        if (HT[i].parent == 0) {
            if (HT[i].weight < min1) {
                min2 = min1;
                s2 = s1;
                min1 = HT[i].weight;
                s1 = i;
            } else if (HT[i].weight < min2) {
                min2 = HT[i].weight;
                s2 = i;
            }
        }
    }
}

void create_HuffmanTree_and_HuffmanCode(HuffmanTree &HT, HuffmanCode &HC, int n) {
    if (n <= 1) {
        return;
    }
    int m = 2 * n - 1, s1, s2;
    for (int i = n + 1; i <= m; i++) {
        select(HT, i-1, s1, s2);
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;
    }

    HC = new char*[n + 1];
    char* cd = new char[n];
    cd[n - 1] = '\0';
    for (int i = 1; i <= n; i++) {
        int start = n - 1;
        int c = i, f = HT[i].parent;
        while (f != 0) {
            --start;
            if (HT[f].lchild == c) {
                cd[start] = '0';
            } else {
                cd[start] = '1';
            }
            c = f;
            f = HT[f].parent;
        }
        HC[i] = new char[n - start];
        strcpy(HC[i], &cd[start]);
    }
    delete[] cd;
    cout << "HuffmanTree和编码表创建完成!" << endl;
}

void print_HuffmanCode(HuffmanCode &HC, int n) {
    cout << "Huffman编码表:" << endl;
    for (int i = 1; i <= n; i++) {
        char symbol = (i == 27) ? ' ' : ('a' + i - 1);
        cout << "字符" << symbol << ": " << HC[i] << endl;

    }
}

void translate(HuffmanCode &HC, int n) {
    cout << "请输入Huffman编码: ";
    string code;
    cin >> code;

    // 构建哈夫曼树，找到根节点
    int root = 2 * n - 1;
    int p = root;
    HuffmanTree HT;
    init_args(HT, n, weights);
    create_HuffmanTree_and_HuffmanCode(HT, HC, n);

    cout << "翻译结果: ";
    for (char bit : code) {
        if (bit == '0') p = HT[p].lchild;
        else if (bit == '1') p = HT[p].rchild;
        else {
            cout << "\n编码中含有非法字符！" << endl;
            return;
        }

        if (HT[p].lchild == 0 && HT[p].rchild == 0) {
            char ch = (p == 27) ? ' ' : ('a' + p - 1);
            cout << ch;
            p = root;
        }
    }
    cout << endl;
}

void translate_to_code(HuffmanCode &HC, int n) {
    cout << "请输入字符串: ";
    string input;
    getline(cin, input); // 读取整行，包括空格

    
    if (input.empty()) getline(cin, input);
    for(int i = 0; i < input.length(); i++) {
        if(input[i] >= 'A' && input[i] <= 'Z') {
            input[i] = tolower(input[i]);
        }
    }
    
    cout << input << endl;

    cout << "Huffman编码: ";
    for (char ch : input) {
        int index;
        if (ch == ' ') {
            index = 27;
        }
        else {
            index = ch - 'a' + 1;
        }
        cout << HC[index];
    }
    cout << endl;
}
