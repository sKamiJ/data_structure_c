#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//����BOOL����
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//������������󳤶�
#define ORDERED_LIST_KEY_MAX_LEN	(32)
//����������ʼ����
#define ORDERED_LIST_INIT_CAPACITY	(10)

//�����������������޸�
typedef struct OrderedListKey {
	char key[ORDERED_LIST_KEY_MAX_LEN];
} OrderedListKey;

//���������ֵ�������޸�
typedef struct OrderedListVal {
	int val;
} OrderedListVal;

//���������ڵ�
typedef struct OrderedListNode {
	OrderedListKey k;	//��
	OrderedListVal v;	//ֵ
} OrderedListNode;

//���������
typedef struct OrderedList {
	OrderedListNode* nodes;	//�ڵ�
	int capacity;	//����������������ڵ���
	int len;	//������ȣ���ͨ���ñ�����ȡ�䳤��
} OrderedList;

/**
 *
 * ���������
 *
 * @param ol ������ַ���������δ������
 *
 * @return TRUE �����ɹ�
 *         FALSE �ڴ治�㣬����ʧ��
 */
BOOL ordered_list_create(OrderedList* ol) {
	//Ϊ�ڵ������ڴ�
	ol->nodes = (OrderedListNode*) malloc(
			sizeof(OrderedListNode) * ORDERED_LIST_INIT_CAPACITY);
	//�ڴ治�㣬����
	if (!ol->nodes) {
		return FALSE;
	}
	//������Ϊ��ʼ����
	ol->capacity = ORDERED_LIST_INIT_CAPACITY;
	//���������Ϊ0
	ol->len = 0;
	return TRUE;
}

/**
 *
 * ������������Ԫ��
 *
 * @param ol ������ַ
 *
 */
void ordered_list_clear(OrderedList* ol) {
	//���������Ϊ0
	ol->len = 0;
}

/**
 *
 * ���������
 *
 * @param ol ������ַ
 *
 */
void ordered_list_destroy(OrderedList* ol) {
	//�ͷ����нڵ��ڴ�
	free(ol->nodes);
	ol->nodes = NULL;
	//�������볤��Ϊ0
	ol->capacity = 0;
	ol->len = 0;
}

/**
 *
 * �Ƚ��������Ĵ�С���ɸ��ݼ��޸�
 *
 * @param k1 ��1
 *        k2 ��2
 *
 * @return С��0 k1<k2
 *         ����0 k1=k2
 *         ����0 k1>k2
 *
 */
