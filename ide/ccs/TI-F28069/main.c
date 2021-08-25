/*
 * main.c
 *
 *  Created on: 29.07.2021
 *      Author: LinusC
 */

//includes
#include "DSP28x_Project.h"
#include <stdint.h>
#include "Scope/Communication/ITransceiver.h"
#include "Scope/Builders/ScopeFramedStack.h"
#include "UartDriver.h"
#include <math.h>

//Timer variables
uint32_t timestamp_ms = 0;
uint32_t deltaTime_ms = 0;
uint32_t timerDelta;
uint32_t timerCounts;
uint32_t timerCounts_0 = 0xFFFFFFFF;
uint32_t timerMax = 0xFFFFFFFF;
uint32_t timer_cycleCount = 0;

uint32_t loopCount = 0;

//Define Scope values
float valueA = 0.0f;
uint16_t valueB = 0;
ScopeFramedStackHandle scopeStack;

int main(void)
{
    //Required setup

    InitSysCtrl();

    //Initialize the Sci
    InitSciaGpio();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.SCIRXINTA = &sciRxFifoIsr;
    PieVectTable.SCITXINTA = &sciTxFifoIsr;
    EDIS;   // This is needed to disable write to EALLOW protected registers

    //initialize the timer
    InitCpuTimers();
    CpuTimer0Regs.TCR.all = 0x0000; //Set timer TSS bit to 0

    //initialize the uart
    uart_init();

    // Enable required interrupts
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, INT1
    PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
    IER = 0x100;                         // Enable CPU INT
    EINT;


    //Scope Setup
    ScopeFramedStackConfig config = {
            .addressesInAddressAnnouncer = 3,
            .callback = &transmit_data,
            .timestamp = &timestamp_ms,
            .amountOfChannels = 3,
            .timebase = 0.001f,
            .sizeOfChannels = 50
    };

    scopeStack = ScopeFramedStack_create(config);
    UartDriver_init();

    AnnounceStorageHandle addressStorage = ScopeFramedStack_getAnnounceStorage(scopeStack);
    AnnounceStorage_addAnnounceAddress(addressStorage, "A", &valueA, SE_FLOAT);
    AnnounceStorage_addAnnounceAddress(addressStorage, "B", &valueB, SE_UINT16);

    //Start CPU timer
    CpuTimer0Regs.TCR.bit.TSS = 0;

    //Scope loop
    for(;;)
    {
        UartDriver_run();

        //Get Timestamp (CPU freq = 90 MHz, 32Bit Timer, counting down)
        timerCounts = CpuTimer0Regs.TIM.all;

        if (timerCounts < timerCounts_0)
        {
            timerDelta = timerCounts_0 - timerCounts;
        }
        else
        {
            timerDelta = timerCounts_0 + (timerMax - timerCounts);
            timer_cycleCount++;
        }
        timerCounts_0 = timerCounts;

        deltaTime_ms = timerDelta/90000;
        timestamp_ms = ((timerMax - timerCounts)/90000)+(timer_cycleCount * 47722);

        //Assign values for scope stack
        valueA = 20;
        valueB = 10;

        ScopeFramedStack_run(scopeStack);

        loopCount++;
    }
}

