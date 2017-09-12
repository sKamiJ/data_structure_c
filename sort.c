#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//定义排序元素
typedef struct Elem {
	int e;
} Elem;

/**
 *
 * 交换两个元素，可根据元素修改
 *
 * @param e1 元素1
 *        e2 元素2
 *
 */
static void swap(Elem* e1, Elem* e2) {
	Elem tmp;
	tmp = *e1;
	*e1 = *e2;
	*e2 = tmp;
}

/**
 *
 * 比较两个元素的大小，可根据元素修改
 *
 * @param e1 元素1
 *        e2 元素2
 *
 * @return 小于0 e1<e2
 *         等于0 e1=e2
 *         大于0 e1>e2
 *
 */
static int cmp(const Elem* e1, const Elem* e2) {
	return e1->e - e2->e;
}

/**
 *
 * 直接插入排序
 *
 * 平均时间复杂度 - O(n^2)
 * 最优时间复杂度 - O(n)，升序情况
 * 最差时间复杂度 - O(n^2)，降序情况
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 是
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void insert_sort(Elem* e, int len) {
	int i, j;
	Elem ref;

	//[0,i)已经有序，将i位置放入有序数组中
	for (i = 1; i < len; ++i) {
		//i位置为参考元素
		ref = e[i];
		//在[0,i)中将大于ref的元素右移
		j = i - 1;
		while (j >= 0 && cmp(&e[j], &ref) > 0) {
			e[j + 1] = e[j];
			--j;
		}
		//ref放在j+1处
		e[j + 1] = ref;
	}
}

/**
 *
 * 二分插入排序
 *
 * 平均时间复杂度 - O(n^2)，只降低查找复杂度，但未降低移动复杂度
 * 最优时间复杂度 - O(nlogn)，因为查找的复杂度需要O(logn)
 * 最差时间复杂度 - O(n^2)
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 是
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void binary_insert_sort(Elem* e, int len) {
	int i;
	int l, r, m;
	int res;
	Elem ref;

	//[0,i)已经有序，将i位置放入有序数组中
	for (i = 1; i < len; ++i) {
		//i位置为参考元素
		ref = e[i];
		//使用二分查找寻找插入位置
		l = 0;
		r = i - 1;
		while (l <= r) {
			m = (l + r) >> 1;
			res = cmp(&e[m], &ref);
			if (res > 0) {
				r = m - 1;
			} else {
				//相等情况等同于小于，保证稳定性
				l = m + 1;
			}
		}
		//l位置为插入位置，将[l,i)元素后移
		for (r = i - 1; r >= l; --r) {
			e[r + 1] = e[r];
		}
		//ref放在l处
		e[l] = ref;
	}
}

/**
 *
 * 希尔排序
 *
 * 平均时间复杂度 - 根据步长序列的不同而不同，最好为O(n(logn)^2)
 * 最优时间复杂度 - O(n)
 * 最差时间复杂度 - 根据步长序列的不同而不同
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 否
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void shell_sort(Elem* e, int len) {
	int h;
	int i, j;
	Elem ref;

	//生成初始增量，为Knuth序列
	h = 1;
	while (h < len) {
		h = 3 * h + 1;
	}
	//增量大于等于1时，进行直接插入排序
	while (h >= 1) {
		//进行分组插入排序
		for (i = h; i < len; ++i) {
			ref = e[i];
			j = i - h;
			while (j >= 0 && cmp(&e[j], &ref) > 0) {
				e[j + h] = e[j];
				j -= h;
			}
			e[j + h] = ref;
		}
		//递减增量
		h = (h - 1) / 3;
	}
}

/**
 *
 * 快速排序内部递归
 *
 * @param e 元素数组
 *        l 开始位置
 *        r 结束位置
 *
 */
