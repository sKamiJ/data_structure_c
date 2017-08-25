#include <stdio.h>
#include <stdlib.h>

//定义BOOL类型
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//定义图顶点最大数量
#define GRAPH_VERTEX_MAX_NUM	(10)

//定义图边节点
typedef struct GraphEdgeNode {
	int dest;	//目的顶点位置
	int weight;	//边权重
	struct GraphEdgeNode* next;	//下一个边节点
} GraphEdgeNode;

//定义图顶点节点
typedef struct GraphVertexNode {
	char info;	//顶点信息
	GraphEdgeNode* first;	//第一个邻接边
} GraphVertex;

//定义图
typedef struct Graph {
	GraphVertex v[GRAPH_VERTEX_MAX_NUM];	//顶点数组
	int vertex_num;	//顶点数
	int edge_num;	//有向边数
} Graph;

/**
 *
 * 创建图
 *
 * @param g 图地址，该图未被创建
 *
 */
void graph_create(Graph* g) {
	//置顶点数和有向边数为0
	g->vertex_num = 0;
	g->edge_num = 0;
}

/**
 *
 * 销毁图
 *
 * @param g 图地址
 *
 */
void graph_destroy(Graph* g) {
	int i;
	GraphEdgeNode* p;

	//清除所有顶点的邻接边
	for (i = 0; i < g->vertex_num; ++i) {
		while (g->v[i].first) {
			p = g->v[i].first;
			g->v[i].first = p->next;
			free(p);
		}
	}
	//置顶点数和有向边数为0
	g->vertex_num = 0;
	g->edge_num = 0;
}

/**
 *
 * 查找该信息在图顶点数组的位置
 *
 * @param g 图地址
 *        vi 顶点信息
 *
 * @return 在图顶点数组的位置，-1代表不存在
 *
 */
int graph_vertex_index(const Graph* g, char vi) {
	int i;

	//遍历顶点，查找信息相同的顶点
	for (i = 0; i < g->vertex_num; ++i) {
		if (g->v[i].info == vi) {
			return i;
		}
	}
	//未发现，返回-1
	return -1;
}

/**
 *
 * 在图中插入有向边
 *
 * @param g 图地址
 *        src 源顶点信息
 *        dest 目的顶点信息
 *        w 边权重
 *
 * @return TRUE 插入成功
 *         FALSE 顶点不存在或者边已存在，插入失败
 *
 */
BOOL graph_edge_insert(Graph* g, char src, char dest, int w) {
	int pos1, pos2;
	GraphEdgeNode** pp;

	//获取源顶点和目的顶点位置
	pos1 = graph_vertex_index(g, src);
	pos2 = graph_vertex_index(g, dest);
	//顶点不存在，错误
	if (pos1 == -1 || pos2 == -1) {
		return FALSE;
	}
	//获取源顶点第一条邻接边
	pp = &g->v[pos1].first;
	//检查该边是否存在
	while (*pp) {
		if ((*pp)->dest == pos2) {
			return FALSE;
		}
		pp = &(*pp)->next;
	}
	//新建邻接边
	*pp = (GraphEdgeNode*) malloc(sizeof(GraphEdgeNode));
	if (!*pp) {
		exit(-1);
	}
	//填写边的信息
	(*pp)->dest = pos2;
	(*pp)->weight = w;
	(*pp)->next = NULL;
	//边数+1
	++g->edge_num;
	return TRUE;
}

/**
 *
 * 在图中删除有向边
 *
 * @param g 图地址
 *        src 源顶点信息
 *        dest 目的顶点信息
 *
 * @return TRUE 删除成功
 *         FALSE 顶点不存在或者边不存在，删除失败
 *
 */
