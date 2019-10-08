
VPATH=./gui:./dsp54:./dsp28:./hardware:./hmi_control_data:./hmi_group:./cnc_comm

libs1 =	./gui/tmFontController.so \
		./gui/tmFontHelpController.so

libs2 =	./dsp54/tmSelvatG.so \
		./dsp54/tmDefaultG.so \
		./dsp54/tmTableG.so \
		./dsp54/tmTaskMoni.so 

libs3 =	./dsp28/tmTaskDsp28.so \
		./dsp28/tmDsp28HostProtocol.so \
		./dsp28/tmDsp28ActualValue.so \
		./dsp28/tmDsp28CommonRead.so \
		./dsp28/tmDsp28CommonWrite.so \
		./dsp28/tmDsp28LineStatus.so \
		./dsp28/tmDsp28Command.so \
		./dsp28/tmDsp28Moldset.so \
		./dsp28/tmDsp28MoldsetStatus.so \
		./dsp28/tmDsp28Alarm.so 

libs4 = ./hardware/tmKeyBoardG.so \
				./hardware/tmGpio437Hmi.so \
        ./hardware/tmTouchScreen.so

libs5 = ./hmi_control_data/tmControlServer_lib.so \
		./hmi_control_data/tmControl_Error.so \
		./hmi_control_data/tmControl_Monitor.so \
		./hmi_control_data/tmControl_MoldHyb5.so \
		./hmi_control_data/tmControl_User.so \
		./hmi_control_data/tmControl_CurveData.so \
		./hmi_control_data/tmControl_Quality.so \
		./hmi_control_data/tmControl_Function.so \
		./hmi_control_data/tmControl_Oper_Log.so 

libs6 = ./hmi_group/tmTaskCmd.so \
		./hmi_group/tmDatabase.so \
		./hmi_group/tmInit.so \
		./hmi_group/tmTaskDataProcess.so \
		./hmi_group/tmTaskHmi.so \
		./hmi_group/tmFreeDataG.so \
		./hmi_group/tmGlobalG.so \
		./hmi_group/tmTaskAmmeterDLT.so \
		./hmi_group/tmTaskAmmeterTM.so \
		./hmi_group/tmTaskAmmeterModbus.so \
		./hmi_group/tmTaskN2CAmmeter.so

#		./hmi_group/tmTaskAmmeterTM.so \

libs7 = ./cnc_comm/tmTaskCnc_Comm.so
#libs7 = ./inet/tmTaskiNet.so

libs9 =	./dsp54_net/tmTaskDsp54.so \
		./dsp54_net/tmDsp54HostProtocol.so \
		./dsp54_net/tmDsp54All.so \
		./dsp54_net/tmDsp54Update.so 

libs10 = ./dsp28_m3/tmTaskDsp28_M3.so \
	 ./dsp28_m3/tmM3HostProtocol.so \
	 ./dsp28_m3/tmM3ActualValue.so \
	 ./dsp28_m3/tmM3Alarm.so \
	 ./dsp28_m3/tmM3CommonRead.so \
	 ./dsp28_m3/tmM3CommonWrite.so \
	 ./dsp28_m3/tmM3LineStatus.so \
	 ./dsp28_m3/tmM3Command.so \
	 ./dsp28_m3/tmM3Moldset.so \
	 ./dsp28_m3/tmM3MoldsetStatus.so \
	 ./dsp28_m3/tmM3SiemRead.so \
	 ./dsp28_m3/tmM3SiemWrite.so \
	 ./dsp28_m3/tmM3Ammeter.so 

libs11 =./picker_comm/tmTaskPickerComm.so \
		./picker_comm/tmHmi485cmd.so 

libs12 =./modbus/tmTaskModbus.so

libs13 =	./dsp28_picker/tmTaskPicker.so \
		./dsp28_picker/tmPickerHostProtocol.so \
		./dsp28_picker/tmPickerActualValue.so \
		./dsp28_picker/tmPickerCommonRead.so \
		./dsp28_picker/tmPickerCommonWrite.so \
		./dsp28_picker/tmPickerLineStatus.so \
		./dsp28_picker/tmPickerCommand.so \
		./dsp28_picker/tmPickerMoldset.so \
		./dsp28_picker/tmPickerMoldsetStatus.so \
		./dsp28_picker/tmPickerAlarm.so
	 
	 
libs = $(libs1) $(libs2) $(libs3) $(libs4) $(libs5) $(libs6) $(libs7) $(libs9) $(libs10) $(libs11) $(libs12) $(libs13)

all:$(libs)


$(libs):%.so:%.cpp %.h
	arm-linux-gnueabihf-g++ -Wno-write-strings -Wno-deprecated -fpermissive -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 -DD_5528 -DD_3354 -DD_CANMASTER $< -o $@
#	g++ -Wall -fpic -shared -g -DD_X86 -DD_5528 -DD_CANMASTER $< -o $@
#	arm-arago-linux-gnueabi-g++ -Wall -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 -DD_CANMASTER $< -o $@
#	arm-linux-g++  -Wno-write-strings -Wno-deprecated -fpermissive -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 -DD_5528 -DD_3354 -DD_CANMASTER -I/usr/local/imx6/include $< -o $@
#	arm-linux-g++ -Wall -fpic -shared -s -DD_ARM $< -o $@
# 	arm-softfloat-linux-gnu-g++ -Wall -fpic -shared -s -DD_ARM -DD_PXA270 -DD_CANMASTER -DD__MAX6951 $< -o $@
#	arm-softfloat-linux-gnu-g++ -Wall -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH -DD_PXA270 $< -o $@
#	arm-none-linux-gnueabi-g++ -Wall -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH -DD_CANMASTER -DD_PXA270 $< -o $@
#	/work/pxa255/arm-linux/bin/arm-linux-g++ -Wall -fpic -shared -s -DD_ARM -DD_NOWRITE_FALSH $< -o $@

.PHONY:clean
clean:
	rm $(libs)
