/*
（1）采用邻接矩阵/邻接表建立图；
（2）采用深度优先/广度优先搜索方式遍历图；
（3）编程实现Dijkstra最短路径算法。
*/
#include <iostream>
#include <queue>
#include <climits>
using namespace std;
#define MAXInt INT_MAX
#define MVNum 100
#define OK 1
#define ERROR -1
#define OVERFLOW 0
typedef int Status;
typedef char VertexType;
typedef int ArcType;
typedef struct {
    VertexType vexs[MVNum]; 
    ArcType arcs[MVNum][MVNum]; 
    int vexnum, arcnum; 
}AMGraph; 
typedef struct ArcNode{ 
    int adjvex; 
    struct ArcNode *nextarc; 
    ArcType weight;
}ArcNode;
typedef struct VNode{
    VertexType data;
    ArcNode *firstarc;
}VNode, AdjList[MVNum];
typedef struct{
    AdjList vertices;
    int vexnum, arcnum;
}ALGraph;

int LocateVex(ALGraph G, VertexType v);
int LocateVex(AMGraph G, VertexType v);
Status CreateUDG(AMGraph &G);
Status CreateUDG(ALGraph &G);
Status CreateUDN(ALGraph &G);
bool visited[MVNum] = {false};
void DFS(ALGraph &G, int v, void (*visit)(VertexType));
void BFS(ALGraph &G, int v, void (*visit)(VertexType));
void Dijkstra(ALGraph &G, int v, int dist[], int path[]);

void menu();

int main(){
    AMGraph G1;
    ALGraph G2;
    int op;
    menu();
    while(cin >> op){
        switch(op){
            case 1: // 创建无向图（邻接矩阵）
                CreateUDG(G1);
                break;
            case 2: // 创建无向图（邻接表）
                CreateUDG(G2);
                break;
            case 3: { // 深度优先搜索
                cout << "请输入起始顶点: ";
                VertexType startVertex;
                cin >> startVertex;
                int startIndex = LocateVex(G2, startVertex);
                if (startIndex != -1) {
                    cout << "DFS遍历结果 (邻接表): ";
                    // 重置visited数组
                    for(int i = 0; i < G2.vexnum; i++) visited[i] = false;
                    DFS(G2, startIndex, [](VertexType v){ cout << v << " "; });
                    cout << endl;
                } else {
                    cout << "顶点不存在!" << endl;
                }
                break;
            }
            case 4: { // 广度优先搜索
                cout << "请输入起始顶点: ";
                VertexType startVertex;
                cin >> startVertex;
                int startIndex = LocateVex(G2, startVertex);
                if (startIndex != -1) {
                    cout << "BFS遍历结果 (邻接表): ";
                    // 重置visited数组
                    for(int i = 0; i < G2.vexnum; i++) visited[i] = false;
                    BFS(G2, startIndex, [](VertexType v){ cout << v << " "; });
                    cout << endl;
                } else {
                    cout << "顶点不存在!" << endl;
                }
                break;
            }
            case 5: { // Dijkstra最短路径算法
                cout << "请输入起始顶点: ";
                VertexType startVertex;
                cin >> startVertex;
                int startIndex = LocateVex(G2, startVertex);
                if (startIndex != -1) {
                    int dist[MVNum], path[MVNum];
                    Dijkstra(G2, startIndex, dist, path);
                    
                    cout << "从顶点 " << startVertex << " 到各顶点的最短距离:" << endl;
                    for (int i = 0; i < G2.vexnum; i++) {
                        cout << G2.vertices[i].data << ": ";
                        if (dist[i] == MAXInt) {
                            cout << "∞";
                        } else {
                            cout << dist[i];
                        }
                        cout << ", 路径: ";
                        if (dist[i] == MAXInt) {
                            cout << "不可达";
                        } else {
                            // 打印路径
                            int p = i;
                            vector<int> pathStack;
                            while (p != -1) {
                                pathStack.push_back(p);
                                p = path[p];
                            }
                            for (int j = pathStack.size()-1; j >= 0; j--) {
                                cout << G2.vertices[pathStack[j]].data;
                                if (j != 0) cout << " -> ";
                            }
                        }
                        cout << endl;
                    }
                } else {
                    cout << "顶点不存在!" << endl;
                }
                break;
            }
            case 6: { // 创建带权无向图（邻接表）
                CreateUDN(G2);
                break;
            }
            case 0: // 退出
                cout << "退出程序!" << endl;
                return 0;
            default:
                cout << "无效操作!" << endl;
        }
        menu();
    }
    return 0;
}

void menu(){
    cout << "1. 创建无向图（邻接矩阵）" << endl;
    cout << "2. 创建无向图（邻接表）" << endl;
    cout << "3. 深度优先搜索（邻接表）" << endl;
    cout << "4. 广度优先搜索（邻接表）" << endl;
    cout << "5. Dijkstra最短路径算法（邻接表）" << endl;
    cout << "6. 创建带权无向图（邻接表）" << endl;
    cout << "0. 退出" << endl;
    cout << "请选择操作: ";
}

int LocateVex(ALGraph G, VertexType v){
    for(int i = 0; i < G.vexnum; i++){
        if(G.vertices[i].data == v){
            return i;
        }
    }
    return -1;
}

