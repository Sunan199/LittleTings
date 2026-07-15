#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>
#include <queue>
using namespace std;

// 并查集结构，用于Kruskal算法判断连通性
struct UnionFind {
    vector<int> parent; // 父节点数组
    vector<int> rank;   // 秩，用于按秩合并

    // 构造函数：初始化并查集
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i; // 初始时每个节点的父节点是自己
        }
    }

    // 查找根节点（带路径压缩）
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩，让节点直接指向根
        }
        return parent[x];
    }

    // 合并两个集合（按秩合并）
    bool unite(int x, int y) {
        int x_root = find(x);
        int y_root = find(y);
        if (x_root == y_root) {
            return false; // 已经在同一集合，合并失败
        }
        // 秩小的树合并到秩大的树下
        if (rank[x_root] < rank[y_root]) {
            parent[x_root] = y_root;
        }
        else {
            parent[y_root] = x_root;
            if (rank[x_root] == rank[y_root]) {
                rank[x_root]++; // 秩相同，合并后秩+1
            }
        }
        return true; // 合并成功
    }
};

// 边的结构体，用于Kruskal算法和回溯法
struct Edge {
    int u;      // 起点
    int v;      // 终点
    int weight; // 权值

    // 重载小于运算符，用于排序
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// 图的类
class Graph {
private:
    int vertex_num;          // 顶点数
    vector<vector<int>> adj; // 邻接矩阵（Prim算法用）
    vector<Edge> edges;      // 边集合（Kruskal算法和回溯法用）

    // 回溯法相关的私有成员
    vector<Edge> mst_backtrack_edges; // 回溯法找到的最小生成树边
    int min_backtrack_weight;         // 回溯法找到的最小总权值

    // 辅助函数：判断选中的边是否能连通所有节点（BFS实现，更高效）
    bool isConnected(const vector<Edge>& selected_edges) {
        if (vertex_num == 1) { // 单个节点，天然连通
            return true;
        }
        // 构建邻接表
        vector<vector<int>> adj_list(vertex_num);
        for (const auto& e : selected_edges) {
            adj_list[e.u].push_back(e.v);
            adj_list[e.v].push_back(e.u);
        }
        // BFS遍历
        vector<bool> visited(vertex_num, false);
        queue<int> q;
        q.push(0); // 从顶点0开始
        visited[0] = true;
        int visited_count = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj_list[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    visited_count++;
                    q.push(v);
                }
            }
        }
        return visited_count == vertex_num; // 所有节点都被访问则连通
    }

    // 回溯法核心函数：递归枚举边的选择
    // index: 当前处理的边的索引
    // selected_count: 已选中的边数
    // current_weight: 已选中边的总权值
    // selected_edges: 已选中的边列表
    void backtrack(int index, int selected_count, int current_weight, vector<Edge>& selected_edges) {
        // 剪枝1：已选边数超过n-1，或当前权值已超过已知最小值，直接返回
        if (selected_count > vertex_num - 1 || current_weight >= min_backtrack_weight) {
            return;
        }
        // 终止条件：选够n-1条边，验证是否连通
        if (selected_count == vertex_num - 1) {
            if (isConnected(selected_edges)) {
                // 更新最小权值和对应边
                min_backtrack_weight = current_weight;
                mst_backtrack_edges = selected_edges;
            }
            return;
        }
        // 递归终止：处理完所有边
        if (index >= edges.size()) {
            return;
        }

        // 选择当前边
        selected_edges.push_back(edges[index]);
        backtrack(index + 1, selected_count + 1, current_weight + edges[index].weight, selected_edges);
        selected_edges.pop_back(); // 回溯

        // 不选择当前边
        backtrack(index + 1, selected_count, current_weight, selected_edges);
    }

