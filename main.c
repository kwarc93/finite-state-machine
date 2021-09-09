/*
 * main.c
 *
 *  Created on: 8 wrz 2021
 *      Author: kwarc
 */

#include <stdlib.h>
#include <stdio.h>

#include "fsm.h"

//-------------------------------------------------------------------------
/* Watch object */

struct watch
{
    unsigned int hours;
    unsigned int minutes;

    struct fsm_obj *fsm;
} watch;

//-------------------------------------------------------------------------
/* Watch states, events & handlers */

enum watch_state
{
    WATCH_STATE_IDLE = 0,
    WATCH_STATE_SET_HOURS,
    WATCH_STATE_SET_MINUTES,

    WATCH_STATE_MAX
};

enum watch_event
{
    WATCH_EVENT_SET = 0,
    WATCH_EVENT_INC,

    WATCH_EVENT_MAX
};

static void on_entry_set_hours_state(void)
{
    printf("Hours: %02u\n", watch.hours);
}

static void on_entry_minutes_state(void)
{
    printf("Minutes: %02u\n", watch.minutes);
}

static void on_entry_idle_state(void)
{
    printf("Time: %02u:%02u\n", watch.hours, watch.minutes);
}

static unsigned int goto_idle(void)
{
    return WATCH_STATE_IDLE;
}

static unsigned int goto_set_hours(void)
{
    return WATCH_STATE_SET_HOURS;
}

static unsigned int goto_set_minutes(void)
{
    return WATCH_STATE_SET_MINUTES;
}

static unsigned int increase_hours(void)
{
    watch.hours = (watch.hours + 1) % 24;
    return WATCH_STATE_SET_HOURS;
}

static unsigned int increase_minutes(void)
{
    watch.minutes = (watch.minutes + 1) % 60;
    return WATCH_STATE_SET_MINUTES;
}

//-------------------------------------------------------------------------

static const fsm_event_handler_t watch_fsm_transitions[WATCH_STATE_MAX][WATCH_EVENT_MAX] =
{
    [WATCH_STATE_IDLE] = { goto_set_hours, NULL },
    [WATCH_STATE_SET_HOURS] = { goto_set_minutes, increase_hours },
    [WATCH_STATE_SET_MINUTES] = { goto_idle, increase_minutes },
};

static const struct fsm_state_handlers watch_state_handlers[] =
{
    [WATCH_STATE_IDLE] = {on_entry_idle_state, NULL},
    [WATCH_STATE_SET_HOURS] = {on_entry_set_hours_state, NULL},
    [WATCH_STATE_SET_MINUTES] = {on_entry_minutes_state, NULL},
};

//-------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    watch.hours = 0;
    watch.minutes = 0;
    watch.fsm = fsm_init(WATCH_STATE_MAX, WATCH_EVENT_MAX, WATCH_STATE_IDLE, watch_fsm_transitions, watch_state_handlers);

    while (1)
    {
        char c = getchar();
        unsigned int event = WATCH_EVENT_MAX;

        switch (c)
        {
            case 's':
                event = WATCH_EVENT_SET;
                break;
            case 'i':
                event = WATCH_EVENT_INC;
                break;
            case 'e':
                return 0;
                break;
            default:
                break;
        }

        fsm_process(watch.fsm, event);
    }

    return 0;
}
