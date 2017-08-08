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

//������ջԪ�ؽṹ�壬�����޸�
typedef struct StackElem {
	int val;
} StackElem;

//������ջ�ڵ�
typedef struct StackNode {
	StackElem e;	//�ýڵ�Ԫ��
	struct StackNode* next;	//��ջ��һ�ڵ�
} StackNode;

//������ջ�ṹ��
typedef struct Stack {
	StackNode* top;	//ջ���ڵ�
} Stack;

/**
 *
 * ��ʾ��ջԪ�أ��ɸ���
 *
 * @param stack ��ջ��ַ
 *
 */
void stack_show(const Stack* stack) {
	StackNode* p;

	//��ջ����ʼ��ʾ����Ԫ��ֵ
	printf("elem:");
	p = stack->top;
	while (p) {
		printf("%d ", p->e.val);
		p = p->next;
	}
	printf("\n");
}

/**
 *
 * ������ջ
 *
 * @param stack ��ջ��ַ������ջδ������
 *
 */
void stack_create(Stack* stack) {
	//��ջ���ڵ�ΪNULL
	stack->top = NULL;
}

/**
 *
 * �����ջ����Ԫ��
 *
 * @param stack ��ջ��ַ
 *
 */
void stack_clear(Stack* stack) {
	StackNode* p;

	//��ջ���ڵ㲻ΪNULLʱ���ͷŸýڵ�
	while (stack->top) {
		p = stack->top;
		stack->top = p->next;
		free(p);
	}
}

/**
 *
 * ������ջ
 *
 * @param stack ��ջ��ַ
 *
 */
void stack_destroy(Stack* stack) {
	//�����ջ���нڵ�
	stack_clear(stack);
}

/**
 *
 * Ԫ����ջ
 *
 * @param stack ��ջ��ַ
 *        e ��ջ��Ԫ��
 *
 * @return TRUE ��ջ�ɹ�
 *         FALSE �ڴ治�㣬��ջʧ��
 *
 */
BOOL stack_push(Stack* stack, const StackElem* e) {
	StackNode* p;

	//�����½ڵ�
	p = (StackNode*) malloc(sizeof(StackNode));
	//���ڴ����ʱ������
	if (!p) {
		return FALSE;
	}
	//�½ڵ���һ�ڵ�Ϊջ���ڵ�
	p->next = stack->top;
	//����Ԫ��
	p->e = *e;
	//ջ���ڵ�Ϊ�½ڵ�
	stack->top = p;
	return TRUE;
}

/**
 *
 * ջ��Ԫ�س�ջ
 *
 * @param stack ��ջ��ַ
 *
 * @return TRUE ��ջ�ɹ�
 *         FALSE ��ջ����ջʧ��
 *
 */
BOOL stack_pop(Stack* stack) {
	StackNode* p;

	//��ջ������
	if (!stack->top) {
		return FALSE;
	}
	//ɾ���ڵ�Ϊջ���ڵ�
	p = stack->top;
	//ջ���ڵ�Ϊ��һ�ڵ�
	stack->top = p->next;
	//�ͷ�ɾ���ڵ�
	free(p);
	return TRUE;
}

/**
 *
 * ��ȡջ��Ԫ��
 *
 * @param stack ��ջ��ַ
 *        ep �����ȡԪ�ص�ָ��
 *
 * @return TRUE ��ȡ�ɹ�
 *         FALSE ��ջ����ȡʧ��
 *
 */
BOOL stack_top(const Stack* stack, StackElem** ep) {
	//��ջ������
	if (!stack->top) {
		return FALSE;
	}
	//��ȡջ��Ԫ��
	*ep = &stack->top->e;
	return TRUE;
}

/**
 *
 * �鿴�Ƿ�Ϊ��ջ
 *
 * @param stack ��ջ��ַ
 *
 * @return TRUE ��ջ
 *         FALSE �ǿ�ջ
 *
 */
BOOL stack_empty(const Stack* stack) {
	return stack->top == NULL ? TRUE : FALSE;
}

int main() {
	Stack s;
	StackElem e;
	StackElem* ep;

	//������ջ
	stack_create(&s);

	//Ԫ����ջ
	printf("Ԫ����ջ��\n");
	e.val = 0;
	stack_push(&s, &e);
	stack_show(&s);

	e.val = 1;
	stack_push(&s, &e);
	stack_show(&s);

	//��ȡջ��Ԫ�أ�������ֵ
	printf("��ȡջ��Ԫ�أ�������ֵ��\n");
	stack_top(&s, &ep);
	ep->val = 2;
	stack_show(&s);

	//ջ��Ԫ�س�ջ
	printf("ջ��Ԫ�س�ջ��\n");
	stack_pop(&s);
	stack_show(&s);

	//��ջ�Ƿ�Ϊ��
	printf("��ջ�Ƿ�Ϊ�գ�\n");
	printf("%s\n", stack_empty(&s) ? "TRUE" : "FALSE");

	//�������Ԫ��
	printf("�������Ԫ�أ�\n");
	stack_clear(&s);
	stack_show(&s);

	//��ջ�Ƿ�Ϊ��
	printf("��ջ�Ƿ�Ϊ�գ�\n");
	printf("%s\n", stack_empty(&s) ? "TRUE" : "FALSE");

	//������ջ
	stack_destroy(&s);

	return 0;
}
