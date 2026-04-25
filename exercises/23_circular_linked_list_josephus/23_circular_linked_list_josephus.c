#include <stdio.h>
#include <stdlib.h>

#include "circular_linked_list.h"

// 使用环形链表模拟约瑟夫环并打印出列顺序
static void josephus_problem(int n, int k, int m) {
    if (n <= 0 || k <= 0 || m <= 0) {
        printf("参数错误\n");
        return;
    }

    Node* head = create_circular_list(n);
    if (!head) {
        printf("\n");
        return;
    }

    // prev 指向 current 的前一个节点
    Node* current = head;
    Node* prev = head;
    while (prev->next != head) prev = prev->next;

    // 起始位置移动到第 k 个
    for (int i = 1; i < k; ++i) {
        prev = current;
        current = current->next;
    }

    while (current != NULL) {
        for (int step = 1; step < m; ++step) {
            prev = current;
            current = current->next;
        }

        printf("%d ", current->id);

        if (current == prev) {
            free(current);
            break;
        }

        prev->next = current->next;
        Node* next = current->next;
        free(current);
        current = next;
    }
    
    printf("\n");
}

int main(void) {
    josephus_problem(5, 1, 2);  // 输出结果：2 4 1 5 3
    return 0;
}
