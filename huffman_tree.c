#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//�������������ڵ�
typedef struct HuffmanCode {
	char data;	//����������
	int weight;	//����Ȩ��
	char* code;	//����������
} HuffmanCode;

//������������ڵ�
typedef struct HuffmanTreeNode {
	int weight;	//Ȩ��
	int parent, lchild, rchild;	//���ڵ�����Һ��ӽڵ㣬-1��������
} HuffmanTreeNode;

//�����������
typedef struct HuffmanTree {
	HuffmanTreeNode* nodes;	//���ڵ�
	int size;	//�ڵ���
	int leaf_count;	//Ҷ�ӽڵ���
} HuffmanTree;

/**
 *
 * �����еĹ������ڵ��У��ҳ�Ȩ����С������
 *
 * @param nodes �������ڵ��׵�ַ
 *        start ������ʼλ��
 *        end ���ҽ���λ��
 *        min1 ������СȨ��λ�õĵ�ַ
 *        min2 ����ڶ�СȨ��λ�õĵ�ַ
 *
 */
static void huffman_tree_select(const HuffmanTreeNode* nodes, int* start,
		int end, int* min1, int* min2) {
	int i;

	//�ҵ���һ��û�и��ڵ�Ľڵ�
	for (i = *start; nodes[i].parent != -1; ++i) {
	}
	//������ʼλ��
	*start = i;
	//��min1Ϊi
	*min1 = i;
	//��ʣ��ڵ�������СȨ�ؽڵ�
	for (++i; i < end; ++i) {
		if (nodes[i].parent == -1 && nodes[i].weight <= nodes[*min1].weight) {
			*min1 = i;
		}
	}
	//�ҵ���һ��û�и��ڵ��ҷ�min1�Ľڵ�
	for (i = *start; nodes[i].parent != -1 || i == *min1; ++i) {
	}
	//������ʼλ��
	*start = i;
	//��min2Ϊi
	*min2 = i;
	//��ʣ��ڵ����ҵ����ڶ�СȨ�ؽڵ�
	for (++i; i < end; ++i) {
		if (i != *min1 && nodes[i].parent == -1
				&& nodes[i].weight <= nodes[*min2].weight) {
			*min2 = i;
		}
	}
}

/**
 *
 * ���ݴ���������Ȩ�أ��������������������й���������
 *
 * @param ht ����������ַ���ù�������δ������
 *        codes ����ڵ������׵�ַ���ڵ�δ������
 *        code_num ����ڵ���
 *
 */
void huffman_coding(HuffmanTree* ht, HuffmanCode* codes, int code_num) {
	int i;
	int start;
	int min1, min2;
	char* code;
	int p, c;

	//Ҷ�ӽڵ������ڱ���ڵ���
	ht->leaf_count = code_num;
	//�ڵ���Ϊ������*2-1
	ht->size = (code_num << 1) - 1;
	//Ϊ���ڵ������ڴ�
	ht->nodes = (HuffmanTreeNode*) malloc(sizeof(HuffmanTreeNode) * ht->size);
	//�ڴ治�㣬����
	if (!ht->nodes) {
		exit(-1);
	}
	//��ʼ��Ҷ�ӽڵ�Ȩ��
	for (i = 0; i < code_num; ++i) {
		ht->nodes[i].weight = codes[i].weight;
		//�ø��ڵ�����Һ��ӽڵ�Ϊ��
		ht->nodes[i].lchild = ht->nodes[i].rchild = ht->nodes[i].parent = -1;
	}
	//��ʼλ��Ϊ0
	start = 0;
	//�����������
	for (; i < ht->size; ++i) {
		//�����нڵ��У��ҳ�Ȩ����С������
		huffman_tree_select(ht->nodes, &start, i, &min1, &min2);
		//�½ڵ�Ȩ��Ϊ���
		ht->nodes[i].weight = ht->nodes[min1].weight + ht->nodes[min2].weight;
		//��Ȩ����С����Ϊ�����Һ���
		ht->nodes[i].lchild = min1;
		ht->nodes[i].rchild = min2;
		//�踸�ڵ�Ϊ��
		ht->nodes[i].parent = -1;
		//�踸�ڵ�Ϊ�½ڵ�
		ht->nodes[min1].parent = ht->nodes[min2].parent = i;
	}
	//����һ����ʱ�ַ����������ַ�����󳤶�Ϊcode_num-1
	code = (char*) malloc(sizeof(char) * code_num);
	//�ڴ治�㣬����
	if (!code) {
		exit(-1);
	}
	//���\0
	code[code_num - 1] = '\0';
	//���й���������
	for (i = 0; i < code_num; ++i) {
		start = code_num - 1;
		//��Ҷ�ӽڵ���������ڵ㣬�������
		for (c = i, p = ht->nodes[i].parent; p != -1;
				c = p, p = ht->nodes[p].parent) {
			code[--start] = ht->nodes[p].lchild == c ? '0' : '1';
		}
		//Ϊ���������������ڴ�
		codes[i].code = (char*) malloc(sizeof(char) * (code_num - start));
		//�ڴ治�㣬����
		if (!codes[i].code) {
			exit(-1);
		}
		//��������
		strcpy(codes[i].code, &code[start]);
	}
	//�ͷ���ʱ�ַ���
	free(code);
}

/**
 *
 * ���ٹ�������
 *
 * @param ht ����������ַ
 *
 */
void huffman_tree_destroy(HuffmanTree* ht) {
	//�ͷ����ڵ��ڴ�
	free(ht->nodes);
	ht->nodes = NULL;
	//�ڵ�����0
	ht->size = ht->leaf_count = 0;
}

/**
 *
 * ���ٹ���������
 *
 * @param codes ����ڵ������׵�ַ
 *        code_num ����ڵ���
 *
 */
void huffman_code_destroy(HuffmanCode* codes, int code_num) {
	int i;
	//�ͷű����ڴ�
	for (i = 0; i < code_num; ++i) {
		free(codes[i].code);
		codes[i].code = NULL;
	}
}

/**
 *
 * ��ʾ������������Ϣ�������޸�
 *
 * @param codes ����ڵ������׵�ַ
 *        code_num ����ڵ���
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

	//�����������������й���������
	huffman_coding(&ht, codes, 7);
	//��ʾ����������
	huffman_code_show(codes, 7);
	//���ٹ��������͹���������
	huffman_tree_destroy(&ht);
	huffman_code_destroy(codes, 7);

	return 0;
}
