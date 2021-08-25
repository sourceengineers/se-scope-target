/*
 * UartDriver.h
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

#include "Scope/Communication/ITransceiver.h"
#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

void UartDriver_init(void);
void UartDriver_run(void);
void UartDriver_test(void);

void UART_CharTransmitComplete_Callback(void);
void UART_Error_Callback(void);
void transmit_data(ITransceiverHandle self);
__interrupt void sciRxFifoIsr(void);
__interrupt void sciTxFifoIsr(void);
void uart_init(void);

#endif


