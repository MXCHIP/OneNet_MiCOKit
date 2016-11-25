
#ifndef REGINA_LIST_H_
#define REGINA_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

typedef struct regina_list_node_t regina_list_node_t;
typedef struct regina_list_t regina_list_t;

/**
 * @brief Return @b zero if data doesn't match search criteria.
 */
typedef int (*match_predicate)(void *data, void *context);

/**
 * @brief Use to deallocate list node data.
 */
typedef void (*deallocate_list_data)(void *);

/**
 * @brief Use to process element data.
 */
typedef void (*process_data)(void *data, void *context);

/**
 * @brief Creates empty list.
 * @return The list object.
 */
REGINA_API regina_list_t *regina_list_create(void);

/**
 * @brief Destroys list and all elements.
 */
REGINA_API void regina_list_destroy(regina_list_t *list, deallocate_list_data deallocator);

/**
 * @brief Removes all elements from the list (which are destroyed), and leaving the list with a size of 0.
 */
REGINA_API void regina_list_clear(regina_list_t *list, deallocate_list_data deallocator);

/**
 * @brief Returns the number of elements in the list.
 * @return The number of elements or NULL if the list is NULL.
 */
REGINA_API size_t regina_list_get_size(regina_list_t *list);

/**
 * @brief Inserts a new element at the beginning of the list, right before its current first element.
 * @return An iterator to the inserted element or NULL if the list or data are NULL.
 */
REGINA_API regina_list_node_t *regina_list_push_front(regina_list_t *list, void *data);

/**
 * @brief Inserts a new element at the end of the list, after its current last element.
 * @return An iterator to the inserted element or NULL if the list or data are NULL.
 */
REGINA_API regina_list_node_t *regina_list_push_back(regina_list_t *list, void *data);

/**
 * @brief Returns an iterator pointing to the first element in the list.
 * @return An iterator or NULL if the list is NULL.
 */
REGINA_API regina_list_node_t *regina_list_begin(regina_list_t *list);

/**
 * @brief Returns an iterator pointing to the last element in the list.
 * @return An iterator or NULL if the list is NULL.
 */
REGINA_API regina_list_node_t *regina_list_back(regina_list_t *list);

/**
 * @brief Gets iterator to the next element.
 * @return An iterator or NULL if the provided iterator is NULL.
 */
REGINA_API regina_list_node_t *regina_list_next(regina_list_node_t *it);

/**
 * @brief Gets iterator to the previous element.
 * @return An iterator or NULL if the provided iterator is NULL.
 */
REGINA_API regina_list_node_t *regina_list_prev(regina_list_node_t *it);

/**
 * @brief Gets data from the iterator.
 * @return Data or NULL if the iterator is NULL.
 */
REGINA_API void *regina_list_get_data(regina_list_node_t *it);

/**
 * @brief Sets new data to the element. Old data will be destroyed.
 */
REGINA_API void regina_list_set_data_at(regina_list_node_t *it, void *data, deallocate_list_data deallocator);

/**
 * @brief Returns an iterator to the first element in the list that matches by the predicate.
 * If no such element is found, the function returns NULL.
 */
REGINA_API regina_list_node_t *regina_list_find_next(regina_list_node_t *from, match_predicate pred, void *context);

/**
 * @brief Merges the source list into the destination list by transferring all of its elements at their respective
 * ordered positions at the end of the source list.
 * @return The result list which contains all merged elements.
 */
REGINA_API regina_list_t *regina_lists_merge(regina_list_t *destination, regina_list_t *source);

/**
 * @brief Removes from the list a single element.
 * @return An iterator pointing to the element that followed the last element erased by the function call or NULL.
 */
REGINA_API regina_list_node_t *regina_list_remove_at(regina_list_t *list, regina_list_node_t *it, deallocate_list_data deallocator);

/**
 * @brief Removes from the list the first element for which the predicate returns true.
 * @return REGINA_ERR_NONE if element was found.
 */
REGINA_API int regina_list_remove_first(regina_list_t *list, match_predicate pred, void *context, deallocate_list_data deallocator);

/**
 * @brief Applies the function process to each of the elements in the range [first,last].
 */
REGINA_API void regina_list_for_each(regina_list_node_t *first, regina_list_node_t *last, process_data process, void *context);

/**
 * @brief Sorts list according to predicate condition.
 * @param pred  Predicate that is used to sort list.
 */
REGINA_API void regina_list_sort(regina_list_t *list, match_predicate pred);

/**
 * @brief Shuffles the list.
 */
REGINA_API void regina_list_shuffle(regina_list_t *list);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* REGINA_LIST_H_ */