int LocateVex(AMGraph G, VertexType v){
    for(int i = 0; i < G.vexnum; i++){
        if(G.vexs[i] == v){
            return i;
        }
    }
    return -1;
}

Status CreateUDG(AMGraph &G){
    cout << "请输入顶点数和边数: ";
    cin >> G.vexnum >> G.arcnum;
    cout << "请输入各顶点名称: ";
    for(int i = 0; i < G.vexnum; i++){
        cin >> G.vexs[i];
        for(int j = 0; j < G.vexnum; j++){
            G.arcs[i][j] = MAXInt;
        }
    }
    cout << "请输入各条边的两个顶点(格式: v1 v2):" << endl;
    for(int k = 0; k < G.arcnum; k++){
        VertexType v1, v2;
        cin >> v1 >> v2;
        int i = LocateVex(G, v1);
        int j = LocateVex(G, v2);
        G.arcs[i][j] = 1;
        G.arcs[j][i] = 1;
    }
    return OK;
}

Status CreateUDG(ALGraph &G){
    cout << "请输入顶点数和边数: ";
    cin >> G.vexnum >> G.arcnum;
    cout << "请输入各顶点名称: ";
    for(int i = 0; i < G.vexnum; i++){
        cin >> G.vertices[i].data;
        G.vertices[i].firstarc = NULL;
    }
    cout << "请输入各条边的两个顶点(格式: v1 v2):" << endl;
    for(int k = 0; k < G.arcnum; k++){
        VertexType v1, v2;
        cin >> v1 >> v2;
        int i = LocateVex(G, v1);
        int j = LocateVex(G, v2);
        
        ArcNode *p1 = new ArcNode;
        p1->adjvex = j;
        p1->weight = 1;  // 无权重图默认为1
        p1->nextarc = G.vertices[i].firstarc;
        G.vertices[i].firstarc = p1;

        ArcNode *p2 = new ArcNode;
        p2->adjvex = i;
        p2->weight = 1;  // 无权重图默认为1
        p2->nextarc = G.vertices[j].firstarc;
        G.vertices[j].firstarc = p2;
    }
    return OK;
}

Status CreateUDN(ALGraph &G) {
    cout << "请输入顶点数和边数: ";
    cin >> G.vexnum >> G.arcnum;
    cout << "请输入各顶点名称: ";
    for(int i = 0; i < G.vexnum; i++){
        cin >> G.vertices[i].data;
        G.vertices[i].firstarc = NULL;
    }
    cout << "请输入各条边的两个顶点及权重(格式: v1 v2 weight):" << endl;
    for(int k = 0; k < G.arcnum; k++){
        VertexType v1, v2;
        ArcType weight;
        cin >> v1 >> v2 >> weight;
        int i = LocateVex(G, v1);
        int j = LocateVex(G, v2);
        
        ArcNode *p1 = new ArcNode;
        p1->adjvex = j;
        p1->weight = weight;
        p1->nextarc = G.vertices[i].firstarc;
        G.vertices[i].firstarc = p1;

        ArcNode *p2 = new ArcNode;
        p2->adjvex = i;
        p2->weight = weight;
        p2->nextarc = G.vertices[j].firstarc;
        G.vertices[j].firstarc = p2;
    }
    return OK;
}

// DFS递归实现（邻接表）
void DFS(ALGraph &G, int v, void (*visit)(VertexType)) {
    visit(G.vertices[v].data);
    visited[v] = true;
    for (ArcNode *p = G.vertices[v].firstarc; p != nullptr; p = p->nextarc) {
        if (!visited[p->adjvex]) {
            DFS(G, p->adjvex, visit);
        }
    }
}

// BFS实现（邻接表）
void BFS(ALGraph &G, int v, void (*visit)(VertexType)) {
    queue<int> Q;
    visit(G.vertices[v].data);
    visited[v] = true;
    Q.push(v);
    while (!Q.empty()) {
        int u = Q.front(); 
        Q.pop();
        for (ArcNode *p = G.vertices[u].firstarc; p != nullptr; p = p->nextarc) {
            if (!visited[p->adjvex]) {
                visit(G.vertices[p->adjvex].data);
                visited[p->adjvex] = true;
                Q.push(p->adjvex);
            }
        }
    }
}

void Dijkstra(ALGraph &G, int v, int dist[], int path[]) {
    bool collected[MVNum] = {false};
    
    // 初始化
    for (int i = 0; i < G.vexnum; i++) {
        dist[i] = MAXInt;
        path[i] = -1;
        collected[i] = false;
    }
    dist[v] = 0;
    
    for (int i = 0; i < G.vexnum; i++) {
        // 找到未收录顶点中dist最小者u
        int u = -1;
        int minDist = MAXInt;
        for (int j = 0; j < G.vexnum; j++) {
            if (!collected[j] && dist[j] < minDist) {
                u = j;
                minDist = dist[j];
            }
        }
        
        if (u == -1) break; // 所有可达顶点都已收录
        collected[u] = true;

        // 更新u的邻接点的dist
        for (ArcNode *p = G.vertices[u].firstarc; p != nullptr; p = p->nextarc) {
            int w = p->adjvex;
            if (!collected[w] && dist[u] + p->weight < dist[w]) {
                dist[w] = dist[u] + p->weight;
                path[w] = u;
            }
        }
    }
}