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
#define		LED_PNL_TM_MANUAL     			0 	//	���         	
#define		LED_PNL_TM_SEMIATUO             1 	//	�b�۰�       
#define		LED_PNL_TM_ATUO                 2 	//	���۰�       
#define		LED_PNL_TM_SESORATUO            3 	//	�q���۰�   
#define		LED_PNL_TM_SESORATUO_OFF  		4 	//	�q���۰���
#define		LED_PNL_TM_HEAT_ON              5 	//	�q���}       
#define		LED_PNL_TM_HEAT_OFF             6 	//	�q����       
#define		LED_PNL_TM_MOTOR_ON             7 	//	���F�}��     
#define		LED_PNL_TM_MOTOR_OFF            8 	//	���F��       
#define		LED_PNL_TM_DOOR_OPEN            9 	//	�w�����}     
#define		LED_PNL_TM_DOOR_CLOSE           10	//	�w������     
#define		LED_PNL_TM_ADJUST_ATUO          11	//	�۰ʽռ�  
#define		LED_PNL_TM_ADJUSTO_ATUO_OFF		12	//	�۰ʽռ���
#define		LED_PNL_TM_ADJUST_FWD           13	//	�ռҶi       
#define		LED_PNL_TM_ADJUST_BWD           14	//	�ռҰh       
#define		LED_PNL_TM_CLOSEMLD             15	//	����         
#define		LED_PNL_TM_OPENMLD              16	//	�}��         
#define		LED_PNL_TM_INJECT               17	//	�g�X         
#define		LED_PNL_TM_INJECTB              18	//	�g�XB        
#define		LED_PNL_TM_INJECT_BWD           19	//	�g�h         
#define		LED_PNL_TM_INJECTB_BWD          20	//	�g�hB        
#define		LED_PNL_TM_CHARGE               21	//	�x��         
#define		LED_PNL_TM_CHARGEB              22	//	�x��B        
#define		LED_PNL_TM_PURGE_ATUO           23	//	�۰ʲM��     
#define		LED_PNL_TM_PURGEB_ATUO          24	//	�۰ʲM��B    
#define		LED_PNL_TM_EJECT_FWD            25	//	���Ҷi       
#define		LED_PNL_TM_EJECT_BWD            26	//	���Ұh       
#define		LED_PNL_TM_EJECTB_FWD           27	//	���ҶiB      
#define		LED_PNL_TM_EJECTB_BWD           28	//	���ҰhB      
#define		LED_PNL_TM_UEJECT_FWD           29	//	�W���i       
#define		LED_PNL_TM_UEJECT_BWD           30	//	�W���h       
#define		LED_PNL_TM_MEJECT_FWD           31	//	�����i       
#define		LED_PNL_TM_MEJECT_BWD           32	//	�����h       
#define		LED_PNL_TM_DEJECT_FWD           33	//	�U���i       
#define		LED_PNL_TM_DEJECT_BWD           34	//	�U���h       
#define		LED_PNL_TM_NOZZLE_FWD           35	//	�y�x�i       
#define		LED_PNL_TM_NOZZLE_BWD           36	//	�y�x�h       
#define		LED_PNL_TM_DIAL_OUT             37	//	��L�X       
#define		LED_PNL_TM_DIAL_IN              38	//	��L�i       
#define		LED_PNL_TM_ANCHOR_BWD           39	//	�w��h       
#define		LED_PNL_TM_ANCHOR_FWD           40	//	�w��i       
#define		LED_PNL_TM_BLASTM               41	//	���ҧj��     
#define		LED_PNL_TM_BLASTS               42	//	���ҧj��     
#define		LED_PNL_TM_BLASTC               43	//	�j��C        
#define		LED_PNL_TM_BLASTD               44	//	�j��D        
#define		LED_PNL_TM_UBLAST               45	//	�W�j��       
#define		LED_PNL_TM_DBLAST               46	//	�U�j��       
#define		LED_PNL_TM_RUB                  47	//	���         
#define		LED_PNL_TM_RUB2                 48	//	���2        
#define		LED_PNL_TM_SLIDEMOLD            49	//	�Ƽ�         
#define		LED_PNL_TM_COREA_FWD            50	//	���lA�i      
#define		LED_PNL_TM_COREA_BWD            51	//	���lA�h      
#define		LED_PNL_TM_COREB_FWD            52	//	���lB�i      
#define		LED_PNL_TM_COREB_BWD            53	//	���lB�h      
#define		LED_PNL_TM_COREC_FWD            54	//	���lC�i      
#define		LED_PNL_TM_COREC_BWD            55	//	���lC�h      
#define		LED_PNL_TM_CORED_FWD            56	//	���lD�i      
#define		LED_PNL_TM_CORED_BWD            57	//	���lD�h      
#define		LED_PNL_TM_COREE_FWD            58	//	���lE�i      
#define		LED_PNL_TM_COREE_BWD            59	//	���lE�h      
#define		LED_PNL_TM_COREF_FWD            60	//	���lF�i      
#define		LED_PNL_TM_COREF_BWD            61	//	���lF�h      
#define		LED_PNL_TM_UCORE_FWD            62	//	�W���l�i     
#define		LED_PNL_TM_UCORE_BWD            63	//	�W���l�h     
#define		LED_PNL_TM_DCORE_FWD            64	//	�U���l�i     
#define		LED_PNL_TM_DCORE_BWD            65	//	�U���l�h     
#define		LED_PNL_TM_MASTER_STATE         66	//	�D�����A(�O) 
#define		LED_PNL_TM_MASTER_UP            67	//	�D���ɳt     
#define		LED_PNL_TM_MASTER               68	//	�D��         
#define		LED_PNL_TM_HEATERONOFF          69	//	�q���Ұ�     
#define		LED_PNL_TM_MOTORONOFF           70	//	�q���Ұ�     
#define		LED_PNL_TM_MOTORUP              71	//	�q���ɳt     
#define		LED_PNL_TM_MOTORDOWN            72	//	�q�����t     
#define		LED_PNL_TM_TOWONOFF             73	//	�o�ޱҰ�     
#define		LED_PNL_TM_TOWUP                74	//	�o�ޤɳt     
#define		LED_PNL_TM_TOWDOWN              75	//	�o�ޭ��t     
#define		LED_PNL_TM_SYNCHROUP            76	//	�P�B�ɳt     
#define		LED_PNL_TM_SYNCHRODOWN          77	//	�P�B���t     
#define		LED_PNL_TM_OILPUMP              78	//	�o��         
#define		LED_PNL_TM_VACUUMPUMP           79	//	�u�Ŭ�       
#define		LED_PNL_TM_SLAVESTATE1          80	//	�@��1���A(�O)
#define		LED_PNL_TM_SLAVESTATE2          81	//	�@��2���A(�O)
#define		LED_PNL_TM_SLAVESTATE3          82	//	�@��3���A(�O)
#define		LED_PNL_TM_SLAVE1               83	//	�@��1        
#define		LED_PNL_TM_SLAVE2               84	//	�@��2        
#define		LED_PNL_TM_SLAVE3               85	//	�@��3        
#define		LED_PNL_TM_FEEDONOFF            86	//	���ƱҰ�     
#define		LED_PNL_TM_FEEDUP               87	//	���Ƥɳt     
#define		LED_PNL_TM_FEEDDOWN             88	//	���ƭ��t     
#define		LED_PNL_GL_ADJSLOW             	89	//JOYCE2008-9-24 �ռҺC�t                                            
#define		LED_PNL_GL_AUTOADJ             	90	//JOYCE2008-9-24 �۰ʽռ�(�W�ߪ�key)
#define		LED_PNL_GL_AUTOADJ_OFF         	91	//JOYCE2008-9-24 �۰ʽռ�(�W�ߪ�key)
#define		LED_PNL_GL_CHARGE_OFF           92  //JOYCE2008-9-24 �x��                                            
#define		LED_PNL_JM_LEFTAUTO_ON          93 	//JOYCE2009-3-5 LEFT�۰�       
#define		LED_PNL_JM_LEFTAUTO_OFF         94 	//JOYCE2009-3-5 LEFT�۰�       
#define		LED_PNL_JM_RIGHTAUTO_ON         95 	//JOYCE2009-3-5 RIGHT�۰�       
#define		LED_PNL_JM_RIGHTAUTO_OFF        96 	//JOYCE2009-3-5 RIGHT�۰�       
#define		LED_PNL_JM_CHARGEMOTOR_ON       97  //JOYCE2009-3-5 ���ƹq��                                            
#define		LED_PNL_JM_CHARGEMOTOR_OFF      98  //JOYCE2009-3-5 ���ƹq��                                                                                     
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

