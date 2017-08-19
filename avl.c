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

//����AVL������󳤶�
#define AVL_KEY_MAX_LEN	(32)

//����AVL�����������޸�
typedef struct AVLKey {
	char key[AVL_KEY_MAX_LEN];
} AVLKey;

//����AVL��ֵ�������޸�
typedef struct AVLVal {
	int val;
} AVLVal;

//����AVL���ڵ�
typedef struct AVLNode {
	AVLKey k;	//��
	AVLVal v;	//ֵ
	int balance;	//�ڵ��ƽ������
	struct AVLNode* lchild;	//���ӽڵ�
	struct AVLNode* rchild;	//�Һ��ӽڵ�
} AVLNode;

//����AVL��
typedef struct AVL {
	AVLNode* root;	//���ڵ�
} AVL;

/**
 *
 * ����AVL��
 *
 * @param avl AVL����ַ����AVL��δ������
 *
 */
void avl_create(AVL* avl) {
	//�ø��ڵ�ΪNULL
	avl->root = NULL;
}

/**
 *
 * ���AVL������Ԫ���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 */
static void avl_clear_internal(AVLNode** root) {
	//���ڵ�ΪNULLʱ���ݹ����
	if (!*root) {
		return;
	}
	//���������
	avl_clear_internal(&(*root)->lchild);
	//���������
	avl_clear_internal(&(*root)->rchild);
	//�ͷŸ��ڵ�
	free(*root);
	//���ڵ���ΪNULL
	*root = NULL;
}

/**
 *
 * ���AVL������Ԫ��
 *
 * @param avl AVL����ַ
 *
 */
void avl_clear(AVL* avl) {
	avl_clear_internal(&avl->root);
}

/**
 *
 * ����AVL��
 *
 * @param avl AVL����ַ
 *
 */
void avl_destroy(AVL* avl) {
	//���AVL������Ԫ��
	avl_clear(avl);
}

/**
 *
 * ��ȡAVL���ڵ������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return �ڵ�����
 *
 */
static int avl_size_internal(const AVLNode* root) {
	int lsize, rsize;

	//���ڵ�ΪNULLʱ������0
	if (!root) {
		return 0;
	}
	//��ȡ�������ڵ�����
	lsize = avl_size_internal(root->lchild);
	//��ȡ�������ڵ�����
	rsize = avl_size_internal(root->rchild);
	//���ؽڵ�����
	return lsize + rsize + 1;
}

/**
 *
 * ��ȡAVL���ڵ�����
 *
 * @param avl AVL����ַ
 *
 * @return �ڵ�����
 *
 */
int avl_size(const AVL* avl) {
	return avl_size_internal(avl->root);
}

/**
 *
 * ��ȡAVL���߶��ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return �߶�
 *
 */
static int avl_height_internal(const AVLNode* root) {
	int lheight, rheight;

	//���ڵ�ΪNULLʱ������0
	if (!root) {
		return 0;
	}
	//��ȡ�������߶�
	lheight = avl_height_internal(root->lchild);
	//��ȡ�������߶�
	rheight = avl_height_internal(root->rchild);
	//���߶�Ϊ���������ϸ���+1
	return (lheight > rheight ? lheight : rheight) + 1;
}

/**
 *
 * ��ȡAVL���߶�
 *
 * @param avl AVL����ַ
 *
 * @return �߶�
 *
 */
int avl_height(const AVL* avl) {
	return avl_height_internal(avl->root);
}

/**
 *
 * ��ȡAVL��Ҷ�������ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *
 * @return Ҷ������
 *
 */
static int avl_leaf_count_internal(const AVLNode* root) {
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
	lleaf = avl_leaf_count_internal(root->lchild);
	//��ȡ������Ҷ������
	rleaf = avl_leaf_count_internal(root->rchild);
	//������Ҷ������
	return lleaf + rleaf;
}

/**
 *
 * ��ȡAVL��Ҷ������
 *
 * @param avl AVL����ַ
 *
 * @return Ҷ������
 *
 */
