#include "tmc_motor_driver.h"

TmcMotorDriver::TmcMotorDriver(HardwareSerial *serialPort, uint8_t addr, float rSense , uint8_t rxPin, uint8_t txPin):
    serialPort(serialPort),
    addr(addr),
#if TMC_DRIVER_MODEL == 2208
    tmc_driver(serialPort, rSense)
#else
    tmc_driver(serialPort, rSense, addr)
#endif
{
    if(serialPort != nullptr)
    {
        serialPort->begin(115200, SERIAL_8N1, rxPin, txPin);
        while(!serialPort);
    }

    tmc_driver.begin();
}

void TmcMotorDriver::setMicrosteps(uint16_t microsteps)
{
    tmc_driver.microsteps(microsteps);
}
