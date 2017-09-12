#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//��������Ԫ��
typedef struct Elem {
	int e;
} Elem;

/**
 *
 * ��������Ԫ�أ��ɸ���Ԫ���޸�
 *
 * @param e1 Ԫ��1
 *        e2 Ԫ��2
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
 * �Ƚ�����Ԫ�صĴ�С���ɸ���Ԫ���޸�
 *
 * @param e1 Ԫ��1
 *        e2 Ԫ��2
 *
 * @return С��0 e1<e2
 *         ����0 e1=e2
 *         ����0 e1>e2
 *
 */
static int cmp(const Elem* e1, const Elem* e2) {
	return e1->e - e2->e;
}

/**
 *
 * ֱ�Ӳ�������
 *
 * ƽ��ʱ�临�Ӷ� - O(n^2)
 * ����ʱ�临�Ӷ� - O(n)���������
 * ���ʱ�临�Ӷ� - O(n^2)���������
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void insert_sort(Elem* e, int len) {
	int i, j;
	Elem ref;

	//[0,i)�Ѿ����򣬽�iλ�÷�������������
	for (i = 1; i < len; ++i) {
		//iλ��Ϊ�ο�Ԫ��
		ref = e[i];
		//��[0,i)�н�����ref��Ԫ������
		j = i - 1;
		while (j >= 0 && cmp(&e[j], &ref) > 0) {
			e[j + 1] = e[j];
			--j;
		}
		//ref����j+1��
		e[j + 1] = ref;
	}
}

/**
 *
 * ���ֲ�������
 *
 * ƽ��ʱ�临�Ӷ� - O(n^2)��ֻ���Ͳ��Ҹ��Ӷȣ���δ�����ƶ����Ӷ�
 * ����ʱ�临�Ӷ� - O(nlogn)����Ϊ���ҵĸ��Ӷ���ҪO(logn)
 * ���ʱ�临�Ӷ� - O(n^2)
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void binary_insert_sort(Elem* e, int len) {
	int i;
	int l, r, m;
	int res;
	Elem ref;

	//[0,i)�Ѿ����򣬽�iλ�÷�������������
	for (i = 1; i < len; ++i) {
		//iλ��Ϊ�ο�Ԫ��
		ref = e[i];
		//ʹ�ö��ֲ���Ѱ�Ҳ���λ��
		l = 0;
		r = i - 1;
		while (l <= r) {
			m = (l + r) >> 1;
			res = cmp(&e[m], &ref);
			if (res > 0) {
				r = m - 1;
			} else {
				//��������ͬ��С�ڣ���֤�ȶ���
				l = m + 1;
			}
		}
		//lλ��Ϊ����λ�ã���[l,i)Ԫ�غ���
		for (r = i - 1; r >= l; --r) {
			e[r + 1] = e[r];
		}
		//ref����l��
		e[l] = ref;
	}
}

/**
 *
 * ϣ������
 *
 * ƽ��ʱ�临�Ӷ� - ���ݲ������еĲ�ͬ����ͬ�����ΪO(n(logn)^2)
 * ����ʱ�临�Ӷ� - O(n)
 * ���ʱ�临�Ӷ� - ���ݲ������еĲ�ͬ����ͬ
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void shell_sort(Elem* e, int len) {
	int h;
	int i, j;
	Elem ref;

	//���ɳ�ʼ������ΪKnuth����
	h = 1;
	while (h < len) {
		h = 3 * h + 1;
	}
	//�������ڵ���1ʱ������ֱ�Ӳ�������
	while (h >= 1) {
		//���з����������
		for (i = h; i < len; ++i) {
			ref = e[i];
			j = i - h;
			while (j >= 0 && cmp(&e[j], &ref) > 0) {
				e[j + h] = e[j];
				j -= h;
			}
			e[j + h] = ref;
		}
		//�ݼ�����
		h = (h - 1) / 3;
	}
}

/**
 *
 * ���������ڲ��ݹ�
 *
 * @param e Ԫ������
 *        l ��ʼλ��
 *        r ����λ��
 *
 */
