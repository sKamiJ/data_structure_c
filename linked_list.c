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

//��������Ԫ�ؽṹ�壬�����޸�
typedef struct ListElem {
	int val;
} ListElem;

//��������ڵ�
typedef struct ListNode {
	ListElem e;	//�ýڵ�Ԫ��
	struct ListNode* next;	//������һ�ڵ�
} ListNode;

//��������ṹ��
typedef struct List {
	ListNode* head;	//ͷ�ڵ�
	int len;	//�����ȣ��ɸ��ݸ�ֵ��ȡ������
} List;

/**
 *
 * ��ʾ�����Ⱥ�Ԫ�أ��ɸ���
 *
 * @param list �����ַ
 *
 */
void list_show(const List* list) {
	ListNode* p;

	//��ʾ������
	printf("size:%d,elem:", list->len);
	//��ʾ����Ԫ��ֵ
	p = list->head->next;
	while (p) {
		printf("%d ", p->e.val);
		p = p->next;
	}
	printf("\n");
}

/**
 *
 * ��������
 *
 * @param list �����ַ��������δ������
 *
 * @return TRUE �����ɹ�
 *         FALSE �ڴ治�㣬����ʧ��
 *
 */
BOOL list_create(List* list) {
	//��������ͷ�ڵ�
	list->head = (ListNode*) malloc(sizeof(ListNode));
	//���ڴ����ʱ������
	if (!list->head) {
		return FALSE;
	}
	//��ͷ�ڵ���һ�ڵ�ΪNULL
	list->head->next = NULL;
	//��������Ϊ0
	list->len = 0;
	return TRUE;
}

/**
 *
 * �����������Ԫ��
 *
 * @param list �����ַ
 *
 */
void list_clear(List* list) {
	ListNode* p;

	//��ͷ�ڵ���һ�ڵ㲻ΪNULLʱ���ͷŸýڵ�
	while (list->head->next) {
		p = list->head->next;
		list->head->next = p->next;
		free(p);
	}
	//��������Ϊ0
	list->len = 0;
}

/**
 *
 * ��������
 *
 * @param list �����ַ
 *
 */
void list_destroy(List* list) {
	//����������нڵ�
	list_clear(list);
	//�ͷ�ͷ�ڵ�
	free(list->head);
	//��ͷ�ڵ�ΪNULL
	list->head = NULL;
}

/**
 *
 * �������в���һ��Ԫ��
 *
 * @param list �����ַ
 *        pos ����Ԫ�ص�λ�ã�[0, list->len]
 *        e �����Ԫ��
 *
 * @return TRUE ����ɹ�
 *         FALSE λ�ô�����ڴ治�㣬����ʧ��
 *
 */
BOOL list_insert(List* list, const int pos, const ListElem* e) {
	ListNode *p, *q;
	int i;

	//λ�ò��Ϸ�ʱ������
	if (pos < 0 || pos > list->len) {
		return FALSE;
	}
	//�����½ڵ�
	q = (ListNode*) malloc(sizeof(ListNode));
	//���ڴ����ʱ������
	if (!q) {
		return FALSE;
	}
	//��p��λ������λ�õ�ǰ���ڵ�
	p = list->head;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//�½ڵ���һ�ڵ�Ϊǰ���ڵ���һ�ڵ�
	q->next = p->next;
	//��������Ԫ��
	q->e = *e;
	//ǰ���ڵ���һ�ڵ�Ϊ�½ڵ�
	p->next = q;
	//�����ȼ�1
	++list->len;
	return TRUE;
}

/**
 *
 * ��������ɾ��һ��Ԫ��
 *
 * @param list �����ַ
 *        pos ɾ��Ԫ�ص�λ�ã�[0, list->len)
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE λ�ô���ɾ��ʧ��
 *
 */
