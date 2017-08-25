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

//����������������󳤶�
#define BST_KEY_MAX_LEN	(32)

//���������������������޸�
typedef struct BSTKey {
	char key[BST_KEY_MAX_LEN];
} BSTKey;

//������������ֵ�������޸�
typedef struct BSTVal {
	int val;
} BSTVal;

//�������������ڵ�
typedef struct BSTNode {
	BSTKey k;	//��
	BSTVal v;	//ֵ
	struct BSTNode* lchild;	//���ӽڵ�
	struct BSTNode* rchild;	//�Һ��ӽڵ�
} BSTNode;

//������������
typedef struct BST {
	BSTNode* root;	//���ڵ�
} BST;

/**
 *
 * �������������
 *
 * @param bst �����������ַ���ö��������δ������
 *
 */
void bst_create(BST* bst) {
	//�ø��ڵ�ΪNULL
	bst->root = NULL;
}

/**
 *
 * ����������������Ԫ���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 */
static void bst_clear_internal(BSTNode** root) {
	//���ڵ�ΪNULLʱ���ݹ����
	if (!*root) {
		return;
	}
	//���������
	bst_clear_internal(&(*root)->lchild);
	//���������
	bst_clear_internal(&(*root)->rchild);
	//�ͷŸ��ڵ�
	free(*root);
	//���ڵ���ΪNULL
	*root = NULL;
}

/**
 *
 * ����������������Ԫ��
 *
 * @param bst �����������ַ
 *
 */
void bst_clear(BST* bst) {
	bst_clear_internal(&bst->root);
}

/**
 *
 * ���ٶ��������
 *
 * @param bst �����������ַ
 *
 */
void bst_destroy(BST* bst) {
	//����������������Ԫ��
	bst_clear(bst);
}

/**
 *
 * ��ȡ����������ڵ������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return �ڵ�����
 *
 */
static int bst_size_internal(const BSTNode* root) {
	int lsize, rsize;

	//���ڵ�ΪNULLʱ������0
	if (!root) {
		return 0;
	}
	//��ȡ�������ڵ�����
	lsize = bst_size_internal(root->lchild);
	//��ȡ�������ڵ�����
	rsize = bst_size_internal(root->rchild);
	//���ؽڵ�����
	return lsize + rsize + 1;
}

/**
 *
 * ��ȡ����������ڵ�����
 *
 * @param bst �����������ַ
 *
 * @return �ڵ�����
 *
 */
int bst_size(const BST* bst) {
	return bst_size_internal(bst->root);
}

/**
 *
 * ��ȡ����������߶��ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return �߶�
 *
 */
static int bst_height_internal(const BSTNode* root) {
	int lheight, rheight;

	//���ڵ�ΪNULLʱ������0
	if (!root) {
		return 0;
	}
	//��ȡ�������߶�
	lheight = bst_height_internal(root->lchild);
	//��ȡ�������߶�
	rheight = bst_height_internal(root->rchild);
	//���߶�Ϊ���������ϸ���+1
	return (lheight > rheight ? lheight : rheight) + 1;
}

/**
 *
 * ��ȡ����������߶�
 *
 * @param bst �����������ַ
 *
 * @return �߶�
 *
 */
int bst_height(const BST* bst) {
	return bst_height_internal(bst->root);
}

/**
 *
 * ��ȡ���������Ҷ�������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return Ҷ������
 *
 */
static int bst_leaf_count_internal(const BSTNode* root) {
	int lleaf, rleaf;

	//���ڵ�ΪNULLʱ������0
	if (!root) {
		return 0;
	}
	//���ڵ�ΪҶ�ӽڵ�ʱ������1
	if (!root->lchild && !root->rchild) {
		return 1;
	}
	//��ȡ������Ҷ������
	lleaf = bst_leaf_count_internal(root->lchild);
	//��ȡ������Ҷ������
	rleaf = bst_leaf_count_internal(root->rchild);
	//������Ҷ������
	return lleaf + rleaf;
}

