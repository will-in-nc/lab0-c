#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *q = (struct list_head *) malloc(sizeof(struct list_head));
    if (!q)
        return NULL;

    INIT_LIST_HEAD(q);

    return q;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *e, *next;

    list_for_each_entry_safe (e, next, head, list) {
        q_release_element(e);
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e_new = (element_t *) malloc(sizeof(element_t));
    if (!e_new)
        return false;

    e_new->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!e_new->value) {
        free(e_new);

        return false;
    }

    INIT_LIST_HEAD(&e_new->list);
    snprintf(e_new->value, strlen(s) + 1, "%s", s);

    list_add(&e_new->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *e_new = (element_t *) malloc(sizeof(element_t));
    if (!e_new)
        return false;

    e_new->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!e_new->value) {
        free(e_new);

        return false;
    }

    INIT_LIST_HEAD(&e_new->list);
    snprintf(e_new->value, strlen(s) + 1, "%s", s);

    list_add_tail(&e_new->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;

    struct list_head *target_list = head->next;
    element_t *target_e = list_entry(target_list, element_t, list);

    list_del(target_list);

    if (sp != NULL)
        snprintf(sp, bufsize, "%s", target_e->value);

    return target_e;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head)
        return NULL;
    if (list_empty(head))
        return NULL;

    struct list_head *target_list = head->prev;
    element_t *target_e = list_entry(target_list, element_t, list);

    list_del(target_list);

    if (sp != NULL)
        snprintf(sp, bufsize, "%s", target_e->value);

    return target_e;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head)
        return false;
    if (list_empty(head))
        return false;

    int mid = q_size(head) / 2;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head) {
        if (len >= mid) {
            list_del(li);

            element_t *e = list_entry(li, element_t, list);
            q_release_element(e);

            return true;
        }

        len++;
    }

    return false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;

    struct list_head *target = head->next;
    bool deleted = false;
    while (target->next != head) {
        struct list_head *next = target->next;

        element_t *e_target = list_entry(target, element_t, list);
        element_t *e_next = list_entry(next, element_t, list);

        int len = strlen(e_target->value);
        if (strncmp(e_target->value, e_next->value, len) == 0) {
            list_del(next);
            q_release_element(e_next);

            deleted = true;
        } else {
            if (deleted) {
                deleted = false;
                list_del(target);
                q_release_element(e_target);
            }

            target = next;
        }
    }

    if (deleted) {
        element_t *e_target = list_entry(target, element_t, list);
        list_del(target);
        q_release_element(e_target);
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;
    if (list_empty(head))
        return;

    q_reverseK(head, 2);

}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;
    if (list_empty(head))
        return;

    struct list_head *l_head = head->next;
    while (l_head->next != head) {
        list_move_tail(head->prev, l_head);
    }

}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head)
        return;
    if (list_empty(head))
        return;

    struct list_head *l_head = head->next;
    int times = q_size(head) / k;
    for (int j = 0; j < times; j++) {
        struct list_head *prev = l_head->prev, *target = l_head->next;
        for (int i = 1; i < k; i++) {
            struct list_head *temp = target;
            target = target->next;

            list_move(temp, prev);
        }

        l_head = l_head->next;
    }
}

struct list_head *merge_sort(struct list_head *head,
                             struct list_head *tail,
                             int size,
                             bool descend)
{
    if (size == 1)
        return head;

    struct list_head *mid = head;
    int len = 0;
    while (len < size / 2) {
        mid = mid->next;
        len++;
    }

    int len2 = size - len;
    struct list_head *l1 = merge_sort(head, mid->prev, len, descend),
                     *l2 = merge_sort(mid, tail, len2, descend),
                     *prev = l1->prev, *lhead = prev;

    element_t *e1 = list_entry(l1, element_t, list),
              *e2 = list_entry(l2, element_t, list);

    while (len > 0 && len2 > 0) {
        struct list_head *target = NULL;
        if ((strcmp(e1->value, e2->value) > 0) == descend) {
            target = &e1->list;

            l1 = l1->next;
            e1 = list_entry(l1, element_t, list);
            len--;
        } else {
            target = &e2->list;

            l2 = l2->next;
            e2 = list_entry(l2, element_t, list);
            len2--;
        }

        list_move(target, lhead);
        lhead = target;
    }

    return prev->next;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head)
        return;
    if (list_empty(head))
        return;

    merge_sort(head->next, head->prev, q_size(head), descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head)
        return 0;

    int len = q_size(head);
    if (len <= 1)
        return len;

    element_t *e = list_entry(head->prev, element_t, list);
    struct list_head *next = e->list.prev;

    while (next != head) {
        element_t *e_next = list_entry(next, element_t, list);

        if (strcmp(e->value, e_next->value) < 0) {
            element_t *temp = e_next;
            next = next->prev;

            list_del_init(&temp->list);
            q_release_element(temp);
            len--;
        } else {
            e = e_next;
            next = next->prev;
        }
    }

    return len;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head)
        return 0;

    int len = q_size(head);
    if (len <= 1)
        return len;

    element_t *e = list_entry(head->prev, element_t, list);
    struct list_head *next = e->list.prev;

    while (next != head) {
        element_t *e_next = list_entry(next, element_t, list);

        if (strcmp(e->value, e_next->value) > 0) {
            element_t *temp = e_next;
            next = next->prev;

            list_del_init(&temp->list);
            q_release_element(temp);
            len--;
        } else {
            e = e_next;
            next = next->prev;
        }
    }

    return len;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
    /*if (!head)
        return 0;
    if (q_size(head) <= 1)
        return 0;

    queue_contex_t *main_q = list_entry(head->next, queue_contex_t, chain);
    queue_contex_t *q, *next;
    list_for_each_entry_safe (q, next, head, chain) {
        list_splice_init(q->q, main_q->q);
    }

    q_sort(main_q->q, descend);

    return q_size(main_q->q);*/
}
