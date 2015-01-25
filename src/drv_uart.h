#ifndef _UART_H_
#define _UART_H_

#define BUFFER_SIZE       2048

/*
 * @brief ����Ʈ ��ġ ����ü
 */
typedef enum {
	UART_DEVICE_1 = 0,
    UART_DEVICE_2,
    UART_DEVICE_3,
    UART_DEVICE_4,
    UART_DEVICE_5,
    UART_DEVICE_6,
    MAX_UART_DEVICE,
} uartDevice_t;

/*
 * @brief ����Ʈ �ϵ���� ������ ���� ����ü
 */
typedef struct {
	USART_TypeDef *uart;
    GPIO_TypeDef *gpio;
    uint8_t txPin;
    uint8_t rxPin;
    uint8_t irq;
    uint32_t gpioClock;
    uint32_t uartClock;
    uint8_t gpioAF;
} uartHardwareMap_t;

/*
 * @brief ����Ʈ �ϵ���� ����
 */
static const uartHardwareMap_t uartHardwareMap[] = {
    { USART1, GPIOA, 9, 10, USART1_IRQn, RCC_AHB1Periph_GPIOA, RCC_APB2Periph_USART1, GPIO_AF_USART1 },
//    { USART1, GPIOB, 6, 7, USART1_IRQn, RCC_AHB1Periph_GPIOB, RCC_APB2Periph_USART1, GPIO_AF_USART1 },
    { USART2, GPIOA, 2, 3, USART2_IRQn, RCC_AHB1Periph_GPIOA, RCC_APB1Periph_USART2, GPIO_AF_USART2 },
    { USART3, GPIOC, 10, 11, USART3_IRQn, RCC_AHB1Periph_GPIOC, RCC_APB1Periph_USART3, GPIO_AF_USART3 },
    { UART4, GPIOA, 0, 1, UART4_IRQn, RCC_AHB1Periph_GPIOA, RCC_APB1Periph_UART4, GPIO_AF_UART4 },
//    { UART4, GPIOC, 10, 11, UART4_IRQn, RCC_AHB1Periph_GPIOC, RCC_APB1Periph_UART4, GPIO_AF_UART4 },
    { UART5, 0, 0, 0, 0, 0, 0, 0 }, // ���� �� ��Ʈ�� ���� �ʾƼ� ���� �ʱ�ȭ �Լ��δ� �ʱ�ȭ �Ұ���
    { USART6, GPIOC, 6, 7, USART6_IRQn, RCC_AHB1Periph_GPIOC, RCC_APB2Periph_USART6, GPIO_AF_USART6 },
};

/*
 * @brief ����Ʈ �ʱ�ȭ Ÿ�� ����ü
 */
typedef struct {
	uint8_t preemptionPriority;
	uint8_t subPriority;
	uint32_t baudRate;
} uartInitTypeDef_t;

/*
 * @brief ����Ʈ ������ ����
 */
typedef struct {
	struct{
		uint16_t BufHead,BufTail;
		uint8_t Buf[BUFFER_SIZE];
	}RX;
	struct{
			uint16_t BufHead,BufTail;
			uint8_t Buf[BUFFER_SIZE];
	}TX;
}uartBuf_t;

void uartInit(uartDevice_t uartDevice, uartInitTypeDef_t* uartInitStruct);
void uartPutChar (uartDevice_t uartChan, uint8_t c);
uint8_t uartGetChar(uartDevice_t uartChan);

#endif
