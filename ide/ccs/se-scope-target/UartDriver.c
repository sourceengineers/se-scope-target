/*
 * UartDriver.c
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

#include "UartDriver.h"
#include <string.h>
#include "Scope/Communication/ITransceiver.h"
#include "Scope/Builders/ScopeFramedStack.h"
#include <stdlib.h>
#include "DSP28x_Project.h"

//
// Function Prototypes
//
void UartDriver_init(void);
void UartDriver_run(void);
void transmit_data(ITransceiverHandle self);
__interrupt void sciRxFifoIsr(void);
__interrupt void sciTxFifoIsr(void);

Uint16 MAX_FIFO_ENTRIES = 4;


typedef struct __UartHandlerPrivateData {
    size_t inputSize;
    uint8_t* rxBuffer;
    uint32_t rxCount;
    uint8_t* txBuffer;
    uint32_t txCount;
    size_t outputSize;
    bool txIsDone;
    uint32_t bytesToSend;
    ITransceiverHandle transceiver;
} PrivateData;
static PrivateData me;

extern ScopeFramedStackHandle scopeStack;

void UartDriver_init() {
    // Make the buffers slightly bigger than needed, just to be sure
    me.inputSize = (uint32_t) 255;
    me.outputSize = (uint32_t) 255;
    me.rxBuffer = malloc(me.inputSize * sizeof(uint8_t));
    me.rxCount = 0;
    me.bytesToSend = 0;
    me.txBuffer = malloc(me.outputSize * sizeof(uint8_t));
    me.txCount = 0;
    me.txIsDone = true;
    me.transceiver = ScopeFramedStack_getTranscevier(scopeStack);

    SciaRegs.SCIFFRX.bit.RXFFIENA = 1;  //Enable Rx not empty interrupt
    SciaRegs.SCICTL1.bit.RXERRINTENA = 1; //Enable Rx error interrupt
}

void UartDriver_run(void) {
    /* Check if the uart output buffer in the scope is empty or not. If yes, send more data */
    uint32_t dataPending = me.transceiver->outputSize(me.transceiver);
    if (me.txIsDone && dataPending > 0) {
        // Only send as many bytes as there are left to be sent
        me.bytesToSend = dataPending >= me.outputSize ? me.outputSize : dataPending;

        // Copy from scope to output buffer
        me.transceiver->get(me.transceiver, me.txBuffer, me.bytesToSend);
        me.txIsDone = false;
        me.txCount = 0;
        SciaRegs.SCIFFTX.bit.TXFFIENA = 1;  //Enable TX empty interrupt
    }

    if (me.rxCount > 1) {

        // Fast copy rx data into temporary buffer.
        SciaRegs.SCIFFRX.bit.RXFFIENA = 0;  //Disable rx empty interrupt
        size_t copyCount = me.rxCount;
        uint8_t tmpBuffer[copyCount];
        memcpy(tmpBuffer, me.rxBuffer, copyCount);
        me.rxCount = 0;
        SciaRegs.SCIFFRX.bit.RXFFIENA = 1;  //Enable rx empty interrupt

        // Send copied rx data into scope
        me.transceiver->put(me.transceiver, tmpBuffer, copyCount);
    }
}

void transmit_data(ITransceiverHandle self) {

}

// rx fifo not empty interrupt
__interrupt void sciRxFifoIsr(void)
{
    while(SciaRegs.SCIFFRX.bit.RXFFST > 0 && me.rxCount < me.inputSize)
    {
        me.rxBuffer[me.rxCount] = SciaRegs.SCIRXBUF.all; //Read Data
        me.rxCount++;
    }

    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag

    PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}

//tx fifo empty interrupt
__interrupt void sciTxFifoIsr(void)
{
    if(me.txCount < me.bytesToSend)
    {
        while(SciaRegs.SCIFFTX.bit.TXFFST < MAX_FIFO_ENTRIES && me.txCount < me.bytesToSend)
        {
            SciaRegs.SCITXBUF = me.txBuffer[me.txCount];     // Send data
            me.txCount++;
        }
    }
    else
    {
        me.txIsDone = true;
        SciaRegs.SCIFFTX.bit.TXFFIENA = 0;  //Disable Tx empty interrupt
        SciaRegs.SCIFFRX.bit.RXFFIENA = 1;  //Enable Rx not empty interrupt
    }

    SciaRegs.SCIFFTX.bit.TXFFINTCLR=1;  // Clear SCI Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK
}

void uart_init(void)
{
    //
    // 1 stop bit,  No loopback, No parity,8 char bits, async mode,
    // idle-line protocol
    //
    SciaRegs.SCICCR.all =0x0007;

    //
    // enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
    //
    SciaRegs.SCICTL1.all =0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;

    //
    //9600 Baud
    //
    SciaRegs.SCIHBAUD = 0x0001;
    SciaRegs.SCILBAUD = 0x0024;

    SciaRegs.SCICCR.bit.LOOPBKENA = 0;   // Disable loop back
    SciaRegs.SCIFFTX.all=0xC020;        //FIFO empty interrupt
    SciaRegs.SCIFFRX.all=0x0021;        //FIFO interupts if one byte arrives
    SciaRegs.SCIFFCT.all=0x00;

    SciaRegs.SCICTL1.all =0x0023;       // Relinquish SCI from Reset
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}
