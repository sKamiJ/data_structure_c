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

//定义AVL树键最大长度
#define AVL_KEY_MAX_LEN	(32)

//定义AVL树键，可以修改
typedef struct AVLKey {
	char key[AVL_KEY_MAX_LEN];
} AVLKey;

//定义AVL树值，可以修改
typedef struct AVLVal {
	int val;
} AVLVal;

//定义AVL树节点
typedef struct AVLNode {
	AVLKey k;	//键
	AVLVal v;	//值
	int balance;	//节点的平衡因子
	struct AVLNode* lchild;	//左孩子节点
	struct AVLNode* rchild;	//右孩子节点
} AVLNode;

//定义AVL树
typedef struct AVL {
	AVLNode* root;	//根节点
} AVL;

/**
 *
 * 创建AVL树
 *
 * @param avl AVL树地址，该AVL树未被创建
 *
 */
void avl_create(AVL* avl) {
	//置根节点为NULL
	avl->root = NULL;
}

/**
 *
 * 清除AVL树所有元素内部递归函数
 *
 * @param root 根节点地址
 *
 */
static void avl_clear_internal(AVLNode** root) {
	//根节点为NULL时，递归结束
	if (!*root) {
		return;
	}
	//清除左子树
	avl_clear_internal(&(*root)->lchild);
	//清除右子树
	avl_clear_internal(&(*root)->rchild);
	//释放根节点
	free(*root);
	//根节点置为NULL
	*root = NULL;
}

/**
 *
 * 清除AVL树所有元素
 *
 * @param avl AVL树地址
 *
 */
void avl_clear(AVL* avl) {
	avl_clear_internal(&avl->root);
}

/**
 *
 * 销毁AVL树
 *
 * @param avl AVL树地址
 *
 */
void avl_destroy(AVL* avl) {
	//清除AVL树所有元素
	avl_clear(avl);
}

/**
 *
 * 获取AVL树节点数量内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 节点数量
 *
 */
static int avl_size_internal(const AVLNode* root) {
	int lsize, rsize;

	//根节点为NULL时，返回0
	if (!root) {
		return 0;
	}
	//获取左子树节点数量
	lsize = avl_size_internal(root->lchild);
	//获取右子树节点数量
	rsize = avl_size_internal(root->rchild);
	//返回节点数量
	return lsize + rsize + 1;
}

/**
 *
 * 获取AVL树节点数量
 *
 * @param avl AVL树地址
 *
 * @return 节点数量
 *
 */
int avl_size(const AVL* avl) {
	return avl_size_internal(avl->root);
}

/**
 *
 * 获取AVL树高度内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 高度
 *
 */
static int avl_height_internal(const AVLNode* root) {
	int lheight, rheight;

	//根节点为NULL时，返回0
	if (!root) {
		return 0;
	}
	//获取左子树高度
	lheight = avl_height_internal(root->lchild);
	//获取右子树高度
	rheight = avl_height_internal(root->rchild);
	//树高度为左右子树较高者+1
	return (lheight > rheight ? lheight : rheight) + 1;
}

/**
 *
 * 获取AVL树高度
 *
 * @param avl AVL树地址
 *
 * @return 高度
 *
 */
int avl_height(const AVL* avl) {
	return avl_height_internal(avl->root);
}

/**
 *
 * 获取AVL树叶子数量内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 叶子数量
 *
 */
static int avl_leaf_count_internal(const AVLNode* root) {
	int lleaf, rleaf;

	//根节点为NULL时，返回0
	if (!root) {
		return 0;
	}
	//根节点为叶子节点时，返回1
	if (!root->lchild && !root->rchild) {
		return 1;
	}
	//获取左子树叶子数量
	lleaf = avl_leaf_count_internal(root->lchild);
	//获取右子树叶子数量
	rleaf = avl_leaf_count_internal(root->rchild);
	//返回总叶子数量
	return lleaf + rleaf;
}

/**
 *
 * 获取AVL树叶子数量
 *
 * @param avl AVL树地址
 *
 * @return 叶子数量
 *
 */