BOOL list_remove(List* list, const int pos) {
	ListNode *p, *q;
	int i;

	//λ�ò��Ϸ�ʱ������
	if (pos < 0 || pos >= list->len) {
		return FALSE;
	}
	//��p��λ��ɾ��λ�õ�ǰ���ڵ�
	p = list->head;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//qΪɾ���ڵ�
	q = p->next;
	//ǰ���ڵ���һ�ڵ�Ϊɾ���ڵ���һ�ڵ�
	p->next = q->next;
	//�ͷ�ɾ���ڵ�
	free(q);
	//�����ȼ�1
	--list->len;
	return TRUE;
}

/**
 *
 * ������β����һ��Ԫ��
 *
 * @param list �����ַ
 *        e �����Ԫ��
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�㣬����ʧ��
 *
 */
BOOL list_push_back(List* list, const ListElem* e) {
	return list_insert(list, list->len, e);
}

/**
 *
 * ������βɾ��һ��Ԫ��
 *
 * @param list �����ַ
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE ������ɾ��ʧ��
 *
 */
BOOL list_pop_back(List* list) {
	return list_remove(list, list->len - 1);
}

/**
 *
 * ������ͷ����һ��Ԫ��
 *
 * @param list �����ַ
 *        e �����Ԫ��
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�㣬����ʧ��
 *
 */
BOOL list_push_front(List* list, const ListElem* e) {
	return list_insert(list, 0, e);
}

/**
 *
 * ������ͷɾ��һ��Ԫ��
 *
 * @param list �����ַ
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE ������ɾ��ʧ��
 *
 */
BOOL list_pop_front(List* list) {
	return list_remove(list, 0);
}

/**
 *
 * ��ȡ����Ԫ��
 *
 * @param list �����ַ
 *        pos ��ȡԪ�ص�λ��
 *        ep �����ȡԪ�ص�ָ��
 *
 * @return TRUE ��ȡ�ɹ�
 *         FALSE λ�ô��󣬻�ȡʧ��
 *
 */
BOOL list_index(const List* list, const int pos, ListElem** ep) {
	ListNode* p;
	int i;

	//λ�ò��Ϸ�ʱ������
	if (pos < 0 || pos >= list->len) {
		return FALSE;
	}
	//��p��λ����ȡλ��
	p = list->head->next;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//��ȡ��λ��Ԫ��
	*ep = &p->e;
	return TRUE;
}

/**
 *
 * ��������Ԫ��
 *
 * @param list �����ַ
 *        pos1 ����Ԫ�ص�λ��1
 *        pos2 ����Ԫ�ص�λ��2
 *
 * @return TRUE �����ɹ�
 *         FALSE λ�ô��󣬽���ʧ��
 *
 */
BOOL list_swap(const List* list, int pos1, int pos2) {
	ListNode *p1, *p2, *tmp1, *tmp2;
	int i;

	//λ�ò��Ϸ�ʱ������
	if (pos1 < 0 || pos1 >= list->len || pos2 < 0 || pos2 >= list->len) {
		return FALSE;
	}
	//ȷ��pos1С�ڵ���pos2
	if (pos1 > pos2) {
		i = pos1;
		pos1 = pos2;
		pos2 = i;
	}
	//��p1��λ��pos1ǰ���ڵ�
	p1 = list->head;
	for (i = 0; i < pos1; ++i) {
		p1 = p1->next;
	}
	//��p2��λ��pos2ǰ���ڵ�
	p2 = p1;
	for (; i < pos2; ++i) {
		p2 = p2->next;
	}
	//ָ�뽻��
	tmp1 = p1->next;
	tmp2 = p2->next->next;
	p1->next = p2->next;
	if (pos1 + 1 == pos2) {
		//���ڽڵ����
		p2->next->next = tmp1;
		p2->next = tmp2;
	} else {
		//�����ڽڵ����
		p2->next->next = tmp1->next;
		p2->next = tmp1;
		tmp1->next = tmp2;
	}
	return TRUE;
}

/**
 *
 * �Ƚ�����Ԫ�ش�С���ɸ�������Ԫ���޸ģ�Ϊ�������
 *
 * @param e1 ����Ԫ��1
 *        e2 ����Ԫ��2
 *
 * @return С��0 e1<e2
 *         ����0 e1=e2
 *         ����0 e1>e2
 *
 */
