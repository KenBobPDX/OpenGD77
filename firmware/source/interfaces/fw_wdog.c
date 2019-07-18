/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "fw_wdog.h"
#include "fw_pit.h"

TaskHandle_t fwwatchdogTaskHandle;

static WDOG_Type *wdog_base = WDOG;
int watchdog_refresh_tick=0;

volatile bool alive_maintask;
volatile bool alive_beeptask;
volatile bool alive_hrc6000task;

int battery_voltage = 0;
int battery_voltage_tick = 0;

void init_watchdog()
{
    wdog_config_t config;
    WDOG_GetDefaultConfig(&config);
    config.timeoutValue = 0x3ffU;
    WDOG_Init(wdog_base, &config);
    for (uint32_t i = 0; i < 256; i++)
    {
    	wdog_base->RSTCNT;
    }

    watchdog_refresh_tick=0;

    alive_maintask = false;
    alive_beeptask = false;
    alive_hrc6000task = false;

	battery_voltage=get_battery_voltage();
	battery_voltage_tick=0;

	xTaskCreate(fw_watchdog_task,                        /* pointer to the task */
				"fw watchdog task",                      /* task name for kernel awareness debugging */
				1000L / sizeof(portSTACK_TYPE),      /* task stack size */
				NULL,                      			 /* optional task startup argument */
				5U,                                  /* initial priority */
				fwwatchdogTaskHandle					 /* optional task handle to create */
				);
}

void fw_watchdog_task()
{
    while (1U)
    {
    	taskENTER_CRITICAL();
    	uint32_t tmp_timer_watchdogtask=timer_watchdogtask;
    	taskEXIT_CRITICAL();
    	if (tmp_timer_watchdogtask==0)
    	{
        	taskENTER_CRITICAL();
        	timer_watchdogtask=10;
        	taskEXIT_CRITICAL();

        	tick_watchdog();
    	}

		vTaskDelay(0);
    }
}

void tick_watchdog()
{
	watchdog_refresh_tick++;
	if (watchdog_refresh_tick==200)
	{
		if (alive_maintask && alive_beeptask && alive_hrc6000task)
		{
	    	WDOG_Refresh(wdog_base);
		}
	    alive_maintask = false;
	    alive_beeptask = false;
	    alive_hrc6000task = false;
    	watchdog_refresh_tick=0;
	}

	battery_voltage_tick++;
	if (battery_voltage_tick==2000)
	{
		int tmp_battery_voltage = get_battery_voltage();
		if (battery_voltage!=tmp_battery_voltage)
		{
			battery_voltage=tmp_battery_voltage;
		}
		battery_voltage_tick=0;
	}
}
