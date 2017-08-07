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

//定义链表元素结构体，可以修改
typedef struct ListElem {
	int val;
} ListElem;

//定义链表节点
typedef struct ListNode {
	ListElem e;	//该节点元素
	struct ListNode* next;	//链表下一节点
} ListNode;

//定义链表结构体
typedef struct List {
	ListNode* head;	//头节点
	int len;	//链表长度，可根据该值获取链表长度
} List;

/**
 *
 * 显示链表长度和元素，可更改
 *
 * @param list 链表地址
 *
 */
void list_show(const List* list) {
	ListNode* p;

	//显示链表长度
	printf("size:%d,elem:", list->len);
	//显示所有元素值
	p = list->head->next;
	while (p) {
		printf("%d ", p->e.val);
		p = p->next;
	}
	printf("\n");
}

/**
 *
 * 创建链表
 *
 * @param list 链表地址，该链表未被创建
 *
 * @return TRUE 创建成功
 *         FALSE 内存不足，创建失败
 *
 */
BOOL list_create(List* list) {
	//创建链表头节点
	list->head = (ListNode*) malloc(sizeof(ListNode));
	//无内存分配时，错误
	if (!list->head) {
		return FALSE;
	}
	//置头节点下一节点为NULL
	list->head->next = NULL;
	//置链表长度为0
	list->len = 0;
	return TRUE;
}

/**
 *
 * 清除链表所有元素
 *
 * @param list 链表地址
 *
 */
void list_clear(List* list) {
	ListNode* p;

	//当头节点下一节点不为NULL时，释放该节点
	while (list->head->next) {
		p = list->head->next;
		list->head->next = p->next;
		free(p);
	}
	//置链表长度为0
	list->len = 0;
}

/**
 *
 * 销毁链表
 *
 * @param list 链表地址
 *
 */
void list_destroy(List* list) {
	//清除链表所有节点
	list_clear(list);
	//释放头节点
	free(list->head);
	//置头节点为NULL
	list->head = NULL;
}

/**
 *
 * 在链表中插入一个元素
 *
 * @param list 链表地址
 *        pos 插入元素的位置，[0, list->len]
 *        e 插入的元素
 *
 * @return TRUE 插入成功
 *         FALSE 位置错误或内存不足，插入失败
 *
 */
BOOL list_insert(List* list, const int pos, const ListElem* e) {
	ListNode *p, *q;
	int i;

	//位置不合法时，错误
	if (pos < 0 || pos > list->len) {
		return FALSE;
	}
	//创建新节点
	q = (ListNode*) malloc(sizeof(ListNode));
	//无内存分配时，错误
	if (!q) {
		return FALSE;
	}
	//将p定位到插入位置的前驱节点
	p = list->head;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//新节点下一节点为前驱节点下一节点
	q->next = p->next;
	//复制链表元素
	q->e = *e;
	//前驱节点下一节点为新节点
	p->next = q;
	//链表长度加1
	++list->len;
	return TRUE;
}

/**
 *
 * 在链表中删除一个元素
 *
 * @param list 链表地址
 *        pos 删除元素的位置，[0, list->len)
 *
 * @return TRUE 删除成功
 *         FALSE 位置错误，删除失败
 *
 */
BOOL list_remove(List* list, const int pos) {
	ListNode *p, *q;
	int i;

	//位置不合法时，错误
	if (pos < 0 || pos >= list->len) {
		return FALSE;
	}
	//将p定位到删除位置的前驱节点
	p = list->head;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//q为删除节点
	q = p->next;
	//前驱节点下一节点为删除节点下一节点
	p->next = q->next;
	//释放删除节点
	free(q);
	//链表长度减1
	--list->len;
	return TRUE;
}

/**
 *
 * 在链表尾插入一个元素
 *
 * @param list 链表地址
 *        e 插入的元素
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足，插入失败
 *
 */
BOOL list_push_back(List* list, const ListElem* e) {
	return list_insert(list, list->len, e);
}

/**
 *
 * 在链表尾删除一个元素
 *
 * @param list 链表地址
 *
 * @return TRUE 删除成功
 *         FALSE 空链表，删除失败
 *
 */
BOOL list_pop_back(List* list) {
	return list_remove(list, list->len - 1);
}

/**
 *
 * 在链表头插入一个元素
 *
 * @param list 链表地址
 *        e 插入的元素
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足，插入失败
 *
 */
BOOL list_push_front(List* list, const ListElem* e) {
	return list_insert(list, 0, e);
}

/**
 *
 * 在链表头删除一个元素
 *
 * @param list 链表地址
 *
 * @return TRUE 删除成功
 *         FALSE 空链表，删除失败
 *
 */
BOOL list_pop_front(List* list) {
	return list_remove(list, 0);
}

/**
 *
 * 获取链表元素
 *
 * @param list 链表地址
 *        pos 获取元素的位置
 *        ep 保存获取元素的指针
 *
 * @return TRUE 获取成功
 *         FALSE 位置错误，获取失败
 *
 */
BOOL list_index(const List* list, const int pos, ListElem** ep) {
	ListNode* p;
	int i;

	//位置不合法时，错误
	if (pos < 0 || pos >= list->len) {
		return FALSE;
	}
	//将p定位到获取位置
	p = list->head->next;
	for (i = 0; i < pos; ++i) {
		p = p->next;
	}
	//获取该位置元素
	*ep = &p->e;
	return TRUE;
}

