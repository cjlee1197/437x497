/*
 * 使用资料库资料进行modbus通信时，需要配置modbus.xml
 * 
*/

/*/*modbus 配置文件说明*/

RTU: 使用Modbus-RTU
	 channel:端口号 
	 buadrate:波特率 
	 parity: 奇偶校验位 ("N" 无校验位 “E”偶校验“O”奇校验)
	 databits:数据校验位 
	 stopbits: 停止位
TCP: 使用Modbus-TCP
	 ip: 服务端ip地址 port: 服务端端口号 src_port:本地端口号

Slave: RTU-从机地址 TCP-Unit单元(十六进制数)
Function: modbus 功能码(十六进制数)
ID:资料库中的StringID
Reg:寄存器地址或开始地址(十六进制数)
Count:资料字节个数(十六进制数)
Attr: 01-开机写一次资料配置	(十六进制数)
	  02-周期性读资料配置
	  03-写资料配置
	  04-读资料配置
	  

					 
