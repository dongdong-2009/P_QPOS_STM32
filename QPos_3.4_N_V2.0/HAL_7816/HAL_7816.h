#ifndef __HAL_7816_H
#define __HAL_7816_H


#ifdef __cplusplus
 extern "C" {
#endif 
#define  RCC_AHBENR_GPIOAEN                  ((uint32_t)0x00000001)        /*!< GPIO port A clock enable */
#define  RCC_AHBENR_GPIOBEN                  ((uint32_t)0x00000002)        /*!< GPIO port B clock enable */
#define  RCC_AHBENR_GPIOCEN                  ((uint32_t)0x00000004)        /*!< GPIO port C clock enable */
#define  RCC_AHBENR_GPIODEN                  ((uint32_t)0x00000008)        /*!< GPIO port D clock enable */
#define  RCC_AHBENR_GPIOEEN                  ((uint32_t)0x00000010)        /*!< GPIO port E clock enable */
#define  RCC_AHBENR_GPIOHEN                  ((uint32_t)0x00000020)        /*!< GPIO port H clock enable */
#define  RCC_AHBENR_GPIOFEN                  ((uint32_t)0x00000040)        /*!< GPIO port F clock enable */
#define  RCC_AHBENR_GPIOGEN                  ((uint32_t)0x00000080)        /*!< GPIO port G clock enable */

#define RCC_AHBPeriph_GPIOA               RCC_AHBENR_GPIOAEN
#define RCC_AHBPeriph_GPIOB               RCC_AHBENR_GPIOBEN
#define RCC_AHBPeriph_GPIOC               RCC_AHBENR_GPIOCEN
#define RCC_AHBPeriph_GPIOD               RCC_AHBENR_GPIODEN
#define RCC_AHBPeriph_GPIOE               RCC_AHBENR_GPIOEEN
#define RCC_AHBPeriph_GPIOH               RCC_AHBENR_GPIOHEN
#define RCC_AHBPeriph_GPIOF               RCC_AHBENR_GPIOFEN
#define RCC_AHBPeriph_GPIOG               RCC_AHBENR_GPIOGEN





extern unsigned char g_Flag_InsertCard;

#define SC_USART                 USART1
#define SC_USART_GPIO            GPIOA
#define SC_USART_CLK             RCC_APB2Periph_USART1
#define SC_USART_GPIO_CLK        RCC_AHBPeriph_GPIOA
#define SC_USART_PIN_TX          GPIO_Pin_9 //pa9
#define SC_USART_PIN_CK          GPIO_Pin_8 //pa8
#define SC_USART_IRQn            USART1_IRQn
//#define SC_USART_IRQHandler      USART1_IRQHandler 
/* Smartcard Inteface GPIO pins */

#define SC_PIN_3_5V			GPIO_Pin_0 
#define SC_PIN_3_5V_GPIO          GPIOB
#define SC_PIN_3_5V_GPIO_CLK      RCC_AHBPeriph_GPIOB

#define SC_PIN_1_8V			GPIO_Pin_3
#define SC_PIN_1_8V_GPIO          GPIOA
#define SC_PIN_1_8V_GPIO_CLK      RCC_AHBPeriph_GPIOA

#define SC_PIN_RESET              GPIO_Pin_1
#define SC_PIN_RESET_GPIO         GPIOA
#define SC_PIN_RESET_GPIO_CLK     RCC_AHBPeriph_GPIOA

#define SC_PIN_CMDVCC             GPIO_Pin_1
#define SC_PIN_CMDVCC_GPIO        GPIOB
#define SC_PIN_CMDVCC_GPIO_CLK    RCC_AHBPeriph_GPIOB


#define SC_PIN_OFF                GPIO_Pin_0
#define SC_PIN_OFF_GPIO           GPIOA
#define SC_PIN_OFF_GPIO_CLK       RCC_AHBPeriph_GPIOA	

//#define SC_DETECT_EXTI            EXTI_Line7
//#define SC_DETECT_PIN             GPIO_PortSourceGPIOA
//#define SC_DETECT_GPIO            GPIO_PinSource7
//#define SC_DETECT_IRQ             EXTI9_5_IRQn
//#define SC_DETECT_IRQHandler      EXTI9_5_IRQHandler


/* Exported constants --------------------------------------------------------*/
#define T0_PROTOCOL        0x00  /* T0 protocol */
#define DIRECT             0x3B  /* Direct bit convention */
#define INDIRECT           0x3F  /* Indirect bit convention */
#define SETUP_LENGTH       20
#define HIST_LENGTH        20
#define LC_MAX             0xFF
#define SC_RECEIVE_TIMEOUT 0x4000 /* Direction to reader */

/* SC Tree Structure -----------------------------------------------------------
                              MasterFile
                           ________|___________
                          |        |           |
                        System   UserData     Note
------------------------------------------------------------------------------*/

/* SC ADPU Command: Operation Code -------------------------------------------*/
#define SC_CLA_GSM11       0xA0

/*------------------------ Data Area Management Commands ---------------------*/
#define SC_SELECT_FILE     0xA4
#define SC_GET_RESPONCE    0xC0
#define SC_STATUS          0xF2
#define SC_UPDATE_BINARY   0xD6
#define SC_READ_BINARY     0xB0
#define SC_WRITE_BINARY    0xD0
#define SC_UPDATE_RECORD   0xDC
#define SC_READ_RECORD     0xB2

/*-------------------------- Administrative Commands -------------------------*/ 
#define SC_CREATE_FILE     0xE0

/*-------------------------- Safety Management Commands ----------------------*/
#define SC_VERIFY          0x20
#define SC_CHANGE          0x24
#define SC_DISABLE         0x26
#define SC_ENABLE          0x28
#define SC_UNBLOCK         0x2C
#define SC_EXTERNAL_AUTH   0x82
#define SC_GET_CHALLENGE   0x84

/*-------------------------- Answer to reset Commands ------------------------*/ 
#define SC_GET_A2R         0x00

/* SC STATUS: Status Code ----------------------------------------------------*/
#define SC_EF_SELECTED     0x9F
#define SC_DF_SELECTED     0x9F
#define SC_OP_TERMINATED   0x9000

/* Smartcard Voltage */
#define SC_VOLTAGE_5V      0
#define SC_VOLTAGE_3V      1
#define SC_VOLTAGE_1_8V      2

typedef struct _apdu{
	unsigned char CLA;
	unsigned char INS;
	unsigned char PP1;
	unsigned char PP2;
	unsigned char PP3;
	unsigned char SW1;
	unsigned char SW2;
	unsigned int LC;
	unsigned int LE;
	unsigned int  RLE;	
	unsigned char pInData[256];		
	unsigned char pOutData[384];
	unsigned char *pCommBuff;
}APDU;

extern APDU	 T_APDU;	
extern APDU	g_APDU;




/******************************************************************************/
/* N8035 EN_SCC_EINT1 */
/******************************************************************************/
#define N8035_EN_SCC_EINT1_PORT	GPIOA//GPIOB
#define N8035_EN_SCC_EINT1_PIN	GPIO_Pin_0//GPIO_Pin_0
#define IO_SET_N8035_EN_SCC_EINT1_LOW   (N8035_EN_SCC_EINT1_PORT->BRR = N8035_EN_SCC_EINT1_PIN)
#define IO_SET_N8035_EN_SCC_EINT1_HIGH  (N8035_EN_SCC_EINT1_PORT->BSRR = N8035_EN_SCC_EINT1_PIN)
#define IO_PIN_N8035_EN_SCC_EINT1_LOW			((N8035_EN_SCC_EINT1_PORT->IDR & N8035_EN_SCC_EINT1_PIN) == 0)  
#define IO_PIN_N8035_EN_SCC_EINT1_HIGH		(!IO_PIN_N8035_EN_SCC_EINT1_LOW) 

extern unsigned char G_CardStats;	

extern unsigned char g_NFC_State;

void HAL_7816_Init(void);
//unsigned char  HAL_7816_GetATR(unsigned char* pATR);
unsigned int HAL_7816_Transmit(unsigned char* pInData, unsigned short Inlen, unsigned char* pOutData, unsigned short* pOutlen);
void HAL_Card_APDU(APDU *pAPDU, unsigned short len);
unsigned int HAL_9541_PowerOn(void);
int HAL_7816_test(void);
int AU9541_ICCRAD_TransCeive(APDU *psAPDU);
int AU9541_ICCARD_Reset(unsigned char *pucATR, unsigned char *puiATRLen);
unsigned char SC_Detect(void);
int vPBOC_7816_Init(void);
#ifdef __cplusplus
}
#endif
#endif //__HAL_USB_H
