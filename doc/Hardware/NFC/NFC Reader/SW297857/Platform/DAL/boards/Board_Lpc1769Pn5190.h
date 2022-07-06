/*----------------------------------------------------------------------------*/
/* Copyright 2017-2021 NXP                                                    */
/*                                                                            */
/* NXP Confidential. This software is owned or controlled by NXP and may only */
/* be used strictly in accordance with the applicable license terms.          */
/* By expressly accepting such terms or by downloading, installing,           */
/* activating and/or otherwise using the software, you are agreeing that you  */
/* have read, and that you agree to comply with and are bound by, such        */
/* license terms. If you do not agree to be bound by the applicable license   */
/* terms, then you may not retain, install, activate or otherwise use the     */
/* software.                                                                  */
/*----------------------------------------------------------------------------*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
*/

#ifndef BOARD_LPC1769PN5190_H
#define BOARD_LPC1769PN5190_H

#include <board.h>

#define PORT0               0          /**< Port0. */
#define PORT1               1          /**< Port1. */
#define PORT2               2          /**< Port2. */
#define PORT3               3          /**< Port3. */
#define PORT4               4          /**< Port4. */


/******************************************************************
 * Board Pin/Gpio configurations
 ******************************************************************/

#define PHDRIVER_PIN_DWL           ((PORT0<<8) | 21)   /**< Download mode pin of Frontend, Port0, pin21 */
#define PHDRIVER_PIN_RESET         ((PORT2<<8) | 5)    /**< Port2, pin05 */
#define PHDRIVER_PIN_IRQ           ((PORT2<<8) | 11)   /**< Interrupt pin from Frontend to Host, Port2, pin11 */
#define PHDRIVER_PIN_BUSY          PHDRIVER_PIN_IRQ    /**< No busy pin in CERES/PN5190, handled by IRQ pin itself. */

#define PHDRIVER_PIN_GPIO0         ((IOCON_FUNC0<<16) | (PORT1<<8) | 29)  /**< LPC1769 GPIO PIN connected to NFC GPIO0  */
#define PHDRIVER_PIN_GPIO1         ((IOCON_FUNC0<<16) | (PORT1<<8) | 25)  /**< LPC1769 GPIO PIN connected to NFC GPIO1  */
#define PHDRIVER_PIN_GPIO2         ((IOCON_FUNC0<<16) | (PORT1<<8) | 28)  /**< LPC1769 GPIO PIN connected to NFC GPIO2  */
#define PHDRIVER_PIN_GPIO3         ((IOCON_FUNC0<<16) | (PORT1<<8) | 26)  /**< LPC1769 GPIO PIN connected to NFC GPIO3  */

/* PN5190 RF BGA Board*/
#define PHDRIVER_PIN_RLED          ((PORT0<<8) | 22)   /**< RED LED,   Port0, pin22, Pin function 0 */
#define PHDRIVER_PIN_GLED          ((PORT3<<8) | 25)   /**< GREEN LED, Port3, pin25, Pin function 0 */
#define PHDRIVER_PIN_BLED          ((PORT3<<8) | 26)   /**< BLUE LED, Port3, pin26, Pin function 0 */
#define PHDRIVER_PIN_OLED          ((PORT2<<8) | 04)   /**< ORANGE LED, Port2, pin04, Pin function 0 */

#define PHDRIVER_PIN_SUCCESS       PHDRIVER_PIN_GPIO1
#define PHDRIVER_PIN_FAIL          PHDRIVER_PIN_GPIO2

/* GPIO and LED for applications use */
#define PHDRIVER_PIN_GPIO          PHDRIVER_PIN_GPIO1    /**< Port1, pin25 */
#define PHDRIVER_PIN_LED           PHDRIVER_PIN_RLED     /**< RED LED */

/******************************************************************
 * PIN Pull-Up/Pull-Down configurations.
 ******************************************************************/
#define PHDRIVER_PIN_RESET_PULL_CFG    PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_IRQ_PULL_CFG      PH_DRIVER_PULL_DOWN
#define PHDRIVER_PIN_BUSY_PULL_CFG     PH_DRIVER_PULL_DOWN
#define PHDRIVER_PIN_DWL_PULL_CFG      PH_DRIVER_PULL_UP
#define PHDRIVER_PIN_NSS_PULL_CFG      PH_DRIVER_PULL_UP

/******************************************************************
 * IRQ PIN NVIC settings
 ******************************************************************/
#define PIN_IRQ_TRIGGER_TYPE    PH_DRIVER_INTERRUPT_RISINGEDGE  /**< IRQ pin RISING edge interrupt */
#define EINT_PRIORITY           5                /**< Interrupt priority. */
#define EINT_IRQn               EINT3_IRQn       /**< NVIC IRQ */

/*****************************************************************
 * Front End Reset logic level settings
 ****************************************************************/
#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */
#define RESET_POWERDOWN_LEVEL PH_DRIVER_SET_LOW
#define RESET_POWERUP_LEVEL   PH_DRIVER_SET_HIGH


/*****************************************************************
 * SPI Configuration
 ****************************************************************/
#define LPC_SSP             LPC_SSP0   /**< SPI Module */
#define SSP_CLOCKRATE       4000000    /**< SPI clock rate. Allowed clock rate: 6 or 4 or 3 or 2.4MHz etc. */

 /******************************************************************/
/* Pin configuration format : Its a 32 bit format where 1st 3 bytes represents each field as shown below.
 * | Byte3 | Byte2 | Byte1 | Byte0 |
 * | ---   | FUNC  | PORT  | PIN   |
 * */
#define PHDRIVER_PIN_MOSI     ((IOCON_FUNC2<<16) | (PORT0<<8) | 18)  /**< MOSI pin, Port0, pin18, Pin function 2 */
#define PHDRIVER_PIN_MISO     ((IOCON_FUNC2<<16) | (PORT0<<8) | 17)  /**< MISO pin, Port0, pin17, Pin function 2 */
#define PHDRIVER_PIN_SCK      ((IOCON_FUNC2<<16) | (PORT0<<8) | 15)  /**< Clock pin, Port0, pin15, Pin function 2 */
#define PHDRIVER_PIN_SSEL     ((IOCON_FUNC0<<16) | (PORT0<<8) | 16)  /**< Slave select, Port0, pin16, Pin function 0 */

#define PHDRIVER_PIN_SDA     ((IOCON_FUNC3<<16) | (PORT0<<8) | 19)  /**< SDA pin, Port0, pin19, Pin function 3 */
#define PHDRIVER_PIN_SCL     ((IOCON_FUNC3<<16) | (PORT0<<8) | 20)  /**< SCL pin, Port0, pin20, Pin function 3 */


/*****************************************************************
 * Timer Configuration
 ****************************************************************/
#define PH_DRIVER_LPC_TIMER                    LPC_TIMER0           /**< Use LPC timer0 */
#define PH_DRIVER_LPC_TIMER_CLK                SYSCTL_CLOCK_TIMER0  /**< Timer 0 clock source */
#define PH_DRIVER_LPC_TIMER_MATCH_REGISTER     0x01  /* use match register 1 always. */
#define PH_DRIVER_LPC_TIMER_IRQ                TIMER0_IRQn          /**< NVIC Timer0 Irq */
#define PH_DRIVER_LPC_TIMER_IRQ_HANDLER        TIMER0_IRQHandler    /**< Timer0 Irq Handler */
#define PH_DRIVER_LPC_TIMER_IRQ_PRIORITY       5                    /**< NVIC Timer0 Irq priority */

#endif /* BOARD_LPC1769PN5190_H */
