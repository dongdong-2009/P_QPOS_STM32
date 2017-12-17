#include "string.h"
#include "APP_qPBOC_Pay.h"
#include "APP_PBOC.h"
#include "MID_PBOC.h"
#include "EMV_Parameter.h"
#include "PBOC_Var.h"
#include "Nfc_main.h"
#include "CCID_VALUE.h"
#include "sercrity.h"

//add by leehuan



unsigned char FLAG9F66;
unsigned char FLAG5A;
unsigned char FLAG57;
unsigned char FLAG5F24;
//unsigned char uchPDOL[128];
unsigned char FLAG_NO9F26;//GPO没有返回9F26报文，需联机
unsigned char aidnum[6];

unsigned char SDAFlag;
unsigned char SDAData[248];
unsigned char SDALen;
unsigned char SDANum;

unsigned char INPutAmount(unsigned char *p)
{
	return 0;
}
unsigned char AffirmAmount()
{
	return 0;
}
unsigned short qPBOC_CheckCard()
{
	return 0;
}
void App_qPBOC_Init(void)
{	
	Clear_Cardlist();

	uchAvailableOFFAmount[0]=0;
	uchCAKeyIndex[0]=0;
	uchSSAData[0]=0;   //签名的静态数据
	uchIssuerKeyCert[0]=0;  //发卡行公钥证书
	uchICCKeyCert[0]=0;//IC卡公钥证书
	uchSDATagList[0]=0;//静态数据列表标签
	uchIssuerKeyReminder[0]=0;//发卡行公钥余项
	uchIssuerKeyExp[0]=0;//发卡行公钥指数
	uchICCKeyReminder[0]=0;
	uchIssuerKeyExp[0]=0;//发卡行公钥指数
	uchSDAData[0]=0;//动态签名数据
	uchICCardProp[0]=0;
	uchICAuthenticationData[0]=0;

//	memset(puchTvr,0,5);	
//	memset(puchTsi,0,2);
	
	FLAG9F66=0;
	FLAG5A=0;
	FLAG57=0;
	FLAG5F24=0;
	FLAG_NO9F26=0;
	
	aidnum[0]=0;
	aidnum[1]=1;
	aidnum[2]=2;
	aidnum[3]=3;
	aidnum[4]=4;
	aidnum[5]=5;

	
	return;
}


unsigned int SelectAID(unsigned char number,unsigned char aidseq[])
{
	uchAid[0]= Candidate_List[aidseq[0]].App_Name_Len;
	memcpy(&uchAid[1],Candidate_List[aidseq[0]].App_Name,uchAid[0]);
	return 0x9000;
}


unsigned short App_qPBOC_PreProcessing()
{

#if 0
	unsigned char i;
	unsigned char *puchTestFlags = Reader_VarDo_List + 105; 
	
	if(memcmp(puchAmount,puchTransUpLimit,6)>=0)
	{
		*puchTestFlags|=0x40;
		DISP_UPLimit();
		return PREPROCESSING_DONE;//select another method
	}
	
	/*if(*puchAmount||*(puchAmount+1)||memcmp(puchAmount+2,puchTerminalFloorLimit,4)>0)
		{
			*(puchTransProp+1)|=0x80;
			*puchTestFlags|=0x80;					
		}*/
	if(memcmp(puchAmount,puchQTerminalFloorLimit,6)>=0)
	{
		*(puchTransProp+1)|=0x80;
	}	
	else
	{
		for(i=0;i<6;i++)
		{
			if(*(puchAmount+i))
				break;
			if(i==5)
			{
				if((*puchTransProp&0x08)==0x08)
				{
					DISP_SELECTANOTHER();
					return PREPROCESSING_DONE;//select another method
				}
				else
				{
					*(puchTransProp+1)|=0x80;
					*puchTestFlags|=0x04;									
				}									
			}
		}
	}
	if(puchAmount[0]==0&&puchAmount[1]==0&&puchAmount[2]==0&&puchAmount[3]==0&&puchAmount[4]==1&&puchAmount[5]==0)
	{
		*(puchTransProp+1)|=0x80;
	}
	if(memcmp(puchAmount,puchTransCVMFloorLimit,6)>=0)
	{
		*(puchTransProp+1)|=0x40;
		*puchTestFlags|=0x20;
	}

#endif

	return SUCCESS;
}

