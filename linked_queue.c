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

//定义链队列元素结构体，可以修改
typedef struct QueueElem {
	int val;
} QueueElem;

//定义链队列节点
typedef struct QueueNode {
	QueueElem e;	//该节点元素
	struct QueueNode* next;	//链队列下一节点
} QueueNode;

//定义链队列结构体
typedef struct Queue {
	QueueNode* front;	//队首节点
	QueueNode* rear;	//队尾节点
} Queue;

/**
 *
 * 显示链队列元素，可更改
 *
 * @param queue 链队列地址
 *
 */
void queue_show(const Queue* queue) {
	QueueNode* p;

	//从队首开始显示所有元素值
	printf("elem:");
	p = queue->front;
	while (p) {
		printf("%d ", p->e.val);
		p = p->next;
	}
	printf("\n");
}

/**
 *
 * 创建链队列
 *
 * @param queue 链队列地址，该链队列未被创建
 *
 */
void queue_create(Queue* queue) {
	//置队首和队尾节点为NULL
	queue->front = queue->rear = NULL;
}

/**
 *
 * 清除链队列所有元素
 *
 * @param queue 链队列地址
 *
 */
void queue_clear(Queue* queue) {
	QueueNode* p;

	//当队首节点不为NULL时，释放该节点
	while (queue->front) {
		p = queue->front;
		queue->front = p->next;
		free(p);
	}
	//置队尾节点为NULL
	queue->rear = NULL;
}

/**
 *
 * 销毁链队列
 *
 * @param queue 链队列地址
 *
 */
void queue_destroy(Queue* queue) {
	//清除链队列所有节点
	queue_clear(queue);
}

/**
 *
 * 元素入队列
 *
 * @param queue 链队列地址
 *        e 入队列的元素
 *
 * @return TRUE 入队列成功
 *         FALSE 内存不足，入队列失败
 *
 */
BOOL queue_enqueue(Queue* queue, const QueueElem* e) {
	QueueNode* p;

	//创建新节点
	p = (QueueNode*) malloc(sizeof(QueueNode));
	//无内存分配时，错误
	if (!p) {
		return FALSE;
	}
	//新节点下一节点为NULL
	p->next = NULL;
	//复制元素
	p->e = *e;
	if (queue->rear) {
		//队列不为空情况
		queue->rear->next = p;
		queue->rear = p;
	} else {
		//队列为空情况
		queue->front = queue->rear = p;
	}
	return TRUE;
}

/**
 *
 * 元素出队列
 *
 * @param queue 链队列地址
 *
 * @return TRUE 出队列成功
 *         FALSE 空队列，出队列失败
 *
 */
BOOL queue_dequeue(Queue* queue) {
	QueueNode* p;

	//空队列，错误
	if (!queue->front) {
		return FALSE;
	}
	//删除节点为队首节点
	p = queue->front;
	//队首节点为下一节点
	queue->front = p->next;
	//释放删除节点
	free(p);
	//若出队列后为空队列，置队尾节点为NULL
	if (!queue->front) {
		queue->rear = NULL;
	}
	return TRUE;
}

/**
 *
 * 获取队首元素
 *
 * @param queue 链队列地址
 *        ep 保存获取元素的指针
 *
 * @return TRUE 获取成功
 *         FALSE 空队列，获取失败
 *
 */
BOOL queue_front(const Queue* queue, QueueElem** ep) {
	//空队列，错误
	if (!queue->front) {
		return FALSE;
	}
	//获取队首元素
	*ep = &queue->front->e;
	return TRUE;
}

/**
 *
 * 查看是否为空队列
 *
 * @param queue 链队列地址
 *
 * @return TRUE 空队列
 *         FALSE 非空队列
 *
 */
BOOL queue_empty(const Queue* queue) {
	return queue->front == NULL ? TRUE : FALSE;
}

int main() {
	Queue q;
	QueueElem e;
	QueueElem* ep;

	//创建队列
	queue_create(&q);

	//元素入队列
	printf("元素入队列：\n");
	e.val = 0;
	queue_enqueue(&q, &e);
	queue_show(&q);

	e.val = 1;
	queue_enqueue(&q, &e);
	queue_show(&q);

	//获取队首元素，更改其值
	printf("获取队首元素，更改其值：\n");
	queue_front(&q, &ep);
	ep->val = 2;
	queue_show(&q);

	//元素出队列
	printf("元素出队列：\n");
	queue_dequeue(&q);
	queue_show(&q);

	//链队列是否为空
	printf("链队列是否为空：\n");
	printf("%s\n", queue_empty(&q) ? "TRUE" : "FALSE");

	//清除所有元素
	printf("清除所有元素：\n");
	queue_clear(&q);
	queue_show(&q);

	//链队列是否为空
	printf("链队列是否为空：\n");
	printf("%s\n", queue_empty(&q) ? "TRUE" : "FALSE");

	//销毁链队列
	queue_destroy(&q);

	return 0;
}
