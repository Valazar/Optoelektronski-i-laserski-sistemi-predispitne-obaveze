/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_cdc_if.h"

#include "SSD1963.h"
#include "STMPE610.h"

#include "GUI.h"
#include "LCDConf.h"

#include <stdint.h>
#include <stdio.h>

#include "Windows/StopwatchDLG.h"


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

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
extern volatile GUI_TIMER_TIME OS_TimeMS;
uint16_t X_koordinata = 0;
uint16_t Y_koordinata = 0;
uint16_t keyPressed, flegStoperica, flegProlazno, flegPonovi = 0;
int i=0;
char message[5];
char timestring[4]="alaa"; // koristimo za proveru EDIT polja


WM_HWIN hButton0, hButton1, hButton2, hButton3, hButton4, hButton5, hButton6, hButton7, hButton8, hButton9, hButton10, hButton11, hButton12, hButton13;
WM_HWIN hEdit0, hEdit1, hEdit2, hEdit3, hEdit4, hEdit5, hEdit6, hEdit7, hEdit8, hEdit9, hEdit10, hEdit11, hEdit12, hEdit13, hEdit14, hEdit15, hEdit16, hEdit17, hEdit18;
WM_HWIN hText0, hText1, hText2, hText3;
//WM_HWIN hImage0;

//RTC_AlarmTypeDef globalAlarmTime = {0};
RTC_TimeTypeDef AlarmPodesavanje = {0}; // globalna promenljiva u kojoj smestamo sate i minute i koristimo za inkrement, dekrement..
RTC_TimeTypeDef AlarmVrijeme = {0};    // globalna proemnljiva pomocu koje kupimo trenutno vreme ( sate i minute )
RTC_TimeTypeDef globalAlarmTime = {0}; // globalna promenljiva za setovano vreme alarma
RTC_TimeTypeDef stoperica, stoperica2 = {0};
RTC_TimeTypeDef prolaznoVrijeme = {0};

GUI_MEMDEV_Handle hMem;

char time[10];
char date[10];
char a[6];
char alarm = 0;
char slika = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