unsigned char App_qPBOC_SelectPPSE()
{	
	unsigned char len;
	unsigned char *ptr=NULL;
	unsigned char *pin=NULL;
	unsigned char i,j,min,temp;
	
	T_APDU.CLA = 0x00;
	T_APDU.INS = 0xA4;
	T_APDU.PP1 = 0x04;
	T_APDU.PP2 = 0x00;
	T_APDU.PP3 = 0x0e;
	T_APDU.LC  = 0x0e;
	T_APDU.LE  = 0x00;
	T_APDU.RLE = 0x00;
	
	memcpy(T_APDU.pInData, "2PAY.SYS.DDF01", 14);
	NFC_Card_APDU(&T_APDU);
	//HAL_USB_BulkIn(2,T_APDU.pOutData,T_APDU.RLE);
	
	if(T_APDU.SW1 !=0x90||T_APDU.SW2 != 0)
	{
		return FAIL;
	}
	if(T_APDU.pOutData[0]!=0x6F)
	{
		return FAIL;
	}
	ptr = SearchTag(T_APDU.pOutData, T_APDU.RLE,0xBF0C);

	if(NULL == ptr)
	{
		return FAIL;
	}	
	else
	{				
		len=*(ptr+2);
		pin=ptr+3;
		Candidate_Number=0;
		while(len>0)
		{
			ptr = SearchTag(pin,len,0x61);
			if(NULL==ptr)
			{
				return FAIL;
			}		
			else
			{							
				if(ptr[2]==0x4f)
				{
					uchAid[0]=*(ptr+3);
					memcpy(&uchAid[1], ptr+4, uchAid[0]);												
					memcpy(Candidate_List[Candidate_Number].App_Name,&uchAid[1],uchAid[0]);
					Candidate_List[Candidate_Number].App_Name_Len=uchAid[0];
					pin = SearchTag(pin,len,0x87);
					if(NULL==pin)
					{
						Candidate_List[Candidate_Number].API=0;
					}	
					else 
						Candidate_List[Candidate_Number].API=*(pin+2);
					
					Candidate_Number++;
				}							
				len-=*(ptr+1)+2;
				pin=ptr+*(ptr+1)+2;						
			}
		}					
	}
	
	if(Candidate_Number==1)
	{
	}
	else if(Candidate_Number>1)
	{
		for(i=0;i<Candidate_Number-1;i++)//将候选列表按API排序，顺序保存在aidnum数组中
		{
			min=aidnum[i];
			if(Candidate_List[min].API==0)
				continue;
			for(j=i+1;j<Candidate_Number;j++)
			{
				if(Candidate_List[min].API>Candidate_List[aidnum[j]].API)
				{	
					if(Candidate_List[j].API==0)
						continue;
					min=aidnum[j];
					temp=aidnum[i];
					aidnum[i]=aidnum[j];
					aidnum[j]=temp;
				}
			}
		}
		SelectAID(Candidate_Number,aidnum);		
	}
	return SUCCESS;
}

