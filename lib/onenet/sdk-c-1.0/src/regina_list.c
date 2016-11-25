#include "regina/regina_list.h"
#include "regina/allocator.h"
#include "std_wrapper.h"
#include "common.h"
#include "util.h"

struct regina_list_node_t {
    void                      *data;
    struct regina_list_node_t    *next;
    struct regina_list_node_t    *prev;
};

struct regina_list_t {
    regina_list_node_t    *head;
    regina_list_node_t    *tail;
    size_t             size;
};

static regina_list_node_t *set_next_neighbor(regina_list_node_t *whom, regina_list_node_t *neighbor)
{
    if (neighbor) {
        neighbor->prev = whom;
    }

    if (whom) {
        whom->next = neighbor;
    }

    return neighbor;
}

static regina_list_node_t *create_node(void *data)
{
    regina_list_node_t *node = NULL;

    REGINA_RETURN_IF_NIL(data, NULL);
    node = (regina_list_node_t *)regina_malloc(sizeof(regina_list_node_t));
    REGINA_RETURN_IF_NIL(node, NULL);
    node->data = data;
    node->next = node->prev = NULL;

    return node;
}

static void destroy_node(regina_list_node_t *it, deallocate_list_data deallocator)
{
    if (!it)
        return;

    if (deallocator) {
        (*deallocator)(it->data);
    } else {
        regina_free(it->data);
    }
    regina_free(it);
}

static void reset_list(regina_list_t *list)
{
    if (!list)
        return;

    list->head = list->tail = NULL;
    list->size = 0;
}

void *regina_list_get_data(regina_list_node_t *it)
{
    return (it ? it->data : NULL);
}

regina_list_node_t *regina_list_next(regina_list_node_t *it)
{
    return (it ? it->next : NULL);
}

regina_list_node_t *regina_list_prev(regina_list_node_t *it)
{
    return (it ? it->prev : NULL);
}

regina_list_t *regina_lists_merge(regina_list_t *destination_head, regina_list_t *tail)
{
    REGINA_RETURN_IF_NIL(destination_head, tail);
    REGINA_RETURN_IF_NIL(tail, destination_head);

    if (!destination_head->head) {
        destination_head->head = tail->head;
    } else {
        set_next_neighbor(destination_head->tail, tail->head);
    }

    destination_head->tail = tail->tail;
    destination_head->size += tail->size;

    reset_list(tail);

    return destination_head;
}

size_t regina_list_get_size(regina_list_t *list)
{
    REGINA_RETURN_IF_NIL(list, 0);
    return list->size;
}

regina_list_t *regina_list_create( void )
{
    return (regina_list_t *) regina_calloc(1, sizeof(regina_list_t));
}

regina_list_node_t *regina_list_push_front(regina_list_t *list, void *data)
{
    regina_list_node_t *node = NULL;

    REGINA_RETURN_IF_NIL2(list, data, NULL);
    node = create_node(data);
    REGINA_RETURN_IF_NIL(node, NULL);

    ++list->size;
    set_next_neighbor(node, list->head);

    list->head = node;
    if (!list->tail) {
        list->tail = node;
    }

    return node;
}

regina_list_node_t *regina_list_push_back(regina_list_t *list, void *data)
{
    regina_list_node_t *node = NULL;

    REGINA_RETURN_IF_NIL(list, NULL);
    node = create_node(data);
    REGINA_RETURN_IF_NIL(node, NULL);

    ++list->size;
    set_next_neighbor(list->tail, node);

    list->tail = node;
    if (!list->head) {
        list->head = node;
    }

    return node;
}

regina_list_node_t *regina_list_begin(regina_list_t *list)
{
    REGINA_RETURN_IF_NIL(list, NULL);
    return list->head;
}

regina_list_node_t *regina_list_back(regina_list_t *list)
{
    REGINA_RETURN_IF_NIL(list, NULL);
    return list->tail;
}

void regina_list_clear(regina_list_t *list, deallocate_list_data deallocator)
{
    regina_list_node_t *it = NULL;

    if (!list || !list->size)
        return;

    it = list->head;
    while (it) {
        regina_list_node_t *next = it->next;
        destroy_node(it, deallocator);
        it = next;
    }

    reset_list(list);
}

void regina_list_destroy(regina_list_t *list, deallocate_list_data deallocator)
{
    if (!list)
        return;

    regina_list_clear(list, deallocator);
    regina_free(list);
}