void set_time (void);
void to_do_on_alarm(void);
void set_alarm(void);
void get_time (void);
void get_alarm (void);
void show_alarm(void);
void sakri_alarm_stopwatch(void);
void sakri_alarm_worldclock(void);
void sakri_worldclock_stop_watch(void);
void set_time_to_edit_box(WM_HWIN hEdit);
void set_time_to_edit_boxes(WM_HWIN hEditBeograd, WM_HWIN hEditHavana, WM_HWIN hEditTokio, WM_HWIN hEditMoskva);
void set_minutiMinus_to_edit_box(WM_HWIN hEdit);
void set_minutiPlus_to_edit_box(WM_HWIN hEdit);
void set_satiMinus_to_edit_box(WM_HWIN hEdit);
void set_satiPlus_to_edit_box(WM_HWIN hEdit);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void upis(WM_HWIN hEdit);
void upis_alarm(WM_HWIN hEdit);
void upis_stoperica(WM_HWIN hEdit);
void upis_stoperica2(WM_HWIN hEdit);
void prolazno_vrijeme(WM_HWIN hEdit);
void upis_reset(void);
void set_buzzer(uint16_t frekvencija, uint8_t faktor_ispune);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_SYSTICK_Callback(void)
{
	OS_TimeMS++;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_I2C1_Init();
  MX_USB_DEVICE_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
  {
	  set_time();

  }

  /* Initialize all configured peripherals */
   Init_LCD_GPIO();
   Init_TOUCH_GPIO(hi2c1);

   STMPE610_Init();

   WM_SetCreateFlags(WM_CF_MEMDEV); // eliminise flickering



   GUI_Init();
   //   STMPE610_Init();


    GUI_SetBkColor(GUI_YELLOW);
    GUI_Clear();

    //BUTTON_Handle hImage0 = BUTTON_CreateEx(335, 115, 145, 155, 0, WM_CF_SHOW, 0, ID_IMAGE_0);
    //WM_SetCallback(hImage0, OnImageClicked);
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetFont(&GUI_Font32_1);
//    GUI_DispString("Hello");

//    hMem=GUI_MEMDEV_Create(0,0,200,200);
//
//      GUI_MEMDEV_Select(hMem);
//       GUI_SetBkColor(GUI_BLUE);
//       GUI_Clear();
//       GUI_SetColor(GUI_RED);
//       GUI_SetDrawMode(GUI_DRAWMODE_NORMAL);
//       GUI_FillCircle(100, 100, 50);
//       GUI_SetDrawMode(GUI_DRAWMODE_XOR);
//       GUI_FillCircle(120, 120, 50);
//       GUI_MEMDEV_CopyToLCD(hMem);
//       GUI_MEMDEV_Delete(hMem);

    CreateStopwatch();
    WM_HWIN hDialog = CreateStopwatch();

    //hImage0  = WM_GetDialogItem(hDialog, ID_IMAGE_0);
    hButton0 = WM_GetDialogItem(hDialog, ID_BUTTON_0);
    hButton1 = WM_GetDialogItem(hDialog, ID_BUTTON_1);
    hButton2 = WM_GetDialogItem(hDialog, ID_BUTTON_2);
    hButton3 = WM_GetDialogItem(hDialog, ID_BUTTON_3);
    hButton4 = WM_GetDialogItem(hDialog, ID_BUTTON_4);
    hButton5 = WM_GetDialogItem(hDialog, ID_BUTTON_5);
    hButton6 = WM_GetDialogItem(hDialog, ID_BUTTON_6);
    hButton7 = WM_GetDialogItem(hDialog, ID_BUTTON_7);
    hButton8 = WM_GetDialogItem(hDialog, ID_BUTTON_8);
    hButton9 = WM_GetDialogItem(hDialog, ID_BUTTON_9);
    hButton10 = WM_GetDialogItem(hDialog, ID_BUTTON_10);
    hButton11 = WM_GetDialogItem(hDialog, ID_BUTTON_11);
    hButton12 = WM_GetDialogItem(hDialog, ID_BUTTON_12);
    hButton13 = WM_GetDialogItem(hDialog, ID_BUTTON_13);
    hEdit0 = WM_GetDialogItem(hDialog, ID_EDIT_0);
    hEdit1 = WM_GetDialogItem(hDialog, ID_EDIT_1);
    hEdit2 = WM_GetDialogItem(hDialog, ID_EDIT_2);
    hEdit3 = WM_GetDialogItem(hDialog, ID_EDIT_3);
    hEdit4 = WM_GetDialogItem(hDialog, ID_EDIT_4);
    hEdit5 = WM_GetDialogItem(hDialog, ID_EDIT_5);
    hEdit6 = WM_GetDialogItem(hDialog, ID_EDIT_6);
    hEdit7 = WM_GetDialogItem(hDialog, ID_EDIT_7);
    hEdit8 = WM_GetDialogItem(hDialog, ID_EDIT_8);
    hEdit9 = WM_GetDialogItem(hDialog, ID_EDIT_9);
    hEdit10 = WM_GetDialogItem(hDialog, ID_EDIT_10);
    hEdit11 = WM_GetDialogItem(hDialog, ID_EDIT_11);
    hEdit12 = WM_GetDialogItem(hDialog, ID_EDIT_12);
    hEdit13 = WM_GetDialogItem(hDialog, ID_EDIT_13);
    hEdit14 = WM_GetDialogItem(hDialog, ID_EDIT_14);
    hEdit15 = WM_GetDialogItem(hDialog, ID_EDIT_15);
    hEdit16 = WM_GetDialogItem(hDialog, ID_EDIT_16);
    hEdit17 = WM_GetDialogItem(hDialog, ID_EDIT_17);
    hEdit18 = WM_GetDialogItem(hDialog, ID_EDIT_18);
    hText0 = WM_GetDialogItem(hDialog, ID_TEXT_0);
    hText1 = WM_GetDialogItem(hDialog, ID_TEXT_1);
    hText2 = WM_GetDialogItem(hDialog, ID_TEXT_2);
    hText3 = WM_GetDialogItem(hDialog, ID_TEXT_3);
    WM_HWIN hEditBeograd = WM_GetDialogItem(hDialog, ID_EDIT_15);
    WM_HWIN hEditHavana = WM_GetDialogItem(hDialog, ID_EDIT_16);
    WM_HWIN hEditTokio = WM_GetDialogItem(hDialog, ID_EDIT_17);
    WM_HWIN hEditMoskva = WM_GetDialogItem(hDialog, ID_EDIT_18);

    WM_HWIN hEditTrenutno1 = WM_GetDialogItem(hDialog, ID_EDIT_11);
    WM_HWIN hEditSati = WM_GetDialogItem(hDialog, ID_EDIT_13);
    WM_HWIN hEditMinuti = WM_GetDialogItem(hDialog, ID_EDIT_14);


    HAL_RTC_GetTime(&hrtc, &AlarmVrijeme, RTC_FORMAT_BIN);

    AlarmPodesavanje.Hours = AlarmVrijeme.Hours;
    AlarmPodesavanje.Minutes = AlarmVrijeme.Minutes;
    stoperica.Seconds=0x00;
    stoperica.Minutes=0x00;
    stoperica2.Seconds=0x00;
    stoperica2.Minutes=0x00;

	sakri_worldclock_stop_watch(); // prikazuje samo polja za ALARM

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  STMPE610_read_xyz();
      GUI_TOUCH_Exec();
      keyPressed=GUI_GetKey();

      GUI_Delay(1);

      X_koordinata=STMPE610_GetX_TOUCH();
      Y_koordinata=STMPE610_GetY_TOUCH();

      switch(keyPressed)
     		  {
     		              case ID_BUTTON_0:
     		            	 sakri_worldclock_stop_watch(); // prikazuje samo polja za ALARM
     		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
     		            	 GUI_Delay(10);

       			  break;
                          case ID_BUTTON_1:
                        	  sakri_alarm_stopwatch(); // prikazuje polja samo za WORLDCLOK
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  GUI_Delay(10);
                              GUI_Delay(10);

       			  break;
                          case ID_BUTTON_2:
                        	  sakri_alarm_worldclock(); // prikazuje polja samo za STOPWATCH
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  GUI_Delay(10);


                  break;
                          case ID_BUTTON_3: //RESUME
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  flegStoperica = 1;
                        	  flegProlazno = 1 ;
                          	  WM_ShowWindow(hButton11);
                          	  WM_ShowWindow(hButton10);
                          	  WM_HideWindow(hButton3);
                          	  WM_HideWindow(hButton4);
                  break;
                          case ID_BUTTON_4: //RESET
                        	  flegStoperica=0;
                          	  WM_ShowWindow(hButton12);
                          	  WM_HideWindow(hButton3);
                          	  WM_HideWindow(hButton4);
                          	  WM_HideWindow(hButton11);
                          	  WM_HideWindow(hButton10);
                              stoperica.Seconds=0x00;
                              stoperica.Minutes=0x00;
                              stoperica2.Seconds=0x00;
                              stoperica2.Minutes=0x00;
                              upis_reset();

      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                  break;
                          case ID_BUTTON_5:
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  set_alarm();
                        	  GUI_Delay(10);
                        	  upis_alarm(hEdit12); // upis setovanog alarma u edit polje iznad slike

                  break;
                          case ID_BUTTON_6:
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  set_satiPlus_to_edit_box(hEditSati);



                  break;
                          case ID_BUTTON_7:
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  set_satiMinus_to_edit_box(hEditSati);

                  break;
                          case ID_BUTTON_8:
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  set_minutiPlus_to_edit_box(hEditMinuti);
                  break;
                          case ID_BUTTON_9:
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                        	  set_minutiMinus_to_edit_box(hEditMinuti);
                  break;
                          case ID_BUTTON_10: //STOP
                        	  flegStoperica=0;
                        	  flegProlazno=0;
                          	  WM_ShowWindow(hButton3);
                          	  WM_ShowWindow(hButton4);
                          	  WM_HideWindow(hButton11);
                          	  WM_HideWindow(hButton10);
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

                  break;
                          case ID_BUTTON_11: //PROLAZNO VRIJEME
                        	  flegProlazno++;
                        	  if(flegProlazno==1){

                        		  prolazno_vrijeme(hEdit2); //prva dva edit boxa za prolazno vrijeme
                        		  prolazno_vrijeme(hEdit4);
                        	  }
                        	  else if(flegProlazno==2){		//druga dva edit boxa za prolazno vrijeme
                        		  prolazno_vrijeme(hEdit6); //ukupno vrijeme za drugp prolazno vrijeme
                        		  upis_stoperica2(hEdit8);
                        	  }

                        	  else if(flegProlazno==3){		//treca dva edit boxa za prolazno vrijeme
                        		  upis_stoperica2(hEdit9);
                        		  prolazno_vrijeme(hEdit7); //ukupno vrijeme za poslednje prolazno vrijeme
                        	  }
                        	  else if(flegProlazno==4){
                        		  flegProlazno=0;
                        	  }
      		            	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

                  break;
                          case ID_BUTTON_12: //START
                        	  flegStoperica=1;
      		            	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
                          	  WM_HideWindow(hButton12);
                          	  WM_ShowWindow(hButton11);
                          	  WM_ShowWindow(hButton10);

       	          break;

                          case ID_IMAGE_0: // SLIKA
//                        	  //flegStoperica=1;
                              	  //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
//                          	  //WM_HideWindow(hButton12);
//                          	  //WM_ShowWindow(hButton11);
//                          	  //WM_ShowWindow(hButton10);
//
                  break;


     		  }


     	  GUI_Delay(10);
     	  get_time();
     	  HAL_Delay(10);
     	  //get_alarm();

    	  if(alarm==1){

    		//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, 1);
    		set_buzzer(400, 80);


    	  }

    	  if(X_koordinata>400 && X_koordinata<480 && Y_koordinata>200 && Y_koordinata<270){

          	 HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
    		  alarm=0;
    		  set_buzzer(0, 0);

    	  }


          set_time_to_edit_boxes(hEditBeograd,hEditHavana, hEditMoskva, hEditTokio);
          set_time_to_edit_box(hEditTrenutno1);

          if(flegStoperica==1){
        	  upis_stoperica(hEdit0);
          }
          if(flegProlazno!=0){
        	  upis_stoperica2(hEdit3);
          }

          GUI_Delay(1000);
      	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, 0); //ISKLJUCI LED ZA ALARM

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x20303E5D;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  //RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x12;
  sTime.Minutes = 0x12;
  sTime.Seconds = 0x12;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x11;
  sDate.Year = 0x24;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A
  */