static int ordered_list_key_cmp(const OrderedListKey* k1,
		const OrderedListKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * ��������в�����Լ����Ӧ��ֵ
 *
 * @param ol ������ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *
 * @return TRUE ����ɹ�
 *         FALSE ���Ѿ����ڻ�����ʱ�ڴ治�㣬����ʧ��
 *
 */
BOOL ordered_list_insert(OrderedList* ol, const OrderedListKey* key,
		const OrderedListVal* val) {
	int l, r, m;
	int res;
	int i;
	OrderedListNode* p;

	//���������������
	if (ol->len >= ol->capacity) {
		//������Ϊԭ����*2+1
		p = (OrderedListNode*) malloc(
				sizeof(OrderedListNode) * ((ol->capacity << 1) + 1));
		//�ڴ治�㣬����
		if (!p) {
			return FALSE;
		}
		//�������нڵ�
		for (i = 0; i < ol->len; ++i) {
			p[i] = ol->nodes[i];
		}
		//�ͷ�ԭ�ڵ�
		free(ol->nodes);
		//���½ڵ������
		ol->nodes = p;
		ol->capacity = (ol->capacity << 1) + 1;
	}

	l = 0;
	r = ol->len - 1;
	//δ���ҽ���ʱ
	while (l <= r) {
		//�����е�λ��
		m = (l + r) >> 1;
		//�Ƚ��е�λ�ü��Ͳ�����Ĵ�С
		res = ordered_list_key_cmp(&ol->nodes[m].k, key);
		if (res < 0) {
			//С��ʱ����߽�Ϊ�е�+1
			l = m + 1;
		} else if (res > 0) {
			//����ʱ���ұ߽�Ϊ�е�-1
			r = m - 1;
		} else {
			//���Ѵ��ڣ�����
			return FALSE;
		}
	}
	//�²������λ��Ϊl
	//����ƶ��ڵ�
	for (i = ol->len - 1; i >= l; --i) {
		ol->nodes[i + 1] = ol->nodes[i];
	}
	//��lλ�ô����Ƽ���ֵ
	ol->nodes[l].k = *key;
	ol->nodes[l].v = *val;
	//����+1
	++ol->len;
	return TRUE;
}

/**
 *
 * ��������в��Ҽ�
 *
 * @param ol ������ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return ����������е�λ�ã�-1�����������
 *
 */
int ordered_list_search(const OrderedList* ol, const OrderedListKey* key,
		OrderedListVal** val) {
	int l, r, m;
	int res;

	l = 0;
	r = ol->len - 1;
	//δ���ҽ���ʱ
	while (l <= r) {
		//�����е�λ��
		m = (l + r) >> 1;
		//�Ƚ��е�λ�ü��Ͳ�����Ĵ�С
		res = ordered_list_key_cmp(&ol->nodes[m].k, key);
		if (res < 0) {
			//С��ʱ����߽�Ϊ�е�+1
			l = m + 1;
		} else if (res > 0) {
			//����ʱ���ұ߽�Ϊ�е�-1
			r = m - 1;
		} else {
			//����ֵ��ָ��
			*val = &ol->nodes[m].v;
			//����λ��
			return m;
		}
	}
	//�������ڣ�����ʧ��
	return -1;
}

/**
 *
 * ���������ɾ����
 *
 * @param ol ������ַ
 *        key ɾ���ļ�
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
BOOL ordered_list_remove(OrderedList* ol, const OrderedListKey* key) {
	int pos;
	OrderedListVal* vp;
	int i;

	//����ɾ����λ��
	pos = ordered_list_search(ol, key, &vp);
	//�������ڣ�����
	if (pos == -1) {
		return FALSE;
	}
	//��ǰ�ƶ��ڵ�
	for (i = pos; i < ol->len - 1; ++i) {
		ol->nodes[i] = ol->nodes[i + 1];
	}
	//����-1
	--ol->len;
	return TRUE;
}

/**
 *
 * ���������
 *
 * @param ol ������ַ
 *        visit ��������ָ��
 *
 */
void ordered_list_traversal(const OrderedList* ol,
		void (*visit)(const OrderedListKey* k, OrderedListVal* v)) {
	int i;

	for (i = 0; i < ol->len; ++i) {
		(*visit)(&ol->nodes[i].k, &ol->nodes[i].v);
	}
}

/**
 *
 * ���������������޸ģ��������ƺ���
 *
 * @param key ��
 *        val ֵ
 *
 */
static void ordered_list_visit(const OrderedListKey* key, OrderedListVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * ��ʾ�������Ϣ�������޸�
 *
 * @param ol ������ַ
 *
 */
void ordered_list_show(const OrderedList* ol) {
	printf("�ڵ�����%d\n", ol->len);
	printf("���������\n");
	ordered_list_traversal(ol, ordered_list_visit);
	printf("\n");
}

int main() {
	OrderedList ol;
	OrderedListKey k;
	OrderedListVal v;
	OrderedListVal* vp;

	//���������
	ordered_list_create(&ol);

	//�����ֵ��
	printf("�����ֵ�ԣ�\n");
	strcpy(k.key, "c");
	v.val = 3;
	ordered_list_insert(&ol, &k, &v);

	strcpy(k.key, "a");
	v.val = 1;
	ordered_list_insert(&ol, &k, &v);

	strcpy(k.key, "e");
	v.val = 5;
	ordered_list_insert(&ol, &k, &v);

	strcpy(k.key, "b");
	v.val = 2;
	ordered_list_insert(&ol, &k, &v);

	strcpy(k.key, "d");
	v.val = 4;
	ordered_list_insert(&ol, &k, &v);

	ordered_list_show(&ol);

	//��ȡ����������ֵ
	printf("��ȡ����������ֵ��\n");
	strcpy(k.key, "e");
	ordered_list_search(&ol, &k, &vp);
	vp->val = 55;
	ordered_list_show(&ol);

	//ɾ����
	printf("ɾ������\n");
	strcpy(k.key, "a");
	ordered_list_remove(&ol, &k);

	strcpy(k.key, "e");
	ordered_list_remove(&ol, &k);

	strcpy(k.key, "c");
	ordered_list_remove(&ol, &k);

	ordered_list_show(&ol);

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	ordered_list_clear(&ol);
	ordered_list_show(&ol);

	//���������
	ordered_list_destroy(&ol);

	return 0;
}
