/*
 *  ABOUT
 *
 * 	Date:		Abril del 2017
 *
 * 	Authors:	MARISCAL ARROYO JUAN PABLO
 * 				MORENO ALONZO LUIS FERNANDO
 * 				ZUÑIGA CARRILLO MARIO EUGENIO
 *
 * 	Description: Práctica #2
 *
 */

#ifndef SOURCE_DEFINITIONS_H_
#define SOURCE_DEFINITIONS_H_

/*******************************************************************************
 * LIBRARIES
 ******************************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "fsl_device_registers.h"
#include "fsl_adc16.h"
#include "Delay.h"
#include <math.h>

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"

/*******************************************************************************
 * GENERAL DEFINITIONS
 ******************************************************************************/
#define TRUE 1
#define FALSE 0
#define CREATETASK_DELAY 100
#define ONE_SECOND 1200
#define NO_WAIT ( TickType_t ) 0
#define WAIT ( TickType_t ) 10
#define pMAX_DELAY ( TickType_t ) 0xffffffffUL
#define ASCII_OFFSET 0x30
#define FREQUENCY 120000000
#define NOT_GIVED 0
#define MAX_COUNT 2
#define CONN2_DELAY 200
#define CONN3_DELAY 400
#define TASK1_SIZE 9
#define TASK3_24H_SIZE 8
#define TASK3_12H_SIZE 11
#define TASK5_SIZE 2
#define TASK6_SIZE 8
#define THERM_CALIBRATION 18

/* CHAT */
#define ANY 0
#define NOTHING 0
#define OFFLINE 0
#define ONLINE 1
#define USER_A 1
#define USER_B 2
#define USER_C 3
#define USER_D 4
#define USER_E 5


/*******************************************************************************
 * BOTTONS DEFINITIONS
 ******************************************************************************/
#define BOTTONS_SEM_WAITTIME ( TickType_t ) 0
#define BOTTONS_MAXCOUNT 1
#define BOTTONS_INITCOUNT 0
#define DEBOUNCER 200
#define BOTON1 0x10000
#define DAYS 	0
#define MONTHS 	1
#define YEARS	2
#define HOURS 	3
#define MINU 	4
#define SEC 	5

/*******************************************************************************
 * TASK PRIORITY DEFINITIONS
 ******************************************************************************/
#define CONN_1_PRIORITY (configMAX_PRIORITIES - 1)
#define CONN_2_PRIORITY (configMAX_PRIORITIES - 1)
#define CONN_3_PRIORITY (configMAX_PRIORITIES - 1)
#define CONN_4_PRIORITY (configMAX_PRIORITIES - 1)
#define CONN_5_PRIORITY (configMAX_PRIORITIES - 1)

/*******************************************************************************
 * BUFFER DEFINITIONS
 ******************************************************************************/
#define N_LETTERS 1
#define UARTTASK_MAXCOUNT 1
#define UARTTASK_INITCOUNT 1
#define BIG_BUFFER_SIZE 35
#define INFO_BUFFER_SIZE BIG_BUFFER_SIZE+10
#define SMALL_BUFFER_SIZE 15
#define MIN_BUFFER_SIZE 1

/*******************************************************************************
 * I2C DEFINITIONS
 ******************************************************************************/
#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U
#define I2C_RELEASE_BUS_COUNT 100U
#define I2C_BAUDRATE 100000U

/* ACCEL */
#define ACCEL_ADDRESS 0x1D
#define ACCEL_STATUS 0x00U
#define ACCEL_XYZ_DATA_CFG 0x0EU
#define ACCEL_CTRL_REG1 0x2AU
#define ACCEL_WHOAMI_REG 0x0DU
#define ACCEL_READ_TIMES 10U
#define LAST_POSITION 5
#define N_DIGITS 5

#define RTC_ADDRESS 0b1101111
#define MEMORY_ADDRESS 0b1010000
#define READDATA_SIZE 1
#define MASK_ST_BIT 0x7F
#define MASK_24H_SET 0x3F
#define MASK_12H_SET 0x40
#define MASK_12H_GET 0x1F
#define MASK_OFFSET_FORMAT 0x0C
#define MASK_AM_FORMAT 0xDF
#define MASK_PM_FORMAT 0x60
#define MASK_INIT_RTC 0x80
#define MASK_INIT_SEC 0x80
#define MASK_INIT_MIN 0x00
#define MASK_INIT_HRS 0x40
#define MASK_INIT_DAY 0x21
#define MASK_INIT_MONTH 0x03
#define MASK_INIT_YEAR 0x17

