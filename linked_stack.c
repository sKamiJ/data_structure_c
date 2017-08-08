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

//定义链栈元素结构体，可以修改
typedef struct StackElem {
	int val;
} StackElem;

//定义链栈节点
typedef struct StackNode {
	StackElem e;	//该节点元素
	struct StackNode* next;	//链栈下一节点
} StackNode;

//定义链栈结构体
typedef struct Stack {
	StackNode* top;	//栈顶节点
} Stack;

/**
 *
 * 显示链栈元素，可更改
 *
 * @param stack 链栈地址
 *
 */
void stack_show(const Stack* stack) {
	StackNode* p;

	//从栈顶开始显示所有元素值
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
 * 创建链栈
 *
 * @param stack 链栈地址，该链栈未被创建
 *
 */
void stack_create(Stack* stack) {
	//置栈顶节点为NULL
	stack->top = NULL;
}

/**
 *
 * 清除链栈所有元素
 *
 * @param stack 链栈地址
 *
 */
void stack_clear(Stack* stack) {
	StackNode* p;

	//当栈顶节点不为NULL时，释放该节点
	while (stack->top) {
		p = stack->top;
		stack->top = p->next;
		free(p);
	}
}

/**
 *
 * 销毁链栈
 *
 * @param stack 链栈地址
 *
 */
void stack_destroy(Stack* stack) {
	//清除链栈所有节点
	stack_clear(stack);
}

/**
 *
 * 元素入栈
 *
 * @param stack 链栈地址
 *        e 入栈的元素
 *
 * @return TRUE 入栈成功
 *         FALSE 内存不足，入栈失败
 *
 */
BOOL stack_push(Stack* stack, const StackElem* e) {
	StackNode* p;

	//创建新节点
	p = (StackNode*) malloc(sizeof(StackNode));
	//无内存分配时，错误
	if (!p) {
		return FALSE;
	}
	//新节点下一节点为栈顶节点
	p->next = stack->top;
	//复制元素
	p->e = *e;
	//栈顶节点为新节点
	stack->top = p;
	return TRUE;
}

/**
 *
 * 栈顶元素出栈
 *
 * @param stack 链栈地址
 *
 * @return TRUE 出栈成功
 *         FALSE 空栈，出栈失败
 *
 */
BOOL stack_pop(Stack* stack) {
	StackNode* p;

	//空栈，错误
	if (!stack->top) {
		return FALSE;
	}
	//删除节点为栈顶节点
	p = stack->top;
	//栈顶节点为下一节点
	stack->top = p->next;
	//释放删除节点
	free(p);
	return TRUE;
}

/**
 *
 * 获取栈顶元素
 *
 * @param stack 链栈地址
 *        ep 保存获取元素的指针
 *
 * @return TRUE 获取成功
 *         FALSE 空栈，获取失败
 *
 */
BOOL stack_top(const Stack* stack, StackElem** ep) {
	//空栈，错误
	if (!stack->top) {
		return FALSE;
	}
	//获取栈顶元素
	*ep = &stack->top->e;
	return TRUE;
}

/**
 *
 * 查看是否为空栈
 *
 * @param stack 链栈地址
 *
 * @return TRUE 空栈
 *         FALSE 非空栈
 *
 */
BOOL stack_empty(const Stack* stack) {
	return stack->top == NULL ? TRUE : FALSE;
}

int main() {
	Stack s;
	StackElem e;
	StackElem* ep;

	//创建链栈
	stack_create(&s);

	//元素入栈
	printf("元素入栈：\n");
	e.val = 0;
	stack_push(&s, &e);
	stack_show(&s);

	e.val = 1;
	stack_push(&s, &e);
	stack_show(&s);

	//获取栈顶元素，更改其值
	printf("获取栈顶元素，更改其值：\n");
	stack_top(&s, &ep);
	ep->val = 2;
	stack_show(&s);

	//栈顶元素出栈
	printf("栈顶元素出栈：\n");
	stack_pop(&s);
	stack_show(&s);

	//链栈是否为空
	printf("链栈是否为空：\n");
	printf("%s\n", stack_empty(&s) ? "TRUE" : "FALSE");

	//清除所有元素
	printf("清除所有元素：\n");
	stack_clear(&s);
	stack_show(&s);

	//链栈是否为空
	printf("链栈是否为空：\n");
	printf("%s\n", stack_empty(&s) ? "TRUE" : "FALSE");

	//销毁链栈
	stack_destroy(&s);

	return 0;
}