/**
 *
 * ��ȡ���������Ҷ������
 *
 * @param bst �����������ַ
 *
 * @return Ҷ������
 *
 */
int bst_leaf_count(const BST* bst) {
	return bst_leaf_count_internal(bst->root);
}

/**
 *
 * �жϸö���������Ƿ�ΪAVL���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return �Ǹ�ʱ��ʾ�����߶ȣ�-1��ʾ��������AVL��
 *
 */
static int bst_is_avl_internal(const BSTNode* root) {
	int lheight, rheight;
	int diff;

	//���ڵ�ΪNULLʱ���߶�Ϊ0
	if (!root) {
		return 0;
	}
	//��ȡ�������߶�
	lheight = bst_is_avl_internal(root->lchild);
	//����������AVL��������Ҳ����AVL��
	if (lheight < 0) {
		return -1;
	}
	//��ȡ�������߶�
	rheight = bst_is_avl_internal(root->rchild);
	//����������AVL��������Ҳ����AVL��
	if (rheight < 0) {
		return -1;
	}
	//�߶Ȳ��1ʱ������AVL��
	diff = lheight >= rheight ? lheight - rheight : rheight - lheight;
	if (diff > 1) {
		return -1;
	}
	//����ΪAVL�������ظ����߶�
	return (lheight > rheight ? lheight : rheight) + 1;
}

/**
 *
 * �жϸö���������Ƿ�ΪAVL��
 *
 * @param bst �����������ַ
 *
 * @return TRUE ΪAVL��
 *         FALSE ��ΪAVL��
 *
 */