static void quick_sort_internal(Elem*e, int l, int r) {
	int lt, gt;
	int i;
	int res;
	Elem ref;

	//�Ѿ����򣬽����ݹ�
	if (l >= r) {
		return;
	}

	//��λ��Ϊl��Ԫ��ѡΪê�ڵ�
	ref = e[l];
	//ltΪ��һ��С��refԪ�صĲ���λ��
	lt = l;
	//gtΪ��һ������refԪ�صĲ���λ��
	gt = r;
	//iΪ���бȽϵ�Ԫ��
	i = l + 1;
	//[l,lt)ΪС��ref��Ԫ�أ�[lt,i)Ϊ����ref��Ԫ�أ�
	//[i,gt]Ϊ���Ƚ�Ԫ�أ�(gt,r]Ϊ����ref��Ԫ��
	while (i <= gt) {
		res = cmp(&e[i], &ref);
		if (res < 0) {
			//С��ʱ���ŵ�lt
			swap(&e[lt++], &e[i++]);
		} else if (res > 0) {
			//����ʱ���ŵ�gt
			swap(&e[gt--], &e[i]);
		} else {
			//����ʱ�������ƶ�
			++i;
		}
	}
	//��С�ںʹ��ڲ��ֽ��п�������
	quick_sort_internal(e, l, lt - 1);
	quick_sort_internal(e, gt + 1, r);
}

/**
 *
 * ��������
 *
 * ƽ��ʱ�临�Ӷ� - O(nlogn)
 * ����ʱ�临�Ӷ� - O(nlogn)��ÿ��ѡ���ê�ڵ�Ϊ��λ��
 * ���ʱ�临�Ӷ� - O(n^2)��ÿ��ѡ���ê�ڵ�Ϊ���/��СԪ��
 * �����ռ� ------- �ݹ���ɵ�ջ�ռ�ʹ�ã�ȡ��������ȣ�һ��ΪO(logn)�����ΪO(n)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void quick_sort(Elem* e, int len) {
	quick_sort_internal(e, 0, len - 1);
}

/**
 *
 * ѡ������
 *
 * ƽ��ʱ�临�Ӷ� - O(n^2)
 * ����ʱ�临�Ӷ� - O(n^2)
 * ���ʱ�临�Ӷ� - O(n^2)
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void select_sort(Elem* e, int len) {
	int i, j;

	//iλ��Ϊ������СԪ�ط��ô�
	for (i = 0; i < len; ++i) {
		//��(i,len)λ�ô�ѡ����СԪ��
		for (j = i + 1; j < len; ++j) {
			if (cmp(&e[i], &e[j]) > 0) {
				swap(&e[i], &e[j]);
			}
		}
	}
}

/**
 *
 * ��λ��Ϊpos�ĶѸ���Ϊ�����
 *
 * @param e Ԫ������
 *        len ���鳤��
 *        pos ��ʼλ��
 *
 */
static void heap_sort_heapify(Elem* e, int len, int pos) {
	int l, r;
	int max;

	//����λ��Ϊ2*pos+1
	l = (pos << 1) + 1;
	//�Һ���λ��Ϊ2*pos+2
	r = l + 1;
	//ѡ������������
	//Ĭ��posλ�����
	max = pos;
	//���Ӵ����Ҵ������ֵʱ���������ֵλ��
	if (l < len && cmp(&e[l], &e[max]) > 0) {
		max = l;
	}
	//�Һ��Ӵ����Ҵ������ֵʱ���������ֵλ��
	if (r < len && cmp(&e[r], &e[max]) > 0) {
		max = r;
	}
	//����Ϊ���ʱ
	if (max != pos) {
		//�����ֵ�ŵ���
		swap(&e[pos], &e[max]);
		//��maxλ�õĶѸ���Ϊ�����
		heap_sort_heapify(e, len, max);
	}
}
;

/**
 *
 * ������
 *
 * ƽ��ʱ�临�Ӷ� - O(nlogn)
 * ����ʱ�临�Ӷ� - O(nlogn)
 * ���ʱ�临�Ӷ� - O(nlogn)
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void heap_sort(Elem* e, int len) {
	int i;

	//�ӵ�һ����Ҷ�ӽڵ㿪ʼ����Ϊ�����
	for (i = (len >> 1) - 1; i >= 0; --i) {
		heap_sort_heapify(e, len, i);
	}
	//�ѳ��ȴ���1ʱ����δ��ȫ����ʱ
	while (len > 1) {
		//0λ��Ϊ���Ԫ�أ��ŵ���β
		swap(&e[0], &e[--len]);
		//��λ��Ϊ0�Ķѱ�Ϊ�����
		heap_sort_heapify(e, len, 0);
	}
}

/**
 *
 * ð������
 *
 * ƽ��ʱ�临�Ӷ� - O(n^2)
 * ����ʱ�临�Ӷ� - O(n^2)
 * ���ʱ�临�Ӷ� - O(n^2)
 * �����ռ� ------- O(1)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void bubble_sort(Elem* e, int len) {
	int i, j;

	//iλ��Ϊð���յ�
	for (i = len - 1; i > 0; --i) {
		//��[0,i)λ������������Ԫ�صĽϴ��ߺ���
		for (j = 0; j < i; ++j) {
			if (cmp(&e[j], &e[j + 1]) > 0) {
				swap(&e[j], &e[j + 1]);
			}
		}
	}
}

/**
 *
 * �鲢�����ڲ��ݹ�
 *
 * @param e Ԫ������
 *        l ��ʼλ��
 *        r ����λ��
 *
 */
