#include "OperateCtrl.h"
typedef struct{
	BOOL Waiting;/*是否正在操作*/
	BOOL OperateState;/*控制状态*/
	BOOL CurrentState;/*当前状态*/
	BOOL OperateResult;/*操作结果*/
	UINT8 CurtTryCount;/*当前重试次数*/
//	UINT8 OperatePortIndex;/*操作引脚序号*/
	UINT8 FeedBackPortIndex;/*反馈引脚序号*/
	UINT8 MaxRetryCount;/*最大重试次数*/
	UINT16 RetryInterval;/*重试间隔in ms*/
	UINT16 DelayTime;/*操作延时时间*/
	SYSTIME LastTryTime;/*上次重试时间*/
}CTRL_DATA;
static CTRL_DATA gCtrl[OUT_PORT_MAX];
void OperateCtrlInit(void)
{
	UINT8 i;
	for(i=0;i<OUT_PORT_MAX;i++)
	{
		gCtrl[i].Waiting=FALSE;
		gCtrl[i].OperateState=FALSE;
		gCtrl[i].CurrentState=FALSE;
		gCtrl[i].OperateResult=OPERATE_OFF_OK;
		gCtrl[i].MaxRetryCount=3;
		gCtrl[i].RetryInterval=1000;
		gCtrl[i].DelayTime=0;
		gCtrl[i].FeedBackPortIndex=IN_PORT_MAX;
	}
}
void OperateCtrl(UINT8 type,BOOL operate,UINT16 delayTime)
{
	if(type>=OUT_PORT_MAX)
	{
		return;
	}
	if((gCtrl[type].Waiting==FALSE)&&(gCtrl[type].CurrentState!=operate))
	{
		gCtrl[type].Waiting=TRUE;
		gCtrl[type].OperateState=operate;
		gCtrl[type].CurtTryCount=gCtrl[type].MaxRetryCount;
		if(delayTime!=0)
		{
			gCtrl[type].DelayTime=delayTime;
		}
		else
		{
			gCtrl[type].DelayTime=0;
			_hardware_operate(type,operate);/*第一次尝试*/

		}
		TIME_GET(&gCtrl[type].LastTryTime);
	}
}
BOOL OperateState(UINT8 type,UINT8* result)
{
	if(type<OUT_PORT_MAX)
	{
		if(gCtrl[type].Waiting==FALSE)
		{
			*result=gCtrl[type].OperateResult;
			return TRUE;
		}
	}
	*result=OPERATE_OFF_OK;
	return TRUE;
}
void OperateRetry(void)
{
	UINT8 i;
	UINT32 diffTm;
	SYSTIME curtTm;
	TIME_GET(&curtTm);
	for(i=0;i<OUT_PORT_MAX;i++)
	{
		gCtrl[i].CurrentState=_hardware_state(i);/*首先获取当前状态*/
		if(gCtrl[i].Waiting)
		{
			diffTm=TIME_INTERVAL(&gCtrl[i].LastTryTime,&curtTm);
			if(gCtrl[i].DelayTime==0)
			{
				if(gCtrl[i].CurrentState!=gCtrl[i].OperateState)/*状态和控制不符*/
				{
					if(diffTm>=gCtrl[i].RetryInterval)
					{
						if(gCtrl[i].CurtTryCount>0)
						{
							gCtrl[i].CurtTryCount--;
							TIME_GET(&gCtrl[i].LastTryTime);/*记录上一次尝试时间*/
							/*再次尝试*/
							_hardware_operate(i,gCtrl[i].OperateState);
						}
						else/*多次尝试失败*/
						{
							if(gCtrl[i].CurrentState)/*闭合失败--拒动*/
							{
								gCtrl[type].OperateResult=OPERATE_NO_ACTION;
							}
							else/*分断失败--拒动--粘连*/
							{
								gCtrl[i].OperateResult=OPERATE_BLOCK;
							}
							gCtrl[i].Waiting=FALSE;/*操作完成*/
						}
					}
				}
				else/*操作完成*/
				{
					if(gCtrl[i].CurrentState)/*闭合成功*/
					{
						gCtrl[type].OperateResult=OPERATE_ON_OK;
					}
					else/*分断成功*/
					{
						gCtrl[i].OperateResult=OPERATE_OFF_OK;
					}
					gCtrl[i].Waiting=FALSE;/*操作完成*/
				}
			}
			else
			{
				if(diffTm>=gCtrl[i].DelayTime)
				{
					gCtrl[i].DelayTime=0;
					TIME_GET(&gCtrl[i].LastTryTime);/*记录第一次尝试时间*/
					_hardware_operate(i,gCtrl[i].OperateState);/*第一次尝试*/
				}
			}
		}
		else/*检查是否误动*/
		{
			if((gCtrl[i].OperateResult==OPERATE_ON_OK)||(gCtrl[i].OperateResult==OPERATE_OFF_OK))/*操作成功才需要检查误动*/
			{
				if(gCtrl[i].CurrentState!=gCtrl[i].OperateState)/*误动*/
				{
					gCtrl[type].OperateResult=OPERATE_MIS_ACTION;
				}
			}
			else/*检查故障是否解除*/
			{
				if(gCtrl[i].CurrentState==gCtrl[i].OperateState)/*故障解除*/
				{
					if(gCtrl[i].CurrentState)/*闭合成功*/
					{
						gCtrl[type].OperateResult=OPERATE_ON_OK;
					}
					else/*分断成功*/
					{
						gCtrl[i].OperateResult=OPERATE_OFF_OK;
					}
				}
			}
		}
	}
}