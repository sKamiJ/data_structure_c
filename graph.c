#include <stdio.h>
#include <stdlib.h>

//����BOOL����
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//����ͼ�����������
#define GRAPH_VERTEX_MAX_NUM	(10)

//����ͼ�߽ڵ�
typedef struct GraphEdgeNode {
	int dest;	//Ŀ�Ķ���λ��
	int weight;	//��Ȩ��
	struct GraphEdgeNode* next;	//��һ���߽ڵ�
} GraphEdgeNode;

//����ͼ����ڵ�
typedef struct GraphVertexNode {
	char info;	//������Ϣ
	GraphEdgeNode* first;	//��һ���ڽӱ�
} GraphVertex;

//����ͼ
typedef struct Graph {
	GraphVertex v[GRAPH_VERTEX_MAX_NUM];	//��������
	int vertex_num;	//������
	int edge_num;	//�������
} Graph;

/**
 *
 * ����ͼ
 *
 * @param g ͼ��ַ����ͼδ������
 *
 */
void graph_create(Graph* g) {
	//�ö��������������Ϊ0
	g->vertex_num = 0;
	g->edge_num = 0;
}

/**
 *
 * ����ͼ
 *
 * @param g ͼ��ַ
 *
 */
void graph_destroy(Graph* g) {
	int i;
	GraphEdgeNode* p;

	//������ж�����ڽӱ�
	for (i = 0; i < g->vertex_num; ++i) {
		while (g->v[i].first) {
			p = g->v[i].first;
			g->v[i].first = p->next;
			free(p);
		}
	}
	//�ö��������������Ϊ0
	g->vertex_num = 0;
	g->edge_num = 0;
}

/**
 *
 * ���Ҹ���Ϣ��ͼ���������λ��
 *
 * @param g ͼ��ַ
 *        vi ������Ϣ
 *
 * @return ��ͼ���������λ�ã�-1��������
 *
 */
int graph_vertex_index(const Graph* g, char vi) {
	int i;

	//�������㣬������Ϣ��ͬ�Ķ���
	for (i = 0; i < g->vertex_num; ++i) {
		if (g->v[i].info == vi) {
			return i;
		}
	}
	//δ���֣�����-1
	return -1;
}

/**
 *
 * ��ͼ�в��������
 *
 * @param g ͼ��ַ
 *        src Դ������Ϣ
 *        dest Ŀ�Ķ�����Ϣ
 *        w ��Ȩ��
 *
 * @return TRUE ����ɹ�
 *         FALSE ���㲻���ڻ��߱��Ѵ��ڣ�����ʧ��
 *
 */
BOOL graph_edge_insert(Graph* g, char src, char dest, int w) {
	int pos1, pos2;
	GraphEdgeNode** pp;

	//��ȡԴ�����Ŀ�Ķ���λ��
	pos1 = graph_vertex_index(g, src);
	pos2 = graph_vertex_index(g, dest);
	//���㲻���ڣ�����
	if (pos1 == -1 || pos2 == -1) {
		return FALSE;
	}
	//��ȡԴ�����һ���ڽӱ�
	pp = &g->v[pos1].first;
	//���ñ��Ƿ����
	while (*pp) {
		if ((*pp)->dest == pos2) {
			return FALSE;
		}
		pp = &(*pp)->next;
	}
	//�½��ڽӱ�
	*pp = (GraphEdgeNode*) malloc(sizeof(GraphEdgeNode));
	if (!*pp) {
		exit(-1);
	}
	//��д�ߵ���Ϣ
	(*pp)->dest = pos2;
	(*pp)->weight = w;
	(*pp)->next = NULL;
	//����+1
	++g->edge_num;
	return TRUE;
}

/**
 *
 * ��ͼ��ɾ�������
 *
 * @param g ͼ��ַ
 *        src Դ������Ϣ
 *        dest Ŀ�Ķ�����Ϣ
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE ���㲻���ڻ��߲߱����ڣ�ɾ��ʧ��
 *
 */
