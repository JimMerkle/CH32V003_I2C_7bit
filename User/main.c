/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/22
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *7-bit addressing mode, master/slave mode, transceiver routine:
 *I2C1_SCL(PC2)\I2C1_SDA(PC1).
 *This routine demonstrates that Master sends and Slave receives.
 *Note: The two boards download the Master and Slave programs respectively,
 *    and power on at the same time.
 *      Hardware connection:
 *            PC2 -- PC2
 *            PC1 -- PC1
 *
 */

#include "debug.h"
#include "i2c.h"

#define I2C_ADDRESS_DS3231  0x68   // 7-bit I2C address

/* Global Variable */
uint8_t ds3231_data[] = { 0x00, 0x00, 0x01, 0x02 }; // write seconds, minutes, hours

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();

    USART_Printf_Init(460800);

    printf("SystemClk:%d\r\n",SystemCoreClock);
    //printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    //printf("IIC Host mode\r\n");
    IIC_Init( 100000, I2C_SELF_ADDRESS); // 80000 creates a nice looking 80KHz, 100K looks good too

    // Write to registers 0,1,2
    i2c_write(I2C_ADDRESS_DS3231, ds3231_data, sizeof(ds3231_data));
    //printf("%s, Write registers, RC: %d\n",__func__,rc);

    //Delay_Ms(1);
    // Set index to register 0
    uint8_t reg=0;
    i2c_write(I2C_ADDRESS_DS3231,&reg,sizeof(reg));
    //printf("%s, Write reg0, RC: %d\n",__func__,rc);

    //Delay_Ms(1);
    // Read registers 0,1,2
    uint8_t reg_data[3]={0xFF,0xFF,0xFF};
    i2c_read(I2C_ADDRESS_DS3231, reg_data, sizeof(reg_data));
    //printf("%s, i2c_read(), RC: %d\n",__func__,rc);
    for(uint8_t i=0;i<3;i++)
        printf("%u: 0x%02X\n",i,reg_data[i]);

    // Continuously read the temperature register 0x11 (Temp, MSB)
    // Set index to register 0x11
    while(1) {
        reg=0x11; // Temperature, MSB (Celcius)
        uint8_t temp_msb;
        i2c_write(I2C_ADDRESS_DS3231,&reg,sizeof(reg));
        i2c_read(I2C_ADDRESS_DS3231, &temp_msb, sizeof(temp_msb));

        // Convert to Fahrenheit
        int16_t temp_f = (((int16_t)temp_msb * 18) / 10) + 32 ; // multiply by 1.8, add 32
        printf("Temp: %dF\n",temp_f);
        Delay_Ms(1000);
    } // while
}
