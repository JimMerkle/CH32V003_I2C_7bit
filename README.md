# I2C_7bit_Mode

Example I2C project for the CH32V003

Create a timer, TIM2, to increment at a 1us rate, providing a 16-bit timer for micro-second timing

        // USART RX - input pin: D6
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOD, &GPIO_InitStructure);
        // Need BOTH TX and RX:
        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

i2c_write
        Implement generic I2C write method that takes a 7-bit I2C address,
        write buffer, and byte count.
        Using loop counters to prevent lock-up

i2c_read
        Implement generic I2C read method that takes a 7-bit I2C address,
        read buffer, and byte count.
        Using loop counters to prevent lock-up
        Added ACK/NAK support for last byte read