BOOL graph_edge_remove(Graph* g, char src, char dest) {
	int pos1, pos2;
	GraphEdgeNode* p;
	GraphEdgeNode** pp;

	//获取源顶点和目的顶点位置
	pos1 = graph_vertex_index(g, src);
	pos2 = graph_vertex_index(g, dest);
	//顶点不存在，错误
	if (pos1 == -1 || pos2 == -1) {
		return FALSE;
	}
	//获取源顶点第一条邻接边
	pp = &g->v[pos1].first;
	//查找该边
	while (*pp) {
		//找到，删除
		if ((*pp)->dest == pos2) {
			p = *pp;
			*pp = p->next;
			free(p);
			//边数-1
			--g->edge_num;
			return TRUE;
		}
		pp = &(*pp)->next;
	}
	//未找到，错误
	return FALSE;
}

/**
 *
 * 在图中插入顶点
 *
 * @param g 图地址
 *        vi 顶点信息
 *
 * @return TRUE 插入成功
 *         FALSE 顶点已经存在或者顶点数已满，插入失败
 *
 */
BOOL graph_vertex_insert(Graph* g, char vi) {
	//顶点数已满，失败
	if (g->vertex_num >= GRAPH_VERTEX_MAX_NUM) {
		return FALSE;
	}
	//顶点已经存在，失败
	if (graph_vertex_index(g, vi) != -1) {
		return FALSE;
	}
	//插入顶点
	g->v[g->vertex_num].info = vi;
	g->v[g->vertex_num].first = NULL;
	//顶点数+1
	++g->vertex_num;
	return TRUE;
}

/**
 *
 * 在图中删除顶点
 *
 * @param g 图地址
 *        vi 顶点信息
 *
 * @return TRUE 删除成功
 *         FALSE 顶点不存在，删除失败
 *
 */
BOOL graph_vertex_remove(Graph* g, char vi) {
	int pos;
	int i;
	GraphEdgeNode* p;
	GraphEdgeNode** pp;

	//找到顶点位置
	pos = graph_vertex_index(g, vi);
	//顶点不存在，错误
	if (pos == -1) {
		return FALSE;
	}
	//删除目的顶点为删除顶点的边
	for (i = 0; i < g->vertex_num; ++i) {
		if (i != pos) {
			pp = &g->v[i].first;
			while (*pp) {
				//删除边
				if ((*pp)->dest == pos) {
					p = *pp;
					*pp = p->next;
					free(p);
					//边数-1
					--g->edge_num;
				} else {
					//更改目的顶点位置
					if ((*pp)->dest > pos) {
						--(*pp)->dest;
					}
					pp = &(*pp)->next;
				}
			}
		}
	}
	//删除源顶点为删除顶点的边
	while (g->v[pos].first) {
		p = g->v[pos].first;
		g->v[pos].first = p->next;
		free(p);
		//边数-1
		--g->edge_num;
	}
	//顶点数-1
	--g->vertex_num;
	//左移剩余顶点
	for (i = pos; i < g->vertex_num; ++i) {
		g->v[i] = g->v[i + 1];
	}
	return TRUE;
}

/**
 *
 * 深度优先遍历内部递归函数
 *
 * @param g 图地址
 *        visit 遍历函数指针
 *        pos 遍历顶点位置
 *        visited 是否已经遍历数组
 *
 */
static void graph_dfs_internal(Graph* g, void (*visit)(char* info), int pos,
		BOOL* visited) {
	GraphEdgeNode* p;

	//访问该位置顶点
	visited[pos] = TRUE;
	(*visit)(&g->v[pos].info);
	//获取该顶点第一条邻接边
	p = g->v[pos].first;
	//遍历其邻接边，深度优先遍历所有未访问顶点
	while (p) {
		if (!visited[p->dest]) {
			graph_dfs_internal(g, visit, p->dest, visited);
		}
		p = p->next;
	}
}

/**
 *
 * 深度优先遍历，复杂度O(n+e)
 *
 * @param g 图地址
 *        visit 遍历函数指针
 *
 */
