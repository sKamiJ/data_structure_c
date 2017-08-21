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

//定义散列表键最大长度
#define HASH_TABLE_KEY_MAX_LEN	(32)
//定义散列表长度
#define HASH_TABLE_LEN		(37)

//定义散列表键，可以修改
typedef struct HashTableKey {
	char key[HASH_TABLE_KEY_MAX_LEN];
} HashTableKey;

//定义散列表值，可以修改
typedef struct HashTableVal {
	int val;
} HashTableVal;

//定义散列表节点
typedef struct HashTableNode {
	HashTableKey k;	//键
	HashTableVal v;	//值
	struct HashTableNode* next;	//下一节点
} HashTableNode;

//定义散列表
typedef struct HashTable {
	HashTableNode* slots[HASH_TABLE_LEN];	//散列表槽
	int size;	//散列表节点数
} HashTable;

/**
 *
 * 创建散列表
 *
 * @param ht 散列表地址，该散列表未被创建
 *
 */
void hash_table_create(HashTable* ht) {
	int i;

	//置所有槽为NULL
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		ht->slots[i] = NULL;
	}
	//置节点数为0
	ht->size = 0;
}

/**
 *
 * 清除散列表所有元素
 *
 * @param ht 散列表地址
 *
 */
void hash_table_clear(HashTable* ht) {
	int i;
	HashTableNode* p;

	//遍历散列表
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		//槽不为NULL时，清空节点
		while (ht->slots[i]) {
			p = ht->slots[i];
			ht->slots[i] = p->next;
			free(p);
		}
	}
	//置节点数为0
	ht->size = 0;
}

/**
 *
 * 销毁散列表
 *
 * @param ht 散列表地址
 *
 */
void hash_table_destroy(HashTable* ht) {
	//清除散列表所有元素
	hash_table_clear(ht);
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
static int hash_table_key_cmp(const HashTableKey* k1, const HashTableKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * 计算键的hash值，可根据键修改
 *
 * @param k 键
 *
 * @return hash值
 *
 */
static unsigned int hash_table_key_hash(const HashTableKey* k) {
	unsigned int hash;
	int i;

	//DJBX33A hash函数
	hash = 5381;
	for (i = 0; k->key[i];) {
		hash += (hash << 5) + k->key[i++];
	}
	//添加取模确保不越界
	return hash % HASH_TABLE_LEN;
}

/**
 *
 * 在散列表中插入键以及其对应的值
 *
 * @param ht 散列表地址
 *        key 插入的键
 *        val 键对应的值
 *
 * @return TRUE 插入成功
 *         FALSE 键已经存在或内存不足，插入失败
 *
 */
BOOL hash_table_insert(HashTable* ht, const HashTableKey* key,
		const HashTableVal* val) {
	int pos;
	HashTableNode* p;
	HashTableNode** pp;

	//计算hash值，获取位置
	pos = hash_table_key_hash(key);
	//获取头指针地址
	pp = &ht->slots[pos];
	//未到空节点时，查看键是否存在
	while (*pp) {
		//键已存在，错误
		if (hash_table_key_cmp(&(*pp)->k, key) == 0) {
			return FALSE;
		}
		//查看下一节点
		pp = &(*pp)->next;
	}
	//申请内存
	p = (HashTableNode*) malloc(sizeof(HashTableNode));
	//内存不足，错误
	if (!p) {
		return FALSE;
	}
	//复制键值对
	p->k = *key;
	p->v = *val;
	//下一节点为NULL
	p->next = NULL;
	//空节点为新建节点
	*pp = p;
	//节点数+1
	++ht->size;
	return TRUE;
}

/**
 *
 * 在散列表中查找键
 *
 * @param ht 散列表地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return TRUE 查找成功
 *         FALSE 键不存在，查找失败
 *
 */
BOOL hash_table_search(const HashTable* ht, const HashTableKey* key,
		HashTableVal** val) {
	int pos;
	HashTableNode* p;

	//计算hash值，获取位置
	pos = hash_table_key_hash(key);
	//指向头节点
	p = ht->slots[pos];
	//遍历链表
	while (p) {
		//找到键，查找成功
		if (hash_table_key_cmp(&p->k, key) == 0) {
			*val = &p->v;
			return TRUE;
		}
		//指向下一节点
		p = p->next;
	}
	//键不存在，错误
	return FALSE;
}

/**
 *
 * 在散列表中删除键
 *
 * @param ht 散列表地址
 *        key 删除的键
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
BOOL hash_table_remove(HashTable* ht, const HashTableKey* key) {
	int pos;
	HashTableNode** pp;
	HashTableNode* p;

	//计算hash值，获取位置
	pos = hash_table_key_hash(key);
	//获取头指针地址
	pp = &ht->slots[pos];
	//未到空节点时，查看键是否存在
	while (*pp) {
		//删除键
		if (hash_table_key_cmp(&(*pp)->k, key) == 0) {
			p = *pp;
			*pp = p->next;
			free(p);
			//节点数-1
			--ht->size;
			return TRUE;
		}
		//查看下一节点
		pp = &(*pp)->next;
	}
	//键不存在，错误
	return FALSE;
}

/**
 *
 * 遍历散列表
 *
 * @param ht 散列表地址
 *        visit 遍历函数指针
 *
 */
void hash_table_traversal(const HashTable* ht,
		void (*visit)(const HashTableKey* k, HashTableVal* v)) {
	int i;
	HashTableNode* p;

	//遍历槽
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		//遍历链表
		p = ht->slots[i];
		while (p) {
			(*visit)(&p->k, &p->v);
			p = p->next;
		}
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
static void hash_table_visit(const HashTableKey* key, HashTableVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * 显示散列表信息，可以修改
 *
 * @param ht 散列表地址
 *
 */
void hash_table_show(const HashTable* ht) {
	printf("节点数：%d\n", ht->size);
	printf("遍历结果：\n");
	hash_table_traversal(ht, hash_table_visit);
	printf("\n");
}

int main() {
	HashTable ht;
	HashTableKey k;
	HashTableVal v;
	HashTableVal* vp;

	//创建散列表
	hash_table_create(&ht);

	//插入键值对
	printf("插入键值对：\n");
	strcpy(k.key, "c");
	v.val = 3;
	hash_table_insert(&ht, &k, &v);

	strcpy(k.key, "a");
	v.val = 1;
	hash_table_insert(&ht, &k, &v);

	strcpy(k.key, "e");
	v.val = 5;
	hash_table_insert(&ht, &k, &v);

	strcpy(k.key, "b");
	v.val = 2;
	hash_table_insert(&ht, &k, &v);

	strcpy(k.key, "d");
	v.val = 4;
	hash_table_insert(&ht, &k, &v);

	hash_table_show(&ht);

	//获取键，更改其值
	printf("获取键，更改其值：\n");
	strcpy(k.key, "e");
	hash_table_search(&ht, &k, &vp);
	vp->val = 55;
	hash_table_show(&ht);

	//删除键
	printf("删除键：\n");
	strcpy(k.key, "a");
	hash_table_remove(&ht, &k);

	strcpy(k.key, "e");
	hash_table_remove(&ht, &k);

	strcpy(k.key, "c");
	hash_table_remove(&ht, &k);

	hash_table_show(&ht);

	//清除所有元素
	printf("清除所有元素：\n");
	hash_table_clear(&ht);
	hash_table_show(&ht);

	//销毁有序表
	hash_table_destroy(&ht);

	return 0;
}
