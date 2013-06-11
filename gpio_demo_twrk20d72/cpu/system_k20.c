/*
 * File: system_k20.c
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

int core_clk_mhz = 80;
int periph_clk_khz;

/**
 * \brief  Clock initialization (80 MHz)
 *
 * \param  void
 *
 * \return void
 */
void SystemInit(void)
{
  uint32_t temp_pfapr, i, temp_reg;

  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
              | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK
              | SIM_SCGC5_PORTE_MASK);

  SIM_CLKDIV1 = ( 0 | SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0)
                          | SIM_CLKDIV1_OUTDIV4(1));

  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE0_MASK | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK);

  temp_reg |= (MCG_C2_RANGE0(1) | (0 << MCG_C2_HGO0_SHIFT) | (1 << MCG_C2_EREFS0_SHIFT));
  MCG_C2 = temp_reg;


  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK);
  temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(0);
  MCG_C1 = temp_reg;

  for (i = 0 ; i < 10000 ; i++)
  {
      if (MCG_S & MCG_S_OSCINIT0_MASK) break;
  }

  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break;
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return;

  MCG_C6 |= MCG_C6_CME0_MASK;

  temp_reg = MCG_C5;
  temp_reg &= ~MCG_C5_PRDIV0_MASK;
  temp_reg |= MCG_C5_PRDIV0(3);
  MCG_C5 = temp_reg;

    temp_reg = MCG_C6;
  temp_reg &= ~MCG_C6_VDIV0_MASK;
  temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(11);
  MCG_C6 = temp_reg;

  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_PLLST_MASK) break;
  }
  if (!(MCG_S & MCG_S_PLLST_MASK)) return;

  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK0_MASK) break;
  }
  if (!(MCG_S & MCG_S_LOCK0_MASK)) return;

  MCG_C1 &= ~MCG_C1_CLKS_MASK;

  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break;
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return;
}
