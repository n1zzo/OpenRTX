/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN                             *
 *                         Silvano Seva IU2KWO                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <platform.h>
#include <gpio.h>
#include <os.h>
#include "hwconfig.h"

#ifdef ENABLE_BKLIGHT_DIMMING
void TIM1_TRG_COM_TIM11_IRQHandler()
{
    OSIntEnter();

    if(TIM11->SR & TIM_SR_CC1IF)
    {
        gpio_clearPin(LCD_BKLIGHT); /* Clear pin on compare match */
    }

    if(TIM11->SR & TIM_SR_UIF)
    {
        gpio_setPin(LCD_BKLIGHT);   /* Set pin on counter reload */
    }

    TIM11->SR = 0;

    OSIntExit();
}
#endif

void platform_init()
{
    /* Configure GPIOs */
    gpio_setMode(GREEN_LED, OUTPUT);
    gpio_setMode(RED_LED,   OUTPUT);

    gpio_setMode(LCD_BKLIGHT, OUTPUT);
    gpio_clearPin(LCD_BKLIGHT);

    #ifdef ENABLE_BKLIGHT_DIMMING
    /*
     * Configure TIM11 for backlight PWM: Fpwm = 256Hz, 8 bit of resolution
     * APB2 freq. is 84MHz, then: PSC = 1281 to have Ftick = 65.52kHz
     * With ARR = 256, Fpwm is 256Hz;
     */
    RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    TIM11->ARR = 255;
    TIM11->PSC = 1282;
    TIM11->CNT = 0;
    TIM11->CR1   |= TIM_CR1_ARPE;
    TIM11->CCMR1 |= TIM_CCMR1_OC1M_2
                 |  TIM_CCMR1_OC1M_1
                 |  TIM_CCMR1_OC1PE;
    TIM11->CCER  |= TIM_CCER_CC1E;
    TIM11->CCR1 = 0;
    TIM11->EGR  = TIM_EGR_UG;        /* Update registers            */
    TIM11->SR   = 0;                 /* Clear interrupt flags       */
    TIM11->DIER = TIM_DIER_CC1IE     /* Interrupt on compare match  */
                | TIM_DIER_UIE;      /* Interrupt on counter reload */
    TIM11->CR1 |= TIM_CR1_CEN;       /* Start timer                 */

    NVIC_ClearPendingIRQ(TIM1_TRG_COM_TIM11_IRQn);
    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn,15);
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    #endif
}

void platform_terminate()
{
    /* Shut down backlight */
    gpio_clearPin(LCD_BKLIGHT);

    gpio_clearPin(GREEN_LED);
    gpio_clearPin(RED_LED);

    #ifdef ENABLE_BKLIGHT_DIMMING
    RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;
    #endif
}

float platform_getVbat()
{
    return 0.0f;
}

float platform_getMicLevel()
{
    return 0.0f;
}

float platform_getVolumeLevel()
{
    return 0.0f;
}

uint8_t platform_getChSelector()
{
    return 0.0f;
}

bool platform_getPttStatus()
{
    return false;
}

void platform_ledOn(led_t led)
{
    switch(led)
    {
        case GREEN:
            gpio_setPin(GREEN_LED);
            break;

        case RED:
            gpio_setPin(RED_LED);
            break;

        default:
            break;
    }
}

void platform_ledOff(led_t led)
{
    switch(led)
    {
        case GREEN:
            gpio_clearPin(GREEN_LED);
            break;

        case RED:
            gpio_clearPin(RED_LED);
            break;

        default:
            break;
    }
}

void platform_beepStart(uint16_t freq)
{
    (void) freq;
}

void platform_beepStop()
{
    
}

void platform_setBacklightLevel(uint8_t level)
{
    /*
     * Little workaround for the following nasty behaviour: if CCR1 value is
     * zero, a waveform with 99% duty cycle is generated. This is because we are
     * emulating pwm with interrupts.
     */
    if(level > 1)
    {
        #ifdef ENABLE_BKLIGHT_DIMMING
        TIM11->CCR1 = level;
        TIM11->CR1 |= TIM_CR1_CEN;
        #else
        gpio_setPin(LCD_BKLIGHT);
        #endif
    }
    else
    {
        #ifdef ENABLE_BKLIGHT_DIMMING
        TIM11->CR1 &= ~TIM_CR1_CEN;
        #endif
        gpio_clearPin(LCD_BKLIGHT);
    }
}