int avl_leaf_count(const AVL* avl) {
	return avl_leaf_count_internal(avl->root);
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
static int avl_key_cmp(const AVLKey* k1, const AVLKey* k2) {
	return strcmp(k1->key, k2->key);
}

/**
 *
 * 对根节点左旋
 *
 * @param root 根节点地址
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
 * 对根节点右旋
 *
 * @param root 根节点地址
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
 * 在AVL树中插入键以及其对应的值内部递归函数
 *
 * @param root 根节点地址
 *        key 插入的键
 *        val 键对应的值
 *        taller 插入节点后，子树是否变高
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足或键已经存在，插入失败
 *
 */
static BOOL avl_insert_internal(AVLNode** root, const AVLKey* key,
		const AVLVal* val, BOOL* taller) {
	int cmp_res;
	BOOL in_res;

	//根节点为NULL时，插入键值对
	if (!*root) {
		//新建节点
		*root = (AVLNode*) malloc(sizeof(AVLNode));
		//内存不足时，错误
		if (!*root) {
			*taller = FALSE;
			return FALSE;
		}
		//左右孩子节点为NULL
		(*root)->lchild = (*root)->rchild = NULL;
		//复制键值对
		(*root)->k = *key;
		(*root)->v = *val;
		//平衡因子置0
		(*root)->balance = 0;
		//子树变高
		*taller = TRUE;
		return TRUE;
	}
	//比较键和该节点键的大小
	cmp_res = avl_key_cmp(key, &(*root)->k);
	if (cmp_res < 0) {
		//小于该节点键时，在左子树插入
		in_res = avl_insert_internal(&(*root)->lchild, key, val, taller);
		//子树变高时
		if (*taller) {
			//根据根节点平衡因子进行操作
			switch ((*root)->balance) {
			case -1:	//右子树高
				//平衡因子置0
				(*root)->balance = 0;
				//子树增高结束
				*taller = FALSE;
				break;
			case 1:		//左子树高
				//子树增高结束
				*taller = FALSE;
				//根据左子树平衡因子进行旋转
				if ((*root)->lchild->balance == 1) {
					//左子树的左子树高
					(*root)->balance = 0;
					(*root)->lchild->balance = 0;
					//右旋根节点
					avl_rotate_right(root);
				} else {
					//左子树的右子树高
					//根据左子树右孩子的平衡因子重置平衡因子
					switch ((*root)->lchild->rchild->balance) {
					case 1:		//左子树高
						(*root)->lchild->balance = 0;
						(*root)->balance = -1;
						break;
					case -1:	//右子树高
						(*root)->lchild->balance = 1;
						(*root)->balance = 0;
						break;
					default:	//一样高
						(*root)->lchild->balance = 0;
						(*root)->balance = 0;
						break;
					}
					(*root)->lchild->rchild->balance = 0;
					//左旋左孩子
					avl_rotate_left(&(*root)->lchild);
					//右旋根节点
					avl_rotate_right(root);
				}
				break;
			default:	//一样高
				(*root)->balance = 1;
				break;
			}
		}
	} else if (cmp_res > 0) {
		//大于该节点键时，在右子树插入
		in_res = avl_insert_internal(&(*root)->rchild, key, val, taller);
		//子树变高时
		if (*taller) {
			//根据根节点平衡因子进行操作
			switch ((*root)->balance) {
			case -1:	//右子树高
				//子树增高结束
				*taller = FALSE;
				//根据右子树平衡因子进行旋转
				if ((*root)->rchild->balance == -1) {
					//右子树的右子树高
					(*root)->balance = 0;
					(*root)->rchild->balance = 0;
					//左旋根节点
					avl_rotate_left(root);
				} else {
					//右子树的左子树高
					//根据右子树左孩子的平衡因子重置平衡因子
					switch ((*root)->rchild->lchild->balance) {
					case 1:		//左子树高
						(*root)->rchild->balance = -1;
						(*root)->balance = 0;
						break;
					case -1:	//右子树高
						(*root)->rchild->balance = 0;
						(*root)->balance = 1;
						break;
					default:	//一样高
						(*root)->rchild->balance = 0;
						(*root)->balance = 0;
						break;
					}
					(*root)->rchild->lchild->balance = 0;
					//右旋右孩子
					avl_rotate_right(&(*root)->rchild);
					//左旋根节点
					avl_rotate_left(root);
				}
				break;
			case 1:		//左子树高
				//平衡因子置0
				(*root)->balance = 0;
				//子树增高结束
				*taller = FALSE;
				break;
			default:	//一样高
				(*root)->balance = -1;
				break;
			}
		}
	} else {
		//等于该节点键，失败
		*taller = FALSE;
		in_res = FALSE;
	}
	return in_res;
}

/**
 *
 * 在AVL树中插入键以及其对应的值
 *
 * @param avl AVL树地址
 *        key 插入的键
 *        val 键对应的值
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足或键已经存在，插入失败
 *
 */
BOOL avl_insert(AVL* avl, const AVLKey* key, const AVLVal* val) {
	BOOL taller;
	return avl_insert_internal(&avl->root, key, val, &taller);
}

/**
 *
 * 左子树变矮后，调整相应节点
 *
 * @param root 根节点地址
 *        shorter 删除节点后，子树是否变矮
 *
 */
static void avl_remove_shorter_left(AVLNode** root, BOOL* shorter) {
	//根据根节点平衡因子进行操作
	switch ((*root)->balance) {
	case 1:		//左子树高
		(*root)->balance = 0;
		break;
	case -1:	//右子树高
		//根据右孩子平衡因子进行旋转操作
		switch ((*root)->rchild->balance) {
		case 1:		//左子树高
			//根据右子树左孩子的平衡因子重置平衡因子
			switch ((*root)->rchild->lchild->balance) {
			case 1:		//左子树高
				(*root)->rchild->balance = -1;
				(*root)->balance = 0;
				break;
			case -1:	//右子树高
				(*root)->rchild->balance = 0;
				(*root)->balance = 1;
				break;
			default:	//一样高
				(*root)->rchild->balance = 0;
				(*root)->balance = 0;
				break;
			}
			(*root)->rchild->lchild->balance = 0;
			//右旋右孩子
			avl_rotate_right(&(*root)->rchild);
			//左旋根节点
			avl_rotate_left(root);
			break;
		case -1:	//右子树高
			(*root)->rchild->balance = 0;
			(*root)->balance = 0;
			//左旋根节点
			avl_rotate_left(root);
			break;
		default:	//一样高
			(*root)->rchild->balance = 1;
			//左旋根节点
			avl_rotate_left(root);
			//停止变矮
			*shorter = FALSE;
			break;
		}
		break;
	default:	//一样高
		(*root)->balance = -1;
		//停止变矮
		*shorter = FALSE;
		break;
	}
}

/**
 *
 * 右子树变矮后，调整相应节点
 *
 * @param root 根节点地址
 *        shorter 删除节点后，子树是否变矮
 *
 */
static void avl_remove_shorter_right(AVLNode** root, BOOL* shorter) {
	//根据根节点平衡因子进行操作
	switch ((*root)->balance) {
	case 1:		//左子树高
		//根据左孩子平衡因子进行旋转操作
		switch ((*root)->lchild->balance) {
		case 1:		//左子树高
			(*root)->lchild->balance = 0;
			(*root)->balance = 0;
			//右旋根节点
			avl_rotate_right(root);
			break;
		case -1:	//右子树高
			//根据左子树右孩子的平衡因子重置平衡因子
			switch ((*root)->lchild->rchild->balance) {
			case 1:		//左子树高
				(*root)->lchild->balance = 0;
				(*root)->balance = -1;
				break;
			case -1:	//右子树高
				(*root)->lchild->balance = 1;
				(*root)->balance = 0;
				break;
			default:	//一样高
				(*root)->lchild->balance = 0;
				(*root)->balance = 0;
				break;
			}
			(*root)->lchild->rchild->balance = 0;
			//左旋左孩子
			avl_rotate_left(&(*root)->lchild);
			//右旋根节点
			avl_rotate_right(root);
			break;
		default:	//一样高
			(*root)->lchild->balance = -1;
			//右旋根节点
			avl_rotate_right(root);
			//停止变矮
			*shorter = FALSE;
			break;
		}
		break;
	case -1:	//右子树高
		(*root)->balance = 0;
		break;
	default:	//一样高
		(*root)->balance = 1;
		//停止变矮
		*shorter = FALSE;
		break;
	}
}

/**
 *
 * 在AVL树中删除键内部递归函数
 *
 * @param root 根节点地址
 *        key 删除的键
 *        shorter 删除节点后，子树是否变矮
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
static BOOL avl_remove_internal(AVLNode** root, const AVLKey* key,
		BOOL* shorter) {
	AVLNode* p;
	int cmp_res;
	BOOL rm_res;

	//根节点为NULL时，无该键，错误
	if (!*root) {
		*shorter = FALSE;
		return FALSE;
	}
	//比较键和该节点键的大小
	cmp_res = avl_key_cmp(key, &(*root)->k);
	if (cmp_res < 0) {
		//小于该节点键时，在左子树删除
		rm_res = avl_remove_internal(&(*root)->lchild, key, shorter);
		//左子树变矮时，进行调整
		if (*shorter) {
			avl_remove_shorter_left(root, shorter);
		}
	} else if (cmp_res > 0) {
		//大于该节点键时，在右子树删除
		rm_res = avl_remove_internal(&(*root)->rchild, key, shorter);
		//右子树变矮时，进行调整
		if (*shorter) {
			avl_remove_shorter_right(root, shorter);
		}
	} else {
		//等于该节点键，删除成功
		rm_res = TRUE;
		//判断该节点有无孩子节点
		if (!(*root)->lchild) {
			//无左孩子时，根节点为右孩子，并删除该节点
			p = *root;
			*root = p->rchild;
			free(p);
			//子树变矮
			*shorter = TRUE;
		} else if (!(*root)->rchild) {
			//无右孩子时，根节点为左孩子，并删除该节点
			p = *root;
			*root = p->lchild;
			free(p);
			//子树变矮
			*shorter = TRUE;
		} else {
			//有左右孩子，根据根节点平衡因子选择替换节点
			if ((*root)->balance == 1) {
				//左子树高时，替换节点为左子树的最右节点
				p = (*root)->lchild;
				while (p->rchild) {
					p = p->rchild;
				}
				//复制键值到根节点
				(*root)->k = p->k;
				(*root)->v = p->v;
				//在左子树删除替换节点
				avl_remove_internal(&(*root)->lchild, &p->k, shorter);
				//左子树变矮时，进行调整
				if (*shorter) {
					avl_remove_shorter_left(root, shorter);
				}
			} else {
				//右子树高或同样高时，替换节点为右子树的最左节点
				p = (*root)->rchild;
				while (p->lchild) {
					p = p->lchild;
				}
				//复制键值到根节点
				(*root)->k = p->k;
				(*root)->v = p->v;
				//在右子树删除替换节点
				avl_remove_internal(&(*root)->rchild, &p->k, shorter);
				//右子树变矮时，进行调整
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
 * 在AVL树中删除键
 *
 * @param avl AVL树地址
 *        key 删除的键
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
BOOL avl_remove(AVL* avl, const AVLKey* key) {
	BOOL shorter;
	return avl_remove_internal(&avl->root, key, &shorter);
}

/**
 *
 * 在AVL树中查找键内部递归函数
 *
 * @param root 根节点地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return TRUE 查找成功
 *         FALSE 键不存在，查找失败
 *
 */
static BOOL avl_search_internal(AVLNode* root, const AVLKey* key, AVLVal** val) {
	int res;

	//根节点为NULL时，无该键，错误
	if (!root) {
		return FALSE;
	}
	//比较键和该节点键的大小
	res = avl_key_cmp(key, &root->k);
	if (res < 0) {
		//小于该节点键时，在左子树查找
		return avl_search_internal(root->lchild, key, val);
	} else if (res > 0) {
		//大于该节点键时，在右子树查找
		return avl_search_internal(root->rchild, key, val);
	} else {
		//等于该节点键，保存值的指针
		*val = &root->v;
		return TRUE;
	}
}

/**
 *
 * 在AVL树中查找键
 *
 * @param avl AVL树地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return TRUE 查找成功
 *         FALSE 键不存在，查找失败
 *
 */
BOOL avl_search(const AVL* avl, const AVLKey* key, AVLVal** val) {
	return avl_search_internal(avl->root, key, val);
}

/**
 *
 * 前序遍历AVL树内部递归函数
 *
 * @param root 根节点地址
 *        visit 遍历函数指针
 *
 */
static void avl_preorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//根节点为NULL时，返回
	if (!root) {
		return;
	}
	//访问根节点键值对
	(*visit)(&root->k, &root->v);
	//前序遍历左子树
	avl_preorder_internal(root->lchild, visit);
	//前序遍历右子树
	avl_preorder_internal(root->rchild, visit);
}

