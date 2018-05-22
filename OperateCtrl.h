#ifndef _OPERATE_CTRL_H_
#define _OPERATE_CTRL_H_
#include "BasicDef.h"
typedef enum{
	OUT_PORT_1=0,
	OUT_PORT_2,
	OUT_PORT_3,
	OUT_PORT_4,
	OUT_PORT_MAX,
}OUTPORTLIST;
typedef enum{
	IN_PORT_1=0,
	IN_PORT_2,
	IN_PORT_3,
	IN_PORT_MAX,
}INPORTLIST;
typedef enum{
	OPERATE_OFF_OK=0,
	OPERATE_ON_OK,
	OPERATE_BLOCK,/*粘连*/
	OPERATE_NO_ACTION,/*拒动*/
	OPERATE_MIS_ACTION,/*误动*/
}OPERATE_RESULT;
extern void OperateCtrl(UINT8 type,BOOL operate,UINT16 delayTime);
extern BOOL OperateState(UINT8 type,UINT8* result);
extern void OperateRetry(void);
extern void OperateCtrlInit(void);
#endif
