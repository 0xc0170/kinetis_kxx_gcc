/*
 * File: main.c
 *
 * Copyright (c) 2013, 0xc0170
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 *
 */
#include "common.h"
#include "system_k20.h"
#include "main.h"

static void gpio_init(void);
static void delay(void);

/**
 * \brief  LED1 Toggle with a delay, button SW1 toggles LED2 (ISR)
 *
 * \param  void
 *
 * \return void
 */
int main(void)
{
  SystemInit();

  NVICICPR2 |= 1 << (89 & 0x1F);
  NVICISER2 |= 1 << (89 & 0x1F);

  gpio_init();

  LED1_OFF;
  LED2_OFF;

  while(1)
  {
    LED1_TOGGLE;

    delay();
  }
}

/**
 * \brief  Button and LED initialization
 *
 * \param  void
 *
 * \return void
 */
void gpio_init(void)
{
  PORTC_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0xA) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
  PORTC_PCR9 = PORT_PCR_MUX(1);
  PORTC_PCR10 = PORT_PCR_MUX(1);
  GPIOC_PDDR = (LED1 | LED2);
}

/**
 * \brief  Port C ISR Handler
 *
 * \param  void
 *
 * \return void
 */
void PORTC_IRQHandler(void)
{
  PORTC_ISFR = 0xFFFFFFFF;
  LED2_TOGGLE;
}

/**
 * \brief  Silly delay
 *
 * \param  void
 *
 * \return void
 */
void delay(void)
{
  volatile unsigned int i,j;

  for(i=0; i<50000; i++)
  {
	for(j=0; j<100; j++)
      __asm__("nop");
  }
}

