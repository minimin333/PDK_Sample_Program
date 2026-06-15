/*For more information see notes.txt in the Documentation folder */
#include "usrcode.h"
#define _PPScriptMode_		// for enum mode, replace this with #define _EnumMode_	

#include "../Include/pp_proj.h"

extern struct SHM        *pshm;  // Pointer to shared memory
extern volatile unsigned *piom;  // Pointer to I/O memory
extern void              *pushm; // Pointer to user memory

void user_phase(struct MotorData *Mptr)
{
}

double user_pid_ctrl(struct MotorData *Mptr)
{
	double *p;
	p = pushm;
	return 0;
}

void CaptCompISR(void)
{
	unsigned *pUnsigned = pushm;
	*pUnsigned = *pUnsigned + 1;
}

double GetLocal(struct LocalData *Ldata, int m)
{
	return *(Ldata->L + Ldata->Lindex + m);
}

void SetLocal(struct LocalData *Ldata, int m, double value)
{
	*(Ldata->L + Ldata->Lindex + m) = value;
}

double *GetLocalPtr(struct LocalData *Ldata, int m)
{
	return (Ldata->L + Ldata->Lindex + m);
}

double CfromScript(double cfrom_type, double arg2, double arg3, double arg4, double arg5, double arg6, double arg7, struct LocalData *Ldata)
{
	int icfrom_type = (int)cfrom_type;
	double *C, *D, *L, *R, rtn; // C, D, R - only needed if doing Kinematics

	C = GetCVarPtr(Ldata);  // Only needed if doing Kinematics
	D = GetDVarPtr(Ldata);  // Only needed if doing Kinematics
	L = GetLVarPtr(Ldata);  // Only needed if using Ldata or Kinematics
	R = GetRVarPtr(Ldata);  // Only needed if doing Kinematics
	rtn = -1.0;
	return rtn;
}

