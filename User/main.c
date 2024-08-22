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

    // Set index to register 0
    uint8_t reg=0;
    i2c_write(I2C_ADDRESS_DS3231,&reg,sizeof(reg));
    //printf("%s, Write reg0, RC: %d\n",__func__,rc);

    // Read registers 0,1,2
    uint8_t reg_data[3]={0xFF,0xFF,0xFF};
    i2c_read(I2C_ADDRESS_DS3231, reg_data, sizeof(reg_data));
    //printf("%s, i2c_read(), RC: %d\n",__func__,rc);
    for(uint8_t i=0;i<3;i++)
        printf("%u: 0x%02X\n",i,reg_data[i]);

    // Test i2c scan
    i2c_scan();

    // Continuously read the temperature register 0x11 (Temp, MSB)
    // Set index to register 0x11
    while(1) {
        // Force a temperature conversion, write 0x3C to control register, 0x0E (set CONV bit, BIT5)
        uint8_t control_reg[2] = {0x0E,0x3C};
        i2c_write(I2C_ADDRESS_DS3231,control_reg,sizeof(control_reg));

        // Read temperature registers, 0x11, 0x12
        reg=0x11; // Temperature, MSB (Celcius)
        uint8_t temp_reg[2] = {0,0};
        i2c_write(I2C_ADDRESS_DS3231,&reg,sizeof(reg));
        i2c_read(I2C_ADDRESS_DS3231, temp_reg, sizeof(temp_reg));
        //printf("temp_reg0: %02X, temp_reg1: %02X\n",temp_reg[0],temp_reg[1]);
        // Combine registers into int16_t
        uint16_t u_temp_c = ((uint16_t)temp_reg[0]<<8) + ((uint16_t)temp_reg[1]);
        int16_t temp_c = (int16_t)u_temp_c;
        //printf("u_temp_c: %04X\n",u_temp_c);
        temp_c /= 64; // convert to 1/4 degree C units
        printf("Temp: %d %d/4C\n",temp_c/4,temp_c%4); // This display method only works for positive temperature values

        Delay_Ms(1000);
    } // while
}
