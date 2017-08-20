#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//定义BOOL类型
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//定义有序表键最大长度
#define ORDERED_LIST_KEY_MAX_LEN	(32)
//定义有序表初始容量
#define ORDERED_LIST_INIT_CAPACITY	(10)

//定义有序表键，可以修改
typedef struct OrderedListKey {
	char key[ORDERED_LIST_KEY_MAX_LEN];
} OrderedListKey;

//定义有序表值，可以修改
typedef struct OrderedListVal {
	int val;
} OrderedListVal;

//定义有序表节点
typedef struct OrderedListNode {
	OrderedListKey k;	//键
	OrderedListVal v;	//值
} OrderedListNode;

//定义有序表
typedef struct OrderedList {
	OrderedListNode* nodes;	//节点
	int capacity;	//有序表容量，即最大节点数
	int len;	//有序表长度，可通过该变量获取其长度
} OrderedList;

/**
 *
 * 创建有序表
 *
 * @param ol 有序表地址，该有序表未被创建
 *
 * @return TRUE 创建成功
 *         FALSE 内存不足，创建失败
 */
BOOL ordered_list_create(OrderedList* ol) {
	//为节点申请内存
	ol->nodes = (OrderedListNode*) malloc(
			sizeof(OrderedListNode) * ORDERED_LIST_INIT_CAPACITY);
	//内存不足，错误
	if (!ol->nodes) {
		return FALSE;
	}
	//置容量为初始容量
	ol->capacity = ORDERED_LIST_INIT_CAPACITY;
	//置有序表长度为0
	ol->len = 0;
	return TRUE;
}

/**
 *
 * 清除有序表所有元素
 *
 * @param ol 有序表地址
 *
 */
void ordered_list_clear(OrderedList* ol) {
	//置有序表长度为0
	ol->len = 0;
}

/**
 *
 * 销毁有序表
 *
 * @param ol 有序表地址
 *
 */
void ordered_list_destroy(OrderedList* ol) {
	//释放所有节点内存
	free(ol->nodes);
	ol->nodes = NULL;
	//置容量与长度为0
	ol->capacity = 0;
	ol->len = 0;
}

/**
 *
 * 比较两个键的大小，可根据键修改
 *
 * @param k1 键1
 *        k2 键2
 *
 * @return 小于0 k1<k2
 *         等于0 k1=k2
 *         大于0 k1>k2
 *
 */
static int ordered_list_key_cmp(const OrderedListKey* k1,
		const OrderedListKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * 在有序表中插入键以及其对应的值
 *
 * @param ol 有序表地址
 *        key 插入的键
 *        val 键对应的值
 *
 * @return TRUE 插入成功
 *         FALSE 键已经存在或扩容时内存不足，插入失败
 *
 */
BOOL ordered_list_insert(OrderedList* ol, const OrderedListKey* key,
		const OrderedListVal* val) {
	int l, r, m;
	int res;
	int i;
	OrderedListNode* p;

	//有序表已满，扩容
	if (ol->len >= ol->capacity) {
		//新容量为原容量*2+1
		p = (OrderedListNode*) malloc(
				sizeof(OrderedListNode) * ((ol->capacity << 1) + 1));
		//内存不足，错误
		if (!p) {
			return FALSE;
		}
		//复制所有节点
		for (i = 0; i < ol->len; ++i) {
			p[i] = ol->nodes[i];
		}
		//释放原节点
		free(ol->nodes);
		//更新节点和容量
		ol->nodes = p;
		ol->capacity = (ol->capacity << 1) + 1;
	}

	l = 0;
	r = ol->len - 1;
	//未查找结束时
	while (l <= r) {
		//计算中点位置
		m = (l + r) >> 1;
		//比较中点位置键和插入键的大小
		res = ordered_list_key_cmp(&ol->nodes[m].k, key);
		if (res < 0) {
			//小于时，左边界为中点+1
			l = m + 1;
		} else if (res > 0) {
			//大于时，右边界为中点-1
			r = m - 1;
		} else {
			//键已存在，错误
			return FALSE;
		}
	}
	//新插入键的位置为l
	//向后移动节点
	for (i = ol->len - 1; i >= l; --i) {
		ol->nodes[i + 1] = ol->nodes[i];
	}
	//在l位置处复制键和值
	ol->nodes[l].k = *key;
	ol->nodes[l].v = *val;
	//长度+1
	++ol->len;
	return TRUE;
}

/**
 *
 * 在有序表中查找键
 *
 * @param ol 有序表地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return 键在有序表中的位置，-1代表键不存在
 *
 */
int ordered_list_search(const OrderedList* ol, const OrderedListKey* key,
		OrderedListVal** val) {
	int l, r, m;
	int res;

	l = 0;
	r = ol->len - 1;
	//未查找结束时
	while (l <= r) {
		//计算中点位置
		m = (l + r) >> 1;
		//比较中点位置键和插入键的大小
		res = ordered_list_key_cmp(&ol->nodes[m].k, key);
		if (res < 0) {
			//小于时，左边界为中点+1
			l = m + 1;
		} else if (res > 0) {
			//大于时，右边界为中点-1
			r = m - 1;
		} else {
			//保存值的指针
			*val = &ol->nodes[m].v;
			//返回位置
			return m;
		}
	}
	//键不存在，查找失败
	return -1;
}

/**
 *
 * 在有序表中删除键
 *
 * @param ol 有序表地址
 *        key 删除的键
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
BOOL ordered_list_remove(OrderedList* ol, const OrderedListKey* key) {
	int pos;
	OrderedListVal* vp;
	int i;

	//查找删除键位置
	pos = ordered_list_search(ol, key, &vp);
	//键不存在，错误
	if (pos == -1) {
		return FALSE;
	}
	//向前移动节点
	for (i = pos; i < ol->len - 1; ++i) {
		ol->nodes[i] = ol->nodes[i + 1];
	}
	//长度-1
	--ol->len;
	return TRUE;
}

/**
 *
 * 遍历有序表
 *
 * @param ol 有序表地址
 *        visit 遍历函数指针
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
 * 遍历函数，可以修改，或建立类似函数
 *
 * @param key 键
 *        val 值
 *
 */
static void ordered_list_visit(const OrderedListKey* key, OrderedListVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * 显示有序表信息，可以修改
 *
 * @param ol 有序表地址
 *
 */
void ordered_list_show(const OrderedList* ol) {
	printf("节点数：%d\n", ol->len);
	printf("遍历结果：\n");
	ordered_list_traversal(ol, ordered_list_visit);
	printf("\n");
}

int main() {
	OrderedList ol;
	OrderedListKey k;
	OrderedListVal v;
	OrderedListVal* vp;

	//创建有序表
	ordered_list_create(&ol);

	//插入键值对
	printf("插入键值对：\n");
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

	//获取键，更改其值
	printf("获取键，更改其值：\n");
	strcpy(k.key, "e");
	ordered_list_search(&ol, &k, &vp);
	vp->val = 55;
	ordered_list_show(&ol);

	//删除键
	printf("删除键：\n");
	strcpy(k.key, "a");
	ordered_list_remove(&ol, &k);

	strcpy(k.key, "e");
	ordered_list_remove(&ol, &k);

	strcpy(k.key, "c");
	ordered_list_remove(&ol, &k);

	ordered_list_show(&ol);

	//清除所有元素
	printf("清除所有元素：\n");
	ordered_list_clear(&ol);
	ordered_list_show(&ol);

	//销毁有序表
	ordered_list_destroy(&ol);

	return 0;
}