BOOL graph_edge_remove(Graph* g, char src, char dest) {
	int pos1, pos2;
	GraphEdgeNode* p;
	GraphEdgeNode** pp;

	//��ȡԴ�����Ŀ�Ķ���λ��
	pos1 = graph_vertex_index(g, src);
	pos2 = graph_vertex_index(g, dest);
	//���㲻���ڣ�����
	if (pos1 == -1 || pos2 == -1) {
		return FALSE;
	}
	//��ȡԴ�����һ���ڽӱ�
	pp = &g->v[pos1].first;
	//���Ҹñ�
	while (*pp) {
		//�ҵ���ɾ��
		if ((*pp)->dest == pos2) {
			p = *pp;
			*pp = p->next;
			free(p);
			//����-1
			--g->edge_num;
			return TRUE;
		}
		pp = &(*pp)->next;
	}
	//δ�ҵ�������
	return FALSE;
}

/**
 *
 * ��ͼ�в��붥��
 *
 * @param g ͼ��ַ
 *        vi ������Ϣ
 *
 * @return TRUE ����ɹ�
 *         FALSE �����Ѿ����ڻ��߶���������������ʧ��
 *
 */
BOOL graph_vertex_insert(Graph* g, char vi) {
	//������������ʧ��
	if (g->vertex_num >= GRAPH_VERTEX_MAX_NUM) {
		return FALSE;
	}
	//�����Ѿ����ڣ�ʧ��
	if (graph_vertex_index(g, vi) != -1) {
		return FALSE;
	}
	//���붥��
	g->v[g->vertex_num].info = vi;
	g->v[g->vertex_num].first = NULL;
	//������+1
	++g->vertex_num;
	return TRUE;
}

/**
 *
 * ��ͼ��ɾ������
 *
 * @param g ͼ��ַ
 *        vi ������Ϣ
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE ���㲻���ڣ�ɾ��ʧ��
 *
 */
BOOL graph_vertex_remove(Graph* g, char vi) {
	int pos;
	int i;
	GraphEdgeNode* p;
	GraphEdgeNode** pp;

	//�ҵ�����λ��
	pos = graph_vertex_index(g, vi);
	//���㲻���ڣ�����
	if (pos == -1) {
		return FALSE;
	}
	//ɾ��Ŀ�Ķ���Ϊɾ������ı�
	for (i = 0; i < g->vertex_num; ++i) {
		if (i != pos) {
			pp = &g->v[i].first;
			while (*pp) {
				//ɾ����
				if ((*pp)->dest == pos) {
					p = *pp;
					*pp = p->next;
					free(p);
					//����-1
					--g->edge_num;
				} else {
					//����Ŀ�Ķ���λ��
					if ((*pp)->dest > pos) {
						--(*pp)->dest;
					}
					pp = &(*pp)->next;
				}
			}
		}
	}
	//ɾ��Դ����Ϊɾ������ı�
	while (g->v[pos].first) {
		p = g->v[pos].first;
		g->v[pos].first = p->next;
		free(p);
		//����-1
		--g->edge_num;
	}
	//������-1
	--g->vertex_num;
	//����ʣ�ඥ��
	for (i = pos; i < g->vertex_num; ++i) {
		g->v[i] = g->v[i + 1];
	}
	return TRUE;
}

/**
 *
 * ������ȱ����ڲ��ݹ麯��
 *
 * @param g ͼ��ַ
 *        visit ��������ָ��
 *        pos ��������λ��
 *        visited �Ƿ��Ѿ���������
 *
 */
static void graph_dfs_internal(Graph* g, void (*visit)(char* info), int pos,
		BOOL* visited) {
	GraphEdgeNode* p;

	//���ʸ�λ�ö���
	visited[pos] = TRUE;
	(*visit)(&g->v[pos].info);
	//��ȡ�ö����һ���ڽӱ�
	p = g->v[pos].first;
	//�������ڽӱߣ�������ȱ�������δ���ʶ���
	while (p) {
		if (!visited[p->dest]) {
			graph_dfs_internal(g, visit, p->dest, visited);
		}
		p = p->next;
	}
}

/**
 *
 * ������ȱ��������Ӷ�O(n+e)
 *
 * @param g ͼ��ַ
 *        visit ��������ָ��
 *
 */
