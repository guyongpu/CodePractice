#include <iostream>

using namespace std;

/**
 * 题目：链表排序
 * 描述：将两个无序链表合并成一个有序链表，要求时间复杂度不大于O(n^2)，空间复杂度为O(1)
 * 思路：分两步实现，1.将单链表进行选择排序；2.将两个有序链表合并.
 * 备注：重要题目
 */


struct ListNode {
    int val;
    ListNode *next;

    ListNode(int x) : val(x), next(nullptr) {};
};

//单个链表排序
ListNode *ListSort(ListNode *head) {
    if (head == nullptr || head->next == nullptr)
        return nullptr;
    ListNode *NewHead = nullptr;

    ListNode *front = nullptr, *curr = nullptr;
    ListNode *minNode = nullptr;
    ListNode *minNodeFront = nullptr;

    minNode = head;
    front = head;
    curr = head->next;
    //1.确定头节点
    while (curr) {
        if (curr->val < minNode->val) {
            minNodeFront = front;
            minNode = curr;
        }
        curr = curr->next;
        front = front->next;
    }
    if (minNode == head) {
        NewHead = head;

        head = head->next;
    } else {
        NewHead = minNode;

        minNodeFront->next = minNode->next;
    }

    ListNode *pNode = NewHead;

    //2.开始排序剩余部分
    while (1) {
        minNode = head;
        front = head;
        curr = head->next;
        //确定头节点
        while (curr) {
            if (curr->val < minNode->val) {
                minNodeFront = front;
                minNode = curr;
            }
            curr = curr->next;
            front = front->next;
        }
        if (minNode == head) {
            pNode->next = head;
            pNode = pNode->next;

            head = head->next;
        } else {
            pNode->next = minNode;
            pNode = pNode->next;

            minNodeFront->next = minNode->next;
        }
        if (head == nullptr) {
            break;
        }
    }

    return NewHead;
}

//两个有序链表合并
ListNode *TwoListSort(ListNode *head1, ListNode *head2) {
    ListNode *NewHead = nullptr;
    if(head1->val < head2->val){
        NewHead = head1;
        head1 = head1->next;
    }else{
        NewHead = head2;
        head2 = head2->next;
    }

    ListNode *pNode = NewHead;

    ListNode *pNode1 = head1;
    ListNode *pNode2 = head2;

    while(pNode1 || pNode2){
        if(pNode1 && pNode2){
            if(pNode1->val < pNode2->val){
                pNode->next = pNode1;
                pNode = pNode->next;

                pNode1 = pNode1->next;
            }
            else{
                pNode->next = pNode2;
                pNode = pNode->next;

                pNode2 = pNode2->next;
            }
        }
        else if(pNode1){
            pNode->next = pNode1;
            break;
        }else{
            pNode->next = pNode2;
            break;
        }

    }
    return NewHead;
}

//遍历输出链表
void DisPlayList(ListNode *head) {
    ListNode *pNode = head;
    while (pNode) {
        cout << pNode->val << " ";
        pNode = pNode->next;
    }
    cout << endl;
}

int main() {
    ListNode *head1 = nullptr;
    ListNode *head2 = nullptr;

    //构造链表1 p1->p2->p3->p4
    ListNode *p1 = new ListNode(5);
    ListNode *p2 = new ListNode(25);
    ListNode *p3 = new ListNode(15);
    ListNode *p4 = new ListNode(9);
    p1->next = p2;
    p2->next = p3;
    p3->next = p4;

    //构造链表2 p5->p6->p7->p8->p9
    ListNode *p5 = new ListNode(11);
    ListNode *p6 = new ListNode(65);
    ListNode *p7 = new ListNode(35);
    ListNode *p8 = new ListNode(1);
    ListNode *p9 = new ListNode(13);
    p5->next = p6;
    p6->next = p7;
    p7->next = p8;
    p8->next = p9;

    head1 = p1;
    head2 = p5;

    //单链表排序
    head1 = ListSort(head1);
    head2 = ListSort(head2);

    DisPlayList(head1);
    DisPlayList(head2);

    //链表1与链表2合并
    ListNode *NewHead = TwoListSort(head1, head2);

    DisPlayList(NewHead);

    return 0;
}