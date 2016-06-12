/*****
*
* Copyright (C) 2005-2016 CS-SI. All Rights Reserved.
* Author: Yoann Vandoorselaere <yoann.v@libiodef-ids.com>
*
* This file is part of the LibIodef library.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*****/

/*
 * This is a blind rewrite of the kernel linked list handling code,
 * done so that we can dual-license libiodef. The code still look
 * pretty similar, but there is no way to write such list implementation
 * in many different manner.
 */

#ifndef HAVE_LIBIODEF_LIBIODEF_LIST_H
#define HAVE_LIBIODEF_LIBIODEF_LIST_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "libiodef-inttypes.h"

#ifdef __cplusplus
  extern "C" {
#endif

#define LIBIODEF_LIST(item) libiodef_list_t (item) = { &(item), &(item) }


typedef struct libiodef_list {
        struct libiodef_list *next;
        struct libiodef_list *prev;
} libiodef_list_t;



static inline void __libiodef_list_splice(const libiodef_list_t *list, libiodef_list_t *prev, libiodef_list_t *next)
{
         libiodef_list_t *first = list->next, *last = list->prev;

         first->prev = prev;
         prev->next = first;

         last->next = next;
         next->prev = last;
}



static inline void __libiodef_list_add(libiodef_list_t *item, libiodef_list_t *prev, libiodef_list_t *next)
{
        prev->next = item;
        item->prev = prev;
        item->next = next;
        next->prev = item;
}


/**
 * libiodef_list_init:
 * @item: Pointer to a #libiodef_list_t object.
 *
 * Initialize @item. Note that this is only required if @item
 * is the head of a list, but might also be useful in case you
 * want to use libiodef_list_del_init().
 */
static inline void libiodef_list_init(libiodef_list_t *item)
{
        item->next = item->prev = item;
}



/**
 * libiodef_list_is_empty:
 * @item: Pointer to a #libiodef_list_t object.
 *
 * Check whether @item is empty or not.
 *
 * Returns: TRUE if @item is empty, FALSE otherwise.
 */
static inline libiodef_bool_t libiodef_list_is_empty(libiodef_list_t *item)
{
        return (item->next == item) ? LIBIODEF_BOOL_TRUE : LIBIODEF_BOOL_FALSE;
}



/**
 * libiodef_list_splice_tail:
 * @head: Pointer to a #libiodef_list_t list.
 * @list: Pointer to a #libiodef_list_t object to add to @head.
 *
 * All item from list @list are added at the beginning of @head.
 */
static inline void libiodef_list_splice(libiodef_list_t *head, libiodef_list_t *list)
{
        if ( ! libiodef_list_is_empty(list) )
                 __libiodef_list_splice(list, head, head->next);
}



/**
 * libiodef_list_splice_tail:
 * @head: Pointer to a #libiodef_list_t list.
 * @list: Pointer to a #libiodef_list_t object to add to @head.
 *
 * All item from list @list are added at the end of @head.
 */
static inline void libiodef_list_splice_tail(libiodef_list_t *head, libiodef_list_t *list)
{
        if ( ! libiodef_list_is_empty(list) )
                 __libiodef_list_splice(list, head->prev, head);
}



/**
 * libiodef_list_add:
 * @head: Pointer to a #libiodef_list_t list.
 * @item: Pointer to a #libiodef_list_t object to add to @head.
 *
 * Add @item at the beginning of @head list.
 */
static inline void libiodef_list_add(libiodef_list_t *head, libiodef_list_t *item)
{
        __libiodef_list_add(item, head, head->next);
}



/**
 * libiodef_list_add_tail:
 * @head: Pointer to a #libiodef_list_t list.
 * @item: Pointer to a #libiodef_list_t object to add to @head.
 *
 * Add @item at the tail of @head list.
 */
static inline void libiodef_list_add_tail(libiodef_list_t *head, libiodef_list_t *item)
{
        __libiodef_list_add(item, head->prev, head);
}



/**
 * libiodef_list_del:
 * @item: Pointer to a #libiodef_list_t object.
 *
 * Delete @item from the list it is linked in.
 */
static inline void libiodef_list_del(libiodef_list_t *item)
{
        item->prev->next = item->next;
        item->next->prev = item->prev;
}



/**
 * libiodef_list_del_init:
 * @item: Pointer to a #libiodef_list_t object.
 *
 * Delete @item from the list it is linked in, and reinitialize
 * @item member so that the list can be considered as empty.
 */
static inline void libiodef_list_del_init(libiodef_list_t *item)
{
        item->prev->next = item->next;
        item->next->prev = item->prev;
        libiodef_list_init(item);
}


/**
 * libiodef_list_entry:
 * @item: Pointer to a #libiodef_list_t object to retrieve the entry from.
 * @type: Type of the entry to retrieve.
 * @member: List member in @type used to link it to a list.
 *
 * Retrieve the entry of type @type from the #libiodef_list_t object @tmp,
 * using the item list member @member. Returns the entry associated with @item.
 */
#define libiodef_list_entry(item, type, member)                             \
        (type *) ((unsigned long) item - (unsigned long) &((type *) 0)->member)



/**
 * libiodef_list_for_each:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry. libiodef_list_entry() can be used to retrieve
 * and entry from the @pos pointer. It is not safe to call libiodef_list_del() while
 * iterating using this function, see libiodef_list_for_each_safe().
 */
#define libiodef_list_for_each(list, pos)                                   \
        for ( (pos) = (list)->next; (pos) != (list); (pos) = (pos)->next )


/**
 * libiodef_list_for_each_safe:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libiodef_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry. libiodef_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libiodef_list_del() while iterating the
 * list is safe.
 */
#define libiodef_list_for_each_safe(list, pos, bkp)                         \
        for ( (pos) = (list)->next, (bkp) = (pos)->next; (pos) != (list); (pos) = (bkp), (bkp) = (pos)->next )



/**
 * libiodef_list_for_each_reversed:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry in reverse order. libiodef_list_entry() can be
 * used to retrieve and entry from the @pos pointer. It is not safe to call
 * libiodef_list_del() while iterating using this function, see
 * libiodef_list_for_each_reversed_safe().
 */
#define libiodef_list_for_each_reversed(list, pos)                          \
        for ( (pos) = (list)->prev; (pos) != (list); (pos) = (pos)->prev )



/**
 * libiodef_list_for_each_reversed_safe:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libiodef_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry in reverse order. libiodef_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libiodef_list_del() while iterating the
 * list is safe.
 */
#define libiodef_list_for_each_reversed_safe(list, pos, bkp)                \
        for ( (pos) = (list)->prev, (bkp) = (pos)->prev; (pos) != (list); (pos) = (bkp), (bkp) = (pos)->prev )


/**
 * libiodef_list_for_each_continue:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 *
 * Iterate through all @list entry starting from @pos if it is not NULL, or from
 * the start of @list if it is. libiodef_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libiodef_list_del() while iterating the
 * list is not safe.
 */
#define libiodef_list_for_each_continue(list, pos)                          \
        for ( (pos) = ((pos) == NULL) ? (list)->next : (pos)->next; (pos) != (list); (pos) = (pos)->next )


/**
 * libiodef_list_for_each_continue_safe:
 * @list: Pointer to a #libiodef_list_t list.
 * @pos: Pointer to a #libiodef_list_t object pointing to the current list member.
 * @bkp: Pointer to a #libiodef_list_t object pointing to the next list member.
 *
 * Iterate through all @list entry starting from @pos if it is not NULL, or from
 * the start of @list if it is. libiodef_list_entry() can be used to retrieve
 * and entry from the @pos pointer. Calling libiodef_list_del() while iterating the
 * list is safe.
 */
#define libiodef_list_for_each_continue_safe(list, pos, bkp)                \
        for ( (pos) = ((bkp) == NULL) ? (list)->next : (bkp); (bkp) = (pos)->next, (pos) != (list); (pos) = (bkp) )



#define libiodef_list_get_next(list, pos, class, member) \
        pos ? \
                ((pos)->member.next == (list)) ? NULL : \
                                libiodef_list_entry((pos)->member.next, class, member) \
        : \
                ((list)->next == (list)) ? NULL : \
                                libiodef_list_entry((list)->next, class, member)


#define libiodef_list_get_next_safe(list, pos, bkp, class, member)                                                                \
        pos ?                                                                                                            \
              (((pos) = bkp),                                                                                            \
               ((bkp) = (! (bkp) || (bkp)->member.next == list) ? NULL : libiodef_list_entry((pos)->member.next, class, member)), \
               (pos))                                                                                                    \
        :                                                                                                                \
              (((pos) = ((list)->next == list) ? NULL : libiodef_list_entry((list)->next, class, member)),                        \
               ((bkp) = (! (pos) ||(pos)->member.next == list ) ? NULL : libiodef_list_entry((pos)->member.next, class, member)), \
               (pos))


#ifdef __cplusplus
  }
#endif

#endif