static void merge_sort_internal(Elem* e, int l, int r) {
	Elem* tmp;
	int index;
	int m;
	int i, j;
	int len;

	//�Ѿ����򣬽����ݹ�
	if (l >= r) {
		return;
	}
	//�����е�
	m = (l + r) >> 1;
	//ʹ[l,m]����
	merge_sort_internal(e, l, m);
	//ʹ[m+1,r]����
	merge_sort_internal(e, m + 1, r);
	//���㸨���ռ䳤��
	len = r - l + 1;
	//�����ڴ�
	tmp = (Elem*) malloc(sizeof(Elem) * len);
	if (!tmp) {
		exit(-1);
	}
	//iΪ���±�
	i = l;
	//jΪ�Ҳ��±�
	j = m + 1;
	//indexΪ�����ռ��±�
	index = 0;
	//�������־���Ԫ��ʱ
	while (i <= m && j <= r) {
		//����С�߷��븨���ռ�
		tmp[index++] = cmp(&e[i], &e[j]) <= 0 ? e[i++] : e[j++];
	}
	//����ʣ��Ԫ�ط��븨���ռ�
	while (i <= m) {
		tmp[index++] = e[i++];
	}
	//���Ҳ�ʣ��Ԫ�ط��븨���ռ�
	while (j <= r) {
		tmp[index++] = e[j++];
	}
	//������ĸ����ռ�д��Ԫ��������
	for (i = 0; i < len; ++i) {
		e[l++] = tmp[i];
	}
	//�ͷ��ڴ�
	free(tmp);
}

/**
 *
 * �鲢����
 *
 * ƽ��ʱ�临�Ӷ� - O(nlogn)
 * ����ʱ�临�Ӷ� - O(nlogn)
 * ���ʱ�临�Ӷ� - O(nlogn)
 * �����ռ� ------- O(n)
 * �ȶ� ----------- ��
 *
 * @param e Ԫ������
 *        len ���鳤��
 *
 */
void merge_sort(Elem* e, int len) {
	merge_sort_internal(e, 0, len - 1);
}

/**
 *
 * ��ʾ����ʱ���������
 *
 * @param e Ԫ������
 *        len ���鳤��
 *        sort ������
 *
 */
void sort_show(const Elem* e, int len, void (*sort)(Elem* e, int len)) {
	Elem* copy;
	int i;
	clock_t start, end;

	//������������
	copy = (Elem*) malloc(sizeof(Elem) * len);
	if (!copy) {
		exit(-1);
	}
	for (i = 0; i < len; ++i) {
		copy[i] = e[i];
	}
	//��ȡ��ʼʱ��
	start = clock();
	//����
	(*sort)(copy, len);
	//��ȡ����ʱ��
	end = clock();
	printf("����ʱ�䣺%dms\n��������\n", (int) (end - start));
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

	//�������Ԫ������
	e = (Elem*) malloc(sizeof(Elem) * len);
	if (!e) {
		exit(-1);
	}
	srand((unsigned int) time(NULL));
	for (i = 0; i < len; ++i) {
		e[i].e = rand() % range;
	}

	//ֱ�Ӳ�������
	printf("ֱ�Ӳ�������\n");
	sort_show(e, len, insert_sort);

	//���ֲ�������
	printf("���ֲ�������\n");
	sort_show(e, len, binary_insert_sort);

	//ϣ������
	printf("ϣ������\n");
	sort_show(e, len, shell_sort);

	//��������
	printf("��������\n");
	sort_show(e, len, quick_sort);

	//ѡ������
	printf("ѡ������\n");
	sort_show(e, len, select_sort);

	//������
	printf("������\n");
	sort_show(e, len, heap_sort);

	//ð������
	printf("ð������\n");
	sort_show(e, len, bubble_sort);

	//�鲢����
	printf("�鲢����\n");
	sort_show(e, len, merge_sort);

	free(e);

	return 0;
}