/**
 *
 * 前序遍历AVL树
 *
 * @param avl AVL树地址
 *        visit 遍历函数指针
 *
 */
void avl_preorder(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_preorder_internal(avl->root, visit);
}

/**
 *
 * 中序遍历AVL树内部递归函数
 *
 * @param root 根节点地址
 *        visit 遍历函数指针
 *
 */
static void avl_inorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//根节点为NULL时，返回
	if (!root) {
		return;
	}
	//中序遍历左子树
	avl_inorder_internal(root->lchild, visit);
	//访问根节点键值对
	(*visit)(&root->k, &root->v);
	//中序遍历右子树
	avl_inorder_internal(root->rchild, visit);
}

/**
 *
 * 中序遍历AVL树
 *
 * @param avl AVL树地址
 *        visit 遍历函数指针
 *
 */
void avl_inorder(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_inorder_internal(avl->root, visit);
}

/**
 *
 * 后序遍历AVL树内部递归函数
 *
 * @param root 根节点地址
 *        visit 遍历函数指针
 *
 */
static void avl_postorder_internal(AVLNode* root,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	//根节点为NULL时，返回
	if (!root) {
		return;
	}
	//后序遍历左子树
	avl_postorder_internal(root->lchild, visit);
	//后序遍历右子树
	avl_postorder_internal(root->rchild, visit);
	//访问根节点键值对
	(*visit)(&root->k, &root->v);
}

