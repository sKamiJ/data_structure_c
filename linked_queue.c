#include <stdio.h>
#include <stdlib.h>

//����BOOL����
typedef int BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//����������Ԫ�ؽṹ�壬�����޸�
typedef struct QueueElem {
	int val;
} QueueElem;

//���������нڵ�
typedef struct QueueNode {
	QueueElem e;	//�ýڵ�Ԫ��
	struct QueueNode* next;	//��������һ�ڵ�
} QueueNode;

//���������нṹ��
typedef struct Queue {
	QueueNode* front;	//���׽ڵ�
	QueueNode* rear;	//��β�ڵ�
} Queue;

/**
 *
 * ��ʾ������Ԫ�أ��ɸ���
 *
 * @param queue �����е�ַ
 *
 */
void queue_show(const Queue* queue) {
	QueueNode* p;

	//�Ӷ��׿�ʼ��ʾ����Ԫ��ֵ
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
 * ����������
 *
 * @param queue �����е�ַ����������δ������
 *
 */
void queue_create(Queue* queue) {
	//�ö��׺Ͷ�β�ڵ�ΪNULL
	queue->front = queue->rear = NULL;
}

/**
 *
 * �������������Ԫ��
 *
 * @param queue �����е�ַ
 *
 */
void queue_clear(Queue* queue) {
	QueueNode* p;

	//�����׽ڵ㲻ΪNULLʱ���ͷŸýڵ�
	while (queue->front) {
		p = queue->front;
		queue->front = p->next;
		free(p);
	}
	//�ö�β�ڵ�ΪNULL
	queue->rear = NULL;
}

/**
 *
 * ����������
 *
 * @param queue �����е�ַ
 *
 */
void queue_destroy(Queue* queue) {
	//������������нڵ�
	queue_clear(queue);
}

/**
 *
 * Ԫ�������
 *
 * @param queue �����е�ַ
 *        e ����е�Ԫ��
 *
 * @return TRUE ����гɹ�
 *         FALSE �ڴ治�㣬�����ʧ��
 *
 */
BOOL queue_enqueue(Queue* queue, const QueueElem* e) {
	QueueNode* p;

	//�����½ڵ�
	p = (QueueNode*) malloc(sizeof(QueueNode));
	//���ڴ����ʱ������
	if (!p) {
		return FALSE;
	}
	//�½ڵ���һ�ڵ�ΪNULL
	p->next = NULL;
	//����Ԫ��
	p->e = *e;
	if (queue->rear) {
		//���в�Ϊ�����
		queue->rear->next = p;
		queue->rear = p;
	} else {
		//����Ϊ�����
		queue->front = queue->rear = p;
	}
	return TRUE;
}

/**
 *
 * Ԫ�س�����
 *
 * @param queue �����е�ַ
 *
 * @return TRUE �����гɹ�
 *         FALSE �ն��У�������ʧ��
 *
 */
BOOL queue_dequeue(Queue* queue) {
	QueueNode* p;

	//�ն��У�����
	if (!queue->front) {
		return FALSE;
	}
	//ɾ���ڵ�Ϊ���׽ڵ�
	p = queue->front;
	//���׽ڵ�Ϊ��һ�ڵ�
	queue->front = p->next;
	//�ͷ�ɾ���ڵ�
	free(p);
	//�������к�Ϊ�ն��У��ö�β�ڵ�ΪNULL
	if (!queue->front) {
		queue->rear = NULL;
	}
	return TRUE;
}

/**
 *
 * ��ȡ����Ԫ��
 *
 * @param queue �����е�ַ
 *        ep �����ȡԪ�ص�ָ��
 *
 * @return TRUE ��ȡ�ɹ�
 *         FALSE �ն��У���ȡʧ��
 *
 */
BOOL queue_front(const Queue* queue, QueueElem** ep) {
	//�ն��У�����
	if (!queue->front) {
		return FALSE;
	}
	//��ȡ����Ԫ��
	*ep = &queue->front->e;
	return TRUE;
}

/**
 *
 * �鿴�Ƿ�Ϊ�ն���
 *
 * @param queue �����е�ַ
 *
 * @return TRUE �ն���
 *         FALSE �ǿն���
 *
 */
BOOL queue_empty(const Queue* queue) {
	return queue->front == NULL ? TRUE : FALSE;
}

int main() {
	Queue q;
	QueueElem e;
	QueueElem* ep;

	//��������
	queue_create(&q);

	//Ԫ�������
	printf("Ԫ������У�\n");
	e.val = 0;
	queue_enqueue(&q, &e);
	queue_show(&q);

	e.val = 1;
	queue_enqueue(&q, &e);
	queue_show(&q);

	//��ȡ����Ԫ�أ�������ֵ
	printf("��ȡ����Ԫ�أ�������ֵ��\n");
	queue_front(&q, &ep);
	ep->val = 2;
	queue_show(&q);

	//Ԫ�س�����
	printf("Ԫ�س����У�\n");
	queue_dequeue(&q);
	queue_show(&q);

	//�������Ƿ�Ϊ��
	printf("�������Ƿ�Ϊ�գ�\n");
	printf("%s\n", queue_empty(&q) ? "TRUE" : "FALSE");

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	queue_clear(&q);
	queue_show(&q);

	//�������Ƿ�Ϊ��
	printf("�������Ƿ�Ϊ�գ�\n");
	printf("%s\n", queue_empty(&q) ? "TRUE" : "FALSE");

	//����������
	queue_destroy(&q);

	return 0;
}
