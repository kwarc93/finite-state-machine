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
/* Watch (clock) object */

struct watch
{
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;

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
    WATCH_EVENT_1S_TICK,

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
    printf("Time: %02u:%02u:%02u\n", watch.hours, watch.minutes, watch.seconds);
}

static void set_minutes_state_set_evt_h(void)
{

}

static void idle_state_set_evt_h(void)
{

}

static void idle_state_tick_evt_h(void)
{
    unsigned int m = watch.minutes;
    unsigned int h = watch.hours;

    if (watch.seconds == 59)
    {
        m = (watch.minutes + 1) % 60;

        if (watch.minutes == 59)
        {
            h = (watch.hours + 1) % 24;
        }
    }

    watch.seconds = (watch.seconds + 1) % 60;
    watch.minutes = m;
    watch.hours = h;

    printf("Time: %02u:%02u:%02u\n", watch.hours, watch.minutes, watch.seconds);
}

static void set_hours_state_set_evt_h(void)
{

}

static void set_hours_state_inc_evt_h(void)
{
    watch.hours = (watch.hours + 1) % 24;
    printf("Hours: %02u\n", watch.hours);
}

static void set_minutes_state_inc_evt_h(void)
{
    watch.minutes = (watch.minutes + 1) % 60;
    printf("Minutes: %02u\n", watch.minutes);
}

//-------------------------------------------------------------------------

static const struct fsm_event watch_fsm_transitions[WATCH_STATE_MAX][WATCH_EVENT_MAX] =
{
    [WATCH_STATE_IDLE] = { {WATCH_STATE_SET_HOURS, idle_state_set_evt_h}, {WATCH_STATE_IDLE, NULL}, {WATCH_STATE_IDLE, idle_state_tick_evt_h} },
    [WATCH_STATE_SET_HOURS] = { {WATCH_STATE_SET_MINUTES, set_hours_state_set_evt_h}, {WATCH_STATE_SET_HOURS, set_hours_state_inc_evt_h}, {WATCH_STATE_SET_HOURS, NULL} },
    [WATCH_STATE_SET_MINUTES] = { {WATCH_STATE_IDLE, set_minutes_state_set_evt_h}, {WATCH_STATE_SET_MINUTES, set_minutes_state_inc_evt_h}, {WATCH_STATE_SET_MINUTES,NULL} },
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
            case 't':
                event = WATCH_EVENT_1S_TICK;
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