/**
 *
 * 后序遍历AVL树
 *
 * @param avl AVL树地址
 *        visit 遍历函数指针
 *
 */
void avl_postorder(const AVL* avl,
		void (*visit)(const AVLKey* key, AVLVal* val)) {
	avl_postorder_internal(avl->root, visit);
}

/**
 *
 * 层序遍历AVL树
 *
 * @param avl AVL树地址
 *        visit 遍历函数指针
 *
 */
void avl_level(const AVL* avl, void (*visit)(const AVLKey* key, AVLVal* val)) {
	int size;
	AVLNode* p;
	AVLNode** q;
	int front, rear;

	//根节点为NULL时，返回
	if (!avl->root) {
		return;
	}
	//获取节点数
	size = avl_size(avl);
	//创建一个队列，这里根据节点数申请内存
	q = (AVLNode**) malloc(sizeof(AVLNode*) * size);
	//0位置元素为根节点
	q[0] = avl->root;
	//队首位置为0
	front = 0;
	//队尾位置为1
	rear = 1;
	//当队列不为空时
	while (front < rear) {
		//获取队首节点
		p = q[front++];
		//左孩子不为空时，添加至队尾
		if (p->lchild) {
			q[rear++] = p->lchild;
		}
		//右孩子不为空时，添加至队尾
		if (p->rchild) {
			q[rear++] = p->rchild;
		}
		//访问队首节点键值对
		(*visit)(&p->k, &p->v);
	}
	//释放队列
	free(q);
}

