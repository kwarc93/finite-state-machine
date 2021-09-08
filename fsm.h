/*
 * fsm.h
 *
 *  Created on: 8 wrz 2021
 *      Author: kwarc
 */

#ifndef FSM_H_
#define FSM_H_

#include <stdint.h>

//-----------------------------------------------------------------------------

/* Finite State Machine object */
struct fsm_obj;

typedef int (*fsm_handler_t)(void);

struct fsm_state_handlers
{
    fsm_handler_t on_entry;
    fsm_handler_t on_exit;
};

//-----------------------------------------------------------------------------

struct fsm_obj *fsm_init(unsigned int states_cnt, unsigned int events_cnt, int initial_state, const fsm_handler_t transision_matrix[states_cnt][events_cnt], const struct fsm_state_handlers state_handlers_array[states_cnt]);
void fsm_deinit(struct fsm_obj *const obj);
void fsm_process(struct fsm_obj *const obj, int event);

//-----------------------------------------------------------------------------

#endif /* FSM_H_ */