//  sAlarm.AlarmTime.Hours = 0x0;
//  sAlarm.AlarmTime.Minutes = 0x0;
//  sAlarm.AlarmTime.Seconds = 0x0;
//  sAlarm.AlarmTime.SubSeconds = 0x0;
//  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
//  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
//  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
//  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
//  sAlarm.AlarmDateWeekDay = 0x1;
//  sAlarm.Alarm = RTC_ALARM_A;
//  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
//  {
//    Error_Handler();
//  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 25;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1920;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 25;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 9600;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void set_time (void)
{

	  RTC_TimeTypeDef sTime = {0};
	  RTC_DateTypeDef sDate = {0};
	 /** Initialize RTC and set the Time and Date
	  */
	  sTime.Hours = 0x15;
	  sTime.Minutes = 0x09;
	  sTime.Seconds = 0x40;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
	  sDate.Month = RTC_MONTH_MARCH;
	  sDate.Date = 0x01;
	  sDate.Year = 0x24;
	  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  //saveTimeToBackupRegister();
	  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);


}


void set_alarm(void) {
    RTC_AlarmTypeDef sAlarm = {0};

    // Postavljanje vremena alarma
    sAlarm.AlarmTime.Hours = globalAlarmTime.Hours ;
    sAlarm.AlarmTime.Minutes = globalAlarmTime.Minutes;
    sAlarm.AlarmTime.Seconds = globalAlarmTime.Seconds;
    sAlarm.AlarmTime.SubSeconds = 0x0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_MINUTES;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 0x1;
    sAlarm.Alarm = RTC_ALARM_A;


    // Postavljanje alarma
    if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
        Error_Handler();
    }
}


