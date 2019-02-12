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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <scope.h>
#include <arm_math.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
volatile bool ms_has_elapsed = false;
static uint32_t timestamp = 0;
volatile uint32_t delta;

/* Callback for the scope */
void transmit_data(UartJsonHandle self){

    /* In a example with interrupts, this function may just enable the interrupt
     * If it would be used, with interrupt, the whole data could be fetched out of the UartJson class
     * with UartJson_getTxData and be transmitted to the host. */
    huart2.Instance->CR1 |= USART_CR1_TXEIE_Msk;
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void){
    /* USER CODE BEGIN USART2_IRQn 0 */
    /* USER CODE END USART2_IRQn 0 */
    if(((huart2.Instance->ISR & USART_ISR_TXE_Msk) >> USART_ISR_TXE_Pos) == 1){
        if(((huart2.Instance->ISR & USART_ISR_BUSY_Msk) >> USART_ISR_BUSY_Pos) == 0){

            /******************************************************
             * Sending
             * Reading bytes out of the UartJson class
             *****************************************************/
            while(UartJson_amountOfTxDataPending(uartJson) > 0){

                uint8_t data;
                UartJson_getTxData(uartJson, &data, 1);
                huart2.Instance->TDR = data;
                while(((huart2.Instance->ISR & USART_ISR_TC_Msk) >> USART_ISR_TC_Pos) == 0);
            }
            huart2.Instance->CR1 = huart2.Instance->CR1 & ~(USART_CR1_TXEIE_Msk);
            UartJson_resetTx(uartJson);

        }
    }
    if(((huart2.Instance->ISR & USART_ISR_RXNE_Msk) >> USART_ISR_RXNE_Pos) == 1){
        uint16_t data;

        /******************************************************
         * Receiving
         * Writing bytes into the UartJson class
        *****************************************************/
        data = huart2.Instance->RDR;
        while(((huart2.Instance->ISR & USART_ISR_RXNE_Msk) >> USART_ISR_RXNE_Pos) == 1);
        UartJson_putRxData(uartJson, (uint8_t * ) & data, 1);
    }

    /* Interrupt error handling */
    if(((huart2.Instance->ISR & USART_ISR_IDLE_Msk) >> USART_ISR_IDLE_Pos) == 1){
        huart2.Instance->ICR |= 0xFF & UART_CLEAR_IDLEF;
    }

    if(((huart2.Instance->ISR & USART_ISR_ORE_Msk) >> USART_ISR_ORE_Pos) == 1){
        (&huart2)->Instance->ICR = USART_ISR_ORE_Msk;
    }

    if(((huart2.Instance->ISR & USART_ISR_CMF_Msk) >> USART_ISR_CMF_Pos) == 1){
        (&huart2)->Instance->ICR = USART_ISR_CMF_Msk;
    }
    if(((huart2.Instance->ISR & USART_ISR_EOBF_Msk) >> USART_ISR_EOBF_Pos) == 1){
        (&huart2)->Instance->ICR = USART_ISR_EOBF_Msk;
    }

    /* USER CODE BEGIN USART2_IRQn 1 */

    /* USER CODE END USART2_IRQn 1 */
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

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
    MX_NVIC_Init();
    CLEAR_BIT((&huart2)->Instance->CR1, USART_CR1_RXNEIE);

    /* USER CODE BEGIN 2 */

    /* If the variables should be found in the map file, they have to be declared as static*/
    static float sinus = 0.0f;
    static float cosinus = 0.0f;
    static float leistung = 0.0f;
    static uint8_t schmitttriggered = 0;
    static const float frequency = 20.0f;

/***********************************************************************************************************************
* Build Scope
***********************************************************************************************************************/


    scope_init(transmit_data, &timestamp);

    /* adding some variables to the AddressStorage
     * These can be trasmitted to the host if wanted */
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    AddressStorage_addAnnounceAddress(addressStorage, "sinus", &sinus, FLOAT, 0);
    AddressStorage_addAnnounceAddress(addressStorage, "cosinus", &cosinus, FLOAT, 1);
    AddressStorage_addAnnounceAddress(addressStorage, "tangent", &leistung, FLOAT, 2);
    AddressStorage_addAnnounceAddress(addressStorage, "schmitttriggered", &schmitttriggered, UINT8, 3);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1){
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        if(ms_has_elapsed == true){

            /* Generating a signal for visualizing */
            float t_in_s = timestamp / 1000.0f;

            float noise = ((float) rand() / RAND_MAX) / 10; // -20dB noise
            float amp = (sinf(2 * PI * frequency / 10 * t_in_s) + noise) * 2; // up to 3dB signal amplification

            sinus = amp * sinf(2 * PI * frequency * t_in_s) + noise;
            cosinus = amp * cosf(2 * PI * frequency * t_in_s) + noise;
            leistung = sinus * cosinus;
            schmitttriggered = (sinus > 0) ? 1 : 0;

            delta = timestamp;
            scope_run();
            ms_has_elapsed = false;
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
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
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

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void){
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */

    ms_has_elapsed = true;

    timestamp++;

    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
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