unsigned char App_qPBOC_SelectADF()
{
	unsigned char *ptr=NULL;
	
	T_APDU.CLA = 0x00;
	T_APDU.INS = 0xa4;
	T_APDU.PP1 = 0x04;
	T_APDU.PP2 = 0x00;
	T_APDU.PP3 = uchAid[0];
	T_APDU.LC  = T_APDU.PP3;
	T_APDU.LE  = 0x00;
	T_APDU.RLE = 0x00;

	memcpy(T_APDU.pInData, &uchAid[1], uchAid[0]);

	NFC_Card_APDU(&T_APDU);

	if(T_APDU.SW1 !=0x90||T_APDU.SW2 !=0)
	{					
		return SELECT_ANOTHER_AID;
	}	
	if(T_APDU.pOutData[0] !=0x6f)
		return SELECT_ANOTHER_AID;
	ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE-2, 0x84);
	if(NULL==ptr)
		return SELECT_ANOTHER_AID;
	ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE-2, 0xa5);
	if(NULL==ptr)
		return SELECT_ANOTHER_AID;
	
	return SUCCESS;
}

int App_qPBOC_GPO()
{
	unsigned char *ptr;
	unsigned char pdoldata[156];
	unsigned char pdollen;
	ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x9f38);


	if(NULL==ptr)
	{
		return PREPROCESSING_DONE;
	}
	else
	{					
		memcpy(uchPDOL, ptr+3, *(ptr+2));//保存PDOL数据		
		pdollen= CollectDol(uchPDOL, *(ptr+2), pdoldata,PDOL_FLAG);					
	}			
/*
	if(FLAG9F66!=1)
	{
		return FAIL;
	}
*/
	*T_APDU.pInData = 0x83;	//设置PDOL数据对象Tag
	if(pdollen>=0x80)
	{
		T_APDU.pInData[1] = 0x81;	//设置PDOL数据对象Tag
		T_APDU.pInData[2] = pdollen;
		memcpy(&T_APDU.pInData[3],pdoldata,pdollen);
		pdollen+=3;
	}			
	else
	{
		T_APDU.pInData[1] = pdollen;
		memcpy(&T_APDU.pInData[2],pdoldata,pdollen);
		pdollen+=2;
	}
								
	/*-----------4. GPO------------*/				
	T_APDU.CLA = 0x80;
	T_APDU.INS = 0xa8;
	T_APDU.PP1 = 0x00;
	T_APDU.PP2 = 0x00;
	T_APDU.PP3 = pdollen;
	T_APDU.LC  = pdollen;
	T_APDU.LE  = 0x00;
	T_APDU.RLE = 0x00;
	NFC_Card_APDU(&T_APDU);
	//HAL_USB_BulkIn(2,T_APDU.pOutData, T_APDU.RLE);
	
	if(T_APDU.SW1==0x69&&T_APDU.SW2==0x84)
	{
		return FAIL;
	}
	
	if(T_APDU.SW1==0x69&&T_APDU.SW2==0x85)
	{					
		return SELECT_ANOTHER_AID;
	}
	//    PBOC3.0 GPO响应数据全部为格式2
	if(T_APDU.pOutData[0]==0x80)
	{
		
		memcpy(&uchAip[1],T_APDU.pOutData+2,2);
		uchAip[0]=2;
		uchAFL[0]=T_APDU.pOutData[1]-2;
		memcpy(&uchAFL[1],T_APDU.pOutData+4,uchAFL[0]);
		FLAG_NO9F26=1;
		return 0x9000;
	}
		
	else if(T_APDU.pOutData[0] == 0x77)
	{
		if(T_APDU.pOutData[1]<=0x80)
		{
			if(T_APDU.pOutData[1]!=T_APDU.RLE-2)
			{
				return FAIL;
			}						
		}
		else if(T_APDU.pOutData[1]==0x81)
		{
			if(T_APDU.pOutData[2]!=T_APDU.RLE-3)
			{
				return FAIL;
			}						
		}
		ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x9f26);		//0x9f26应用密文
		if(NULL==ptr)
		{				
			FLAG_NO9F26=1;
			ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x94);		//0x94应用文件定位AFL
			if(NULL==ptr)
			{
				return FAIL;
			}
			else
			{
				memcpy(uchAFL,ptr+1,*(ptr+1)+1);
			}			
		}
		ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x82);
		if(NULL==ptr)
		{
			if(FLAG_NO9F26==0)
			{
				return FAIL;				
			}
		}
		else
		{
			memcpy(uchAip,ptr+1,*(ptr+1)+1);
			if(FLAG_NO9F26==1)
				return 0x9000;
		}
		ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x9f36);
		if(NULL==ptr)
		{
			return FAIL;
		}
		ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x57);
		if(NULL==ptr)
		{
		}
		else
			FLAG57++;
		ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x9f10);
		if(NULL==ptr)
		{
			return FAIL;
		}	
		else
		{									
			SaveCardDo(T_APDU.pOutData, T_APDU.RLE);
			if((*(ptr+7)&0x30)==0x00)//脱机拒绝AAC
			{	
				//MID_DISP_ShowString(6,24,(unsigned char *)"请移开卡片",10);				
				if(FLAG57==0)//脱机拒绝必备数据
				{
					//交易终止，使用其他界面
					return FAIL;
				}
				else
				{
					//MID_DISP_ShowString(2,32,(unsigned char *)"脱机拒绝",10);
					if(uchAvailableOFFAmount[0])
					{
										
					}
				}				
				return FAIL;
			}				
			else if((*(ptr+7)&0x30)==0x20)//终端联机ARQC
			{
				return 0x9000;
			}
			else if((*(ptr+7)&0x30)==0x10)//脱机批准TC
			{	
				
				ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x94);
				if(NULL==ptr)
				{
					return FAIL;
				}
				else
				{
					if(*(ptr+1)<4||*(ptr+1)%4)
					{
						return FAIL;
					}
					memcpy(uchAFL,ptr+1,*(ptr+1)+1);
				}								
			}				
		}
	}
	return 0x9000;
}


