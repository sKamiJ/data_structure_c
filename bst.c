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

//定义二叉查找树键最大长度
#define BST_KEY_MAX_LEN	(32)

//定义二叉查找树键，可以修改
typedef struct BSTKey {
	char key[BST_KEY_MAX_LEN];
} BSTKey;

//定义二叉查找树值，可以修改
typedef struct BSTVal {
	int val;
} BSTVal;

//定义二叉查找树节点
typedef struct BSTNode {
	BSTKey k;	//键
	BSTVal v;	//值
	struct BSTNode* lchild;	//左孩子节点
	struct BSTNode* rchild;	//右孩子节点
} BSTNode;

//定义二叉查找树
typedef struct BST {
	BSTNode* root;	//根节点
} BST;

/**
 *
 * 创建二叉查找树
 *
 * @param bst 二叉查找树地址，该二叉查找树未被创建
 *
 */
void bst_create(BST* bst) {
	//置根节点为NULL
	bst->root = NULL;
}

/**
 *
 * 清除二叉查找树所有元素内部递归函数
 *
 * @param root 根节点地址
 *
 */
static void bst_clear_internal(BSTNode** root){
	//根节点为NULL时，递归结束
	if(!*root){
		return;
	}
	//清除左子树
	bst_clear_internal(&(*root)->lchild);
	//清除右子树
	bst_clear_internal(&(*root)->rchild);
	//释放根节点
	free(*root);
	//根节点置为NULL
	*root = NULL;
}

/**
 *
 * 清除二叉查找树所有元素
 *
 * @param bst 二叉查找树地址
 *
 */
void bst_clear(BST* bst) {
	bst_clear_internal(&bst->root);
}

/**
 *
 * 销毁二叉查找树
 *
 * @param bst 二叉查找树地址
 *
 */
void bst_destroy(BST* bst) {
	//清除二叉查找树所有元素
	bst_clear(bst);
}

/**
 *
 * 获取二叉查找树节点数量内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 节点数量
 *
 */
static int bst_size_internal(const BSTNode* root){
	int lsize,rsize;

	//根节点为NULL时，返回0
	if(!root){
		return 0;
	}
	//获取左子树节点数量
	lsize = bst_size_internal(root->lchild);
	//获取右子树节点数量
	rsize = bst_size_internal(root->rchild);
	//返回节点数量
	return lsize + rsize + 1;
}

/**
 *
 * 获取二叉查找树节点数量
 *
 * @param bst 二叉查找树地址
 *
 * @return 节点数量
 *
 */
int bst_size(const BST* bst){
	return bst_size_internal(bst->root);
}

/**
 *
 * 获取二叉查找树高度内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 高度
 *
 */
static int bst_height_internal(const BSTNode* root){
	int lheight,rheight;

	//根节点为NULL时，返回0
	if(!root){
		return 0;
	}
	//获取左子树高度
	lheight = bst_height_internal(root->lchild);
	//获取右子树高度
	rheight = bst_height_internal(root->rchild);
	//树高度为左右子树较高者+1
	return (lheight > rheight ? lheight : rheight) + 1;
}

/**
 *
 * 获取二叉查找树高度
 *
 * @param bst 二叉查找树地址
 *
 * @return 高度
 *
 */
int bst_height(const BST* bst){
	return bst_height_internal(bst->root);
}

/**
 *
 * 获取二叉查找树叶子数量内部递归函数
 *
 * @param root 根节点地址
 *
 * @return 叶子数量
 *
 */
static int bst_leaf_count_internal(const BSTNode* root){
	int lleaf,rleaf;

	//根节点为NULL时，返回0
	if(!root){
		return 0;
	}
	//根节点为叶子节点时，返回1
	if(!root->lchild && !root->rchild){
		return 1;
	}
	//获取左子树叶子数量
	lleaf = bst_leaf_count_internal(root->lchild);
	//获取右子树叶子数量
	rleaf = bst_leaf_count_internal(root->rchild);
	//返回总叶子数量
	return lleaf + rleaf;
}

/**
 *
 * 获取二叉查找树叶子数量
 *
 * @param bst 二叉查找树地址
 *
 * @return 叶子数量
 *
 */
