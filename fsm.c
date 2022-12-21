/*
 * fsm.c
 *
 *  Created on: 8 wrz 2021
 *      Author: kwarc
 */

#include "fsm.h"

#include <stdlib.h>
#include <stddef.h>

//-----------------------------------------------------------------------------

struct fsm_obj
{
    unsigned int current_state;
    unsigned int states_cnt, events_cnt;

    const struct fsm_event *transition_matrix;
    const struct fsm_state_handlers *state_handlers_array;
};

//-----------------------------------------------------------------------------

struct fsm_obj *fsm_init(unsigned int states_cnt, unsigned int events_cnt, unsigned int initial_state,
                         const struct fsm_event transision_matrix[states_cnt][events_cnt],
                         const struct fsm_state_handlers state_handlers_array[states_cnt])
{
    if (transision_matrix == NULL)
        return NULL;

    struct fsm_obj *obj = malloc(sizeof(struct fsm_obj));
    if (obj == NULL)
        return NULL;

    obj->current_state = initial_state;
    obj->states_cnt = states_cnt;
    obj->events_cnt = events_cnt;
    obj->transition_matrix = &transision_matrix[0][0];

    /* Its optional */
    if (state_handlers_array != NULL)
    {
        obj->state_handlers_array = &state_handlers_array[0];
        if (obj->state_handlers_array[obj->current_state].on_entry != NULL)
            obj->state_handlers_array[obj->current_state].on_entry();
    }

    return obj;
}

//-----------------------------------------------------------------------------

void fsm_deinit(struct fsm_obj *const obj)
{
    free(obj);
}

//-----------------------------------------------------------------------------

void fsm_process(struct fsm_obj *const obj, unsigned int event)
{
    if (obj == NULL)
        return;

    if (obj->current_state >= obj->states_cnt || event >= obj->events_cnt)
        return;

    fsm_event_handler_t on_event = obj->transition_matrix[obj->current_state * obj->events_cnt + event].on_event;
    unsigned int next_state = obj->transition_matrix[obj->current_state * obj->events_cnt + event].next_state;
    unsigned int state_changed = obj->current_state != next_state;

    if (state_changed && obj->state_handlers_array[obj->current_state].on_exit != NULL)
        obj->state_handlers_array[obj->current_state].on_exit();

    if (on_event != NULL)
        on_event();

    obj->current_state = next_state;

    if (state_changed && obj->state_handlers_array[obj->current_state].on_entry != NULL)
        obj->state_handlers_array[obj->current_state].on_entry();
}

//-----------------------------------------------------------------------------