static void quick_sort_internal(Elem*e, int l, int r) {
	int lt, gt;
	int i;
	int res;
	Elem ref;

	//已经有序，结束递归
	if (l >= r) {
		return;
	}

	//将位置为l的元素选为锚节点
	ref = e[l];
	//lt为下一个小于ref元素的插入位置
	lt = l;
	//gt为下一个大于ref元素的插入位置
	gt = r;
	//i为进行比较的元素
	i = l + 1;
	//[l,lt)为小于ref的元素，[lt,i)为等于ref的元素，
	//[i,gt]为待比较元素，(gt,r]为大于ref的元素
	while (i <= gt) {
		res = cmp(&e[i], &ref);
		if (res < 0) {
			//小于时，放到lt
			swap(&e[lt++], &e[i++]);
		} else if (res > 0) {
			//大于时，放到gt
			swap(&e[gt--], &e[i]);
		} else {
			//等于时，不用移动
			++i;
		}
	}
	//对小于和大于部分进行快速排序
	quick_sort_internal(e, l, lt - 1);
	quick_sort_internal(e, gt + 1, r);
}

/**
 *
 * 快速排序
 *
 * 平均时间复杂度 - O(nlogn)
 * 最优时间复杂度 - O(nlogn)，每次选择的锚节点为中位数
 * 最差时间复杂度 - O(n^2)，每次选择的锚节点为最大/最小元素
 * 辅助空间 ------- 递归造成的栈空间使用，取决于树深度，一般为O(logn)，最差为O(n)
 * 稳定 ----------- 否
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void quick_sort(Elem* e, int len) {
	quick_sort_internal(e, 0, len - 1);
}

/**
 *
 * 选择排序
 *
 * 平均时间复杂度 - O(n^2)
 * 最优时间复杂度 - O(n^2)
 * 最差时间复杂度 - O(n^2)
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 否
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void select_sort(Elem* e, int len) {
	int i, j;

	//i位置为本轮最小元素放置处
	for (i = 0; i < len; ++i) {
		//在(i,len)位置处选择最小元素
		for (j = i + 1; j < len; ++j) {
			if (cmp(&e[i], &e[j]) > 0) {
				swap(&e[i], &e[j]);
			}
		}
	}
}

/**
 *
 * 将位置为pos的堆更改为大根堆
 *
 * @param e 元素数组
 *        len 数组长度
 *        pos 起始位置
 *
 */
static void heap_sort_heapify(Elem* e, int len, int pos) {
	int l, r;
	int max;

	//左孩子位置为2*pos+1
	l = (pos << 1) + 1;
	//右孩子位置为2*pos+2
	r = l + 1;
	//选择三者中最大的
	//默认pos位置最大
	max = pos;
	//左孩子存在且大于最大值时，更新最大值位置
	if (l < len && cmp(&e[l], &e[max]) > 0) {
		max = l;
	}
	//右孩子存在且大于最大值时，更新最大值位置
	if (r < len && cmp(&e[r], &e[max]) > 0) {
		max = r;
	}
	//根不为最大时
	if (max != pos) {
		//将最大值放到根
		swap(&e[pos], &e[max]);
		//将max位置的堆更改为大根堆
		heap_sort_heapify(e, len, max);
	}
}
;

/**
 *
 * 堆排序
 *
 * 平均时间复杂度 - O(nlogn)
 * 最优时间复杂度 - O(nlogn)
 * 最差时间复杂度 - O(nlogn)
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 否
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void heap_sort(Elem* e, int len) {
	int i;

	//从第一个非叶子节点开始，变为大根堆
	for (i = (len >> 1) - 1; i >= 0; --i) {
		heap_sort_heapify(e, len, i);
	}
	//堆长度大于1时，即未完全有序时
	while (len > 1) {
		//0位置为最大元素，放到堆尾
		swap(&e[0], &e[--len]);
		//将位置为0的堆变为大根堆
		heap_sort_heapify(e, len, 0);
	}
}

/**
 *
 * 冒泡排序
 *
 * 平均时间复杂度 - O(n^2)
 * 最优时间复杂度 - O(n^2)
 * 最差时间复杂度 - O(n^2)
 * 辅助空间 ------- O(1)
 * 稳定 ----------- 是
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void bubble_sort(Elem* e, int len) {
	int i, j;

	//i位置为冒泡终点
	for (i = len - 1; i > 0; --i) {
		//将[0,i)位置中相邻两个元素的较大者后移
		for (j = 0; j < i; ++j) {
			if (cmp(&e[j], &e[j + 1]) > 0) {
				swap(&e[j], &e[j + 1]);
			}
		}
	}
}

/**
 *
 * 归并排序内部递归
 *
 * @param e 元素数组
 *        l 开始位置
 *        r 结束位置
 *
 */