public:
    // 构造函数：初始化顶点数
    Graph(int n) : vertex_num(n) {
        adj.resize(n, vector<int>(n, 0)); // 邻接矩阵初始化为0
        // 初始化回溯法的最小权值为无穷大
        min_backtrack_weight = INT_MAX;
    }

    // 添加边（双向边，因为最小生成树处理无向图）
    void addEdge(int u, int v, int weight) {
        if (u < 0 || u >= vertex_num || v < 0 || v >= vertex_num) {
            cout << "顶点编号非法，添加失败！" << endl;
            return;
        }
        adj[u][v] = weight;
        adj[v][u] = weight;
        edges.push_back({ u, v, weight });
    }

    // Prim算法实现最小生成树
    void prim(int start = 0) {
        if (start < 0 || start >= vertex_num) {
            cout << "起始顶点非法！" << endl;
            return;
        }

        vector<int> key(vertex_num, INT_MAX); // 每个顶点到生成树的最小权值
        vector<int> parent(vertex_num, -1);   // 记录生成树中顶点的父节点
        vector<bool> in_mst(vertex_num, false); // 标记顶点是否在生成树中

        key[start] = 0; // 起始顶点的权值设为0

        // 遍历所有顶点（需要选n-1条边）
        for (int i = 0; i < vertex_num - 1; i++) {
            // 步骤1：找到权值最小且不在生成树中的顶点u
            int u = -1;
            int min_key = INT_MAX;
            for (int v = 0; v < vertex_num; v++) {
                if (!in_mst[v] && key[v] < min_key) {
                    min_key = key[v];
                    u = v;
                }
            }

            if (u == -1) {
                cout << "图不连通，无法生成最小生成树！" << endl;
                return;
            }

            in_mst[u] = true; // 将u加入生成树

            // 步骤2：更新u的邻接顶点的权值
            for (int v = 0; v < vertex_num; v++) {
                if (!in_mst[v] && adj[u][v] != 0 && adj[u][v] < key[v]) {
                    key[v] = adj[u][v];
                    parent[v] = u;
                }
            }
        }

        // 输出Prim算法的结果
        cout << "\n===== Prim算法生成的最小生成树 =====" << endl;
        int total_weight = 0;
        cout << "边（u -> v）\t权值" << endl;
        for (int v = 1; v < vertex_num; v++) {
            if (parent[v] == -1) {
                cout << "图不连通，无法生成最小生成树！" << endl;
                return;
            }
            cout << parent[v] << " -> " << v << "\t\t" << adj[parent[v]][v] << endl;
            total_weight += adj[parent[v]][v];
        }
        cout << "最小生成树的总权值：" << total_weight << endl;
    }

    // Kruskal算法实现最小生成树
    void kruskal() {
        // 步骤1：将所有边按权值从小到大排序
        sort(edges.begin(), edges.end());

        UnionFind uf(vertex_num); // 初始化并查集
        vector<Edge> mst_edges;   // 存储最小生成树的边
        int total_weight = 0;     // 最小生成树的总权值

        // 步骤2：遍历排序后的边，选择不形成环的边
        for (const auto& edge : edges) {
            int u = edge.u;
            int v = edge.v;
            int weight = edge.weight;

            // 如果u和v不在同一连通分量，加入生成树
            if (uf.unite(u, v)) {
                mst_edges.push_back(edge);
                total_weight += weight;
                // 最小生成树有n-1条边时，提前结束
                if (mst_edges.size() == vertex_num - 1) {
                    break;
                }
            }
        }

        // 输出Kruskal算法的结果
        cout << "\n===== Kruskal算法生成的最小生成树 =====" << endl;
        if (mst_edges.size() != vertex_num - 1) {
            cout << "图不连通，无法生成最小生成树！" << endl;
            return;
        }
        cout << "边（u - v）\t权值" << endl;
        for (const auto& edge : mst_edges) {
            cout << edge.u << " - " << edge.v << "\t\t" << edge.weight << endl;
        }
        cout << "最小生成树的总权值：" << total_weight << endl;
    }

    // 回溯法实现最小生成树（新增函数）
    void backtrackMST() {
        // 重置回溯法的状态
        min_backtrack_weight = INT_MAX;
        mst_backtrack_edges.clear();
        vector<Edge> selected_edges; // 存储当前选中的边

        // 调用回溯函数，从第0条边开始处理
        backtrack(0, 0, 0, selected_edges);

        // 输出回溯法的结果
        cout << "\n===== 回溯法生成的最小生成树 =====" << endl;
        if (mst_backtrack_edges.empty()) {
            cout << "图不连通，无法生成最小生成树！" << endl;
            return;
        }
        cout << "边（u - v）\t权值" << endl;
        for (const auto& edge : mst_backtrack_edges) {
            cout << edge.u << " - " << edge.v << "\t\t" << edge.weight << endl;
        }
        cout << "最小生成树的总权值：" << min_backtrack_weight << endl;
    }

    // 打印图的邻接矩阵
    void printAdjMatrix() {
        cout << "\n===== 图的邻接矩阵 =====" << endl;
        cout << "顶点：";
        for (int i = 0; i < vertex_num; i++) {
            cout << setw(4) << i;
        }
        cout << endl;
        for (int i = 0; i < vertex_num; i++) {
            cout << "顶点" << i << "：";
            for (int j = 0; j < vertex_num; j++) {
                cout << setw(4) << adj[i][j];
            }
            cout << endl;
        }
    }
};

// 主函数：提供交互界面
int main() {
    int vertex_num, edge_num;
    cout << "===== 最小生成树课程设计 =====" << endl;
    cout << "请输入图的顶点数（顶点编号从0开始）：";
    cin >> vertex_num;
    if (vertex_num <= 0) {
        cout << "顶点数必须为正整数！" << endl;
        return 1;
    }

    Graph g(vertex_num);

    cout << "请输入图的边数：";
    cin >> edge_num;
    if (edge_num < 0) {
        cout << "边数不能为负数！" << endl;
        return 1;
    }

    cout << "请依次输入每条边的起点、终点、权值（以空格分隔）：" << endl;
    for (int i = 0; i < edge_num; i++) {
        int u, v, weight;
        cin >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }

    // 打印邻接矩阵
    g.printAdjMatrix();

    // 执行Prim算法
    g.prim(0); // 从顶点0开始

    // 执行Kruskal算法
    g.kruskal();

    // 执行回溯法（新增调用）
    g.backtrackMST();

    return 0;
}