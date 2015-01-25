#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <drv_uart.h>

static void serialInit(uartDevice_t uartDevice_);
void setSystemClock(uint32_t clocks);
uint32_t getSystemClock(void);

/*
 * @brief �ý��� Ŭ������ ����� ����
 */
static uint32_t systemClocks = 0;

/*
 * @brief �ý��� tickŸ�̸� ī���Ͱ� ����� �Լ�, �и���
 * @note !���, uint32_t ������ Ÿ���� ������ �����ϹǷ� 50���� ������ 0���� ���ư�
 */
static volatile uint32_t sysTickNum;

/*
 * @brief ����� �ܺ����ͷ�Ʈ ��ġ�� ������ ������
 */
static uint32_t usTicks;

/*
 * @brief ����� �ܺ����ͷ�Ʈ ��ġ�� ������ ������
 */
static uartDevice_t uartDevice = 0;

/*
 * @brief �ý��� �ʱ�ȭ
 * @param ����
 * @retval ����
 */
void systemInit(void) {
	SystemInit();
	RCC_ClocksTypeDef rcc_clocks;
	RCC_GetClocksFreq(&rcc_clocks);
	uint32_t systemClocks_ = rcc_clocks.SYSCLK_Frequency;
	setSystemClock(systemClocks_);
	//72000000 / 1000000 = 72
	usTicks = systemClocks_ / 1000000;
	//72000000 / 1000 = 72000, sysTick init
	SysTick_Config(systemClocks_ / 1000);

	serialInit(UART_DEVICE_6);
}

/*
 * @brief �ø��� �ʱ�ȭ
 * @param uartDevice_: ����Ʈ ��ġ ����ü
 * @retval ����
 */
static void serialInit(uartDevice_t uartDevice_) {
	uartDevice = uartDevice_;
	uartInitTypeDef_t uartInitStructure;
	uartInitStructure.preemptionPriority = 0;
	uartInitStructure.subPriority = 1;
	uartInitStructure.baudRate = 115200;
	uartInit(uartDevice_, &uartInitStructure);
}

/*
 * @brief �ø���  ���� ����
 * @param c: �� ������
 * @retval ����
 */
void serialPutChar(uint8_t c) {
	uartPutChar(uartDevice, c);
}

/*
 * @brief �ø��� ���� �б�
 * @param ����
 * @retval ���� ������(uint8_t)
 */
uint8_t serialGetChar(void) {
	return uartGetChar(uartDevice);
}

/*
 * @brief �ý��� Ŭ�� ����
 * @param clocks: ������ Ŭ����
 * @retval ����
 */
void setSystemClock(uint32_t clocks) {
	systemClocks = clocks;
}

/*
 * @brief �ý��� Ŭ�� �б�
 * @param ����
 * @retval �ý��� Ŭ��(uint32_t)
 */
uint32_t getSystemClock(void) {
	return systemClocks;
}

/*
 * @brief ���� ����ũ���� �б�
 * @note !���, uint32_t ������ Ÿ���� ������ �����ϹǷ� 70���� ������ 0���� ���ư�
 * @param ����
 * @retval �ý��� �ʱ�ȭ�� ���� ����ũ����(uint32_t)
 */
uint32_t micros(void)
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickNum;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickNum); // SysTick->VAL�� ��ȿ�Ҷ�
    // �и��ʴ� ����ũ������ 10^3���̹Ƿ� *1000
    // �ý���  tick�� val���� �����ߴ� ���ֺ� �������� ������Ʈ �ǰ�, �⺻ Ŭ���� 72Mhz�̰� ������ 1Mhz�� �����Ƿ� / 72
    return (ms * 1000) + ((usTicks * 1000 - cycle_cnt) / usTicks);
}

/*
 * @brief ���� �и����б�
 * @note !���, uint32_t ������ Ÿ���� ������ �����ϹǷ� 50���� ������ 0���� ���ư�
 * @param ����
 * @retval �ý��� �ʱ�ȭ�� ���� �и���(uint32_t)
 */
uint32_t millis(void)
{
	// sysTickUptime(ms)
    return sysTickNum;
}

/*
 * @brief ����ũ���� ������
 * @param us: ������ �� ����ũ����
 * @retval ����
 */
void delayMicroseconds(uint32_t us)
{
    uint32_t now = micros();
    while (micros() - now < us);
}

/*
 * @brief �и��� ������
 * @param ms: ������ �� �и���
 * @retval ����
 */
void delay(uint32_t ms)
{
    while (ms--) {
    	// 1ms = 1000us
        delayMicroseconds(1000);
    }
}

void SysTick_Handler() {
	sysTickNum++;
}
