#ifndef _RC_H_
#define _RC_H_

/*
 * @brief ���ű� ��ġ ����ü
 */
typedef enum {
    RC_1 = 0,
    RC_2,
    MAX_RC,
} rcDevice_t;

/*
 * @brief ���ű� ä�� ����ü
 */
typedef enum {
	THR,
	AIL,
	ELE,
	RUD,
	AUX1,
	RC_CHANNEL_MAX,
} rcChannel_t;

/*
 * @brief ���ű� Ÿ�� ����ü
 */
typedef enum {
	PWM = 0,
	PPM,
	S_BUS,
} rcType_t;

void rcInit(rcDevice_t rcDevice, rcType_t type);
void rcRead(uint16_t *data);

#endif
