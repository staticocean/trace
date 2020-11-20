
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __INS_PROC__
#define __INS_PROC__

//------------------------------------------------------------------------------

#include "vl.h"

#include "trj_utils.h"
#include "trj_obj.h"

//------------------------------------------------------------------------------

typedef struct ins_proc_euler
{
    s_trj_obj *parent;
    
    uint64_t offset;
    vlf_t pos_0[4][3];
    vlf_t rot_0[4][9];
    vlf_t time_0[4];
    
//    vlf_t pos_0[3];
//    vlf_t pos_1[3];
//    vlf_t pos_2[3];
    
}   ins_proc_euler_t;

//------------------------------------------------------------------------------

inline void ins_proc_euler(ins_proc_euler_t *proc, s_trj_obj *obj)
{
	vl_vcopy(&proc->pos_0[proc->offset & 0x03][0], obj->pos[0]);
	vl_mcopy(&proc->rot_0[proc->offset & 0x03][0], obj->rot[0]);
	proc->time_0[proc->offset & 0x03] = obj->time[0];
	proc->offset++;
	
	vlf_t *p0 = &proc->pos_0[(proc->offset-3) & 0x03][0];
	vlf_t *p1 = &proc->pos_0[(proc->offset-2) & 0x03][0];
	vlf_t *p2 = &proc->pos_0[(proc->offset-1) & 0x03][0];

//    vlf_t *r0 = &proc->rot_0[(proc->offset-3) & 0x03][0];
	vlf_t *r1 = &proc->rot_0[(proc->offset-2) & 0x03][0];
	vlf_t *r2 = &proc->rot_0[(proc->offset-1) & 0x03][0];
//    vlf_t *p3 = &proc->pos_0[(proc->offset-1) & 0x03][0];
	
	vlf_t t0 = proc->time_0[(proc->offset-3) & 0x03];
	vlf_t t1 = proc->time_0[(proc->offset-2) & 0x03];
	vlf_t t2 = proc->time_0[(proc->offset-1) & 0x03];
//    vlf_t t3 = proc->time_0[(proc->offset-1) & 0x03];
	
	vlf_t v0[3];
	vlf_t v1[3];
	
	vlf_t a0[3];
	
	if (proc->offset > 2)
	{
		vl_vsub(v0, p1, p0);
		vl_vmul_s(v0, v0, 1 / (t1-t0));
		
		vl_vsub(v1, p2, p1);
		vl_vmul_s(v1, v1, 1 / (t2-t1));
		
		vl_vsub(a0, v1, v0);
		vl_vmul_s(a0, a0, 1 / (t2-t1));
		
		vl_vcopy(proc->parent->pos[1], v1);
		vl_vcopy(proc->parent->pos[2], a0);
		
		// R1*R0.I = C
//        vlf_t temp_w0[9];
		vlf_t temp_w1[9];

//        vlf_t r0_tnp[0];
		vlf_t r1_tnp[9];

//        vl_tnp(r0_tnp, r0);
		vl_tnp(r1_tnp, r1);

//        vl_mmul_m(temp_w0, r1, r0_tnp);
		vl_mmul_m(temp_w1, r2, r1_tnp);
		
		vl_unskew(proc->parent->rot[1], temp_w1);
		vl_vmul_s(proc->parent->rot[1], proc->parent->rot[1], 1 / (t2-t1));

//        rot_1 = self.parent.rot_0[-2].dot(self.parent.rot_0[-3].I);
//        self.parent.rot_1.append(trj_utils.unskew(rot_1) / (self.parent.time_0[-2] - self.parent.time_0[-3]));
	}
	
	return;
}

//------------------------------------------------------------------------------

#endif /* __INS_PROC__ */


