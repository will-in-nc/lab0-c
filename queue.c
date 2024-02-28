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

    struct list_head *li = head->next, *next = li->next;
    while (li != head && next != head) {
        list_del_init(li);
        list_add(li, next);

        li = li->next;
        next = li->next;
    }

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
        element_t *t_temp = q_remove_tail(head, NULL, 0);
        list_add_tail(&t_temp->list, l_head);
    }

}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