void get_time(void){

	RTC_DateTypeDef gDate;
	RTC_TimeTypeDef gTime;

	HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);


	sprintf((char*)time, "%02d:%02d:%02d\r",gTime.Hours, gTime.Minutes, gTime.Seconds);
	 CDC_Transmit_FS((uint8_t *)time, strlen((const char *)time));

	sprintf((char*)date, "%02d-%02d-%2d\r", gDate.Date, gDate.Month, 2000 + gDate.Year);
	 CDC_Transmit_FS((uint8_t *)date, strlen((const char *)date));
}

void get_alarm (void){

	RTC_AlarmTypeDef gAlarm;

	HAL_RTC_GetAlarm(&hrtc, &gAlarm, RTC_ALARM_A, RTC_FORMAT_BCD);

}


void sakri_worldclock_stop_watch(void){
	WM_HideWindow(hText0);
	WM_HideWindow(hText1);
    WM_HideWindow(hText2);
	WM_HideWindow(hText3);
	WM_HideWindow(hButton3);
	WM_HideWindow(hButton4);
	//WM_HideWindow(hButton5);
	WM_HideWindow(hEdit0);
	WM_HideWindow(hEdit1);
	WM_HideWindow(hEdit2);
	WM_HideWindow(hEdit3);
	WM_HideWindow(hEdit4);
	WM_HideWindow(hEdit5);
	WM_HideWindow(hEdit6);
	WM_HideWindow(hEdit7);
	WM_HideWindow(hEdit8);
	WM_HideWindow(hEdit9);
	WM_HideWindow(hEdit10);
	//WM_HideWindow(hEdit11);
	//WM_HideWindow(hEdit12);
	//WM_HideWindow(hEdit13);
	//WM_HideWindow(hEdit14);
	WM_HideWindow(hEdit15);
	WM_HideWindow(hEdit16);
	WM_HideWindow(hEdit17);
	WM_HideWindow(hEdit18);
	WM_ShowWindow(hEdit11);
	WM_ShowWindow(hEdit12);
	WM_ShowWindow(hEdit13);
	WM_ShowWindow(hEdit14);
	WM_ShowWindow(hButton6);
	WM_ShowWindow(hButton7);
	WM_ShowWindow(hButton8);
	WM_ShowWindow(hButton9);
	WM_ShowWindow(hButton5);
	WM_HideWindow(hButton10);
	WM_HideWindow(hButton11);
	WM_HideWindow(hButton12);
	WM_HideWindow(hButton13);

}


