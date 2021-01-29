
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_PROC__
#define __TRJ_PROC__

//------------------------------------------------------------------------------

#include "vl.h"

#include "trj_api.h"

//------------------------------------------------------------------------------

#define trj_proc_euler_id (0x00000001)

typedef struct trj_proc_euler
{
	vlf_t d1p[3];
	vlf_t d1n[3];
	vlf_t d2 [3];
	
	vlf_t rnt[9]; // rotation next transpose
	
	// https://math.stackexchange.com/questions/668866/how-do-you-find-angular-velocity-given-a-pair-of-3x3-rotation-matrices
	vlf_t a[9];
	vlf_t at[9];
	vlf_t da[9];
	vlf_t tra;
	vlf_t omg;
	vlf_t d1rp[9];
	vlf_t d1rn[9];
	
}   s_trj_proc_euler;

typedef struct trj_proc_euler_init
{
	uint32_t temp;

}   s_trj_proc_euler_init;

//------------------------------------------------------------------------------

inline uint8_t trj_proc_euler_init(s_trj_proc_euler *self, s_trj_proc_euler_init attr)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_save(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_load(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_update(s_trj_proc_euler *self, s_trj_obj *obj, uint32_t offset)
{
	s_trj_obj_data *data = obj->log_list;
	
	if (offset > 0x00)
	{
		vl_vsub (self->d1p, &data[offset].pos[0][0], &data[offset-1].pos[0][0]);
		vl_vmul_s (self->d1p, self->d1p, (vlf_t) 1.0 / (data[offset].time[0] - data[offset-1].time[0]));
	}
	else { vl_vzero(self->d1p); }
	
	if (offset < (obj->log_offset-1))
	{
		vl_vsub (self->d1n, &data[offset+1].pos[0][0], &data[offset].pos[0][0]);
		vl_vmul_s (self->d1n, self->d1n, (vlf_t) 1.0 / (data[offset+1].time[0] - data[offset].time[0]));
	}
	else { vl_vzero(self->d1n); }
	
	if 		(offset == 0x00) 				{ vl_vinter(&data[offset].pos[1][0], self->d1p, self->d1n, 1.0); }
	else if (offset == (obj->log_offset-1)) { vl_vinter(&data[offset].pos[1][0], self->d1p, self->d1n, 0.0); }
	else 									{ vl_vinter(&data[offset].pos[1][0], self->d1p, self->d1n, 0.5); }
	
	vl_vsub (self->d2, self->d1n, self->d1p);
	
	if 		(offset == 0x00) 				{ vl_vmul_s(&data[offset].pos[2][0], self->d2, (vlf_t) 1.0 / (data[offset+1].time[0] - data[offset].time[0])); }
	else if (offset == (obj->log_offset-1)) { vl_vmul_s(&data[offset].pos[2][0], self->d2, (vlf_t) 1.0 / (data[offset].time[0] - data[offset-1].time[0])); }
	else 									{ vl_vmul_s(&data[offset].pos[2][0], self->d2, (vlf_t) 0.5 / (data[offset+1].time[0] - data[offset-1].time[0])); }
	
	if (offset > 0x00
		&& fabs(vl_mdist(&data[offset-1].rot[0][0], &data[offset].rot[0][0])) > 1E-16)
	{
		vl_tnp(self->rnt, &data[offset-1].rot[0][0]);
		vl_mmul_m(self->a, &data[offset].rot[0][0], self->rnt);
		self->tra = vl_mtrace(self->a);
		self->omg = acos(0.5*(self->tra - 1.0));
		vl_tnp(self->at, self->a);
		vl_msub(self->da, self->a, self->at);
		
		vl_mmul_s(self->d1rp, self->da,
				  ( 0.5 / (data[offset].time[0] - data[offset-1].time[0]) )
				  * ( self->omg / vl_sin(self->omg) ));
	}
	else { vl_mzero(self->d1rp); }
	
	if (offset < (obj->log_offset-1)
	&& fabs(vl_mdist(&data[offset].rot[0][0], &data[offset+1].rot[0][0])) > 1E-16)
	{
		vl_tnp(self->rnt, &data[offset].rot[0][0]);
		vl_mmul_m(self->a, &data[offset+1].rot[0][0], self->rnt);
		self->tra = vl_mtrace(self->a);
		self->omg = acos(0.5*(self->tra - 1.0));
		vl_tnp(self->at, self->a);
		vl_msub(self->da, self->a, self->at);
		
		vl_mmul_s(self->d1rn, self->da,
				  ( 0.5 / (data[offset+1].time[0] - data[offset].time[0]) )
				  * ( self->omg / vl_sin(self->omg) ));
	}
	else { vl_mzero(self->d1rn); }
	
	if (offset == 0x00) { vl_minter(&data[offset].rot[1][0], self->d1rp, self->d1rn, 0.0); }
	else if (offset == (obj->log_offset-1)) { vl_minter(&data[offset].rot[1][0], self->d1rp, self->d1rn, 1.0); }
	else { vl_minter(&data[offset].rot[1][0], self->d1rp, self->d1rn, 0.5); }
	
	// Error calculation
	// if first log then set
	if (offset == 0x00)
	{
		vl_vcopy(&obj->pos[0][0], &obj->log_list[0].pos[0][0]);
		vl_vcopy(&obj->pos[1][0], &obj->log_list[0].pos[1][0]);
		vl_vcopy(&obj->pos[2][0], &obj->log_list[0].pos[2][0]);
		
		vl_mcopy(&obj->rot[0][0], &obj->log_list[0].rot[0][0]);
		vl_mcopy(&obj->rot[1][0], &obj->log_list[0].rot[1][0]);
		vl_mcopy(&obj->rot[2][0], &obj->log_list[0].rot[2][0]);
	}
	
	else
	{
		vlf_t tied_acc[3];
		vlf_t tied_grs[9];
		
		vl_mtmul_v(tied_acc, &obj->log_list[offset-1].rot[0][0],
				&obj->log_list[offset-1].pos[2][0]);
		
		vl_mtmul_m(tied_grs, &obj->log_list[offset-1].rot[0][0],
				   &obj->log_list[offset-1].rot[1][0]);
		
		vlf_t inert_acc[3];
		vlf_t inert_grs[9];
		
		vl_mmul_v(inert_acc, &obj->rot[0][0], tied_acc);
		vl_mmul_m(inert_grs, &obj->rot[0][0], tied_grs);
		
		vl_vcopy(inert_acc, &obj->log_list[offset-1].pos[2][0]);
		
		vl_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
		vl_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_list[offset+1].time[1]);
		vl_rnorm(&obj->rot[0][0]);
//
		vl_vcopy(&obj->pos[1][0], &obj->log_list[offset-1].pos[1][0]);
		vl_vcopy(&obj->pos[2][0], &obj->log_list[offset-1].pos[2][0]);
//
		
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_list[offset].time[1]);
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_list[offset].time[1]*obj->log_list[offset].time[1]*0.5);

		vl_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_list[offset].time[1]);
		vl_vcopy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_list[offset].pos_error = fabs(vl_vdist(&obj->log_list[offset].pos[0][0], &obj->pos[0][0]));
	obj->log_list[offset].rot_error = fabs(vl_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0]));
	
//	printf("%s %f \n", obj->desc, fabs(vl_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0])));
//	fflush(stdout);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_proc_euler_init_ (void **data, void *config)
{
	*data = (s_trj_proc_euler*) malloc(sizeof(s_trj_proc_euler));
	
	s_trj_proc_euler *proc = (s_trj_proc_euler*) *data;
	s_trj_proc_euler_init *proc_init = (s_trj_proc_euler_init*) config;
	
	return trj_proc_euler_init(proc, *proc_init);
}

inline uint8_t trj_proc_euler_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_free_ (void **data)
{
	s_trj_proc_euler *proc = (s_trj_proc_euler*) *data;
	
	free(proc);
	
	return 0x00;
}

inline uint8_t trj_proc_euler_update_ (void *data, void *obj, uint32_t offset)
{
	s_trj_proc_euler *proc = (s_trj_proc_euler*) data;
	
	return trj_proc_euler_update(proc, (s_trj_obj*) obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __INS_PROC__ */


