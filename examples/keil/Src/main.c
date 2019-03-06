/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <scope.h>
//#include <arm_math.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* UART handler declaration */
__IO ITStatus RxReady = RESET;
__IO ITStatus TxReady = RESET;

uint8_t aTxBuffer[6000];
#define UART_INPUT_SIZE 300
uint8_t aRxBuffer[UART_INPUT_SIZE];

volatile bool adc_it_is_cleared = false;
volatile float adcValue = 0.0f;
uint32_t timestamp = 0;
uint32_t timestamp_safe = 0;

static const float frequency = 20.0f;
float sinus = 0.0f;
float cosinus = 0.0f;
float leistung = 0.0f;
uint8_t schmitttriggered = 0;

bool transmitting = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
void transmit_data(UartJsonHandle self);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_ADC1_Init();

    /* Initialize interrupts */
    MX_NVIC_Init();
    /* USER CODE BEGIN 2 */

    scope_init(transmit_data, &timestamp_safe);

    /* adding some variables to the AddressStorage
     * These can be trasmitted to the host if wanted */
    AddressStorage_addAnnounceAddress(addressStorage, "sinus", &sinus, SE_FLOAT, 0);
    AddressStorage_addAnnounceAddress(addressStorage, "cosinus", &cosinus, SE_FLOAT, 1);
    AddressStorage_addAnnounceAddress(addressStorage, "tangent", &leistung, SE_FLOAT, 2);
    AddressStorage_addAnnounceAddress(addressStorage, "schmitttriggered", &schmitttriggered, SE_UINT8, 3);

    HAL_UART_Receive_IT(&huart2, (uint8_t*) aRxBuffer, UART_INPUT_SIZE);
    HAL_ADC_Start_IT(&hadc1);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1){
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /* Update timestmap */

	      timestamp = HAL_GetTick();

        if(adc_it_is_cleared == true){
            adc_it_is_cleared = false;
            HAL_ADC_Start_IT(&hadc1);
        }

		    if(TxReady == SET){
			    UartJson_resetTx(uartJson);
			    TxReady = RESET;
		    }
		    if(RxReady == SET){
			    UartJson_putRxData(uartJson, aRxBuffer, UART_INPUT_SIZE);
			    RxReady = RESET;
			    HAL_UART_Receive_IT(&huart2, (uint8_t*) aRxBuffer, UART_INPUT_SIZE);
		    }

        if(timestamp > timestamp_safe && transmitting == false){

            timestamp_safe = timestamp;
            float t_in_s = timestamp / 1000.0f;

            // TODO take pi from math.h
            sinus = 2 * sinf(2 *  3.14159f * frequency * t_in_s);
            cosinus = 2 * cosf(2 *  3.14159f * frequency * t_in_s);
            leistung = sinus * cosinus;
            schmitttriggered = (adcValue > 1) ? 2 : 0;

            scope_run();
        }
    }
    return 0;
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void){
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
        Error_Handler();
    }
    /**Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK){
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_ADC;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
    PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
    PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
    PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
    PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
    PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
    PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK){
        Error_Handler();
    }
    /**Configure the main internal regulator output voltage
    */
    if(HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK){
        Error_Handler();
    }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void){
    /* USART2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/* USER CODE BEGIN 4 */
void transmit_data(UartJsonHandle self){

    size_t length = UartJson_amountOfTxDataPending(uartJson);
    UartJson_getTxData(uartJson, aTxBuffer, length);
    HAL_UART_Transmit_IT(&huart2, aTxBuffer, (uint16_t) length);
		transmitting = true;
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of IT Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* UartHandle){
    /* Set transmission flag: transfer complete */
    HAL_UART_Receive_IT(&huart2, (uint8_t*) aRxBuffer, UART_INPUT_SIZE);
		TxReady = SET;
		transmitting = false;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* UartHandle){
    /* Set transmission flag: transfer complete */
    RxReady = SET;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){

    uint32_t adc = HAL_ADC_GetValue(hadc);

    adcValue =  (float)(((float) adc / (1 << 12)) -0.49) * 4;
    adc_it_is_cleared = true;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void){
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    volatile int error;

    while(1){
        error = 1;
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
         tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