static int list_elem_cmp(const ListElem* e1, const ListElem* e2) {
	return e1->val - e2->val;
}

/**
 *
 * ��·��������ݹ��ں�
 *
 * @param list �����ַ
 *        start ��ʼλ��
 *        end ����λ��
 *
 */
static void list_sort_internal(const List* list, const int start, const int end) {
	int i, l, r;
	int res;
	ListElem *epi, *ep_ref;

	//��ʼλ�ô��ڵ��ڽ���λ��ʱ���ݹ����
	if (start >= end) {
		return;
	}
	//[start,l)Ϊ�Ȳο�ֵС��Ԫ��
	l = start;
	//(r,end]Ϊ�Ȳο�ֵ���Ԫ��
	r = end;
	//[l,i)Ϊ���ڲο�ֵ��Ԫ��
	i = start + 1;
	//��startλ�õ�Ԫ����Ϊ�ο�Ԫ��
	list_index(list, start, &ep_ref);
	//������Ԫ��δ�Ƚ�ʱ
	while (i <= r) {
		//��ȡiλ�õ�Ԫ��
		list_index(list, i, &epi);
		//��ο�Ԫ�رȽ�
		res = list_elem_cmp(epi, ep_ref);
		if (res > 0) {
			//��Ԫ�طŵ�rλ��
			list_swap(list, i, r--);
		} else if (res < 0) {
			//СԪ�طŵ�lλ��
			list_swap(list, i++, l++);
		} else {
			//���Ԫ�ز��ƶ�
			++i;
		}
	}
	//�ݹ�����[start,l-1]
	list_sort_internal(list, start, l - 1);
	//�ݹ�����[r+1,end]
	list_sort_internal(list, r + 1, end);
}

/**
 *
 * �����������·��������
 *
 * @param list �����ַ
 *
 */
void list_sort(const List* list) {
	list_sort_internal(list, 0, list->len - 1);
}

int main() {
	List l;
	ListElem e;
	ListElem* ep;

	//��������
	list_create(&l);

	//������β����Ԫ��
	printf("������β����Ԫ�أ�\n");
	e.val = 0;
	list_push_back(&l, &e);
	list_show(&l);

	//������ͷ����Ԫ��
	printf("������ͷ����Ԫ�أ�\n");
	e.val = 1;
	list_push_front(&l, &e);
	list_show(&l);

	//��1λ�ò���2��Ԫ��
	printf("��1λ�ò���2��Ԫ�أ�\n");
	e.val = 2;
	list_insert(&l, 1, &e);
	e.val = 3;
	list_insert(&l, 1, &e);
	list_show(&l);

	//��ȡ1λ��Ԫ�أ�������ֵ
	printf("��ȡ1λ��Ԫ�أ�������ֵ��\n");
	list_index(&l, 1, &ep);
	ep->val = 4;
	list_show(&l);

	//����0��1λ��Ԫ��
	printf("����0��1λ��Ԫ�أ�\n");
	list_swap(&l, 0, 1);
	list_show(&l);

	//����1��3λ��Ԫ��
	printf("����1��3λ��Ԫ�أ�\n");
	list_swap(&l, 1, 3);
	list_show(&l);

	//��������
	printf("��������\n");
	list_sort(&l);
	list_show(&l);

	//ɾ��1λ��Ԫ��
	printf("ɾ��1λ��Ԫ�أ�\n");
	list_remove(&l, 1);
	list_show(&l);

	//ɾ������ͷԪ��
	printf("ɾ������ͷԪ�أ�\n");
	list_pop_front(&l);
	list_show(&l);

	//ɾ������βԪ��
	printf("ɾ������βԪ�أ�\n");
	list_pop_back(&l);
	list_show(&l);

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	list_clear(&l);
	list_show(&l);

	//��������
	list_destroy(&l);

	return 0;
}
