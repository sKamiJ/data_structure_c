#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义哈夫曼编码节点
typedef struct HuffmanCode {
	char data;	//待编码数据
	int weight;	//数据权重
	char* code;	//哈夫曼编码
} HuffmanCode;

//定义哈夫曼树节点
typedef struct HuffmanTreeNode {
	int weight;	//权重
	int parent, lchild, rchild;	//父节点和左右孩子节点，-1代表不存在
} HuffmanTreeNode;

//定义哈夫曼树
typedef struct HuffmanTree {
	HuffmanTreeNode* nodes;	//树节点
	int size;	//节点数
	int leaf_count;	//叶子节点数
} HuffmanTree;

/**
 *
 * 在已有的哈夫曼节点中，找出权重最小的两个
 *
 * @param nodes 哈夫曼节点首地址
 *        start 查找起始位置
 *        end 查找结束位置
 *        min1 保存最小权重位置的地址
 *        min2 保存第二小权重位置的地址
 *
 */
static void huffman_tree_select(const HuffmanTreeNode* nodes, int* start,
		int end, int* min1, int* min2) {
	int i;

	//找到第一个没有父节点的节点
	for (i = *start; nodes[i].parent != -1; ++i) {
	}
	//更新起始位置
	*start = i;
	//设min1为i
	*min1 = i;
	//在剩余节点中找最小权重节点
	for (++i; i < end; ++i) {
		if (nodes[i].parent == -1 && nodes[i].weight <= nodes[*min1].weight) {
			*min1 = i;
		}
	}
	//找到第一个没有父节点且非min1的节点
	for (i = *start; nodes[i].parent != -1 || i == *min1; ++i) {
	}
	//更新起始位置
	*start = i;
	//设min2为i
	*min2 = i;
	//在剩余节点中找倒数第二小权重节点
	for (++i; i < end; ++i) {
		if (i != *min1 && nodes[i].parent == -1
				&& nodes[i].weight <= nodes[*min2].weight) {
			*min2 = i;
		}
	}
}

/**
 *
 * 根据待编码数据权重，创建哈夫曼树，并进行哈夫曼编码
 *
 * @param ht 哈夫曼树地址，该哈夫曼树未被创建
 *        codes 编码节点数组首地址，节点未被编码
 *        code_num 编码节点数
 *
 */
void huffman_coding(HuffmanTree* ht, HuffmanCode* codes, int code_num) {
	int i;
	int start;
	int min1, min2;
	char* code;
	int p, c;

	//叶子节点数等于编码节点数
	ht->leaf_count = code_num;
	//节点数为编码数*2-1
	ht->size = (code_num << 1) - 1;
	//为树节点申请内存
	ht->nodes = (HuffmanTreeNode*) malloc(sizeof(HuffmanTreeNode) * ht->size);
	//内存不足，错误
	if (!ht->nodes) {
		exit(-1);
	}
	//初始化叶子节点权重
	for (i = 0; i < code_num; ++i) {
		ht->nodes[i].weight = codes[i].weight;
		//置父节点和左右孩子节点为空
		ht->nodes[i].lchild = ht->nodes[i].rchild = ht->nodes[i].parent = -1;
	}
	//起始位置为0
	start = 0;
	//构造哈夫曼树
	for (; i < ht->size; ++i) {
		//在已有节点中，找出权重最小的两个
		huffman_tree_select(ht->nodes, &start, i, &min1, &min2);
		//新节点权重为其和
		ht->nodes[i].weight = ht->nodes[min1].weight + ht->nodes[min2].weight;
		//设权重最小两个为其左右孩子
		ht->nodes[i].lchild = min1;
		ht->nodes[i].rchild = min2;
		//设父节点为空
		ht->nodes[i].parent = -1;
		//设父节点为新节点
		ht->nodes[min1].parent = ht->nodes[min2].parent = i;
	}
	//设置一个临时字符串，编码字符串最大长度为code_num-1
	code = (char*) malloc(sizeof(char) * code_num);
	//内存不足，错误
	if (!code) {
		exit(-1);
	}
	//添加\0
	code[code_num - 1] = '\0';
	//进行哈夫曼编码
	for (i = 0; i < code_num; ++i) {
		start = code_num - 1;
		//从叶子节点遍历至根节点，逆序编码
		for (c = i, p = ht->nodes[i].parent; p != -1;
				c = p, p = ht->nodes[p].parent) {
			code[--start] = ht->nodes[p].lchild == c ? '0' : '1';
		}
		//为哈夫曼编码申请内存
		codes[i].code = (char*) malloc(sizeof(char) * (code_num - start));
		//内存不足，错误
		if (!codes[i].code) {
			exit(-1);
		}
		//拷贝编码
		strcpy(codes[i].code, &code[start]);
	}
	//释放临时字符串
	free(code);
}

/**
 *
 * 销毁哈夫曼树
 *
 * @param ht 哈夫曼树地址
 *
 */
void huffman_tree_destroy(HuffmanTree* ht) {
	//释放树节点内存
	free(ht->nodes);
	ht->nodes = NULL;
	//节点数置0
	ht->size = ht->leaf_count = 0;
}

/**
 *
 * 销毁哈夫曼编码
 *
 * @param codes 编码节点数组首地址
 *        code_num 编码节点数
 *
 */
void huffman_code_destroy(HuffmanCode* codes, int code_num) {
	int i;
	//释放编码内存
	for (i = 0; i < code_num; ++i) {
		free(codes[i].code);
		codes[i].code = NULL;
	}
}

/**
 *
 * 显示哈夫曼编码信息，可以修改
 *
 * @param codes 编码节点数组首地址
 *        code_num 编码节点数
 *
 */
void huffman_code_show(const HuffmanCode* codes, int code_num) {
	int i;

	for (i = 0; i < code_num; ++i) {
		printf("data:%c code:%s\n", codes[i].data, codes[i].code);
	}
}

int main() {
	HuffmanTree ht;
	HuffmanCode codes[7];

	codes[0].data = 'A';
	codes[0].weight = 5;

	codes[1].data = 'B';
	codes[1].weight = 24;

	codes[2].data = 'C';
	codes[2].weight = 7;

	codes[3].data = 'D';
	codes[3].weight = 17;

	codes[4].data = 'E';
	codes[4].weight = 34;

	codes[5].data = 'F';
	codes[5].weight = 5;

	codes[6].data = 'G';
	codes[6].weight = 13;

	//创建哈夫曼树并进行哈夫曼编码
	huffman_coding(&ht, codes, 7);
	//显示哈夫曼编码
	huffman_code_show(codes, 7);
	//销毁哈夫曼树和哈夫曼编码
	huffman_tree_destroy(&ht);
	huffman_code_destroy(codes, 7);

	return 0;
}