void graph_dfs(Graph* g, void (*visit)(char* info)) {
	BOOL* visited;
	int i;

	//为访问数组申请内存
	visited = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!visited) {
		exit(-1);
	}
	//置所有顶点为未访问
	for (i = 0; i < g->vertex_num; ++i) {
		visited[i] = FALSE;
	}
	//深度优先遍历未访问顶点
	for (i = 0; i < g->vertex_num; ++i) {
		if (!visited[i]) {
			graph_dfs_internal(g, visit, i, visited);
		}
	}
	free(visited);
}

/**
 *
 * 广度优先遍历，复杂度O(n+e)
 *
 * @param g 图地址
 *        visit 遍历函数指针
 *
 */
void graph_bfs(Graph* g, void (*visit)(char* info)) {
	BOOL* visited;
	int i;
	int* queue;
	int start, end;
	int pos;
	GraphEdgeNode* p;

	//为访问数组申请内存
	visited = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!visited) {
		exit(-1);
	}
	//置所有顶点为未访问
	for (i = 0; i < g->vertex_num; ++i) {
		visited[i] = FALSE;
	}
	//为队列申请内存
	queue = (int*) malloc(sizeof(int) * g->vertex_num);
	if (!queue) {
		exit(-1);
	}
	//初始化队列
	start = end = 0;
	//广度优先遍历未访问顶点
	for (i = 0; i < g->vertex_num; ++i) {
		if (!visited[i]) {
			//添加至队列
			visited[i] = TRUE;
			queue[end++] = i;
			//队列非空时
			while (start < end) {
				//访问队首顶点
				pos = queue[start++];
				(*visit)(&g->v[pos].info);
				//获取该顶点第一条邻接边
				p = g->v[pos].first;
				//遍历其邻接边，将未访问顶点加入队列
				while (p) {
					if (!visited[p->dest]) {
						visited[p->dest] = TRUE;
						queue[end++] = p->dest;
					}
					p = p->next;
				}
			}
		}
	}
	free(visited);
	free(queue);
}

//定义非邻接边权重
#define GRAPH_WEIGHT_INF	(1000000)

//定义图边信息
typedef struct GraphEdgeInfo {
	char src;	//源顶点
	char dest;	//目的顶点
	int weight;	//边权重
} GraphEdgeInfo;

/**
 *
 * 将邻接表转化为邻接矩阵
 *
 * @param g 图地址
 *
 * @return 邻接矩阵，使用完毕后需free
 *
 */
int** graph_to_matrix(const Graph* g) {
	int** mat;
	int i, j;
	GraphEdgeNode* p;

	//为邻接矩阵申请内存
	mat = (int**) malloc(sizeof(int*) * g->vertex_num);
	if (!mat) {
		exit(-1);
	}
	//填充矩阵内容
	for (i = 0; i < g->vertex_num; ++i) {
		//申请内存
		mat[i] = (int*) malloc(sizeof(int) * g->vertex_num);
		if (!mat[i]) {
			exit(-1);
		}
		//默认置权重为INF
		for (j = 0; j < g->vertex_num; ++j) {
			mat[i][j] = GRAPH_WEIGHT_INF;
		}
		//遍历该顶点邻接边
		p = g->v[i].first;
		while (p) {
			//更新邻接边权重
			if (p->weight < mat[i][p->dest]) {
				mat[i][p->dest] = p->weight;
			}
			p = p->next;
		}
	}
	return mat;
}

/**
 *
 * 销毁邻接矩阵
 *
 * @param mat 邻接矩阵
 *        len 邻接矩阵长度
 *
 */
void graph_matrix_destroy(int** mat, int len) {
	int i;

	for (i = 0; i < len; ++i) {
		free(mat[i]);
	}
	free(mat);
}

/**
 *
 * prim算法求最小生成树，复杂度O(n^2)
 *
 * @param g 图地址，该图需为无向连通图
 *
 * @return 最小生成树边信息，长度为顶点数-1，使用完毕后需free
 *
 */
