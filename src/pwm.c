#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <drv_exti.h>
#include <pwm.h>
#include <rc.h>
#include <system.h>

#ifndef bool
typedef uint8_t bool;
#define false (bool) 0
#define true (bool) 1
#endif

static volatile uint32_t rcRising[RC_CHANNEL_MAX] = {0, };
static volatile uint32_t rcFalling[RC_CHANNEL_MAX] = {0, };
static volatile uint16_t rcRawData[RC_CHANNEL_MAX] = {1000, 1000, 1000, 1000, 1000};
static volatile bool rcDataVaild[RC_CHANNEL_MAX] = {true, true, true, true, true};

static const extiDevice_t* extiChannel = 0; //NULL

void pwmHandler(extiDevice_t extiDevice);

/*
 * @brief pwm �ʱ�ȭ
 * @note ���� ���� �κ� ���� ��������, �÷��׸� ���� �����ϵ���..
 * @param extiDevicePtr: �ܺ����ͷ�Ʈ ��ġ�� ������
 * @param type: ���ű� �ʱ�ȭ�� ���� ����ü
 * @retval ����
 */
void pwmInit(const extiDevice_t* extiDevicePtr, pwmType_t type) {
	extiChannel = extiDevicePtr; //����� ���ű��� ä���� ����
	uint8_t i = 0;
	for(i = 0; i < MAX_EXTI_DEVICE; i++) {
		if(extiDevicePtr[i] == 0xff) {
			break;
		}

		extiDevice_t extiDevice = extiDevicePtr[i];
		extiChannelMapping(extiDevice, i);

		extiInitTypeDef_t extiInitStructure;
		extiInitStructure.Trigger = EXTI_Trigger_Rising_Falling;
		extiInitStructure.PreemptionPriority = 1; //�⺻������ 1, 1�� �����ϰ� ���߿� ���ͷ�Ʈ�� �������� ��Ȳ�� ���� ����
		extiInitStructure.SubPriority = 1;

		switch(type) {
		case PWM_FILTER_DISABLE: //���� ���� ����, ������ �������� �ǵ��� �Ǿ�����
			break;
		case PWM_FILTER_ENABLE:
			break;
		}

		extiInit(extiDevice, &extiInitStructure, pwmHandler);
	}
}

#define LPF_FACTOR 1 //0.4

/*
 * @brief ���ű� �б�
 * @param data: 5���� ������ ��ȯ�� ���� ������
 * @retval ����
 */
void pwmRead(uint16_t *data) {
	uint8_t i = 0;
	static uint16_t preRcData[5] = { 0, }; //���� ������ ������ ���� ��������
	for(;i < RC_CHANNEL_MAX;i++) { //THR~AUX1
		if(rcDataVaild[i] == true) { //�����Ͱ� ��ȿ��
			rcDataVaild[i] = false; //������ ������ ǥ��
			if(preRcData[i] != 0) { //�Լ��� ó�� ȣ����� �ʾ��� ���
				data[i] = LPF_FACTOR * rcRawData[i] + (1 - LPF_FACTOR) * preRcData[i]; //������ ��� ����(Lpf)
			}
			else { //�Լ��� ó�� ȣ��Ȱ��
				data[i] = rcRawData[i];
			}
			preRcData[i] = rcRawData[i]; //���� ������ ����
		}
		else {
			data[i] = preRcData[i];
		}
	}
}

/*
 * @brief ���ű� ���ͷ�Ʈ �ڵ鷯
 * @note �ܺ����ͷ�Ʈ �ڵ鷯���� ȣ��� �ڵ鷯
 * @param extiDevice:
 * @retval ����
 */
void pwmHandler(extiDevice_t extiDevice){
	if(GPIO_ReadInputDataBit(extiHardwareMap[extiChannel[extiDevice]].gpio, extiHardwareMap[extiChannel[extiDevice]].pin) == SET) { //rising �����϶�
		rcRising[extiDevice] = micros(); //������  micro�ʸ� ����
	}
	else { //falling �����ϋ�
		rcFalling[extiDevice] = micros();
		uint16_t buf = rcFalling[extiDevice] - rcRising[extiDevice]; //falling �� rising�ð��� ���� pulse �ð� ���
		if(buf > RC_MIN && buf < RC_MAX) { //���� ���� �ּҰ��� �ִ밪 ���� ���϶�, ���� �̰��� ���� ����쿡 ���� �ݿ����� �ʵ��� �ص�
			rcRawData[extiDevice] = buf;
		}
		else {
			(buf <= RC_MIN) ? (rcRawData[extiDevice] = RC_MIN) : ((buf >= RC_MAX) ? (rcRawData[extiDevice] = RC_MAX) : (0));
		}
		rcDataVaild[extiDevice] = true; //�����Ͱ� ��ȿ�ϴ�
	}
}