void sakri_alarm_stopwatch (void) {
	//WM_HideWindow(hEdit11);
	//WM_HideWindow(hEdit12);
	WM_HideWindow(hEdit13);
	WM_HideWindow(hEdit14);
	WM_HideWindow(hButton6);
	WM_HideWindow(hButton7);
	WM_HideWindow(hButton8);
	WM_HideWindow(hButton9);
	WM_HideWindow(hButton10);
	WM_HideWindow(hButton11);
	WM_HideWindow(hButton12);
	WM_HideWindow(hButton13);
	WM_HideWindow(hButton3);
	WM_HideWindow(hButton4);
	WM_HideWindow(hButton5);
	WM_HideWindow(hEdit0);
	WM_HideWindow(hEdit1);
	WM_HideWindow(hEdit2);
	WM_HideWindow(hEdit3);
	WM_HideWindow(hEdit4);
	WM_HideWindow(hEdit5);
	WM_HideWindow(hEdit6);
	WM_HideWindow(hEdit7);
	WM_HideWindow(hEdit8);
	WM_HideWindow(hEdit9);
	WM_HideWindow(hEdit10);
	//WM_HideWindow(hEdit15);
	//WM_HideWindow(hEdit16);
	//WM_HideWindow(hEdit17);
	//WM_HideWindow(hEdit18);
	WM_ShowWindow(hText0);
	WM_ShowWindow(hText1);
	WM_ShowWindow(hText2);
    WM_ShowWindow(hText3);
    WM_ShowWindow(hEdit15);
    WM_ShowWindow(hEdit16);
    WM_ShowWindow(hEdit17);
    WM_ShowWindow(hEdit18);



}


void sakri_alarm_worldclock(void) {
	WM_HideWindow(hText0);
	WM_HideWindow(hText1);
	WM_HideWindow(hText2);
	WM_HideWindow(hText3);
	WM_HideWindow(hEdit15);
	WM_HideWindow(hEdit16);
	WM_HideWindow(hEdit17);
	WM_HideWindow(hEdit18);
	//WM_HideWindow(hEdit11);
	//WM_HideWindow(hEdit12);
	WM_HideWindow(hEdit13);
	WM_HideWindow(hEdit14);
	WM_HideWindow(hButton3);
	WM_HideWindow(hButton4);
	WM_HideWindow(hButton5);
	WM_HideWindow(hButton6);
	WM_HideWindow(hButton7);
	WM_HideWindow(hButton8);
	WM_HideWindow(hButton9);
	WM_HideWindow(hButton10);
	WM_HideWindow(hButton11);
	WM_HideWindow(hButton13);
	WM_ShowWindow(hEdit0);
	WM_ShowWindow(hEdit1);
	WM_ShowWindow(hEdit2);
	WM_ShowWindow(hEdit3);
	WM_ShowWindow(hEdit4);
	WM_ShowWindow(hEdit5);
	WM_ShowWindow(hEdit6);
	WM_ShowWindow(hEdit7);
	WM_ShowWindow(hEdit8);
	WM_ShowWindow(hEdit9);
	WM_ShowWindow(hEdit10);
	//WM_ShowWindow(hButton4); //reset
	//WM_ShowWindow(hButton3); //resume
	//WM_ShowWindow(hButton10);
	//WM_ShowWindow(hButton11);
	WM_ShowWindow(hButton12);


}


