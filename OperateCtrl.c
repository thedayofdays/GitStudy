#include "OperateCtrl.h"
typedef struct{
	BOOL Waiting;/*是否正在操作*/
	BOOL OperateState;/*控制状态*/
	BOOL CurrentState;/*当前状态*/
	BOOL OperateResult;/*操作结果*/
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

}
BOOL OperateState(UINT8 type,UINT8* result)
{

}
void OperateRetry(void)
{

}