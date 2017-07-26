/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
/* Macros to enable & disable CS pin */
//#define CS_ENABLE		do { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); } while(0);
//#define CS_DISABLE		do { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); } while(0);

/* SPI TIMEOUT Value*/
#define TIMEOUT_VAL 60

// The value of the Rref resistor in Ohm
#define RREF 430.0

/* Read Register Address */
#define REG_CONFIG                  0x00
#define REG_RTD_MSB                 0x01
#define REG_RTD_LSB                 0x02
#define REG_HIGH_FAULT_THR_MSB      0x03
#define REG_HIGH_FAULT_THR_LSB      0x04
#define REG_LOW_FAULT_THR_MSB       0x05
#define REG_LOW_FAULT_THR_LSB       0x06
#define REG_FAULT_STATUS            0x07
#define WR(reg)                     ( (reg) | 0x80 )
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
struct __attribute__((packed)) var_max31865
{
  uint16_t rtd_res_raw;			// RTD IC raw resistance register
  uint8_t  status;					// RTD status - full status code
  uint8_t  conf_reg;				// Configuration register readout
  uint16_t  HFT_val;				// High fault threshold register readout
  uint16_t  LFT_val;				// Low fault threshold register readout
};

struct var_max31865 rtd_data;
uint8_t read_addr = 0x00; //Read address of Configuration register

/*variables of resistance and temperature*/
double resistanceRTD;
double tmp;

/*register to initiate SPI*/
uint8_t config_reg_write[] = {WR(REG_CONFIG), 0xC2};

/*arrays to print values in for resistance and temperature*/
char Rrtd[30]; //array to print RTD resistance
char Trtd[30]; //array to print RTD temperature
char Stop[30]; //indicates all is read

/*ChipSelect pins and ports*/
GPIO_TypeDef* CS_GPIO_Port[10]={CS1_GPIO_Port, CS2_GPIO_Port,CS3_GPIO_Port,CS4_GPIO_Port,CS5_GPIO_Port,CS6_GPIO_Port,CS7_GPIO_Port,CS8_GPIO_Port,CS9_GPIO_Port,CS10_GPIO_Port};
uint16_t CS_Pin[10]={CS1_Pin,CS2_Pin,CS3_Pin,CS4_Pin,CS5_Pin,CS6_Pin,CS7_Pin,CS8_Pin,CS9_Pin,CS10_Pin};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void CS_ENSABLE(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void CS_DISABLE(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void configureSPI(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);
void MAX31865_full_read(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* Functions to en- and disable the CS_Pin from Port and Pin */
void CS_ENABLE(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
}

void CS_DISABLE(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

/* Function to configure and initiate SPI */

void configureSPI(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
 /* (1) : SPI Transmit, write to config reg on address 0x80 */
  // Step(1): Bring the CS pin low to activate the slave device
  CS_ENABLE(CS_GPIO_Port, CS_Pin);
  HAL_Delay(10); //This delay is very important in the case of STM32F334 in order to work with MAX31865
  // Step(2): Transmit config reg address  & data
  HAL_SPI_Transmit(&hspi2, &config_reg_write[0], 1, TIMEOUT_VAL);
  HAL_SPI_Transmit(&hspi2, &config_reg_write[1], 1, TIMEOUT_VAL);
  // Step(3): Bring the CS pin high again
  CS_DISABLE(CS_GPIO_Port, CS_Pin);
}

/* Function to unpack and store MAX31865 data */

void MAX31865_full_read(GPIO_TypeDef* CS_GPIO_Port, uint16_t CS_Pin)
{
	uint8_t read_data[8]; //variable to store the contents of the registers
	//printf("read_data raw: %u", read_data);
	uint8_t i = 0; //loop variable
	
	// Step(1): Bring the CS pin low to activate the slave device
	CS_ENABLE(CS_GPIO_Port, CS_Pin);
	// Step(2): Transmit config reg address telling IC that we want to 'read' and start at register 0
	HAL_SPI_Transmit(&hspi2, &read_addr, 1, TIMEOUT_VAL);
	/* Step (3): Receive the first 8 bits (Config reg data) */
	for(i = 0; i < 8; i++)
	{
		HAL_SPI_Receive(&hspi2, &read_data[i], 1, TIMEOUT_VAL);
	}
	// Step(4): Bring the CS pin high again
	CS_DISABLE( CS_GPIO_Port, CS_Pin);
	/* Step (5): Store the data read from the sensor */
	rtd_data.conf_reg = read_data[0]; //Store config reg data
	rtd_data.rtd_res_raw = ((read_data[1] << 8) | read_data[2]) >> 1; // Store rtd_res_raw
	rtd_data.HFT_val = ((read_data[3] << 8) | read_data[4]) >> 1; // Store HFT_val
	rtd_data.LFT_val = (read_data[5] << 8) | read_data[6]; // Store LFT_val
	rtd_data.status = read_data[7]; //Store fault status reg data	

  /* calculate RTD resistance */
    resistanceRTD = ((double)rtd_data.rtd_res_raw * RREF) / 32768; // Replace 4000 by 400 for PT100
		sprintf(Rrtd, "\n%u: \nRrtd = %lf\n", CS_Pin, resistanceRTD);
    HAL_UART_Transmit(&huart2, (uint8_t *)Rrtd, 30, TIMEOUT_VAL); // print RTD resistance
	
	/* calculate RTD temperature (simple calc, +/- 2 deg C from -100C to 100C) */
    /* CALCULATION OF TEMP MUST BE ADJUSTED TO RTD  */
    tmp = ((double)rtd_data.rtd_res_raw / 32) - 256;
		sprintf(Trtd, "Trtd = %lf\n", tmp);
    HAL_UART_Transmit(&huart2, (uint8_t *)Trtd, 30, TIMEOUT_VAL); // print RTD temperature
	
	HAL_Delay(1000);
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI2_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

for(int conf=0;conf< 10;conf++)
	{
	configureSPI(CS_GPIO_Port[conf],CS_Pin[conf]);
	HAL_Delay(500);

	}	
	// give the sensor time to set up
  HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	for(int i=0;i< 10;i++)
		{
		MAX31865_full_read(CS_GPIO_Port[i],CS_Pin[i]);
		
		}
	HAL_Delay(2000);
	sprintf(Stop, "%i sensors read\n", 8);

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CS4_Pin|CS3_Pin|CS9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CS8_Pin|CS7_Pin|CS2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CS1_Pin|CS5_Pin|CS6_Pin|CS10_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS4_Pin CS3_Pin CS9_Pin */
  GPIO_InitStruct.Pin = CS4_Pin|CS3_Pin|CS9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CS8_Pin CS7_Pin CS2_Pin */
  GPIO_InitStruct.Pin = CS8_Pin|CS7_Pin|CS2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : CS1_Pin CS5_Pin CS6_Pin CS10_Pin */
  GPIO_InitStruct.Pin = CS1_Pin|CS5_Pin|CS6_Pin|CS10_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