GraphEdgeInfo* graph_prim(const Graph* g) {
	GraphEdgeInfo* infos;
	char* srcs;	//已有顶点集到目的顶点最短距离的源顶点
	int* min_w;	//已有顶点集到目的顶点的最短距离，0代表已加入顶点集
	int i, j;
	int** mat;
	int min;
	int pos;

	//申请内存
	infos = (GraphEdgeInfo*) malloc(
			sizeof(GraphEdgeInfo) * (g->vertex_num - 1));
	srcs = (char*) malloc(sizeof(char) * g->vertex_num);
	min_w = (int*) malloc(sizeof(int) * g->vertex_num);
	//内存不足，错误
	if (!infos || !srcs || !min_w) {
		exit(-1);
	}
	//将邻接表转化为邻接矩阵
	mat = graph_to_matrix(g);
	//从0位置顶点开始生成最小生成树，初始化信息
	for (i = 0; i < g->vertex_num; ++i) {
		//源顶点为0位置顶点
		srcs[i] = g->v[0].info;
		//最短距离为0顶点到各顶点距离
		min_w[i] = mat[0][i];
	}
	//0位置顶点加入顶点集
	min_w[0] = 0;
	//将剩余顶点加入顶点集
	for (i = 0; i < g->vertex_num - 1; ++i) {
		//找出已有顶点集到剩余顶点的最短距离
		min = GRAPH_WEIGHT_INF;
		for (j = 0; j < g->vertex_num; ++j) {
			//j位置顶点未加入顶点集且到其距离小于最短距离时
			if (min_w[j] != 0 && min_w[j] < min) {
				//更新最短距离和顶点位置
				min = min_w[j];
				pos = j;
			}
		}
		//pos位置为新加入的顶点，填写边信息
		infos[i].src = srcs[pos];
		infos[i].dest = g->v[pos].info;
		infos[i].weight = min_w[pos];
		//加入顶点集
		min_w[pos] = 0;
		//更新到剩余顶点的最短距离
		for (j = 0; j < g->vertex_num; ++j) {
			if (mat[pos][j] < min_w[j]) {
				min_w[j] = mat[pos][j];
				srcs[j] = g->v[pos].info;
			}
		}
	}
	//释放内存
	graph_matrix_destroy(mat, g->vertex_num);
	free(srcs);
	free(min_w);
	return infos;
}

/**
 *
 * 并查集查找位置为pos节点的祖先节点，使用了路径压缩
 *
 * @param parent 父节点数组
 *        pos 节点位置
 *
 * @return 祖先节点位置
 *
 */
static int mfset_find(int* parent, int pos) {
	int anc;
	int tmp;

	//查找祖先节点
	anc = pos;
	while (parent[anc] >= 0) {
		anc = parent[anc];
	}
	//将沿路节点的父节点设为祖先节点
	while (parent[pos] >= 0) {
		tmp = parent[pos];
		parent[pos] = anc;
		pos = tmp;
	}
	return anc;
}

/**
 *
 * 并查集合并，使用了按秩合并
 *
 * @param parent 父节点数组
 *        pos1 祖先节点位置1
 *        pos2 祖先节点位置2
 *
 */
static void mfset_union(int* parent, int pos1, int pos2) {
	if (parent[pos1] > parent[pos2]) {
		//pos1集合的元素少于pos2集合
		//pos2集合个数添加pos1集合个数
		parent[pos2] += parent[pos1];
		//pos1父节点为pos2
		parent[pos1] = pos2;
	} else {
		//pos1集合的元素大于等于pos2集合
		//pos1集合个数添加pos2集合个数
		parent[pos1] += parent[pos2];
		//pos2父节点为pos1
		parent[pos2] = pos1;
	}
}