/**
 *
 * 遍历函数，可以修改，或建立类似函数
 *
 * @param key 键
 *        val 值
 *
 */
static void avl_visit(const AVLKey* key, AVLVal* val) {
	printf("%s %d\n", key->key, val->val);
}

/**
 *
 * 显示AVL树信息，可以修改
 *
 * @param avl AVL树地址
 *
 */
void avl_show(const AVL* avl) {
	printf("节点数：%d\n", avl_size(avl));
	printf("高度：%d\n", avl_height(avl));
	printf("叶子数：%d\n", avl_leaf_count(avl));
	printf("前序遍历结果：\n");
	avl_preorder(avl, avl_visit);
	printf("中序遍历结果：\n");
	avl_inorder(avl, avl_visit);
	printf("后序遍历结果：\n");
	avl_postorder(avl, avl_visit);
	printf("层序遍历结果：\n");
	avl_level(avl, avl_visit);
	printf("\n");
}

int main() {
	AVL a;
	AVLKey k;
	AVLVal v;
	AVLVal* vp;

	//创建AVL树
	avl_create(&a);

	//插入键值对
	printf("插入键值对：\n");
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

	//获取键，更改其值
	printf("获取键，更改其值：\n");
	strcpy(k.key, "c");
	avl_search(&a, &k, &vp);
	vp->val = 5;
	avl_show(&a);

	//删除键
	printf("删除键：\n");
	strcpy(k.key, "d");
	avl_remove(&a, &k);
	avl_show(&a);

	//清除所有元素
	printf("清除所有元素：\n");
	avl_clear(&a);
	avl_show(&a);

	//销毁AVL树
	avl_destroy(&a);

	return 0;
}
