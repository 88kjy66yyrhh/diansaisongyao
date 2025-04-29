#ifndef _NRF24L01_INS_H_
#define _NRF24L01_INS_H_

/**********	NRF24L01���Ŷ���	**********/
#define CSN_Port	GPIOB
#define	CE_Port		GPIOB
#define	MOSI_Port	GPIOB
#define	SCK_Port	GPIOB
#define	IRQ_Port	GPIOB
#define	MISO_Port	GPIOB

#define	CSN_Pin		GPIO_Pin_1
#define	CE_Pin		GPIO_Pin_0
#define	MOSI_Pin	GPIO_Pin_15
#define	SCK_Pin		GPIO_Pin_13
#define	IRQ_Pin		GPIO_Pin_12
#define	MISO_Pin	GPIO_Pin_14
/*************************************/

/**********	�Ĵ�����������	**********/
#define NRF24_R_REG		0x00	//�����üĴ�������5λΪ�Ĵ�����ַ
#define NRF24_W_REG		0x20	//д���üĴ�������5λΪ�Ĵ�����ַ
#define R_RX_PAYLOAD	0x61	//��RX��Ч���ݣ�1-32�ֽ�
#define W_TX_PAYLOAD	0xA0	//дTX��Ч���ݣ�1-32�ֽ�
#define	FLUSH_TX		0xE1	//���TX FIFO�Ĵ���������ģʽ����
#define	FLUSH_RX		0xE2	//���RX FIFO�Ĵ���������ģʽ����
#define	REUSE_TX_PL		0xE3	//�����ϴη����غɣ�TX �غɱ�����ֱ�� W_TX_PAYLOAD �� FLUSH_TX ��ִ�У������ڴ����ڼ�ı临��״̬
								//������ʹ����һ�����ݣ�CE��1�����ݰ����Ϸ���
#define	NOP				0xFF	//�ղ��������Զ�ȡSTATUS(״̬)�Ĵ���
/*************************************/

/************	�Ĵ�����ַ	***********/
#define	CONFIG		0x00	//���üĴ�����ַ
							//bit0,1��PRX,0��PTX��bit1,��ѡ��bit2,CRCģʽ��0��1�ֽڣ�1��2�ֽڣ�bit3��CRCʹ��
							//bit4,�ж�MAX_RT(�ﵽ����ط������ж�)ʹ�ܣ�bit5,�ж�TX_DSʹ�ܣ�bit6,�ж�RX_DRʹ��
#define	EN_AA		0x01	//ʹ���Զ�Ӧ����
							//bit0-5,��Ӧͨ��0-5���ĸ�bitλ��1��ѡ���ĸ�ͨ��
#define	EN_RXADDR	0x02	//���յ�ַ����
							//bit0-5,��Ӧͨ��0-5
#define	SETUP_AW	0x03	//���õ�ַ���
							//bit1:0  00���Ƿ���01��3�ֽڣ�10��4�ֽڣ�11��5�ֽ�
#define	SETUP_RETR	0x04	//�����Զ��ط�
							//bit3:0,�Զ��ط�������0000-1111 1-15���Զ��ط���
							//bit7:�Զ��ط���ʱ��0000��250us,����1111��4000us
#define	RF_CH		0x05	//RFͨ��(��Ƶͨ��)
							//bit6:0,����Ƶ��ͨ��
#define	RF_SETUP	0x06	//RF�Ĵ���
							//bit3,��������(0,1Mbps 1,2Mbps)��bit2:1�����书�ʣ�bit0:�������Ŵ�������
#define	STATUS		0x07	//״̬�Ĵ���
							//bit0,TXFIFO����־(1Ϊ��)��
							//bit3:1,��������ͨ����(000-101Ϊͨ���ţ�110��ʾδʹ�ã�111��ʾRXFIFO��)
							//bit4���TX�ش��ж�������1�����־λ�����MAX_RT���ԣ���������������ͨ��
							//bit5,���ݷ�������ж�λ��bit6�����������ж�λ	//��1�����־λ
#define	OBSERVE_TX	0x08	//���ͼ��Ĵ���
							//bit7:4�����ݶ�ʧ��������bit3:0���ط�������
#define	RPD			0x09	//�ز����Ĵ���
#define	RX_ADDR_P0	0x0A	//����ͨ��0���յ�ַ	��󳤶�5�ֽڣ����ֽ���ǰ
#define	RX_ADDR_P1	0x0B	//����ͨ��1���յ�ַ	��󳤶�5�ֽڣ����ֽ���ǰ
#define	RX_ADDR_P2	0x0C	//����ͨ��2���յ�ַ	����ֽڿ����ã����ֽڱ���ͬRX_ADDR_P1[39:8]���
#define	RX_ADDR_P3	0x0D	//����ͨ��3���յ�ַ	����ֽڿ����ã����ֽڱ���ͬRX_ADDR_P1[39:8]���
#define	RX_ADDR_P4	0x0E	//����ͨ��4���յ�ַ	����ֽڿ����ã����ֽڱ���ͬRX_ADDR_P1[39:8]���
#define	RX_ADDR_P5	0x0F	//����ͨ��5���յ�ַ	����ֽڿ����ã����ֽڱ���ͬRX_ADDR_P1[39:8]���
#define	TX_ADDR		0x10	//���͵�ַ	����ģʽ����
							//���ֽ���ǰ��ShockBurstģʽ��RX_ADDR_P0��˵�ַ��ͬ
#define	RX_PW_P0	0x11	//��������ͨ��0��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define	RX_PW_P1	0x12	//��������ͨ��1��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define	RX_PW_P2	0x13	//��������ͨ��2��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define	RX_PW_P3	0x14	//��������ͨ��3��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define	RX_PW_P4	0x15	//��������ͨ��4��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define	RX_PW_P5	0x16	//��������ͨ��5��Ч���ݿ��	1-32�ֽڣ�0Ϊ�Ƿ�
#define FIFO_STATUS	0x17	//FIFO״̬�Ĵ���
							//bit0,RX FIFO�Ĵ����ձ�־(1Ϊ��)��bit1,RX FIFO�Ĵ�������־(1Ϊ��)
							//bit3:2,����λ��д0��bit7,����λ��д0
							//bit4,TX FIFO�Ĵ����ձ�־(1λ��)��bit5,TX FIFO�Ĵ�������־(1Ϊ��)
							//bit6,���ڷ���ģʽ����1ѭ�������������ݰ�����0��ѭ��
/**************************************/

/******	STATUS�Ĵ���bitλ����	******/
#define MAX_TX  	0x10  	  //�ﵽ����ʹ����ж�
#define TX_OK   	0x20  	  //TX��������ж�
#define RX_OK   	0x40  	  //���յ������ж�
/*************************************/

#endif
