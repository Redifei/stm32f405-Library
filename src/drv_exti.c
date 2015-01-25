#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <drv_exti.h>

/*
 * @brief �ܺ����ͷ�Ʈ  �ݹ� �Լ� �迭
 */
static extiFuncPtr_t extiFuncPtr[16];

/*
 * @brief �ܺ����ͷ�Ʈ���� ȣ���� ������ ����
 */
static uint8_t extiChannelMap[16] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/*
 * @brief �ܺ����ͷ�Ʈ ä�� ����
 * @param extiDevice: �ܺ����ͷ�Ʈ ��ġ ����ü
 * @param channel: �ܺ����ͷ�Ʈ �ʱ�ȭ�� ���� ����ü ������
 * @retval ����
 */
void extiChannelMapping(extiDevice_t extiDevice, uint8_t channel) {
	extiChannelMap[extiDevice] = channel;
}

/*
 * @brief �ܺ����ͷ�Ʈ �ʱ�ȭ
 * @param extiDevice: �ܺ����ͷ�Ʈ ��ġ ����ü
 * @param extiInitStruct: �ܺ����ͷ�Ʈ �ʱ�ȭ�� ���� ����ü ������
 * @param extiFuncPtr_: �ܺ����ͷ�Ʈ ISR���� ȣ���� �Լ� ������
 * @retval ����
 */
void extiInit(extiDevice_t extiDevice, extiInitTypeDef_t* extiInitStruct, extiFuncPtr_t extiFuncPtr_) {
	// clock Ȱ��ȭ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(extiHardwareMap[extiDevice].periph, ENABLE);

	// gpio ����
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = extiHardwareMap[extiDevice].pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(extiHardwareMap[extiDevice].gpio, &GPIO_InitStructure);

	// exti�� �Ҵ�
	SYSCFG_EXTILineConfig(extiHardwareMap[extiDevice].portSource, extiHardwareMap[extiDevice].pinSource);

	// exti ����
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = extiHardwareMap[extiDevice].exti;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = extiInitStruct->Trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// nvic ����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = extiHardwareMap[extiDevice].irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = extiInitStruct->PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = extiInitStruct->SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// ȣ���� �Լ� ����
	extiFuncPtr[extiDevice] = extiFuncPtr_;
}

/*
 * @brief �ܺ����ͷ�Ʈ ���ͷ�Ʈ �ڵ鷯
 * @note 1020~1980�� �����͸� ���������� �޵��� ������ �Ǿ� �����Ƿ�
 * 		  �������� ������ ���� ���ű��� ��� �����ϰ� �����ؼ� ����ϵ��� .. ���� �����ʹ� 1100~1900���� �����ؼ� ��뿹��
 * 		  ����, ���� ����� 1050~1950�̰�, �ִ����� 1020~1980, �ּһ���� 1100~1900
 * @param channel: x�� 1����  5�� ���ڰ� ������
 * @retval ����
 */
static void extiHandler(extiDevice_t channel) {
	uint8_t chan = extiChannelMap[channel]; //EXTI������ Channel�� �о����, ��) exti2 -> THR
	if(chan == 0xff) {
		return;
	}
	extiFuncPtr[channel](chan);
}

void EXTI0_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line0) == SET) { //���ͷ�Ʈ �÷��װ� set�Ǿ������
		EXTI_ClearITPendingBit(EXTI_Line0); //���ͷ�Ʈ �÷��� clear
		extiHandler(EXTI_DEVICE_0); //pulse �ð������ ���� �Լ� ȣ��
	}
}

void EXTI1_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line1) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line1);
		extiHandler(EXTI_DEVICE_1);
	}
}

void EXTI2_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line2) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line2);
		extiHandler(EXTI_DEVICE_2);
	}
}

void EXTI3_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line3) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line3);
		extiHandler(EXTI_DEVICE_3);
	}
}

void EXTI4_IRQHandler(void) {
	if(EXTI_GetITStatus(EXTI_Line4) == SET) {
		EXTI_ClearITPendingBit(EXTI_Line4);
		extiHandler(EXTI_DEVICE_4);
	}
}

void EXTI9_5_IRQHandler(void) {
	uint8_t i = 5;
	for(; i <= 9; i++) {//5 ~ 9
		if(EXTI_GetITStatus(1 << i) == SET) { // 1<<5 ~ 1<<9, EXTI_Line5 ~ EXTI_Line9
			EXTI_ClearITPendingBit(1 << i);
			extiHandler(i); //5 ~ 9, EXTI5 ~ EXTI9
		}
	}
}

void EXTI15_10_IRQHandler(void) {
	uint8_t i = 10;
	for(; i <= 15; i++) {
		if(EXTI_GetITStatus(1 << i) == SET) {
			EXTI_ClearITPendingBit(1 << i);
			extiHandler(i);
		}
	}
}