//定义边顶点位置结构体
typedef struct GraphEdgePos {
	int src;	//源顶点位置
	int dest;	//目的顶点位置
	int weight;	//边权重
} GraphEdgePos;

/**
 *
 * 根据图，找出所有的无向边
 *
 * @param g 图地址，需为无向图
 *
 * @return 图所有的无向边，长度为边数的一半，使用完毕后需free
 *
 */
static GraphEdgePos* graph_create_edges(const Graph* g) {
	GraphEdgePos* edges;
	GraphEdgeNode* p;
	int i, j;

	//申请内存
	edges = (GraphEdgePos*) malloc(sizeof(GraphEdgePos) * (g->edge_num >> 1));
	if (!edges) {
		exit(-1);
	}
	//遍历所有顶点
	for (i = 0, j = 0; i < g->vertex_num; ++i) {
		p = g->v[i].first;
		while (p) {
			//仅获取上三角边
			if (p->dest > i) {
				edges[j].src = i;
				edges[j].dest = p->dest;
				edges[j++].weight = p->weight;
			}
			p = p->next;
		}
	}
	return edges;
}

/**
 *
 * 根据边的权重，从小到大排序
 *
 * @param edges 无向边数组
 *        l 起始位置
 *        r 结束位置
 *
 */
static void graph_edge_sort(GraphEdgePos* edges, int l, int r) {
	int lt, gt;
	int ref;
	int i;
	GraphEdgePos tmp;
	//三路快速排序
	if (l >= r) {
		return;
	}
	ref = edges[l].weight;
	lt = l;
	gt = r;
	i = l + 1;
	while (i <= gt) {
		if (edges[i].weight < ref) {
			tmp = edges[i];
			edges[i] = edges[lt];
			edges[lt] = tmp;
			++i;
			++lt;
		} else if (edges[i].weight > ref) {
			tmp = edges[i];
			edges[i] = edges[gt];
			edges[gt] = tmp;
			--gt;
		} else {
			++i;
		}
	}
	graph_edge_sort(edges, l, lt - 1);
	graph_edge_sort(edges, gt + 1, r);
}

/**
 *
 * kruskal算法求最小生成树，复杂度O(eloge)
 *
 * @param g 图地址，该图需为无向连通图
 *
 * @return 最小生成树边信息，长度为顶点数-1，使用完毕后需free
 *
 */
GraphEdgeInfo* graph_kruskal(const Graph* g) {
	GraphEdgeInfo* infos;
	GraphEdgePos* edges;
	int i, j;
	int* parent;	//为并查集，负数时为根节点，数字表示集合内元素数，正数表示父节点位置
	int anc1, anc2;

	//申请内存
	infos = (GraphEdgeInfo*) malloc(
			sizeof(GraphEdgeInfo) * (g->vertex_num - 1));
	parent = (int*) malloc(sizeof(int) * g->vertex_num);
	//内存不足，错误
	if (!infos || !parent) {
		exit(-1);
	}
	//所有顶点单独为一个集合
	for (i = 0; i < g->vertex_num; ++i) {
		parent[i] = -1;
	}
	//获取图的无向边
	edges = graph_create_edges(g);
	//对无向边按权重排序，复杂度为O(eloge)
	graph_edge_sort(edges, 0, (g->edge_num >> 1) - 1);
	//加入顶点数-1条边，对e条边进行并查集操作，每次查找复杂度为O(a(n))，因此复杂度为O(e*a(n))
	//a(n)为反阿克曼函数，通常<=4
	for (i = 0, j = 0; j < g->vertex_num - 1; ++i) {
		//查找第i条边两个顶点所属集合
		anc1 = mfset_find(parent, edges[i].src);
		anc2 = mfset_find(parent, edges[i].dest);
		//不为一个集合时
		if (anc1 != anc2) {
			//合并两个集合
			mfset_union(parent, anc1, anc2);
			//该条边为最小生成树边
			infos[j].src = g->v[edges[i].src].info;
			infos[j].dest = g->v[edges[i].dest].info;
			infos[j].weight = edges[i].weight;
			++j;
		}
	}
	//释放内存
	free(parent);
	free(edges);
	return infos;
}