unsigned short App_qPBOC_ReadData()
{
	unsigned char sfi;
	unsigned char start_record,end_record;
	unsigned char i,j;
	unsigned char *ptr;
	SDALen=0;
	SDAFlag=0;
	SDANum=0;
	
	for(i=0;i<uchAFL[0]/4;i++) //根据AFL循环读取指定记录，AFL四字节为一组
	{
		sfi = uchAFL[i*4+1]>>3;
		start_record = uchAFL[i*4+2];
		end_record = uchAFL[i*4+3];

		if((sfi<1) || (sfi>30) || (start_record ==0) || (start_record > end_record) || (uchAFL[i*4+4] > (end_record-start_record+1)))		 //SFI 取值范围1~30, Record Number >0
		{
			return PREPROCESSING_END;
		}
		
		
		if(uchAFL[i*4+4])
		{
			SDANum=uchAFL[i*4+4];
			if(sfi<=10)
			{
				SDAFlag=1;						
			}
			else
			{
				SDAFlag=2;						
			}
		}	
		for(j=start_record;j<=end_record;j++)  //根据AFL项中指定的SFI和记录号范围读取记录
		{
			T_APDU.CLA = 0x00;
			T_APDU.INS = 0xb2;
			T_APDU.PP1 =j;
			T_APDU.PP2 = uchAFL[i*4+1]| 0x04;;
			T_APDU.PP3 = 0x00;
			T_APDU.LC  = 0x00;
			T_APDU.LE  = 0x00;
			T_APDU.RLE = 0x00;

			NFC_Card_APDU(&T_APDU);
			//HAL_USB_BulkIn(2,T_APDU.pOutData,T_APDU.RLE);
			
			if(T_APDU.SW1 !=0x90||T_APDU.SW2!=0)
			{
				return FAIL;
			}
			
			if(T_APDU.pOutData[0]!=0x70)
			{			
				return FAIL;
			}	
			if(T_APDU.pOutData[1]<=0x80)
			{
				if(T_APDU.pOutData[1]!=T_APDU.RLE-2)
				{
					return FAIL;
				}						
			}
			else if(T_APDU.pOutData[1]==0x81)
			{
				if(T_APDU.pOutData[2]!=T_APDU.RLE-3)
				{
					return FAIL;
				}						
			}
			if(FLAG5A<2)
			{
				ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x5a);
				if(NULL==ptr)
				{
				}
				else
				{
					FLAG5A++;
					if(FLAG5A==2)
					{
						return FAIL;
					}					
				}						
			}
			if(FLAG57<2)
			{
				ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x57);
				if(NULL==ptr)
				{							
				}
				else
				{
					FLAG57++;
					if(FLAG57==2)
					{
						return FAIL;
					}
				}
			}
			if(FLAG5F24<2)
			{
				ptr=SearchTag(T_APDU.pOutData, T_APDU.RLE, 0x5f24);
				if(NULL==ptr)
				{							
				}
				else
				{
					FLAG5F24++;
					if(FLAG5F24==2)
					{
						return FAIL;
					}
					memcpy(uchExpirationDate,ptr+2,4);	
					if((uchExpirationDate[1]>=0x50)||(memcmp(&uchExpirationDate[1],puchDate,3)<0))
					{
						//MID_DISP_ShowString(2,32,(unsigned char *)"卡片过期",8);
						//MID_DISP_ShowString(4,32,(unsigned char *)"脱机拒绝",8);
					}
				}
			}
			
			if(SDAFlag==1&&SDANum>0)
			{						
				if(T_APDU.pOutData[1]<=0x80)
				{
					memcpy(&SDAData[SDALen],&T_APDU.pOutData[2],T_APDU.pOutData[1]);
					SDALen+=T_APDU.pOutData[1];
				}						
				else if(T_APDU.pOutData[1]==0x81)
				{
					memcpy(&SDAData[SDALen],&T_APDU.pOutData[3],T_APDU.pOutData[2]);
					SDALen+=T_APDU.pOutData[2];
				}
			}
			if(SDAFlag==2&&SDANum>0)
			{
					
				if(T_APDU.pOutData[1]<=0x80)
				{
					memcpy(&SDAData[SDALen],T_APDU.pOutData,T_APDU.pOutData[1]+2);
					SDALen+=T_APDU.pOutData[1]+2;
				}						
				else if(T_APDU.pOutData[1]==0x81)
				{
					memcpy(&SDAData[SDALen],T_APDU.pOutData,T_APDU.pOutData[2]+3);
					SDALen+=T_APDU.pOutData[2]+3;
				}							
				
			}
			SaveCardDo(T_APDU.pOutData, T_APDU.RLE);	
			if(SDAFlag&&SDANum>0)
			SDANum--;
		}
	}	
	return SUCCESS;
}

