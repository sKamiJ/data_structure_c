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

//����ɢ�б����󳤶�
#define HASH_TABLE_KEY_MAX_LEN	(32)
//����ɢ�б���
#define HASH_TABLE_LEN		(37)

//����ɢ�б���������޸�
typedef struct HashTableKey {
	char key[HASH_TABLE_KEY_MAX_LEN];
} HashTableKey;

//����ɢ�б�ֵ�������޸�
typedef struct HashTableVal {
	int val;
} HashTableVal;

//����ɢ�б�ڵ�
typedef struct HashTableNode {
	HashTableKey k;	//��
	HashTableVal v;	//ֵ
	struct HashTableNode* next;	//��һ�ڵ�
} HashTableNode;

//����ɢ�б�
typedef struct HashTable {
	HashTableNode* slots[HASH_TABLE_LEN];	//ɢ�б��
	int size;	//ɢ�б�ڵ���
} HashTable;

/**
 *
 * ����ɢ�б�
 *
 * @param ht ɢ�б��ַ����ɢ�б�δ������
 *
 */
void hash_table_create(HashTable* ht) {
	int i;

	//�����в�ΪNULL
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		ht->slots[i] = NULL;
	}
	//�ýڵ���Ϊ0
	ht->size = 0;
}

/**
 *
 * ���ɢ�б�����Ԫ��
 *
 * @param ht ɢ�б��ַ
 *
 */
void hash_table_clear(HashTable* ht) {
	int i;
	HashTableNode* p;

	//����ɢ�б�
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		//�۲�ΪNULLʱ����սڵ�
		while (ht->slots[i]) {
			p = ht->slots[i];
			ht->slots[i] = p->next;
			free(p);
		}
	}
	//�ýڵ���Ϊ0
	ht->size = 0;
}

/**
 *
 * ����ɢ�б�
 *
 * @param ht ɢ�б��ַ
 *
 */
void hash_table_destroy(HashTable* ht) {
	//���ɢ�б�����Ԫ��
	hash_table_clear(ht);
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
static int hash_table_key_cmp(const HashTableKey* k1, const HashTableKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * �������hashֵ���ɸ��ݼ��޸�
 *
 * @param k ��
 *
 * @return hashֵ
 *
 */
static unsigned int hash_table_key_hash(const HashTableKey* k) {
	unsigned int hash;
	int i;

	//DJBX33A hash����
	hash = 5381;
	for (i = 0; k->key[i];) {
		hash += (hash << 5) + k->key[i++];
	}
	//���ȡģȷ����Խ��
	return hash % HASH_TABLE_LEN;
}

/**
 *
 * ��ɢ�б��в�����Լ����Ӧ��ֵ
 *
 * @param ht ɢ�б��ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *
 * @return TRUE ����ɹ�
 *         FALSE ���Ѿ����ڻ��ڴ治�㣬����ʧ��
 *
 */
BOOL hash_table_insert(HashTable* ht, const HashTableKey* key,
		const HashTableVal* val) {
	int pos;
	HashTableNode* p;
	HashTableNode** pp;

	//����hashֵ����ȡλ��
	pos = hash_table_key_hash(key);
	//��ȡͷָ���ַ
	pp = &ht->slots[pos];
	//δ���սڵ�ʱ���鿴���Ƿ����
	while (*pp) {
		//���Ѵ��ڣ�����
		if (hash_table_key_cmp(&(*pp)->k, key) == 0) {
			return FALSE;
		}
		//�鿴��һ�ڵ�
		pp = &(*pp)->next;
	}
	//�����ڴ�
	p = (HashTableNode*) malloc(sizeof(HashTableNode));
	//�ڴ治�㣬����
	if (!p) {
		return FALSE;
	}
	//���Ƽ�ֵ��
	p->k = *key;
	p->v = *val;
	//��һ�ڵ�ΪNULL
	p->next = NULL;
	//�սڵ�Ϊ�½��ڵ�
	*pp = p;
	//�ڵ���+1
	++ht->size;
	return TRUE;
}

/**
 *
 * ��ɢ�б��в��Ҽ�
 *
 * @param ht ɢ�б��ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return TRUE ���ҳɹ�
 *         FALSE �������ڣ�����ʧ��
 *
 */
BOOL hash_table_search(const HashTable* ht, const HashTableKey* key,
		HashTableVal** val) {
	int pos;
	HashTableNode* p;

	//����hashֵ����ȡλ��
	pos = hash_table_key_hash(key);
	//ָ��ͷ�ڵ�
	p = ht->slots[pos];
	//��������
	while (p) {
		//�ҵ��������ҳɹ�
		if (hash_table_key_cmp(&p->k, key) == 0) {
			*val = &p->v;
			return TRUE;
		}
		//ָ����һ�ڵ�
		p = p->next;
	}
	//�������ڣ�����
	return FALSE;
}

/**
 *
 * ��ɢ�б���ɾ����
 *
 * @param ht ɢ�б��ַ
 *        key ɾ���ļ�
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
BOOL hash_table_remove(HashTable* ht, const HashTableKey* key) {
	int pos;
	HashTableNode** pp;
	HashTableNode* p;

	//����hashֵ����ȡλ��
	pos = hash_table_key_hash(key);
	//��ȡͷָ���ַ
	pp = &ht->slots[pos];
	//δ���սڵ�ʱ���鿴���Ƿ����
	while (*pp) {
		//ɾ����
		if (hash_table_key_cmp(&(*pp)->k, key) == 0) {
			p = *pp;
			*pp = p->next;
			free(p);
			//�ڵ���-1
			--ht->size;
			return TRUE;
		}
		//�鿴��һ�ڵ�
		pp = &(*pp)->next;
	}
	//�������ڣ�����
	return FALSE;
}

/**
 *
 * ����ɢ�б�
 *
 * @param ht ɢ�б��ַ
 *        visit ��������ָ��
 *
 */
void hash_table_traversal(const HashTable* ht,
		void (*visit)(const HashTableKey* k, HashTableVal* v)) {
	int i;
	HashTableNode* p;

	//������
	for (i = 0; i < HASH_TABLE_LEN; ++i) {
		//��������
		p = ht->slots[i];
		while (p) {
			(*visit)(&p->k, &p->v);
			p = p->next;
		}
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
static void hash_table_visit(const HashTableKey* key, HashTableVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * ��ʾɢ�б���Ϣ�������޸�
 *
 * @param ht ɢ�б��ַ
 *
 */
void hash_table_show(const HashTable* ht) {
	printf("�ڵ�����%d\n", ht->size);
	printf("���������\n");
	hash_table_traversal(ht, hash_table_visit);
	printf("\n");
}

int main() {
	HashTable ht;
	HashTableKey k;
	HashTableVal v;
	HashTableVal* vp;

	//����ɢ�б�
	hash_table_create(&ht);

	//�����ֵ��
	printf("�����ֵ�ԣ�\n");
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

	//��ȡ����������ֵ
	printf("��ȡ����������ֵ��\n");
	strcpy(k.key, "e");
	hash_table_search(&ht, &k, &vp);
	vp->val = 55;
	hash_table_show(&ht);

	//ɾ����
	printf("ɾ������\n");
	strcpy(k.key, "a");
	hash_table_remove(&ht, &k);

	strcpy(k.key, "e");
	hash_table_remove(&ht, &k);

	strcpy(k.key, "c");
	hash_table_remove(&ht, &k);

	hash_table_show(&ht);

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	hash_table_clear(&ht);
	hash_table_show(&ht);

	//���������
	hash_table_destroy(&ht);

	return 0;
}