static void merge_sort_internal(Elem* e, int l, int r) {
	Elem* tmp;
	int index;
	int m;
	int i, j;
	int len;

	//已经有序，结束递归
	if (l >= r) {
		return;
	}
	//计算中点
	m = (l + r) >> 1;
	//使[l,m]有序
	merge_sort_internal(e, l, m);
	//使[m+1,r]有序
	merge_sort_internal(e, m + 1, r);
	//计算辅助空间长度
	len = r - l + 1;
	//申请内存
	tmp = (Elem*) malloc(sizeof(Elem) * len);
	if (!tmp) {
		exit(-1);
	}
	//i为左部下标
	i = l;
	//j为右部下标
	j = m + 1;
	//index为辅助空间下标
	index = 0;
	//当两部分均有元素时
	while (i <= m && j <= r) {
		//将较小者放入辅助空间
		tmp[index++] = cmp(&e[i], &e[j]) <= 0 ? e[i++] : e[j++];
	}
	//将左部剩余元素放入辅助空间
	while (i <= m) {
		tmp[index++] = e[i++];
	}
	//将右部剩余元素放入辅助空间
	while (j <= r) {
		tmp[index++] = e[j++];
	}
	//将有序的辅助空间写回元素数组中
	for (i = 0; i < len; ++i) {
		e[l++] = tmp[i];
	}
	//释放内存
	free(tmp);
}

/**
 *
 * 归并排序
 *
 * 平均时间复杂度 - O(nlogn)
 * 最优时间复杂度 - O(nlogn)
 * 最差时间复杂度 - O(nlogn)
 * 辅助空间 ------- O(n)
 * 稳定 ----------- 是
 *
 * @param e 元素数组
 *        len 数组长度
 *
 */
void merge_sort(Elem* e, int len) {
	merge_sort_internal(e, 0, len - 1);
}

/**
 *
 * 显示排序时间和排序结果
 *
 * @param e 元素数组
 *        len 数组长度
 *        sort 排序函数
 *
 */
void sort_show(const Elem* e, int len, void (*sort)(Elem* e, int len)) {
	Elem* copy;
	int i;
	clock_t start, end;

	//复制排序数组
	copy = (Elem*) malloc(sizeof(Elem) * len);
	if (!copy) {
		exit(-1);
	}
	for (i = 0; i < len; ++i) {
		copy[i] = e[i];
	}
	//获取开始时间
	start = clock();
	//排序
	(*sort)(copy, len);
	//获取结束时间
	end = clock();
	printf("排序时间：%dms\n排序结果：\n", (int) (end - start));
	for (i = 0; i < len; ++i) {
		printf("%d ", copy[i].e);
	}
	printf("\n\n");
	free(copy);
}

int main() {
	Elem* e;
	int len;
	int i;
	int range;

	len = 10;
	range = 20;

	//产生随机元素数组
	e = (Elem*) malloc(sizeof(Elem) * len);
	if (!e) {
		exit(-1);
	}
	srand((unsigned int) time(NULL));
	for (i = 0; i < len; ++i) {
		e[i].e = rand() % range;
	}

	//直接插入排序
	printf("直接插入排序：\n");
	sort_show(e, len, insert_sort);

	//二分插入排序
	printf("二分插入排序：\n");
	sort_show(e, len, binary_insert_sort);

	//希尔排序
	printf("希尔排序：\n");
	sort_show(e, len, shell_sort);

	//快速排序
	printf("快速排序：\n");
	sort_show(e, len, quick_sort);

	//选择排序
	printf("选择排序：\n");
	sort_show(e, len, select_sort);

	//堆排序
	printf("堆排序：\n");
	sort_show(e, len, heap_sort);

	//冒泡排序
	printf("冒泡排序：\n");
	sort_show(e, len, bubble_sort);

	//归并排序
	printf("归并排序：\n");
	sort_show(e, len, merge_sort);

	free(e);

	return 0;
}