unsigned short App_qPBOC_Offline()
{
	unsigned short status;
	
	//MID_DISP_ShowString(6,24,(unsigned char *)"请移开卡片",10);	
	//HAL_Delay_ms(1000);
	
	if((uchAip[1]&0x20)==0)//卡片不支持脱机
	{
		if(uchICCardProp[1]&0x20)
		{
			//g_APDU.RLE=DATAUPLOAD(2);
			return REQUEST_MESSAGE;
		}
	}
	
	if(FLAG5A==1)
	{
		status=OfflineDynamicDataAuthentication();
	}
	else
	{
		status=0;
	}
	
	if(status==0x9000)
	{
		//MID_DISP_ShowString(2, 28,(unsigned char *)"扣款成功!", 9);
		if(uchAvailableOFFAmount[0])
		{
			//MID_DISP_ShowString(4,16,(unsigned char *)"电子现金金额",12);
			//MidShowAmount(6,uchAvailableOFFAmount+1);	
		}
	}
	else
	{
		//HAL_DISP_CLS();
		//MID_DISP_ShowString(2, 28,(unsigned char *)"扣款失败!", 9);
		return FAIL;
		//HAL_Delay_ms(2000);
	}

	return SUCCESS;
}
unsigned char APP_qPBOC_SaveRecord()
{
	return 0;
}

unsigned char App_qPBOC_DataUpLoad()
{
	return 0;
}

