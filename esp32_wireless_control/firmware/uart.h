/**
 * @file uart.h
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2025, Sylensky
 */

#ifndef UART_H
#define UART_H
#include <Arduino.h>
#include <ErriezSerialTerminal.h>

#define MAX_UART_LINE_LEN 128

#define CLI_DELIMITER_CHAR ' '
#define CLI_NEWLINE_CHAR '\r'

extern SerialTerminal term;

void print_out(const char* format, ...);
void print_out_nonl(const char* format, ...);
void print_out_tbl(uint8_t index);
void setup_uart(HardwareSerial* serial, long baudrate);
void uart_task();

#endif