/**
 *
 * 拓扑排序，复杂度O(n+e)
 * 这里通过入度为0判断，也可以通过dfs求，dfs退出节点的顺序
 * 为拓扑序列的逆序，因为dfs退出节点时，该节点出度为0;
 * 可以通过拓扑排序判断有向图是否有环，对无向图，可以使用dfs，
 * 判断每个连通分量的边数是否为顶点数-1
 *
 * @param g 图地址，该图需为有向无环图
 *        order 保存拓扑序列地址，使用完毕后需free，长度为顶点数
 *
 * @return TRUE 排序成功
 *         FALSE 该图有环，排序失败
 *
 */
BOOL graph_topological_sort(const Graph* g, char** order) {
	int* stack;
	int top;
	int* indegree;
	int i;
	GraphEdgeNode* p;
	int count;

	//申请内存
	stack = (int*) malloc(sizeof(int) * g->vertex_num);
	indegree = (int*) malloc(sizeof(int) * g->vertex_num);
	*order = (char*) malloc(sizeof(char) * g->vertex_num);
	if (!stack || !indegree || !*order) {
		exit(-1);
	}
	//初始化各顶点入度为0
	for (i = 0; i < g->vertex_num; ++i) {
		indegree[i] = 0;
	}
	//遍历各边，添加目的顶点的入度
	for (i = 0; i < g->vertex_num; ++i) {
		p = g->v[i].first;
		while (p) {
			++indegree[p->dest];
			p = p->next;
		}
	}
	//初始化栈顶
	top = -1;
	//将入度为0的顶点入栈，这里使用栈/队列均可
	for (i = 0; i < g->vertex_num; ++i) {
		if (!indegree[i]) {
			stack[++top] = i;
		}
	}
	count = 0;
	//栈非空时
	while (top >= 0) {
		//栈顶出栈
		i = stack[top--];
		//添加该顶点信息
		(*order)[count++] = g->v[i].info;
		//遍历该顶点邻接边，目的顶点入度-1
		p = g->v[i].first;
		while (p) {
			//入度降至0，入栈
			if (--indegree[p->dest] <= 0) {
				stack[++top] = p->dest;
			}
			p = p->next;
		}
	}
	//释放内存
	free(indegree);
	free(stack);
	if (count < g->vertex_num) {
		//不是所有顶点都入过栈，说明该图有环
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
 *
 * dijkstra求单源最短路径，复杂度O(n^2)
 *
 * @param g 图地址
 *        vi 源顶点信息
 *
 * @return 到各顶点最短距离，int[i]为到位置为i的顶点的最短距离，
 *         使用完毕后需free，长度为顶点数
 *
 */
int* graph_dijkstra(const Graph* g, char vi) {
	int** mat;
	int* dis;	//dis[i]为到位置为i的顶点的最短距离
	BOOL* final;	//final[i]为位置为i的顶点是否已求得最短距离
	int i, j;
	int pos;
	int min;

	//将图转化为邻接矩阵
	mat = graph_to_matrix(g);
	//申请内存
	dis = (int*) malloc(sizeof(int) * g->vertex_num);
	final = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!dis || !final) {
		exit(-1);
	}
	//获取源顶点位置
	pos = graph_vertex_index(g, vi);
	//初始化到各顶点最短距离
	for (i = 0; i < g->vertex_num; ++i) {
		//最短距离为源顶点到该顶点距离
		dis[i] = mat[pos][i];
		//所有顶点未求得最短距离
		final[i] = FALSE;
	}
	//到源顶点最短距离为0
	dis[pos] = 0;
	//源顶点已求得最短距离
	final[pos] = TRUE;
	//求剩余顶点数-1个顶点的最短距离
	for (i = 1; i < g->vertex_num; ++i) {
		//在剩余顶点中找出距离最短的顶点
		min = GRAPH_WEIGHT_INF;
		for (j = 0; j < g->vertex_num; ++j) {
			if (!final[j] && dis[j] <= min) {
				pos = j;
				min = dis[j];
			}
		}
		//该顶点已求得最短距离
		final[pos] = TRUE;
		//更新到剩余顶点的最短距离
		for (j = 0; j < g->vertex_num; ++j) {
			//到该顶点的最短距离加上该顶点到剩余顶点的距离小于最短距离时，更新
			if (!final[j] && min + mat[pos][j] < dis[j]) {
				dis[j] = min + mat[pos][j];
			}
		}
	}
	//释放内存
	graph_matrix_destroy(mat, g->vertex_num);
	free(final);
	return dis;
}

/**
 *
 * floyd求任意两顶点的最短距离，复杂度O(n^3)
 *
 * @param g 图地址
 *
 * @return 最短距离矩阵，int[i][j]为位置为i顶点到位置为j顶点的最短距离，
 *         使用完毕后需graph_matrix_destroy，长度为顶点数
 *
 */
int** graph_floyd(const Graph* g) {
	int** mat;
	int i, j, k;

	//将图转换为邻接矩阵
	mat = graph_to_matrix(g);
	//各顶点到自己的最短距离为0
	for (i = 0; i < g->vertex_num; ++i) {
		mat[i][i] = 0;
	}
	for (k = 0; k < g->vertex_num; ++k) {
		for (i = 0; i < g->vertex_num; ++i) {
			for (j = 0; j < g->vertex_num; ++j) {
				//允许经过k时的距离小于原先距离时，更新
				if (mat[i][k] + mat[k][j] < mat[i][j]) {
					mat[i][j] = mat[i][k] + mat[k][j];
				}
			}
		}
	}
	return mat;
}

/**
 *
 * 遍历函数，可以修改，或建立类似函数
 *
 * @param info 顶点信息
 *
 */
static void graph_vertex_visit(char* info) {
	printf("%c ", *info);
}

/**
 *
 * 显示图顶点和边信息，可以修改
 *
 * @param g 图地址
 *
 */
void graph_show(Graph* g) {
	int i;
	GraphEdgeNode* p;

	printf("图的顶点数：%d 边数：%d\n", g->vertex_num, g->edge_num);
	printf("图的顶点和边信息：\n");
	for (i = 0; i < g->vertex_num; ++i) {
		//显示顶点信息
		printf("%c:", g->v[i].info);
		p = g->v[i].first;
		//显示各边信息
		while (p) {
			printf("%c %d; ", g->v[p->dest].info, p->weight);
			p = p->next;
		}
		printf("\n");
	}
	printf("深度优先遍历结果：\n");
	graph_dfs(g, graph_vertex_visit);
	printf("\n");
	printf("广度优先遍历结果：\n");
	graph_bfs(g, graph_vertex_visit);
	printf("\n");
	printf("\n");
}

int main() {
	Graph g;
	GraphEdgeInfo* infos;
	int i, j;
	char* order;
	int* dis;
	int** mat;

	//创建图
	graph_create(&g);

	//插入顶点
	graph_vertex_insert(&g, '1');
	graph_vertex_insert(&g, '2');
	graph_vertex_insert(&g, '3');
	graph_vertex_insert(&g, '4');
	graph_vertex_insert(&g, '5');
	graph_vertex_insert(&g, '6');

	//插入无向边
	graph_edge_insert(&g, '1', '2', 6);
	graph_edge_insert(&g, '2', '1', 6);

	graph_edge_insert(&g, '1', '3', 1);
	graph_edge_insert(&g, '3', '1', 1);

	graph_edge_insert(&g, '1', '4', 5);
	graph_edge_insert(&g, '4', '1', 5);

	graph_edge_insert(&g, '2', '3', 5);
	graph_edge_insert(&g, '3', '2', 5);

	graph_edge_insert(&g, '2', '5', 3);
	graph_edge_insert(&g, '5', '2', 3);

	graph_edge_insert(&g, '3', '4', 5);
	graph_edge_insert(&g, '4', '3', 5);

	graph_edge_insert(&g, '3', '5', 6);
	graph_edge_insert(&g, '5', '3', 6);

	graph_edge_insert(&g, '3', '6', 4);
	graph_edge_insert(&g, '6', '3', 4);

	graph_edge_insert(&g, '6', '4', 2);
	graph_edge_insert(&g, '4', '6', 2);

	graph_edge_insert(&g, '6', '5', 6);
	graph_edge_insert(&g, '5', '6', 6);

	graph_show(&g);

	//prim算法生成最小生成树
	printf("prim算法生成最小生成树：\n");
	infos = graph_prim(&g);
	for (i = 0; i < g.vertex_num - 1; ++i) {
		printf("s:%c d:%c w:%d\n", infos[i].src, infos[i].dest,
				infos[i].weight);
	}
	free(infos);
	printf("\n");

	//kruskal算法生成最小生成树
	printf("kruskal算法生成最小生成树：\n");
	infos = graph_kruskal(&g);
	for (i = 0; i < g.vertex_num - 1; ++i) {
		printf("s:%c d:%c w:%d\n", infos[i].src, infos[i].dest,
				infos[i].weight);
	}
	free(infos);
	printf("\n");

	//移除顶点
	graph_vertex_remove(&g, '3');
	graph_show(&g);

	//销毁图
	graph_destroy(&g);

	//创建图
	graph_create(&g);

	//插入顶点
	graph_vertex_insert(&g, '0');
	graph_vertex_insert(&g, '1');
	graph_vertex_insert(&g, '2');
	graph_vertex_insert(&g, '3');
	graph_vertex_insert(&g, '4');
	graph_vertex_insert(&g, '5');

	//插入有向边
	graph_edge_insert(&g, '0', '5', 100);
	graph_edge_insert(&g, '0', '4', 30);
	graph_edge_insert(&g, '0', '2', 10);
	graph_edge_insert(&g, '1', '2', 5);
	graph_edge_insert(&g, '2', '3', 50);
	graph_edge_insert(&g, '3', '5', 10);
	graph_edge_insert(&g, '4', '3', 20);
	graph_edge_insert(&g, '4', '5', 60);

	graph_show(&g);

	//拓扑排序
	printf("拓扑排序：\n");
	if (graph_topological_sort(&g, &order)) {
		for (i = 0; i < g.vertex_num; ++i) {
			printf("%c ", order[i]);
		}
		printf("\n");
	} else {
		printf("该图有环！\n");
	}
	free(order);
	printf("\n");

	//dijkstra求'0'到各点最短距离
	printf("dijkstra求'0'到各点最短距离：\n");
	dis = graph_dijkstra(&g, '0');
	for (i = 0; i < g.vertex_num; ++i) {
		if (dis[i] < GRAPH_WEIGHT_INF) {
			printf("%d ", dis[i]);
		} else {
			printf("INF ");
		}
	}
	printf("\n");
	free(dis);
	printf("\n");

	//floyd求各点之间最短距离
	printf("floyd求各点之间最短距离：\n");
	mat = graph_floyd(&g);
	for (i = 0; i < g.vertex_num; ++i) {
		for (j = 0; j < g.vertex_num; ++j) {
			if (mat[i][j] < GRAPH_WEIGHT_INF) {
				printf("%d ", mat[i][j]);
			} else {
				printf("INF ");
			}
		}
		printf("\n");
	}
	graph_matrix_destroy(mat, g.vertex_num);
	printf("\n");

	graph_destroy(&g);

	return 0;
}