BOOL bst_is_avl(const BST* bst) {
	int height;

	//��ȡ�����߶�
	height = bst_is_avl_internal(bst->root);
	//С��0ʱ��ʾ��������AVL��
	return height < 0 ? FALSE : TRUE;
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
static int bst_key_cmp(const BSTKey* k1, const BSTKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * �ڶ���������в�����Լ����Ӧ��ֵ�ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�����Ѿ����ڣ�����ʧ��
 *
 */
static BOOL bst_insert_internal(BSTNode** root, const BSTKey* key,
		const BSTVal* val) {
	int res;

	//���ڵ�ΪNULLʱ�������ֵ��
	if (!*root) {
		//�½��ڵ�
		*root = (BSTNode*) malloc(sizeof(BSTNode));
		//�ڴ治��ʱ������
		if (!*root) {
			return FALSE;
		}
		//���Һ��ӽڵ�ΪNULL
		(*root)->lchild = (*root)->rchild = NULL;
		//���Ƽ�ֵ��
		(*root)->k = *key;
		(*root)->v = *val;
		return TRUE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	res = bst_key_cmp(key, &(*root)->k);
	if (res < 0) {
		//С�ڸýڵ��ʱ��������������
		return bst_insert_internal(&(*root)->lchild, key, val);
	} else if (res > 0) {
		//���ڸýڵ��ʱ��������������
		return bst_insert_internal(&(*root)->rchild, key, val);
	} else {
		//���ڸýڵ����ʧ��
		return FALSE;
	}
}

/**
 *
 * �ڶ���������в�����Լ����Ӧ��ֵ
 *
 * @param bst �����������ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�����Ѿ����ڣ�����ʧ��
 *
 */
BOOL bst_insert(BST* bst, const BSTKey* key, const BSTVal* val) {
	return bst_insert_internal(&bst->root, key, val);
}

/**
 *
 * �ڶ����������ɾ�����ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ɾ���ļ�
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
static BOOL bst_remove_internal(BSTNode** root, const BSTKey* key) {
	BSTNode *p, *q, *del;
	int res;

	//���ڵ�ΪNULLʱ���޸ü�������
	if (!*root) {
		return FALSE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	res = bst_key_cmp(key, &(*root)->k);
	if (res < 0) {
		//С�ڸýڵ��ʱ����������ɾ��
		return bst_remove_internal(&(*root)->lchild, key);
	} else if (res > 0) {
		//���ڸýڵ��ʱ����������ɾ��
		return bst_remove_internal(&(*root)->rchild, key);
	} else {
		//���ڸýڵ����ɾ���ýڵ�
		del = *root;
		if (!del->lchild) {
			//ɾ���ڵ�������ʱ�����ڵ�Ϊ���Һ���
			*root = del->rchild;
		} else if (!del->rchild) {
			//ɾ���ڵ����Һ���ʱ�����ڵ�Ϊ������
			*root = del->lchild;
		} else {
			//ɾ���ڵ������Һ��ӣ��������ӵ����ҽڵ����ɾ���ڵ㴦
			//pΪ���ҽڵ�ĸ��ڵ�
			p = del;
			//qΪ���ҽڵ�
			q = p->lchild;
			//Ѱ�����ҽڵ�
			while (q->rchild) {
				p = q;
				q = q->rchild;
			}
			//ɾ���ڵ�����Ӳ�Ϊ���ҽڵ�ʱ
			if (p != del) {
				//���ڵ���Һ���Ϊ���ҽڵ������
				p->rchild = q->lchild;
				//���ҽڵ������Ϊɾ���ڵ������
				q->lchild = del->lchild;
			}
			//���ҽڵ���Һ���Ϊɾ���ڵ���Һ���
			q->rchild = del->rchild;
			//���ڵ�Ϊ���ҽڵ�
			*root = q;
		}
		//�ͷ�ɾ���ڵ�
		free(del);
		return TRUE;
	}
}

/**
 *
 * �ڶ����������ɾ����
 *
 * @param bst �����������ַ
 *        key ɾ���ļ�
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
BOOL bst_remove(BST* bst, const BSTKey* key) {
	return bst_remove_internal(&bst->root, key);
}

/**
 *
 * �ڶ���������в��Ҽ��ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return TRUE ���ҳɹ�
 *         FALSE �������ڣ�����ʧ��
 *
 */
static BOOL bst_search_internal(BSTNode* root, const BSTKey* key, BSTVal** val) {
	int res;

	//���ڵ�ΪNULLʱ���޸ü�������
	if (!root) {
		return FALSE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	res = bst_key_cmp(key, &root->k);
	if (res < 0) {
		//С�ڸýڵ��ʱ��������������
		return bst_search_internal(root->lchild, key, val);
	} else if (res > 0) {
		//���ڸýڵ��ʱ��������������
		return bst_search_internal(root->rchild, key, val);
	} else {
		//���ڸýڵ��������ֵ��ָ��
		*val = &root->v;
		return TRUE;
	}
}

/**
 *
 * �ڶ���������в��Ҽ�
 *
 * @param bst �����������ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return TRUE ���ҳɹ�
 *         FALSE �������ڣ�����ʧ��
 *
 */
BOOL bst_search(const BST* bst, const BSTKey* key, BSTVal** val) {
	return bst_search_internal(bst->root, key, val);
}

/**
 *
 * ǰ���������������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void bst_preorder_internal(BSTNode* root,
		void (*visit)(const BSTKey* key, BSTVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
	//ǰ�����������
	bst_preorder_internal(root->lchild, visit);
	//ǰ�����������
	bst_preorder_internal(root->rchild, visit);
}

/**
 *
 * ǰ��������������
 *
 * @param bst �����������ַ
 *        visit ��������ָ��
 *
 */
void bst_preorder(const BST* bst, void (*visit)(const BSTKey* key, BSTVal* val)) {
	bst_preorder_internal(bst->root, visit);
}

/**
 *
 * �����������������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void bst_inorder_internal(BSTNode* root,
		void (*visit)(const BSTKey* key, BSTVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//�������������
	bst_inorder_internal(root->lchild, visit);
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
	//�������������
	bst_inorder_internal(root->rchild, visit);
}

/**
 *
 * ����������������
 *
 * @param bst �����������ַ
 *        visit ��������ָ��
 *
 */
void bst_inorder(const BST* bst, void (*visit)(const BSTKey* key, BSTVal* val)) {
	bst_inorder_internal(bst->root, visit);
}

/**
 *
 * �����������������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void bst_postorder_internal(BSTNode* root,
		void (*visit)(const BSTKey* key, BSTVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//�������������
	bst_postorder_internal(root->lchild, visit);
	//�������������
	bst_postorder_internal(root->rchild, visit);
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
}

/**
 *
 * ����������������
 *
 * @param bst �����������ַ
 *        visit ��������ָ��
 *
 */
void bst_postorder(const BST* bst,
		void (*visit)(const BSTKey* key, BSTVal* val)) {
	bst_postorder_internal(bst->root, visit);
}

/**
 *
 * ����������������
 *
 * @param bst �����������ַ
 *        visit ��������ָ��
 *
 */
void bst_level(const BST* bst, void (*visit)(const BSTKey* key, BSTVal* val)) {
	int size;
	BSTNode* p;
	BSTNode** q;
	int front, rear;

	//���ڵ�ΪNULLʱ������
	if (!bst->root) {
		return;
	}
	//��ȡ�ڵ���
	size = bst_size(bst);
	//����һ�����У�������ݽڵ��������ڴ�
	q = (BSTNode**) malloc(sizeof(BSTNode*) * size);
	//0λ��Ԫ��Ϊ���ڵ�
	q[0] = bst->root;
	//����λ��Ϊ0
	front = 0;
	//��βλ��Ϊ1
	rear = 1;
	//�����в�Ϊ��ʱ
	while (front < rear) {
		//��ȡ���׽ڵ�
		p = q[front++];
		//���Ӳ�Ϊ��ʱ���������β
		if (p->lchild) {
			q[rear++] = p->lchild;
		}
		//�Һ��Ӳ�Ϊ��ʱ���������β
		if (p->rchild) {
			q[rear++] = p->rchild;
		}
		//���ʶ��׽ڵ��ֵ��
		(*visit)(&p->k, &p->v);
	}
	//�ͷŶ���
	free(q);
}

/**
 *
 * ���������������޸ģ��������ƺ���
 *
 * @param key ��
 *        val ֵ
 *
 */
static void bst_visit(const BSTKey* key, BSTVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * ��ʾ�����������Ϣ�������޸�
 *
 * @param bst �����������ַ
 *
 */
void bst_show(const BST* bst) {
	printf("�ڵ�����%d\n", bst_size(bst));
	printf("�߶ȣ�%d\n", bst_height(bst));
	printf("Ҷ������%d\n", bst_leaf_count(bst));
	printf("�Ƿ�ΪAVL����%s\n", bst_is_avl(bst) ? "TRUE" : "FALSE");
	printf("ǰ����������\n");
	bst_preorder(bst, bst_visit);
	printf("������������\n");
	bst_inorder(bst, bst_visit);
	printf("������������\n");
	bst_postorder(bst, bst_visit);
	printf("������������\n");
	bst_level(bst, bst_visit);
	printf("\n");
}

int main() {
	BST b;
	BSTKey k;
	BSTVal v;
	BSTVal* vp;

	//�������������
	bst_create(&b);

	//�����ֵ��
	printf("�����ֵ�ԣ�\n");
	strcpy(k.key, "boy");
	v.val = 1;
	bst_insert(&b, &k, &v);
	bst_show(&b);

	strcpy(k.key, "apple");
	v.val = 2;
	bst_insert(&b, &k, &v);
	bst_show(&b);

	strcpy(k.key, "dog");
	v.val = 3;
	bst_insert(&b, &k, &v);
	bst_show(&b);

	strcpy(k.key, "cat");
	v.val = 4;
	bst_insert(&b, &k, &v);
	bst_show(&b);

	//��ȡ����������ֵ
	printf("��ȡ����������ֵ��\n");
	strcpy(k.key, "dog");
	bst_search(&b, &k, &vp);
	vp->val = 5;
	bst_show(&b);

	//ɾ����
	printf("ɾ������\n");
	bst_remove(&b, &k);
	bst_show(&b);

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	bst_clear(&b);
	bst_show(&b);

	//���ٶ��������
	bst_destroy(&b);

	return 0;
}