#define RTC_SECONDS_REG 0x00
#define RTC_MINUTES_REG 0x01
#define RTC_HOURS_REG 	0x02
#define RTC_DAY_REG 	0x04
#define RTC_MONTH_REG 	0x05
#define RTC_YEAR_REG 	0x06

/*******************************************************************************
 * MENU DEFINITIONS
 ******************************************************************************/
#define CERO 	0x30
#define ONE 	0x31
#define TWO 	0x32
#define THREE 	0x33
#define FOUR 	0x34
#define FIVE 	0x35
#define SIX 	0x36
#define SEVEN	0x37
#define EIGHT 	0x38
#define NINE 	0x39
#define ESC 	0x3C

/*******************************************************************************
 * EVENT GROUP DEFINITIONS
 ******************************************************************************/
#define NO_DELAY ( TickType_t ) 0
#define FT 0
#define CONN1 1
#define CONN2 2
#define CONN3 3
#define CONN4 4
#define CONN5 5

#define Task_CERO		(1 << 0)
#define Task_ONE		(1 << 1)
#define Task_TWO 		(1 << 2)
#define Task_THREE		(1 << 3)
#define Task_FOUR 		(1 << 4)
#define Task_FIVE		(1 << 5)
#define Task_SIX	 	(1 << 6)
#define Task_SEVEN		(1 << 7)
#define Task_EIGHT	 	(1 << 8)
#define Task_NINE		(1 << 9)

#define CONN_A (1 << 10)
#define CONN_B (1 << 11)
#define CONN_C (1 << 12)
#define CONN_D (1 << 13)
#define CONN_E (1 << 14)

/*******************************************************************************
 * TASKS
 ******************************************************************************/
void Read_Memory_Task(struct netconn*,uint8_t*);
void Write_Memory_Task(struct netconn*,uint8_t*);
void Set_Hour_Task(struct netconn*,uint8_t*);
void Get_Hour_Task(struct netconn*);
void Hour_Format_Task(struct netconn*,uint8_t*);
void Set_Date_Task(struct netconn*,uint8_t*);
void Get_Date_Task(struct netconn*,uint8_t*);
void Get_Accel_Task(struct netconn*,uint8_t*);
void Get_Therm_Task(struct netconn * , uint8_t*);
void Chat_task(struct netconn *,uint8_t*,uint8_t);

/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/
/* General Functions */
void bufferBackup(uint8_t *,uint8_t *);
void zerosBuffer(uint8_t *,uint8_t);
uint8_t hexToASCII(uint8_t);
uint8_t BCDtoHex(uint8_t);
uint8_t HexToBCD (uint8_t);
uint8_t DecToBCD(uint8_t);
int8_t* int16ToString(int16_t);

/* UART Functions */
void print_ESC(struct netconn *);
void print_BTN(struct netconn *);
void print_Key(uint8_t* msj);
void print_Hex(struct netconn *,uint8_t*,uint8_t*);
void print_String(struct netconn *,uint8_t*, uint8_t*);
void waitingKey(uint8_t*);

/* I2C Functions */
bool I2C_WriteReg(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t* value);
bool I2C_WriteReg_Hex(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t value);
bool I2C_ReadReg(I2C_Type *base, uint8_t device_addr, uint16_t reg_addr, uint8_t *rxBuff, uint32_t rxSize);
void i2c_release_bus_delay(void);
void BOARD_I2C_ReleaseBus(void);
void RTC_Hour_Init();

/* MENU Functions */
void printMenu(struct netconn *);
void print_ERROR(struct netconn *);

/* ADC Functions */
void ADC_init();

/* Event groups Functions */
EventBits_t updatedBits(uint8_t);
EventBits_t active(uint8_t,uint32_t);
EventBits_t notActive(uint8_t,uint32_t);

/*******************************************************************************
 * GETTERS & SETTERS FUNCTIONS
 ******************************************************************************/
/* SYNCRONIZATION G&S */
SemaphoreHandle_t getMutex();
void setMutex(SemaphoreHandle_t);
EventGroupHandle_t getEventGroup_Handler(uint8_t group);

/* BOTTONS G&S */
SemaphoreHandle_t getBotton1Sem();
void setBotton1Sem(SemaphoreHandle_t);

/* I2C G&S */
uint32_t get_I2CMasterHandle();
bool get_CompletionFlag();
bool get_nakFlag();
void set_CompletionFlag(bool value);
void set_nakFlag(bool value);

/*******************************************************************************
 * END
 ******************************************************************************/
#endif /* SOURCE_DEFINITIONS_H_ */
