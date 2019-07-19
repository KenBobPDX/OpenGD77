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

#include "fw_main.h"
#include "menu/menuSystem.h"
#include "menu/menuUtilityQSOData.h"
#include "fw_settings.h"

#if defined(USE_SEGGER_RTT)
#include <SeggerRTT/RTT/SEGGER_RTT.h>
#endif

void fw_main_task();

const char *FIRMWARE_VERSION_STRING = "V0.3.1";
TaskHandle_t fwMainTaskHandle;

void fw_init()
{
	xTaskCreate(fw_main_task,                        /* pointer to the task */
				"fw main task",                      /* task name for kernel awareness debugging */
				5000L / sizeof(portSTACK_TYPE),      /* task stack size */
				NULL,                      			 /* optional task startup argument */
				5U,                                  /* initial priority */
				fwMainTaskHandle					 /* optional task handle to create */
				);

    vTaskStartScheduler();
}

static void show_lowbattery()
{
	UC1701_clearBuf();
	UC1701_printCentered(32, "LOW BATTERY !!!", UC1701_FONT_GD77_8x16);
	UC1701_render();
}

void fw_main_task()
{
	uint32_t keys;
	int key_event;
	uint32_t buttons;
	int button_event;
	
    USB_DeviceApplicationInit();

    // Init I2C
    init_I2C0a();
    setup_I2C0();
    settingsLoadSettings();

	fw_init_common();
	fw_init_buttons();
	fw_init_LEDs();
	fw_init_keyboard();
	fw_init_display();

    // Init SPI
    init_SPI();
    setup_SPI0();
    setup_SPI1();

    // Init I2S
    init_I2S();
    setup_I2S();

    // Init ADC
    adc_init();

    // Init DAC
    dac_init();

    SPI_Flash_init();

    // Init AT1846S
    I2C_AT1846S_init();

    // Init HR-C6000
    SPI_HR_C6000_init();

    // Additional init stuff
    SPI_C6000_postinit();
    I2C_AT1846_Postinit();

    // Init HR-C6000 interrupts
    init_HR_C6000_interrupts();

    // Small startup delay after initialization to stabilize system
    vTaskDelay(portTICK_PERIOD_MS * 500);

	init_pit();

	open_squelch=false;
	HR_C6000_datalogging=false;

	trx_measure_count = 0;

	if (get_battery_voltage()<CUTOFF_VOLTAGE_UPPER_HYST)
	{
		show_lowbattery();
		GPIO_PinWrite(GPIO_Keep_Power_On, Pin_Keep_Power_On, 0);
		while(1U) {};
	}

	init_hrc6000_task();

	init_watchdog();

    fw_init_beep_task();

    set_melody(melody_poweron);

#if defined(USE_SEGGER_RTT)
    SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_TRIM);
    SEGGER_RTT_printf(0,"Segger RTT initialised\n");
#endif

    lastheardInitList();
    menuInitMenuSystem();

    while (1U)
    {
    	taskENTER_CRITICAL();
    	uint32_t tmp_timer_maintask=timer_maintask;
    	taskEXIT_CRITICAL();
    	if (tmp_timer_maintask==0)
    	{
        	taskENTER_CRITICAL();
    		timer_maintask=10;
    	    alive_maintask=true;
        	taskEXIT_CRITICAL();

        	tick_com_request();

        	fw_check_button_event(&buttons, &button_event);// Read button state and event
        	fw_check_key_event(&keys, &key_event);// Read keyboard state and event

        	if (key_event==EVENT_KEY_CHANGE)
        	{
        		if (keys!=0)
        		{
            	    set_melody(melody_key_beep);
        		}
        	}

        	if (button_event==EVENT_BUTTON_CHANGE)
        	{
        		/*
        		if ((buttons & BUTTON_SK1)!=0)
        		{
            	    set_melody(melody_sk1_beep);
        		}
        		else if ((buttons & BUTTON_SK2)!=0)
        		{
            	    set_melody(melody_sk2_beep);
        		}
        		else if ((buttons & BUTTON_ORANGE)!=0)
        		{
            	    set_melody(melody_orange_beep);
        		}
        		*/

        		if (((buttons & BUTTON_PTT)!=0) && (slot_state==DMR_STATE_IDLE))
        		{
        			menuSystemPushNewMenu(MENU_TX_SCREEN);
        		}
        	}

        	menuSystemCallCurrentMenuTick(buttons,keys,(button_event<<1) | key_event);

        	if (((GPIO_PinRead(GPIO_Power_Switch, Pin_Power_Switch)!=0)
        			|| (battery_voltage<CUTOFF_VOLTAGE_LOWER_HYST))
        			&& (menuSystemGetCurrentMenuNumber() != MENU_POWER_OFF))
        	{
				settingsSaveSettings();

        		if (battery_voltage<CUTOFF_VOLTAGE_LOWER_HYST)
        		{
        			show_lowbattery();

                	if (GPIO_PinRead(GPIO_Power_Switch, Pin_Power_Switch)!=0)
					{
        				// This turns the power off to the CPU.
        				GPIO_PinWrite(GPIO_Keep_Power_On, Pin_Keep_Power_On, 0);
        			}
        		}
        		else
        		{
					menuSystemPushNewMenu(MENU_POWER_OFF);
        		}
    		    GPIO_PinWrite(GPIO_speaker_mute, Pin_speaker_mute, 0);
    		    set_melody(NULL);
        	}

    		if (menuDisplayLightTimer > 0)
    		{
    			menuDisplayLightTimer--;
    			if (menuDisplayLightTimer==0)
    			{
    				fw_displayEnableBacklight(false);
    			}
    		}

    		tick_melody();
    	}

		vTaskDelay(0);
    }
}