int avl_leaf_count(const AVL* avl) {
	return avl_leaf_count_internal(avl->root);
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
static int avl_key_cmp(const AVLKey* k1, const AVLKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * �Ը��ڵ�����
 *
 * @param root ���ڵ��ַ
 *
 */
static void avl_rotate_left(AVLNode** root) {
	AVLNode* p;

	p = *root;
	*root = p->rchild;
	p->rchild = (*root)->lchild;
	(*root)->lchild = p;
}

/**
 *
 * �Ը��ڵ�����
 *
 * @param root ���ڵ��ַ
 *
 */
static void avl_rotate_right(AVLNode** root) {
	AVLNode* p;

	p = *root;
	*root = p->lchild;
	p->lchild = (*root)->rchild;
	(*root)->rchild = p;
}

/**
 *
 * ��AVL���в�����Լ����Ӧ��ֵ�ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *        taller ����ڵ�������Ƿ���
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�����Ѿ����ڣ�����ʧ��
 *
 */
static BOOL avl_insert_internal(AVLNode** root, const AVLKey* key,
		const AVLVal* val, BOOL* taller) {
	int cmp_res;
	BOOL in_res;

	//���ڵ�ΪNULLʱ�������ֵ��
	if (!*root) {
		//�½��ڵ�
		*root = (AVLNode*) malloc(sizeof(AVLNode));
		//�ڴ治��ʱ������
		if (!*root) {
			*taller = FALSE;
			return FALSE;
		}
		//���Һ��ӽڵ�ΪNULL
		(*root)->lchild = (*root)->rchild = NULL;
		//���Ƽ�ֵ��
		(*root)->k = *key;
		(*root)->v = *val;
		//ƽ��������0
		(*root)->balance = 0;
		//�������
		*taller = TRUE;
		return TRUE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	cmp_res = avl_key_cmp(key, &(*root)->k);
	if (cmp_res < 0) {
		//С�ڸýڵ��ʱ��������������
		in_res = avl_insert_internal(&(*root)->lchild, key, val, taller);
		//�������ʱ
		if (*taller) {
			//���ݸ��ڵ�ƽ�����ӽ��в���
			switch ((*root)->balance) {
			case -1:	//��������
				//ƽ��������0
				(*root)->balance = 0;
				//�������߽���
				*taller = FALSE;
				break;
			case 1:		//��������
				//�������߽���
				*taller = FALSE;
				//����������ƽ�����ӽ�����ת
				if ((*root)->lchild->balance == 1) {
					//����������������
					(*root)->balance = 0;
					(*root)->lchild->balance = 0;
					//�������ڵ�
					avl_rotate_right(root);
				} else {
					//����������������
					//�����������Һ��ӵ�ƽ����������ƽ������
					switch ((*root)->lchild->rchild->balance) {
					case 1:		//��������
						(*root)->lchild->balance = 0;
						(*root)->balance = -1;
						break;
					case -1:	//��������
						(*root)->lchild->balance = 1;
						(*root)->balance = 0;
						break;
					default:	//һ����
						(*root)->lchild->balance = 0;
						(*root)->balance = 0;
						break;
					}
					(*root)->lchild->rchild->balance = 0;
					//��������
					avl_rotate_left(&(*root)->lchild);
					//�������ڵ�
					avl_rotate_right(root);
				}
				break;
			default:	//һ����
				(*root)->balance = 1;
				break;
			}
		}
	} else if (cmp_res > 0) {
		//���ڸýڵ��ʱ��������������
		in_res = avl_insert_internal(&(*root)->rchild, key, val, taller);
		//�������ʱ
		if (*taller) {
			//���ݸ��ڵ�ƽ�����ӽ��в���
			switch ((*root)->balance) {
			case -1:	//��������
				//�������߽���
				*taller = FALSE;
				//����������ƽ�����ӽ�����ת
				if ((*root)->rchild->balance == -1) {
					//����������������
					(*root)->balance = 0;
					(*root)->rchild->balance = 0;
					//�������ڵ�
					avl_rotate_left(root);
				} else {
					//����������������
					//�������������ӵ�ƽ����������ƽ������
					switch ((*root)->rchild->lchild->balance) {
					case 1:		//��������
						(*root)->rchild->balance = -1;
						(*root)->balance = 0;
						break;
					case -1:	//��������
						(*root)->rchild->balance = 0;
						(*root)->balance = 1;
						break;
					default:	//һ����
						(*root)->rchild->balance = 0;
						(*root)->balance = 0;
						break;
					}
					(*root)->rchild->lchild->balance = 0;
					//�����Һ���
					avl_rotate_right(&(*root)->rchild);
					//�������ڵ�
					avl_rotate_left(root);
				}
				break;
			case 1:		//��������
				//ƽ��������0
				(*root)->balance = 0;
				//�������߽���
				*taller = FALSE;
				break;
			default:	//һ����
				(*root)->balance = -1;
				break;
			}
		}
	} else {
		//���ڸýڵ����ʧ��
		*taller = FALSE;
		in_res = FALSE;
	}
	return in_res;
}

/**
 *
 * ��AVL���в�����Լ����Ӧ��ֵ
 *
 * @param avl AVL����ַ
 *        key ����ļ�
 *        val ����Ӧ��ֵ
 *
 * @return TRUE ����ɹ�
 *         FALSE �ڴ治�����Ѿ����ڣ�����ʧ��
 *
 */
BOOL avl_insert(AVL* avl, const AVLKey* key, const AVLVal* val) {
	BOOL taller;
	return avl_insert_internal(&avl->root, key, val, &taller);
}

/**
 *
 * �������䰫�󣬵�����Ӧ�ڵ�
 *
 * @param root ���ڵ��ַ
 *        shorter ɾ���ڵ�������Ƿ�䰫
 *
 */
static void avl_remove_shorter_left(AVLNode** root, BOOL* shorter) {
	//���ݸ��ڵ�ƽ�����ӽ��в���
	switch ((*root)->balance) {
	case 1:		//��������
		(*root)->balance = 0;
		break;
	case -1:	//��������
		//�����Һ���ƽ�����ӽ�����ת����
		switch ((*root)->rchild->balance) {
		case 1:		//��������
			//�������������ӵ�ƽ����������ƽ������
			switch ((*root)->rchild->lchild->balance) {
			case 1:		//��������
				(*root)->rchild->balance = -1;
				(*root)->balance = 0;
				break;
			case -1:	//��������
				(*root)->rchild->balance = 0;
				(*root)->balance = 1;
				break;
			default:	//һ����
				(*root)->rchild->balance = 0;
				(*root)->balance = 0;
				break;
			}
			(*root)->rchild->lchild->balance = 0;
			//�����Һ���
			avl_rotate_right(&(*root)->rchild);
			//�������ڵ�
			avl_rotate_left(root);
			break;
		case -1:	//��������
			(*root)->rchild->balance = 0;
			(*root)->balance = 0;
			//�������ڵ�
			avl_rotate_left(root);
			break;
		default:	//һ����
			(*root)->rchild->balance = 1;
			//�������ڵ�
			avl_rotate_left(root);
			//ֹͣ�䰫
			*shorter = FALSE;
			break;
		}
		break;
	default:	//һ����
		(*root)->balance = -1;
		//ֹͣ�䰫
		*shorter = FALSE;
		break;
	}
}

/**
 *
 * �������䰫�󣬵�����Ӧ�ڵ�
 *
 * @param root ���ڵ��ַ
 *        shorter ɾ���ڵ�������Ƿ�䰫
 *
 */
static void avl_remove_shorter_right(AVLNode** root, BOOL* shorter) {
	//���ݸ��ڵ�ƽ�����ӽ��в���
	switch ((*root)->balance) {
	case 1:		//��������
		//��������ƽ�����ӽ�����ת����
		switch ((*root)->lchild->balance) {
		case 1:		//��������
			(*root)->lchild->balance = 0;
			(*root)->balance = 0;
			//�������ڵ�
			avl_rotate_right(root);
			break;
		case -1:	//��������
			//�����������Һ��ӵ�ƽ����������ƽ������
			switch ((*root)->lchild->rchild->balance) {
			case 1:		//��������
				(*root)->lchild->balance = 0;
				(*root)->balance = -1;
				break;
			case -1:	//��������
				(*root)->lchild->balance = 1;
				(*root)->balance = 0;
				break;
			default:	//һ����
				(*root)->lchild->balance = 0;
				(*root)->balance = 0;
				break;
			}
			(*root)->lchild->rchild->balance = 0;
			//��������
			avl_rotate_left(&(*root)->lchild);
			//�������ڵ�
			avl_rotate_right(root);
			break;
		default:	//һ����
			(*root)->lchild->balance = -1;
			//�������ڵ�
			avl_rotate_right(root);
			//ֹͣ�䰫
			*shorter = FALSE;
			break;
		}
		break;
	case -1:	//��������
		(*root)->balance = 0;
		break;
	default:	//һ����
		(*root)->balance = 1;
		//ֹͣ�䰫
		*shorter = FALSE;
		break;
	}
}

/**
 *
 * ��AVL����ɾ�����ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ɾ���ļ�
 *        shorter ɾ���ڵ�������Ƿ�䰫
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
static BOOL avl_remove_internal(AVLNode** root, const AVLKey* key,
		BOOL* shorter) {
	AVLNode* p;
	int cmp_res;
	BOOL rm_res;

	//���ڵ�ΪNULLʱ���޸ü�������
	if (!*root) {
		*shorter = FALSE;
		return FALSE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	cmp_res = avl_key_cmp(key, &(*root)->k);
	if (cmp_res < 0) {
		//С�ڸýڵ��ʱ����������ɾ��
		rm_res = avl_remove_internal(&(*root)->lchild, key, shorter);
		//�������䰫ʱ�����е���
		if (*shorter) {
			avl_remove_shorter_left(root, shorter);
		}
	} else if (cmp_res > 0) {
		//���ڸýڵ��ʱ����������ɾ��
		rm_res = avl_remove_internal(&(*root)->rchild, key, shorter);
		//�������䰫ʱ�����е���
		if (*shorter) {
			avl_remove_shorter_right(root, shorter);
		}
	} else {
		//���ڸýڵ����ɾ���ɹ�
		rm_res = TRUE;
		//�жϸýڵ����޺��ӽڵ�
		if (!(*root)->lchild) {
			//������ʱ�����ڵ�Ϊ�Һ��ӣ���ɾ���ýڵ�
			p = *root;
			*root = p->rchild;
			free(p);
			//�����䰫
			*shorter = TRUE;
		} else if (!(*root)->rchild) {
			//���Һ���ʱ�����ڵ�Ϊ���ӣ���ɾ���ýڵ�
			p = *root;
			*root = p->lchild;
			free(p);
			//�����䰫
			*shorter = TRUE;
		} else {
			//�����Һ��ӣ����ݸ��ڵ�ƽ������ѡ���滻�ڵ�
			if ((*root)->balance == 1) {
				//��������ʱ���滻�ڵ�Ϊ�����������ҽڵ�
				p = (*root)->lchild;
				while (p->rchild) {
					p = p->rchild;
				}
				//���Ƽ�ֵ�����ڵ�
				(*root)->k = p->k;
				(*root)->v = p->v;
				//��������ɾ���滻�ڵ�
				avl_remove_internal(&(*root)->lchild, &p->k, shorter);
				//�������䰫ʱ�����е���
				if (*shorter) {
					avl_remove_shorter_left(root, shorter);
				}
			} else {
				//�������߻�ͬ����ʱ���滻�ڵ�Ϊ������������ڵ�
				p = (*root)->rchild;
				while (p->lchild) {
					p = p->lchild;
				}
				//���Ƽ�ֵ�����ڵ�
				(*root)->k = p->k;
				(*root)->v = p->v;
				//��������ɾ���滻�ڵ�
				avl_remove_internal(&(*root)->rchild, &p->k, shorter);
				//�������䰫ʱ�����е���
				if (*shorter) {
					avl_remove_shorter_right(root, shorter);
				}
			}
		}
	}
	return rm_res;
}

/**
 *
 * ��AVL����ɾ����
 *
 * @param avl AVL����ַ
 *        key ɾ���ļ�
 *
 * @return TRUE ɾ���ɹ�
 *         FALSE �������ڣ�ɾ��ʧ��
 *
 */
BOOL avl_remove(AVL* avl, const AVLKey* key) {
	BOOL shorter;
	return avl_remove_internal(&avl->root, key, &shorter);
}

/**
 *
 * ��AVL���в��Ҽ��ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return TRUE ���ҳɹ�
 *         FALSE �������ڣ�����ʧ��
 *
 */
static BOOL avl_search_internal(AVLNode* root, const AVLKey* key, AVLVal** val) {
	int res;

	//���ڵ�ΪNULLʱ���޸ü�������
	if (!root) {
		return FALSE;
	}
	//�Ƚϼ��͸ýڵ���Ĵ�С
	res = avl_key_cmp(key, &root->k);
	if (res < 0) {
		//С�ڸýڵ��ʱ��������������
		return avl_search_internal(root->lchild, key, val);
	} else if (res > 0) {
		//���ڸýڵ��ʱ��������������
		return avl_search_internal(root->rchild, key, val);
	} else {
		//���ڸýڵ��������ֵ��ָ��
		*val = &root->v;
		return TRUE;
	}
}

/**
 *
 * ��AVL���в��Ҽ�
 *
 * @param avl AVL����ַ
 *        key ���ҵļ�
 *        val �������Ӧֵ��ָ��
 *
 * @return TRUE ���ҳɹ�
 *         FALSE �������ڣ�����ʧ��
 *
 */
BOOL avl_search(const AVL* avl, const AVLKey* key, AVLVal** val) {
	return avl_search_internal(avl->root, key, val);
}

/**
 *
 * ǰ�����AVL���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void avl_preorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
	//ǰ�����������
	avl_preorder_internal(root->lchild, visit);
	//ǰ�����������
	avl_preorder_internal(root->rchild, visit);
}

/**
 *
 * ǰ�����AVL��
 *
 * @param avl AVL����ַ
 *        visit ��������ָ��
 *
 */
void avl_preorder(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_preorder_internal(avl->root, visit);
}

/**
 *
 * �������AVL���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void avl_inorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//�������������
	avl_inorder_internal(root->lchild, visit);
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
	//�������������
	avl_inorder_internal(root->rchild, visit);
}

/**
 *
 * �������AVL��
 *
 * @param avl AVL����ַ
 *        visit ��������ָ��
 *
 */
void avl_inorder(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_inorder_internal(avl->root, visit);
}

/**
 *
 * �������AVL���ڲ��ݹ麯��
 *
 * @param root ���ڵ��ַ
 *        visit ��������ָ��
 *
 */
static void avl_postorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//���ڵ�ΪNULLʱ������
	if (!root) {
		return;
	}
	//�������������
	avl_postorder_internal(root->lchild, visit);
	//�������������
	avl_postorder_internal(root->rchild, visit);
	//���ʸ��ڵ��ֵ��
	(*visit)(&root->k, &root->v);
}

/**
 *
 * �������AVL��
 *
 * @param avl AVL����ַ
 *        visit ��������ָ��
 *
 */
void avl_postorder(const AVL* avl,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_postorder_internal(avl->root, visit);
}

/**
 *
 * �������AVL��
 *
 * @param avl AVL����ַ
 *        visit ��������ָ��
 *
 */
void avl_level(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	int size;
	AVLNode* p;
	AVLNode** q;
	int front, rear;

	//���ڵ�ΪNULLʱ������
	if (!avl->root) {
		return;
	}
	//��ȡ�ڵ���
	size = avl_size(avl);
	//����һ�����У�������ݽڵ��������ڴ�
	q = (AVLNode**) malloc(sizeof(AVLNode*) * size);
	//0λ��Ԫ��Ϊ���ڵ�
	q[0] = avl->root;
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
static void avl_visit(const AVLKey* key, AVLVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * ��ʾAVL����Ϣ�������޸�
 *
 * @param avl AVL����ַ
 *
 */
void avl_show(const AVL* avl) {
	printf("�ڵ�����%d\n", avl_size(avl));
	printf("�߶ȣ�%d\n", avl_height(avl));
	printf("Ҷ������%d\n", avl_leaf_count(avl));
	printf("ǰ����������\n");
	avl_preorder(avl, avl_visit);
	printf("������������\n");
	avl_inorder(avl, avl_visit);
	printf("������������\n");
	avl_postorder(avl, avl_visit);
	printf("������������\n");
	avl_level(avl, avl_visit);
	printf("\n");
}

int main() {
	AVL a;
	AVLKey k;
	AVLVal v;
	AVLVal* vp;

	//����AVL��
	avl_create(&a);

	//�����ֵ��
	printf("�����ֵ�ԣ�\n");
	strcpy(k.key, "a");
	v.val = 1;
	avl_insert(&a, &k, &v);
	avl_show(&a);

	strcpy(k.key, "b");
	v.val = 2;
	avl_insert(&a, &k, &v);
	avl_show(&a);

	strcpy(k.key, "c");
	v.val = 3;
	avl_insert(&a, &k, &v);
	avl_show(&a);

	strcpy(k.key, "d");
	v.val = 4;
	avl_insert(&a, &k, &v);
	avl_show(&a);

	strcpy(k.key, "e");
	v.val = 6;
	avl_insert(&a, &k, &v);
	avl_show(&a);

	//��ȡ����������ֵ
	printf("��ȡ����������ֵ��\n");
	strcpy(k.key, "c");
	avl_search(&a, &k, &vp);
	vp->val = 5;
	avl_show(&a);

	//ɾ����
	printf("ɾ������\n");
	strcpy(k.key, "d");
	avl_remove(&a, &k);
	avl_show(&a);

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	avl_clear(&a);
	avl_show(&a);

	//����AVL��
	avl_destroy(&a);

	return 0;
}
