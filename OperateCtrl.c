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
void OperateCtrlInit(void)
{

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