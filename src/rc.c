#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <drv_uart.h>
#include <pwm.h>
#include <rc.h>

#ifndef bool
typedef uint8_t bool;
#define false (bool) 0
#define true (bool) 1
#define NULL ((void *)0)
#endif

/*
 * @brief ����� �ܺ����ͷ�Ʈ ��ġ�� ������ ������
 */
static const extiDevice_t* rcExtiDevicePtr = NULL; // NULL

/*
 * @brief ����� ���ű��� Ÿ���� ������ ����
 */
static rcType_t rcType = 0;

/*
 * @brief ���ű� �ʱ�ȭ
 * @param rcDevice: ���ű� ��ġ ����ü
 * @param type: ���ű� �ʱ�ȭ�� ���� ����ü
 * @retval ����
 */
void rcInit(rcDevice_t rcDevice, rcType_t type) { // rcDevice
	rcType = type; // PWM, PPM
	rcExtiDevicePtr = pwmExtiMap[rcDevice].a; // a[] = { EXTI1, TIM2, TIM3, TIM4 }, pointer value
	switch(rcType) {
	case PWM:
		pwmInit(rcExtiDevicePtr, PWM_FILTER_DISABLE);
		break;
	case PPM:
		break;
	case S_BUS:
		break;
	}
}

/*
 * @brief ���ű� �б�
 * @param data: 5���� ������ ��ȯ�� ���� ������
 * @retval ����
 */
void rcRead(uint16_t *data) {
	switch(rcType) {
	case PWM:
		pwmRead(data);
		break;
	case PPM:
		break;
	case S_BUS:
		break;
	}
}
