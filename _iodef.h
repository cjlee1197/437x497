/*===========================================================================+
|  Function : IODef                                                          |
|  Task     : IO Definition                                                  |
|----------------------------------------------------------------------------|
|  Compile  : MSVC V1.52 -                                                   |
|  Link     : MSVC V1.52 -                                                   |
|  Call     :                                                                |
|----------------------------------------------------------------------------|
|  Author   : C.C. Chen                                                      |
|  Version  : V1.00                                                          |
|  Creation : 06/14/1997                                                     |
|  Revision :                                                                |
+===========================================================================*/

#ifndef     D_IODEF
#define     D_IODEF

/*===========================================================================+
|           Constant                                                         |
+===========================================================================*/
#define		LED_PNL_TM_MANUAL     			0 	//	手動         	
#define		LED_PNL_TM_SEMIATUO             1 	//	半自動       
#define		LED_PNL_TM_ATUO                 2 	//	全自動       
#define		LED_PNL_TM_SESORATUO            3 	//	電眼自動   
#define		LED_PNL_TM_SESORATUO_OFF  		4 	//	電眼自動關
#define		LED_PNL_TM_HEAT_ON              5 	//	電熱開       
#define		LED_PNL_TM_HEAT_OFF             6 	//	電熱關       
#define		LED_PNL_TM_MOTOR_ON             7 	//	馬達開關     
#define		LED_PNL_TM_MOTOR_OFF            8 	//	馬達關       
#define		LED_PNL_TM_DOOR_OPEN            9 	//	安全門開     
#define		LED_PNL_TM_DOOR_CLOSE           10	//	安全門關     
#define		LED_PNL_TM_ADJUST_ATUO          11	//	自動調模  
#define		LED_PNL_TM_ADJUSTO_ATUO_OFF		12	//	自動調模關
#define		LED_PNL_TM_ADJUST_FWD           13	//	調模進       
#define		LED_PNL_TM_ADJUST_BWD           14	//	調模退       
#define		LED_PNL_TM_CLOSEMLD             15	//	關模         
#define		LED_PNL_TM_OPENMLD              16	//	開模         
#define		LED_PNL_TM_INJECT               17	//	射出         
#define		LED_PNL_TM_INJECTB              18	//	射出B        
#define		LED_PNL_TM_INJECT_BWD           19	//	射退         
#define		LED_PNL_TM_INJECTB_BWD          20	//	射退B        
#define		LED_PNL_TM_CHARGE               21	//	儲料         
#define		LED_PNL_TM_CHARGEB              22	//	儲料B        
#define		LED_PNL_TM_PURGE_ATUO           23	//	自動清料     
#define		LED_PNL_TM_PURGEB_ATUO          24	//	自動清料B    
#define		LED_PNL_TM_EJECT_FWD            25	//	托模進       
#define		LED_PNL_TM_EJECT_BWD            26	//	托模退       
#define		LED_PNL_TM_EJECTB_FWD           27	//	托模進B      
#define		LED_PNL_TM_EJECTB_BWD           28	//	托模退B      
#define		LED_PNL_TM_UEJECT_FWD           29	//	上托進       
#define		LED_PNL_TM_UEJECT_BWD           30	//	上托退       
#define		LED_PNL_TM_MEJECT_FWD           31	//	中托進       
#define		LED_PNL_TM_MEJECT_BWD           32	//	中托退       
#define		LED_PNL_TM_DEJECT_FWD           33	//	下托進       
#define		LED_PNL_TM_DEJECT_BWD           34	//	下托退       
#define		LED_PNL_TM_NOZZLE_FWD           35	//	座台進       
#define		LED_PNL_TM_NOZZLE_BWD           36	//	座台退       
#define		LED_PNL_TM_DIAL_OUT             37	//	轉盤出       
#define		LED_PNL_TM_DIAL_IN              38	//	轉盤進       
#define		LED_PNL_TM_ANCHOR_BWD           39	//	定位退       
#define		LED_PNL_TM_ANCHOR_FWD           40	//	定位進       
#define		LED_PNL_TM_BLASTM               41	//	公模吹氣     
#define		LED_PNL_TM_BLASTS               42	//	母模吹氣     
#define		LED_PNL_TM_BLASTC               43	//	吹氣C        
#define		LED_PNL_TM_BLASTD               44	//	吹氣D        
#define		LED_PNL_TM_UBLAST               45	//	上吹氣       
#define		LED_PNL_TM_DBLAST               46	//	下吹氣       
#define		LED_PNL_TM_RUB                  47	//	潤滑         
#define		LED_PNL_TM_RUB2                 48	//	潤滑2        
#define		LED_PNL_TM_SLIDEMOLD            49	//	滑模         
#define		LED_PNL_TM_COREA_FWD            50	//	中子A進      
#define		LED_PNL_TM_COREA_BWD            51	//	中子A退      
#define		LED_PNL_TM_COREB_FWD            52	//	中子B進      
#define		LED_PNL_TM_COREB_BWD            53	//	中子B退      
#define		LED_PNL_TM_COREC_FWD            54	//	中子C進      
#define		LED_PNL_TM_COREC_BWD            55	//	中子C退      
#define		LED_PNL_TM_CORED_FWD            56	//	中子D進      
#define		LED_PNL_TM_CORED_BWD            57	//	中子D退      
#define		LED_PNL_TM_COREE_FWD            58	//	中子E進      
#define		LED_PNL_TM_COREE_BWD            59	//	中子E退      
#define		LED_PNL_TM_COREF_FWD            60	//	中子F進      
#define		LED_PNL_TM_COREF_BWD            61	//	中子F退      
#define		LED_PNL_TM_UCORE_FWD            62	//	上中子進     
#define		LED_PNL_TM_UCORE_BWD            63	//	上中子退     
#define		LED_PNL_TM_DCORE_FWD            64	//	下中子進     
#define		LED_PNL_TM_DCORE_BWD            65	//	下中子退     
#define		LED_PNL_TM_MASTER_STATE         66	//	主機狀態(燈) 
#define		LED_PNL_TM_MASTER_UP            67	//	主機升速     
#define		LED_PNL_TM_MASTER               68	//	主機         
#define		LED_PNL_TM_HEATERONOFF          69	//	電熱啟停     
#define		LED_PNL_TM_MOTORONOFF           70	//	電機啟停     
#define		LED_PNL_TM_MOTORUP              71	//	電機升速     
#define		LED_PNL_TM_MOTORDOWN            72	//	電機降速     
#define		LED_PNL_TM_TOWONOFF             73	//	牽引啟停     
#define		LED_PNL_TM_TOWUP                74	//	牽引升速     
#define		LED_PNL_TM_TOWDOWN              75	//	牽引降速     
#define		LED_PNL_TM_SYNCHROUP            76	//	同步升速     
#define		LED_PNL_TM_SYNCHRODOWN          77	//	同步降速     
#define		LED_PNL_TM_OILPUMP              78	//	油泵         
#define		LED_PNL_TM_VACUUMPUMP           79	//	真空泵       
#define		LED_PNL_TM_SLAVESTATE1          80	//	共擠1狀態(燈)
#define		LED_PNL_TM_SLAVESTATE2          81	//	共擠2狀態(燈)
#define		LED_PNL_TM_SLAVESTATE3          82	//	共擠3狀態(燈)
#define		LED_PNL_TM_SLAVE1               83	//	共擠1        
#define		LED_PNL_TM_SLAVE2               84	//	共擠2        
#define		LED_PNL_TM_SLAVE3               85	//	共擠3        
#define		LED_PNL_TM_FEEDONOFF            86	//	餵料啟停     
#define		LED_PNL_TM_FEEDUP               87	//	餵料升速     
#define		LED_PNL_TM_FEEDDOWN             88	//	餵料降速     
#define		LED_PNL_GL_ADJSLOW             	89	//JOYCE2008-9-24 調模慢速                                            
#define		LED_PNL_GL_AUTOADJ             	90	//JOYCE2008-9-24 自動調模(獨立的key)
#define		LED_PNL_GL_AUTOADJ_OFF         	91	//JOYCE2008-9-24 自動調模(獨立的key)
#define		LED_PNL_GL_CHARGE_OFF           92  //JOYCE2008-9-24 儲料                                            
#define		LED_PNL_JM_LEFTAUTO_ON          93 	//JOYCE2009-3-5 LEFT自動       
#define		LED_PNL_JM_LEFTAUTO_OFF         94 	//JOYCE2009-3-5 LEFT自動       
#define		LED_PNL_JM_RIGHTAUTO_ON         95 	//JOYCE2009-3-5 RIGHT自動       
#define		LED_PNL_JM_RIGHTAUTO_OFF        96 	//JOYCE2009-3-5 RIGHT自動       
#define		LED_PNL_JM_CHARGEMOTOR_ON       97  //JOYCE2009-3-5 擠料電機                                            
#define		LED_PNL_JM_CHARGEMOTOR_OFF      98  //JOYCE2009-3-5 擠料電機                                                                                     
#define		LED_PNL_TM_RIGHTHEATER_ON       97  //2012-6-20                                     
#define		LED_PNL_TM_RIGHTHEATER_OFF      98  //2012-6-20                                                                                    
#define		LED_PNL_TM_LEFTHEATER_ON        99  //2012-6-20                                     
#define		LED_PNL_TM_LEFTHEATER_OFF       100  //2012-6-20                                                                                    
                     
#define		LED_PNL_TM_Spare1				224	
#define		LED_PNL_TM_Spare2				225	
#define		LED_PNL_TM_Spare3				226	
#define		LED_PNL_TM_Spare4				227	
#define		LED_PNL_TM_Spare5				228	
#define		LED_PNL_TM_Spare6				229	
#define		LED_PNL_TM_Spare7				230	
#define		LED_PNL_TM_Spare8				231	
#define		LED_PNL_TM_Spare9				232	
#define		LED_PNL_TM_Spare10				233	
#define		LED_PNL_TM_Spare11				234	
#define		LED_PNL_TM_Spare12				235	
#define		LED_PNL_TM_Spare13				236	
#define		LED_PNL_TM_Spare14				237	
#define		LED_PNL_TM_Spare15				238	
#define		LED_PNL_TM_Spare16				239	




#endif