int bst_leaf_count(const BST* bst){
	return bst_leaf_count_internal(bst->root);
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
static int bst_key_cmp(const BSTKey* k1, const BSTKey* k2){
	return strcmp(k1->key,k2->key);
}

/**
 *
 * 在二叉查找树中插入键以及其对应的值内部递归函数
 *
 * @param root 根节点地址
 *        key 插入的键
 *        val 键对应的值
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足或键已经存在，插入失败
 *
 */
static BOOL bst_insert_internal(BSTNode** root,const BSTKey* key, const BSTVal* val){
	int res;

	//根节点为NULL时，插入键值对
	if(!*root){
		//新建节点
		*root = (BSTNode*)malloc(sizeof(BSTNode));
		//内存不足时，错误
		if(!*root){
			return FALSE;
		}
		//左右孩子节点为NULL
		(*root)->lchild = (*root)->rchild = NULL;
		//复制键值对
		(*root)->k = *key;
		(*root)->v = *val;
		return TRUE;
	}
	//比较键和该节点键的大小
	res = bst_key_cmp(key,&(*root)->k);
	if(res<0){
		//小于该节点键时，在左子树插入
		return bst_insert_internal(&(*root)->lchild,key,val);
	} else if(res > 0){
		//大于该节点键时，在右子树插入
		return bst_insert_internal(&(*root)->rchild,key,val);
	} else {
		//等于该节点键，失败
		return FALSE;
	}
}

/**
 *
 * 在二叉查找树中插入键以及其对应的值
 *
 * @param bst 二叉查找树地址
 *        key 插入的键
 *        val 键对应的值
 *
 * @return TRUE 插入成功
 *         FALSE 内存不足或键已经存在，插入失败
 *
 */
BOOL bst_insert(BST* bst, const BSTKey* key, const BSTVal* val){
	return bst_insert_internal(&bst->root,key,val);
}

/**
 *
 * 在二叉查找树中删除键内部递归函数
 *
 * @param root 根节点地址
 *        key 删除的键
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
static BOOL bst_remove_internal(BSTNode** root,const BSTKey* key){
	BSTNode *p, *q, *del;
	int res;

	//根节点为NULL时，无该键，错误
	if(!*root){
		return FALSE;
	}
	//比较键和该节点键的大小
	res = bst_key_cmp(key,&(*root)->k);
	if(res < 0){
		//小于该节点键时，在左子树删除
		return bst_remove_internal(&(*root)->lchild,key);
	}else if(res >0){
		//大于该节点键时，在右子树删除
		return bst_remove_internal(&(*root)->rchild,key);
	} else {
		//等于该节点键，删除该节点
		del = *root;
		if(!del->lchild){
			//删除节点无左孩子时，根节点为其右孩子
			*root = del->rchild;
		} else if(!del->rchild){
			//删除节点无右孩子时，根节点为其左孩子
			*root = del->lchild;
		} else {
			//删除节点有左右孩子，将其左孩子的最右节点放在删除节点处
			//p为最右节点的父节点
			p = del;
			//q为最右节点
			q=p->lchild;
			//寻找最右节点
			while(q->rchild){
				p = q;
				q = q->rchild;
			}
			//删除节点的左孩子不为最右节点时
			if(p!=del){
				//父节点的右孩子为最右节点的左孩子
				p->rchild = q->lchild;
				//最右节点的左孩子为删除节点的左孩子
				q->lchild = del->lchild;
			}
			//最右节点的右孩子为删除节点的右孩子
			q->rchild = del->rchild;
			//根节点为最右节点
			*root = q;
		}
		//释放删除节点
		free(del);
		return TRUE;
	}
}

/**
 *
 * 在二叉查找树中删除键
 *
 * @param bst 二叉查找树地址
 *        key 删除的键
 *
 * @return TRUE 删除成功
 *         FALSE 键不存在，删除失败
 *
 */
BOOL bst_remove(BST* bst,const BSTKey* key){
	return bst_remove_internal(&bst->root,key);
}

/**
 *
 * 在二叉查找树中查找键内部递归函数
 *
 * @param root 根节点地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return TRUE 查找成功
 *         FALSE 键不存在，查找失败
 *
 */
static BOOL bst_search_internal(BSTNode* root, const BSTKey* key, BSTVal** val){
	int res;

	//根节点为NULL时，无该键，错误
	if(!root){
		return FALSE;
	}
	//比较键和该节点键的大小
	res = bst_key_cmp(key,&root->k);
	if(res<0){
		//小于该节点键时，在左子树查找
		return bst_search_internal(root->lchild,key,val);
	}else if(res>0){
		//大于该节点键时，在右子树查找
		return bst_search_internal(root->rchild,key,val);
	} else{
		//等于该节点键，保存值的指针
		*val = &root->v;
		return TRUE;
	}
}

/**
 *
 * 在二叉查找树中查找键
 *
 * @param bst 二叉查找树地址
 *        key 查找的键
 *        val 保存键对应值的指针
 *
 * @return TRUE 查找成功
 *         FALSE 键不存在，查找失败
 *
 */
BOOL bst_search(const BST* bst, const BSTKey* key, BSTVal** val){
	return bst_search_internal(bst->root,key,val);
}

static void bst_preorder(){
}

int main() {
	//Queue q;
	//QueueElem e;
	//QueueElem* ep;
	//int i;

	////创建队列
	//queue_create(&q);

	////元素入队列
	//for(i=0;i<100;++i){
	//	e.val = i;
	//	queue_enqueue(&q,&e);
	//}
	//queue_show(&q);

	//while(!queue_empty(&q)){
	//	queue_dequeue(&q);
	//}
	//queue_show(&q);

	//for(i=0;i<100;++i){
	//	e.val = i;
	//	queue_enqueue(&q,&e);
	//}
	//queue_show(&q);

	////销毁链队列
	//queue_destroy(&q);

	return 0;
}
