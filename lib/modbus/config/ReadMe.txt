/*
 * ʹ�����Ͽ����Ͻ���modbusͨ��ʱ����Ҫ����modbus.xml
 * 
*/

/*/*modbus �����ļ�˵��*/

RTU: ʹ��Modbus-RTU
	 channel:�˿ں� 
	 buadrate:������ 
	 parity: ��żУ��λ ("N" ��У��λ ��E��żУ�顰O����У��)
	 databits:����У��λ 
	 stopbits: ֹͣλ
TCP: ʹ��Modbus-TCP
	 ip: �����ip��ַ port: ����˶˿ں� src_port:���ض˿ں�

Slave: RTU-�ӻ���ַ TCP-Unit��Ԫ(ʮ��������)
Function: modbus ������(ʮ��������)
ID:���Ͽ��е�StringID
Reg:�Ĵ�����ַ��ʼ��ַ(ʮ��������)
Count:�����ֽڸ���(ʮ��������)
Attr: 01-����дһ����������	(ʮ��������)
	  02-�����Զ���������
	  03-д��������
	  04-����������
	  

					 