unsigned short App_qPBOC_ReadRecordInfo()
{
	return 0;
}

unsigned short App_qPBOC_Purchase(unsigned char OnlineFlag)
{
	unsigned short status;
	unsigned char i;
	int ret;
	
	switch(g_APDU.PP2)
	{
		case 0x01:		//联机查余额获取55域数据
			//交易类型
			*puchTranType = INQUIRY;
		    break;

		case 0x00:		//消费交易获取55域数据
			//交易类型
			*puchTranType = GOODS_PURCHASE;
		    break;	

		default:
		    G_flag_BalanceOrLoad = 0;
			break;
	}
	App_qPBOC_Init();	

	//保存交易金额，日期，时间
	memcpy(puchAmount,g_APDU.pInData,6);			  //金额		  
	memcpy(puchDate,g_APDU.pInData + 6,3);			  //日期
	memcpy(puchTime,g_APDU.pInData + 9,3);			  //时间
	*puchTranType = GOODS_PURCHASE;

	if(OnlineFlag==1)
	{
		puchTerminalTransProp[1]&=0x80;//要求联机
	}
	GetChallenge(puchUnpreData,4);	//不可预知数，应该用随机数


	/*-----------1. Select PPSE--------------------*/
	ret=App_qPBOC_SelectPPSE();
	if(ret!=SUCCESS)
	{
		goto TRANS_END;
	}					
		
	/*-----------3. Select ADF------------*/
SelectaADF:
	ret=App_qPBOC_SelectADF();
	if(ret==SELECT_ANOTHER_AID)
	{
		Candidate_Number--;
		if(Candidate_Number==0)
		{
			goto TRANS_END;
		}					
		else
		{
			for(i=0;i<Candidate_Number;i++)
			{
				aidnum[i]=aidnum[i+1];
			}						
		}	
		SelectAID(Candidate_Number,aidnum);					
		goto SelectaADF;
	}
	/*---------------------GPO--------------------*/
	ret = App_qPBOC_GPO();
	if(ret == SELECT_ANOTHER_AID)
	{
		Candidate_Number--;
		if(Candidate_Number==0)
		{
			goto TRANS_END;
		}					
		else
		{
			for(i=0;i<Candidate_Number;i++)
			{
				aidnum[i]=aidnum[i+1];
			}						
		}	
		SelectAID(Candidate_Number,aidnum);					
		goto SelectaADF;
	}
	if(ret!=0x9000)
		goto TRANS_END;
	
	/*--------------5. Read Application Data-------------*/
	ret=App_qPBOC_ReadData();
	if(ret!=SUCCESS)
	{
		goto TRANS_END;
	}
	if(FLAG_NO9F26==0)
	{
		ret=App_qPBOC_Offline();
		if(ret!=SUCCESS)
		{
			goto TRANS_END;
		}
	}
	else
	{
		//处理限制
		ret = EMV_ProcessingRestriction();
		if(NOK == ret)
		{
			g_APDU.RLE = 0;
			ret =  0x0803;  	
			return SW_OPERATION_SUCCESS;
		}
			
		//持卡人验证
		ret = EMV_CardholderVerification();
		if(SW_OPERATION_SUCCESS != ret)
		{		
			g_APDU.RLE = 0;
			ret =  0x0804;  	
			return SW_OPERATION_SUCCESS;
		}
			
		//终端风险管理
		EMV_TerminalRiskManagement();	
					
		//终端行为分析	
		if (SW_OPERATION_SUCCESS != EMV_TerminalActionAnalysis(0))
		{
			g_APDU.RLE = 0;
			ret =  0x0805;  	
			return SW_OPERATION_SUCCESS;
		} 		
		else
		{
			return SW_OPERATION_SUCCESS;
		}
	}

TRANS_END:
	//MID_DISP_ShowString(2, 28,(unsigned char *)"扣款失败!", 9);
	g_APDU.RLE=0;
	return 0x1111;
	
}	