/**
 *
 * 交换链表元素
 *
 * @param list 链表地址
 *        pos1 交换元素的位置1
 *        pos2 交换元素的位置2
 *
 * @return TRUE 交换成功
 *         FALSE 位置错误，交换失败
 *
 */
BOOL list_swap(const List* list, int pos1, int pos2) {
	ListNode *p1, *p2, *tmp1, *tmp2;
	int i;

	//位置不合法时，错误
	if (pos1 < 0 || pos1 >= list->len || pos2 < 0 || pos2 >= list->len) {
		return FALSE;
	}
	//确保pos1小于等于pos2
	if (pos1 > pos2) {
		i = pos1;
		pos1 = pos2;
		pos2 = i;
	}
	//将p1定位到pos1前驱节点
	p1 = list->head;
	for (i = 0; i < pos1; ++i) {
		p1 = p1->next;
	}
	//将p2定位到pos2前驱节点
	p2 = p1;
	for (; i < pos2; ++i) {
		p2 = p2->next;
	}
	//指针交换
	tmp1 = p1->next;
	tmp2 = p2->next->next;
	p1->next = p2->next;
	if (pos1 + 1 == pos2) {
		//相邻节点情况
		p2->next->next = tmp1;
		p2->next = tmp2;
	} else {
		//非相邻节点情况
		p2->next->next = tmp1->next;
		p2->next = tmp1;
		tmp1->next = tmp2;
	}
	return TRUE;
}

/**
 *
 * 比较链表元素大小，可根据链表元素修改，为排序规则
 *
 * @param e1 链表元素1
 *        e2 链表元素2
 *
 * @return 小于0 e1<e2
 *         等于0 e1=e2
 *         大于0 e1>e2
 *
 */
static int list_elem_cmp(const ListElem* e1, const ListElem* e2) {
	return e1->val - e2->val;
}

/**
 *
 * 三路快速排序递归内核
 *
 * @param list 链表地址
 *        start 起始位置
 *        end 结束位置
 *
 */
static void list_sort_internal(const List* list, const int start, const int end) {
	int i, l, r;
	int res;
	ListElem *epi, *ep_ref;

	//起始位置大于等于结束位置时，递归结束
	if (start >= end) {
		return;
	}
	//[start,l)为比参考值小的元素
	l = start;
	//(r,end]为比参考值大的元素
	r = end;
	//[l,i)为等于参考值的元素
	i = start + 1;
	//把start位置的元素作为参考元素
	list_index(list, start, &ep_ref);
	//当还有元素未比较时
	while (i <= r) {
		//获取i位置的元素
		list_index(list, i, &epi);
		//与参考元素比较
		res = list_elem_cmp(epi, ep_ref);
		if (res > 0) {
			//大元素放到r位置
			list_swap(list, i, r--);
		} else if (res < 0) {
			//小元素放到l位置
			list_swap(list, i++, l++);
		} else {
			//相等元素不移动
			++i;
		}
	}
	//递归排序[start,l-1]
	list_sort_internal(list, start, l - 1);
	//递归排序[r+1,end]
	list_sort_internal(list, r + 1, end);
}

/**
 *
 * 对链表进行三路快速排序
 *
 * @param list 链表地址
 *
 */
void list_sort(const List* list) {
	list_sort_internal(list, 0, list->len - 1);
}

int main() {
	List l;
	ListElem e;
	ListElem* ep;

	//创建链表
	list_create(&l);

	//在链表尾插入元素
	printf("在链表尾插入元素：\n");
	e.val = 0;
	list_push_back(&l, &e);
	list_show(&l);

	//在链表头插入元素
	printf("在链表头插入元素：\n");
	e.val = 1;
	list_push_front(&l, &e);
	list_show(&l);

	//在1位置插入2个元素
	printf("在1位置插入2个元素：\n");
	e.val = 2;
	list_insert(&l, 1, &e);
	e.val = 3;
	list_insert(&l, 1, &e);
	list_show(&l);

	//获取1位置元素，更改其值
	printf("获取1位置元素，更改其值：\n");
	list_index(&l, 1, &ep);
	ep->val = 4;
	list_show(&l);

	//交换0和1位置元素
	printf("交换0和1位置元素：\n");
	list_swap(&l, 0, 1);
	list_show(&l);

	//交换1和3位置元素
	printf("交换1和3位置元素：\n");
	list_swap(&l, 1, 3);
	list_show(&l);

	//链表排序
	printf("链表排序：\n");
	list_sort(&l);
	list_show(&l);

	//删除1位置元素
	printf("删除1位置元素：\n");
	list_remove(&l, 1);
	list_show(&l);

	//删除链表头元素
	printf("删除链表头元素：\n");
	list_pop_front(&l);
	list_show(&l);

	//删除链表尾元素
	printf("删除链表尾元素：\n");
	list_pop_back(&l);
	list_show(&l);

	//清除所有元素
	printf("清除所有元素：\n");
	list_clear(&l);
	list_show(&l);

	//销毁链表
	list_destroy(&l);

	return 0;
}