regina_list_node_t *regina_list_remove_at(regina_list_t *list, regina_list_node_t *it, deallocate_list_data deallocator)
{
    regina_list_node_t *next = NULL;

    REGINA_RETURN_IF_NIL3(list, it, list->size, NULL);

    next = it->next;
    if (list->head == it) {
        list->head = next;
    }
    if (list->tail == it) {
        list->tail = it->prev;
    }

    set_next_neighbor(it->prev, next);
    destroy_node(it, deallocator);
    --list->size;

    return next;
}

int regina_list_remove_first(regina_list_t *list, match_predicate pred, void *context, deallocate_list_data deallocator)
{
    regina_list_node_t *it = NULL;

    REGINA_RETURN_IF_NIL3(list, pred, list->size, REGINA_ERR_BADPARAM);

    it = regina_list_find_next(regina_list_begin(list), pred, context);
    if (it) {
        if (it == list->head) {
            list->head = it->next;
        }
        if (it == list->tail) {
            list->tail = list->tail->prev;
        }

        set_next_neighbor(it->prev, it->next);
        destroy_node(it, deallocator);
        --list->size;

        return REGINA_ERR_NONE;
    }

    return REGINA_ERR_NOT_FOUND;
}

void regina_list_set_data_at(regina_list_node_t *it, void *data, deallocate_list_data deallocator)
{
    if (!it)
        return;

    if (deallocator) {
        (*deallocator)(it->data);
    } else {
        regina_free(it->data);
    }
    it->data = data;
}

regina_list_node_t *regina_list_find_next(regina_list_node_t *from, match_predicate pred, void *context)
{
    REGINA_RETURN_IF_NIL2(from, pred, NULL);
    while (from) {
        if (pred(from->data, context)) {
            return from;
        }
        from = from->next;
    }
    return NULL;
}

void regina_list_for_each(regina_list_node_t *first, regina_list_node_t *last, process_data process, void *context)
{
    if (!first || !last || !process)
        return;

    while (first) {
        process(first->data, context);
        if (first == last) {
            break;
        }

        first = regina_list_next(first);
    }
}

static regina_list_node_t *regina_split_util(regina_list_node_t *head);


static regina_list_node_t *regina_merge_util(regina_list_node_t *first, regina_list_node_t *second, match_predicate pred)
{
    REGINA_RETURN_IF_NIL(first, second);
    REGINA_RETURN_IF_NIL(second, first)

    if (pred(first->data, second->data)) {

        first->next = regina_merge_util(first->next, second, pred);
        first->next->prev = first;
        first->prev = NULL;

        return first;
    } else {
        second->next = regina_merge_util(first, second->next, pred);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

static regina_list_node_t *regina_merge_sort(regina_list_node_t *head, match_predicate pred)
{
    regina_list_node_t *second = NULL;

    REGINA_RETURN_IF_NIL2(head, head->next, head);

    second = regina_split_util(head);

    head = regina_merge_sort(head, pred);
    second = regina_merge_sort(second, pred);

    head = regina_merge_util(head, second, pred);
    return head;
}

static regina_list_node_t *regina_split_util( regina_list_node_t *head)
{
    regina_list_node_t *temp = NULL;
    regina_list_node_t *fast = head;
    regina_list_node_t *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    temp = slow->next;
    slow->next = NULL;

    return temp;
}

void regina_list_sort(regina_list_t *list, match_predicate pred)
{
    regina_list_node_t *node = regina_merge_sort(regina_list_begin(list), pred);
    list->head =  node;
    while (node->next) {
        node = node->next;
    }
    list->tail = node;
}

static regina_list_node_t *regina_list_get_node_by_index(regina_list_t *list, size_t index)
{
    regina_list_node_t *cursor;

    for (cursor = regina_list_begin(list); cursor;
         cursor = regina_list_next(cursor)) {
        if (0 == index--)
            return cursor;
    }
    return NULL;
}

REGINA_API void regina_list_shuffle(regina_list_t *list)
{
    uint32_t pos;
    size_t i;

    size_t count = regina_list_get_size(list);
    if (0 == count)
        return;

    pos = (uint32_t)regina_time(NULL);

    for (i = 0; i < count; ++i) {
        regina_list_node_t *n1;
        regina_list_node_t *n2;
        void *tmp;

        n1 = regina_list_get_node_by_index(list, i);

        pos = i + REGINA_RANDOM(pos) % ((uint32_t)(count - i));
        n2 = regina_list_get_node_by_index(list, pos);

        /* assert(NULL != n1 && NULL != n2) */
        tmp = n1->data;
        n1->data = n2->data;
        n2->data = tmp;
    }
}