void graph_dfs(Graph* g, void (*visit)(char* info)) {
	BOOL* visited;
	int i;

	//Ϊ�������������ڴ�
	visited = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!visited) {
		exit(-1);
	}
	//�����ж���Ϊδ����
	for (i = 0; i < g->vertex_num; ++i) {
		visited[i] = FALSE;
	}
	//������ȱ���δ���ʶ���
	for (i = 0; i < g->vertex_num; ++i) {
		if (!visited[i]) {
			graph_dfs_internal(g, visit, i, visited);
		}
	}
	free(visited);
}

/**
 *
 * ������ȱ��������Ӷ�O(n+e)
 *
 * @param g ͼ��ַ
 *        visit ��������ָ��
 *
 */
void graph_bfs(Graph* g, void (*visit)(char* info)) {
	BOOL* visited;
	int i;
	int* queue;
	int start, end;
	int pos;
	GraphEdgeNode* p;

	//Ϊ�������������ڴ�
	visited = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!visited) {
		exit(-1);
	}
	//�����ж���Ϊδ����
	for (i = 0; i < g->vertex_num; ++i) {
		visited[i] = FALSE;
	}
	//Ϊ���������ڴ�
	queue = (int*) malloc(sizeof(int) * g->vertex_num);
	if (!queue) {
		exit(-1);
	}
	//��ʼ������
	start = end = 0;
	//������ȱ���δ���ʶ���
	for (i = 0; i < g->vertex_num; ++i) {
		if (!visited[i]) {
			//���������
			visited[i] = TRUE;
			queue[end++] = i;
			//���зǿ�ʱ
			while (start < end) {
				//���ʶ��׶���
				pos = queue[start++];
				(*visit)(&g->v[pos].info);
				//��ȡ�ö����һ���ڽӱ�
				p = g->v[pos].first;
				//�������ڽӱߣ���δ���ʶ���������
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

//������ڽӱ�Ȩ��
#define GRAPH_WEIGHT_INF	(1000000)

//����ͼ����Ϣ
typedef struct GraphEdgeInfo {
	char src;	//Դ����
	char dest;	//Ŀ�Ķ���
	int weight;	//��Ȩ��
} GraphEdgeInfo;

/**
 *
 * ���ڽӱ�ת��Ϊ�ڽӾ���
 *
 * @param g ͼ��ַ
 *
 * @return �ڽӾ���ʹ����Ϻ���free
 *
 */
int** graph_to_matrix(const Graph* g) {
	int** mat;
	int i, j;
	GraphEdgeNode* p;

	//Ϊ�ڽӾ��������ڴ�
	mat = (int**) malloc(sizeof(int*) * g->vertex_num);
	if (!mat) {
		exit(-1);
	}
	//����������
	for (i = 0; i < g->vertex_num; ++i) {
		//�����ڴ�
		mat[i] = (int*) malloc(sizeof(int) * g->vertex_num);
		if (!mat[i]) {
			exit(-1);
		}
		//Ĭ����Ȩ��ΪINF
		for (j = 0; j < g->vertex_num; ++j) {
			mat[i][j] = GRAPH_WEIGHT_INF;
		}
		//�����ö����ڽӱ�
		p = g->v[i].first;
		while (p) {
			//�����ڽӱ�Ȩ��
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
 * �����ڽӾ���
 *
 * @param mat �ڽӾ���
 *        len �ڽӾ��󳤶�
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
 * prim�㷨����С�����������Ӷ�O(n^2)
 *
 * @param g ͼ��ַ����ͼ��Ϊ������ͨͼ
 *
 * @return ��С����������Ϣ������Ϊ������-1��ʹ����Ϻ���free
 *
 */
GraphEdgeInfo* graph_prim(const Graph* g) {
	GraphEdgeInfo* infos;
	char* srcs;	//���ж��㼯��Ŀ�Ķ�����̾����Դ����
	int* min_w;	//���ж��㼯��Ŀ�Ķ������̾��룬0�����Ѽ��붥�㼯
	int i, j;
	int** mat;
	int min;
	int pos;

	//�����ڴ�
	infos = (GraphEdgeInfo*) malloc(
			sizeof(GraphEdgeInfo) * (g->vertex_num - 1));
	srcs = (char*) malloc(sizeof(char) * g->vertex_num);
	min_w = (int*) malloc(sizeof(int) * g->vertex_num);
	//�ڴ治�㣬����
	if (!infos || !srcs || !min_w) {
		exit(-1);
	}
	//���ڽӱ�ת��Ϊ�ڽӾ���
	mat = graph_to_matrix(g);
	//��0λ�ö��㿪ʼ������С����������ʼ����Ϣ
	for (i = 0; i < g->vertex_num; ++i) {
		//Դ����Ϊ0λ�ö���
		srcs[i] = g->v[0].info;
		//��̾���Ϊ0���㵽���������
		min_w[i] = mat[0][i];
	}
	//0λ�ö�����붥�㼯
	min_w[0] = 0;
	//��ʣ�ඥ����붥�㼯
	for (i = 0; i < g->vertex_num - 1; ++i) {
		//�ҳ����ж��㼯��ʣ�ඥ�����̾���
		min = GRAPH_WEIGHT_INF;
		for (j = 0; j < g->vertex_num; ++j) {
			//jλ�ö���δ���붥�㼯�ҵ������С����̾���ʱ
			if (min_w[j] != 0 && min_w[j] < min) {
				//������̾���Ͷ���λ��
				min = min_w[j];
				pos = j;
			}
		}
		//posλ��Ϊ�¼���Ķ��㣬��д����Ϣ
		infos[i].src = srcs[pos];
		infos[i].dest = g->v[pos].info;
		infos[i].weight = min_w[pos];
		//���붥�㼯
		min_w[pos] = 0;
		//���µ�ʣ�ඥ�����̾���
		for (j = 0; j < g->vertex_num; ++j) {
			if (mat[pos][j] < min_w[j]) {
				min_w[j] = mat[pos][j];
				srcs[j] = g->v[pos].info;
			}
		}
	}
	//�ͷ��ڴ�
	graph_matrix_destroy(mat, g->vertex_num);
	free(srcs);
	free(min_w);
	return infos;
}

/**
 *
 * ���鼯����λ��Ϊpos�ڵ�����Ƚڵ㣬ʹ����·��ѹ��
 *
 * @param parent ���ڵ�����
 *        pos �ڵ�λ��
 *
 * @return ���Ƚڵ�λ��
 *
 */
static int mfset_find(int* parent, int pos) {
	int anc;
	int tmp;

	//�������Ƚڵ�
	anc = pos;
	while (parent[anc] >= 0) {
		anc = parent[anc];
	}
	//����·�ڵ�ĸ��ڵ���Ϊ���Ƚڵ�
	while (parent[pos] >= 0) {
		tmp = parent[pos];
		parent[pos] = anc;
		pos = tmp;
	}
	return anc;
}

/**
 *
 * ���鼯�ϲ���ʹ���˰��Ⱥϲ�
 *
 * @param parent ���ڵ�����
 *        pos1 ���Ƚڵ�λ��1
 *        pos2 ���Ƚڵ�λ��2
 *
 */
static void mfset_union(int* parent, int pos1, int pos2) {
	if (parent[pos1] > parent[pos2]) {
		//pos1���ϵ�Ԫ������pos2����
		//pos2���ϸ������pos1���ϸ���
		parent[pos2] += parent[pos1];
		//pos1���ڵ�Ϊpos2
		parent[pos1] = pos2;
	} else {
		//pos1���ϵ�Ԫ�ش��ڵ���pos2����
		//pos1���ϸ������pos2���ϸ���
		parent[pos1] += parent[pos2];
		//pos2���ڵ�Ϊpos1
		parent[pos2] = pos1;
	}
}

//����߶���λ�ýṹ��
typedef struct GraphEdgePos {
	int src;	//Դ����λ��
	int dest;	//Ŀ�Ķ���λ��
	int weight;	//��Ȩ��
} GraphEdgePos;

/**
 *
 * ����ͼ���ҳ����е������
 *
 * @param g ͼ��ַ����Ϊ����ͼ
 *
 * @return ͼ���е�����ߣ�����Ϊ������һ�룬ʹ����Ϻ���free
 *
 */
static GraphEdgePos* graph_create_edges(const Graph* g) {
	GraphEdgePos* edges;
	GraphEdgeNode* p;
	int i, j;

	//�����ڴ�
	edges = (GraphEdgePos*) malloc(sizeof(GraphEdgePos) * (g->edge_num >> 1));
	if (!edges) {
		exit(-1);
	}
	//�������ж���
	for (i = 0, j = 0; i < g->vertex_num; ++i) {
		p = g->v[i].first;
		while (p) {
			//����ȡ�����Ǳ�
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
 * ���ݱߵ�Ȩ�أ���С��������
 *
 * @param edges ���������
 *        l ��ʼλ��
 *        r ����λ��
 *
 */
static void graph_edge_sort(GraphEdgePos* edges, int l, int r) {
	int lt, gt;
	int ref;
	int i;
	GraphEdgePos tmp;
	//��·��������
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
 * kruskal�㷨����С�����������Ӷ�O(eloge)
 *
 * @param g ͼ��ַ����ͼ��Ϊ������ͨͼ
 *
 * @return ��С����������Ϣ������Ϊ������-1��ʹ����Ϻ���free
 *
 */
GraphEdgeInfo* graph_kruskal(const Graph* g) {
	GraphEdgeInfo* infos;
	GraphEdgePos* edges;
	int i, j;
	int* parent;	//Ϊ���鼯������ʱΪ���ڵ㣬���ֱ�ʾ������Ԫ������������ʾ���ڵ�λ��
	int anc1, anc2;

	//�����ڴ�
	infos = (GraphEdgeInfo*) malloc(
			sizeof(GraphEdgeInfo) * (g->vertex_num - 1));
	parent = (int*) malloc(sizeof(int) * g->vertex_num);
	//�ڴ治�㣬����
	if (!infos || !parent) {
		exit(-1);
	}
	//���ж��㵥��Ϊһ������
	for (i = 0; i < g->vertex_num; ++i) {
		parent[i] = -1;
	}
	//��ȡͼ�������
	edges = graph_create_edges(g);
	//������߰�Ȩ�����򣬸��Ӷ�ΪO(eloge)
	graph_edge_sort(edges, 0, (g->edge_num >> 1) - 1);
	//���붥����-1���ߣ���e���߽��в��鼯������ÿ�β��Ҹ��Ӷ�ΪO(a(n))����˸��Ӷ�ΪO(e*a(n))
	//a(n)Ϊ��������������ͨ��<=4
	for (i = 0, j = 0; j < g->vertex_num - 1; ++i) {
		//���ҵ�i��������������������
		anc1 = mfset_find(parent, edges[i].src);
		anc2 = mfset_find(parent, edges[i].dest);
		//��Ϊһ������ʱ
		if (anc1 != anc2) {
			//�ϲ���������
			mfset_union(parent, anc1, anc2);
			//������Ϊ��С��������
			infos[j].src = g->v[edges[i].src].info;
			infos[j].dest = g->v[edges[i].dest].info;
			infos[j].weight = edges[i].weight;
			++j;
		}
	}
	//�ͷ��ڴ�
	free(parent);
	free(edges);
	return infos;
}

/**
 *
 * �������򣬸��Ӷ�O(n+e)
 * ����ͨ�����Ϊ0�жϣ�Ҳ����ͨ��dfs��dfs�˳��ڵ��˳��
 * Ϊ�������е�������Ϊdfs�˳��ڵ�ʱ���ýڵ����Ϊ0;
 * ����ͨ�����������ж�����ͼ�Ƿ��л���������ͼ������ʹ��dfs��
 * �ж�ÿ����ͨ�����ı����Ƿ�Ϊ������-1
 *
 * @param g ͼ��ַ����ͼ��Ϊ�����޻�ͼ
 *        order �����������е�ַ��ʹ����Ϻ���free������Ϊ������
 *
 * @return TRUE ����ɹ�
 *         FALSE ��ͼ�л�������ʧ��
 *
 */
BOOL graph_topological_sort(const Graph* g, char** order) {
	int* stack;
	int top;
	int* indegree;
	int i;
	GraphEdgeNode* p;
	int count;

	//�����ڴ�
	stack = (int*) malloc(sizeof(int) * g->vertex_num);
	indegree = (int*) malloc(sizeof(int) * g->vertex_num);
	*order = (char*) malloc(sizeof(char) * g->vertex_num);
	if (!stack || !indegree || !*order) {
		exit(-1);
	}
	//��ʼ�����������Ϊ0
	for (i = 0; i < g->vertex_num; ++i) {
		indegree[i] = 0;
	}
	//�������ߣ����Ŀ�Ķ�������
	for (i = 0; i < g->vertex_num; ++i) {
		p = g->v[i].first;
		while (p) {
			++indegree[p->dest];
			p = p->next;
		}
	}
	//��ʼ��ջ��
	top = -1;
	//�����Ϊ0�Ķ�����ջ������ʹ��ջ/���о���
	for (i = 0; i < g->vertex_num; ++i) {
		if (!indegree[i]) {
			stack[++top] = i;
		}
	}
	count = 0;
	//ջ�ǿ�ʱ
	while (top >= 0) {
		//ջ����ջ
		i = stack[top--];
		//��Ӹö�����Ϣ
		(*order)[count++] = g->v[i].info;
		//�����ö����ڽӱߣ�Ŀ�Ķ������-1
		p = g->v[i].first;
		while (p) {
			//��Ƚ���0����ջ
			if (--indegree[p->dest] <= 0) {
				stack[++top] = p->dest;
			}
			p = p->next;
		}
	}
	//�ͷ��ڴ�
	free(indegree);
	free(stack);
	if (count < g->vertex_num) {
		//�������ж��㶼���ջ��˵����ͼ�л�
		return FALSE;
	} else {
		return TRUE;
	}
}

/**
 *
 * dijkstra��Դ���·�������Ӷ�O(n^2)
 *
 * @param g ͼ��ַ
 *        vi Դ������Ϣ
 *
 * @return ����������̾��룬int[i]Ϊ��λ��Ϊi�Ķ������̾��룬
 *         ʹ����Ϻ���free������Ϊ������
 *
 */
int* graph_dijkstra(const Graph* g, char vi) {
	int** mat;
	int* dis;	//dis[i]Ϊ��λ��Ϊi�Ķ������̾���
	BOOL* final;	//final[i]Ϊλ��Ϊi�Ķ����Ƿ��������̾���
	int i, j;
	int pos;
	int min;

	//��ͼת��Ϊ�ڽӾ���
	mat = graph_to_matrix(g);
	//�����ڴ�
	dis = (int*) malloc(sizeof(int) * g->vertex_num);
	final = (BOOL*) malloc(sizeof(BOOL) * g->vertex_num);
	if (!dis || !final) {
		exit(-1);
	}
	//��ȡԴ����λ��
	pos = graph_vertex_index(g, vi);
	//��ʼ������������̾���
	for (i = 0; i < g->vertex_num; ++i) {
		//��̾���ΪԴ���㵽�ö������
		dis[i] = mat[pos][i];
		//���ж���δ�����̾���
		final[i] = FALSE;
	}
	//��Դ������̾���Ϊ0
	dis[pos] = 0;
	//Դ�����������̾���
	final[pos] = TRUE;
	//��ʣ�ඥ����-1���������̾���
	for (i = 1; i < g->vertex_num; ++i) {
		//��ʣ�ඥ�����ҳ�������̵Ķ���
		min = GRAPH_WEIGHT_INF;
		for (j = 0; j < g->vertex_num; ++j) {
			if (!final[j] && dis[j] <= min) {
				pos = j;
				min = dis[j];
			}
		}
		//�ö����������̾���
		final[pos] = TRUE;
		//���µ�ʣ�ඥ�����̾���
		for (j = 0; j < g->vertex_num; ++j) {
			//���ö������̾�����ϸö��㵽ʣ�ඥ��ľ���С����̾���ʱ������
			if (!final[j] && min + mat[pos][j] < dis[j]) {
				dis[j] = min + mat[pos][j];
			}
		}
	}
	//�ͷ��ڴ�
	graph_matrix_destroy(mat, g->vertex_num);
	free(final);
	return dis;
}

/**
 *
 * floyd���������������̾��룬���Ӷ�O(n^3)
 *
 * @param g ͼ��ַ
 *
 * @return ��̾������int[i][j]Ϊλ��Ϊi���㵽λ��Ϊj�������̾��룬
 *         ʹ����Ϻ���graph_matrix_destroy������Ϊ������
 *
 */
int** graph_floyd(const Graph* g) {
	int** mat;
	int i, j, k;

	//��ͼת��Ϊ�ڽӾ���
	mat = graph_to_matrix(g);
	//�����㵽�Լ�����̾���Ϊ0
	for (i = 0; i < g->vertex_num; ++i) {
		mat[i][i] = 0;
	}
	for (k = 0; k < g->vertex_num; ++k) {
		for (i = 0; i < g->vertex_num; ++i) {
			for (j = 0; j < g->vertex_num; ++j) {
				//������kʱ�ľ���С��ԭ�Ⱦ���ʱ������
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
 * ���������������޸ģ��������ƺ���
 *
 * @param info ������Ϣ
 *
 */
static void graph_vertex_visit(char* info) {
	printf("%c ", *info);
}

/**
 *
 * ��ʾͼ����ͱ���Ϣ�������޸�
 *
 * @param g ͼ��ַ
 *
 */
void graph_show(Graph* g) {
	int i;
	GraphEdgeNode* p;

	printf("ͼ�Ķ�������%d ������%d\n", g->vertex_num, g->edge_num);
	printf("ͼ�Ķ���ͱ���Ϣ��\n");
	for (i = 0; i < g->vertex_num; ++i) {
		//��ʾ������Ϣ
		printf("%c:", g->v[i].info);
		p = g->v[i].first;
		//��ʾ������Ϣ
		while (p) {
			printf("%c %d; ", g->v[p->dest].info, p->weight);
			p = p->next;
		}
		printf("\n");
	}
	printf("������ȱ��������\n");
	graph_dfs(g, graph_vertex_visit);
	printf("\n");
	printf("������ȱ��������\n");
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

	//����ͼ
	graph_create(&g);

	//���붥��
	graph_vertex_insert(&g, '1');
	graph_vertex_insert(&g, '2');
	graph_vertex_insert(&g, '3');
	graph_vertex_insert(&g, '4');
	graph_vertex_insert(&g, '5');
	graph_vertex_insert(&g, '6');

	//���������
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

	//prim�㷨������С������
	printf("prim�㷨������С��������\n");
	infos = graph_prim(&g);
	for (i = 0; i < g.vertex_num - 1; ++i) {
		printf("s:%c d:%c w:%d\n", infos[i].src, infos[i].dest,
				infos[i].weight);
	}
	free(infos);
	printf("\n");

	//kruskal�㷨������С������
	printf("kruskal�㷨������С��������\n");
	infos = graph_kruskal(&g);
	for (i = 0; i < g.vertex_num - 1; ++i) {
		printf("s:%c d:%c w:%d\n", infos[i].src, infos[i].dest,
				infos[i].weight);
	}
	free(infos);
	printf("\n");

	//�Ƴ�����
	graph_vertex_remove(&g, '3');
	graph_show(&g);

	//����ͼ
	graph_destroy(&g);

	//����ͼ
	graph_create(&g);

	//���붥��
	graph_vertex_insert(&g, '0');
	graph_vertex_insert(&g, '1');
	graph_vertex_insert(&g, '2');
	graph_vertex_insert(&g, '3');
	graph_vertex_insert(&g, '4');
	graph_vertex_insert(&g, '5');

	//���������
	graph_edge_insert(&g, '0', '5', 100);
	graph_edge_insert(&g, '0', '4', 30);
	graph_edge_insert(&g, '0', '2', 10);
	graph_edge_insert(&g, '1', '2', 5);
	graph_edge_insert(&g, '2', '3', 50);
	graph_edge_insert(&g, '3', '5', 10);
	graph_edge_insert(&g, '4', '3', 20);
	graph_edge_insert(&g, '4', '5', 60);

	graph_show(&g);

	//��������
	printf("��������\n");
	if (graph_topological_sort(&g, &order)) {
		for (i = 0; i < g.vertex_num; ++i) {
			printf("%c ", order[i]);
		}
		printf("\n");
	} else {
		printf("��ͼ�л���\n");
	}
	free(order);
	printf("\n");

	//dijkstra��'0'��������̾���
	printf("dijkstra��'0'��������̾��룺\n");
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

	//floyd�����֮����̾���
	printf("floyd�����֮����̾��룺\n");
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
