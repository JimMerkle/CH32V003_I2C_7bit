# I2C_7bit_Mode

Example I2C project for the CH32V003

Implement an example I2C project that writes to DS3231 registers and then reads back and displays the register data.


i2c_write
        
        Implement generic I2C write method that takes a 7-bit I2C address,
        write buffer, and byte count.
        Using loop counters to prevent lock-up

i2c_read
        
        Implement generic I2C read method that takes a 7-bit I2C address,
        read buffer, and byte count.
        Using loop counters to prevent lock-up
        Added ACK/NAK support for last byte read

Temperature
        
        Each second, force a temperature conversion.
        Read DS3231 temperature registers, 0x11 and 0x12
        Convert two bytes into single signed 16-bit value
        Display Celcius value