void set_time_to_edit_boxes(WM_HWIN hEditBeograd, WM_HWIN hEditHavana, WM_HWIN hEditTokio, WM_HWIN hEditMoskva) {
	//while (1) {
        RTC_TimeTypeDef gTime;

        HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);

        char timeBeograd[9];
        char timeHavana[9];
        char timeTokio[9];
        char timeMoskva[9];

        // Postavljanje vremena za Beograd
        sprintf(timeBeograd, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);
        EDIT_SetText(hEditBeograd, timeBeograd);

        // Postavljanje vremena za Havana (recimo -7 sati u odnosu na GMT)
        gTime.Hours -= 7;
        if (gTime.Hours < 0) {
            gTime.Hours += 24;
        }
        sprintf(timeHavana, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);
        EDIT_SetText(hEditHavana, timeHavana);

        // Postavljanje vremena za Moskvu (recimo +3 sata u odnosu na GMT)
        gTime.Hours += 9;
        if (gTime.Hours >= 24) {
            gTime.Hours -= 24;
        }
        sprintf(timeMoskva, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);
        EDIT_SetText(hEditMoskva, timeMoskva);

        // Postavljanje vremena za Tokio (recimo +9 sati u odnosu na GMT)
        gTime.Hours += 7;
        if (gTime.Hours >= 24) {
            gTime.Hours -= 24;
        }
        sprintf(timeTokio, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);
        EDIT_SetText(hEditTokio, timeTokio);



    //}
}



void set_time_to_edit_box(WM_HWIN hEdit) {

    RTC_TimeTypeDef gTime;

    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);

    char timeString[9]; // Deklaracija timeString unutar funkcije

    sprintf(timeString, "%02d:%02d:%02d", gTime.Hours, gTime.Minutes, gTime.Seconds);

    // Postavljanje teksta u edit box
    EDIT_SetText(hEdit, timeString);


}

void set_satiPlus_to_edit_box(WM_HWIN hEdit) {

    AlarmPodesavanje.Hours++;
            if (AlarmPodesavanje.Hours >= 24) {
                AlarmPodesavanje.Hours = 0;
            }

    char timeString[9]; // Deklaracija timeString unutar funkcije

    sprintf(timeString, "%02d", AlarmPodesavanje.Hours);

    // Postavljanje teksta u edit box
    EDIT_SetText(hEdit, timeString);
    GUI_Delay(1000);


}

void set_satiMinus_to_edit_box(WM_HWIN hEdit) {


    AlarmPodesavanje.Hours--;
            if (AlarmPodesavanje.Hours < 0) {
                AlarmPodesavanje.Hours = 24;
            }

    char timeString[9]; // Deklaracija timeString unutar funkcije

    sprintf(timeString, "%02d", AlarmPodesavanje.Hours);

    // Postavljanje teksta u edit box
    EDIT_SetText(hEdit, timeString);
    GUI_Delay(1000);


}

void set_minutiPlus_to_edit_box(WM_HWIN hEdit) {

    AlarmPodesavanje.Minutes++;
            if (AlarmPodesavanje.Minutes  >= 60) {
                AlarmPodesavanje.Minutes  = 0;
            }

    char timeString[9]; // Deklaracija timeString unutar funkcije

    sprintf(timeString, "%02d", AlarmPodesavanje.Minutes );

    // Postavljanje teksta u edit box
    EDIT_SetText(hEdit, timeString);
    GUI_Delay(1000);

}

