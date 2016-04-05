/******************************************************************************
* Copyright (C) 2015, Scanimetrics Inc - All Rights Reserved                  *
*                                                                             *
* Unauthorized copying of this file, via any medium is strictly prohibited.   *
* Proprietary and confidential.                                               *
******************************************************************************/
/******************************************************************************
* This implements a simple rpl-IPv6 node which periodically pings a specific  *
* static address                                                              *
*                                                                             *
* Author - Billy Kozak, Scanimetrics                                          *
* Created - Wed. Oct. 28th, 2015                                              *
******************************************************************************/
/******************************************************************************
*                                   INCLUDES                                  *
******************************************************************************/
#include "contiki.h"

#include "etimer.h"
#include "clock.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#if CONTIKI_TARGET_SRF06_CC26XX && BOARD_SENSORTAG
#include "sensortag/button-sensor.h"
#include "sensortag/buzzer.h"
#elif CONTIKI_TARGET_SRF06_CC26XX
#include "srf06/button-sensor.h"
#else
#include "dev/button-sensor.h"
#endif
/******************************************************************************
*                                    DEFINES                                  *
******************************************************************************/
#if (BOARD_SENSORTAG)
#define SET_CLOCK_BUTTON button_left_sensor
#elif ( CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_SRF06_CC26XX)
#define SET_CLOCK_BUTTON button_select_sensor
#else
#define SET_CLOCK_BUTTON button_sensor
#endif

#define DEBUG 0
#if defined(DEBUG) && (DEBUG)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...) do {} while (0)
#endif

#define CLOCK_COUNTDOWN (60 * CLOCK_SECOND)
/******************************************************************************
*                                    GLOBALS                                  *
******************************************************************************/
static struct etimer countdown_timer;

static unsigned long start_seconds;
static clock_time_t  start_ticks;
/******************************************************************************
*                              FUNCTION PROTOTYPES                            *
******************************************************************************/
PROCESS(clock_test_process, "clock test process");
AUTOSTART_PROCESSES(&clock_test_process);

static void print_results(void);
static void print_results(void);
/******************************************************************************
*                             FUNCTION DEFINITIONS                            *
******************************************************************************/
static int clock_start_event(process_event_t ev,process_data_t data)
{
	return (ev == sensors_event) && (data == &(SET_CLOCK_BUTTON));
}
/*****************************************************************************/
static void print_results(void)
{
	unsigned long end_seconds;
	clock_time_t  end_ticks;
	unsigned long dff_seconds;
	clock_time_t  diff_ticks;

	end_seconds = clock_seconds();
	end_ticks   = clock_time();

	dff_seconds = end_seconds - start_seconds;
	diff_ticks  = end_ticks - start_ticks;

	printf("clock measure results:\n");
	printf("start - %lu secs (%lu ticks)\n", start_seconds, start_ticks);
	printf("ended - %lu secs (%lu ticks)\n", end_seconds, end_ticks);
	printf("diff - %lu secs (%lu ticks)\n", dff_seconds, diff_ticks);
}
/*****************************************************************************/
static void start_test(void)
{
	start_seconds = clock_seconds();
	start_ticks   = clock_time();
	printf("clock test started\n");
}
/*****************************************************************************/
PROCESS_THREAD(clock_test_process, ev, data)
{
	PROCESS_BEGIN();

	while(1) {
		PROCESS_WAIT_UNTIL(clock_start_event(ev, data));
		etimer_set(&countdown_timer, CLOCK_COUNTDOWN);

		start_test();

		PROCESS_WAIT_UNTIL(etimer_expired(&countdown_timer));

		print_results();
	}

	PROCESS_END();
}
/*****************************************************************************/