void set_minutiMinus_to_edit_box(WM_HWIN hEdit) {

    AlarmPodesavanje.Minutes--;
            if (AlarmPodesavanje.Minutes  ==0 ) {
                AlarmPodesavanje.Minutes  = 59;
            }

    char timeString[9]; // Deklaracija timeString unutar funkcije

    sprintf(timeString, "%02d", AlarmPodesavanje.Minutes );

    // Postavljanje teksta u edit box
    EDIT_SetText(hEdit, timeString);
    GUI_Delay(1000);

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{

	alarm = 1;


}


void upis(WM_HWIN hEdit) {
    RTC_TimeTypeDef gTime;

    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);

    // Kreiranje stringa za vreme
    char timeString[20];
    sprintf(timeString, "%02d:%02d:%02d\r", gTime.Hours, gTime.Minutes, gTime.Seconds);

    // Postavljanje vremena u EDIT polje (prilagoditi ID_EDIT_X na ta�?an ID vašeg EDIT polja)
    if (hEdit != 0) {
        EDIT_SetText(hEdit, timeString);
    }

    // Slanje vremena preko neophodnog mehanizma (npr. CDC_Transmit_FS)
}

void upis_alarm(WM_HWIN hEdit) {
    // Kreiranje stringa za vreme postavljeno u set_alarm funkciji
    char timeString[20];

    //globalAlarmTime=AlarmPodesavanje;
    sprintf(timeString, "%02d:%02d:%02d\r", AlarmPodesavanje.Hours, AlarmPodesavanje.Minutes, AlarmPodesavanje.Seconds);

    // Postavljanje vremena u EDIT polje
    if (hEdit != 0) {
        EDIT_SetText(hEdit, timeString);
    }

    // Dodatne radnje po potrebi
}

void upis_stoperica(WM_HWIN hEdit) { //funkcija za upis vremena za stopericu
    // Kreiranje stringa za vreme postavljeno u set_alarm funkciji

    char timeString[20];

    stoperica.Seconds++;
    if(stoperica.Seconds==59){
    	stoperica.Seconds=0;
        stoperica.Minutes++;
    }
    sprintf(timeString, "%02d:%02d\r", stoperica.Minutes, stoperica.Seconds);

    // Postavljanje vremena u EDIT polje
    if (hEdit != 0) {
        EDIT_SetText(hEdit, timeString);
    }

    // Dodatne radnje po potrebi
}

void prolazno_vrijeme(WM_HWIN hEdit){

	char timeString[20];

    sprintf(timeString, "%02d:%02d\r", stoperica.Minutes, stoperica.Seconds);

    // Postavljanje vremena u EDIT polje
    if (hEdit != 0) {
        EDIT_SetText(hEdit, timeString);
    }

}

void upis_stoperica2(WM_HWIN hEdit) { //funkcija za upis vremena za stopericu
    // Kreiranje stringa za vreme postavljeno u set_alarm funkciji

    char timeString[20];

    stoperica2.Seconds++;
    if(stoperica2.Seconds==59){
    	stoperica2.Seconds=0;
    	stoperica2.Minutes++;
    }
    sprintf(timeString, "%02d:%02d\r", stoperica2.Minutes, stoperica2.Seconds);

    // Postavljanje vremena u EDIT polje
    if (hEdit != 0) {
        EDIT_SetText(hEdit, timeString);
    }

    // Dodatne radnje po potrebi
}

void upis_reset(void){

	char timeString[20];
	RTC_TimeTypeDef nulaTime = {0};

	nulaTime.Minutes=0;
	nulaTime.Seconds=0;

	sprintf(timeString, "%02d:%02d\r", nulaTime.Minutes, nulaTime.Seconds);

    if (hEdit0 != 0) {
        EDIT_SetText(hEdit0, timeString);
    }
    if (hEdit2 != 0) {
        EDIT_SetText(hEdit2, timeString);
    }
    if (hEdit3 != 0) {
        EDIT_SetText(hEdit3, timeString);
    }
    if (hEdit4 != 0) {
        EDIT_SetText(hEdit4, timeString);
    }
    if (hEdit6 != 0) {
        EDIT_SetText(hEdit6, timeString);
    }
    if (hEdit8 != 0) {
        EDIT_SetText(hEdit8, timeString);
    }
    if (hEdit7 != 0) {
        EDIT_SetText(hEdit7, timeString);
    }
    if (hEdit9 != 0) {
        EDIT_SetText(hEdit9, timeString);
    }
}

void set_buzzer(uint16_t frekvencija, uint8_t faktor_ispune)
{

__HAL_TIM_SetAutoreload(&htim4, 3840000/frekvencija); // Set Frequency
 __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3,faktor_ispune); // Set Volume

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
