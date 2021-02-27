
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_PROC__
#define __TRJ_PROC__

//----------------------------------------------------------------

#include <softael_lib/vl.h>

#include "trj_api.h"

//----------------------------------------------------------------

typedef struct trj_proc_euler
{
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t rd1[2][9];
	
	s_vl_rd1 rd1_data;
	
}   s_trj_proc_euler;

typedef struct trj_proc_euler_init
{
	uint32_t temp;

}   s_trj_proc_euler_init;

//----------------------------------------------------------------

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
	s_trj_obj_data *l0 = &obj->log_list[offset-1];
	s_trj_obj_data *l1 = &obj->log_list[offset+0];
	s_trj_obj_data *l2 = &obj->log_list[offset+1];
	
	vlf_t *vel = &l1->pos[1][0];
	vlf_t *acc = &l1->pos[2][0];
	
	// assume fixed step for now
	vlf_t h = obj->log_list[0x01].time[1];
	
	if (offset >= 0x01) { vl_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
	else  				{ vl_vinter(&self->pos[0][0], &l1->pos[0][0], &l2->pos[0][0], -1.0); }
	
	vl_vcopy(&self->pos[1][0], &l1->pos[0][0]);
	
	if (offset <= obj->log_offset-2) { vl_vcopy(&self->pos[2][0], &l2->pos[0][0]); }
	else  							 { vl_vinter(&self->pos[2][0], &l1->pos[0][0], &l0->pos[0][0], -1.0); }
	
	vl_vset(vel, 0.0);
	vl_vsumm(vel, vel, &self->pos[0][0], -1.0 / (2.0 * h));
	vl_vsumm(vel, vel, &self->pos[2][0], +1.0 / (2.0 * h));
	
	vl_vset(acc, 0.0);
	vl_vsumm(acc, acc, &self->pos[0][0], +1.0 / (h * h));
	vl_vsumm(acc, acc, &self->pos[1][0], -2.0 / (h * h));
	vl_vsumm(acc, acc, &self->pos[2][0], +1.0 / (h * h));
	
	//handle last and first
	//acc
	if (offset == 0x01) { vl_vcopy(&l0->pos[2][0], &l1->pos[2][0]); }
	if (offset == obj->log_offset-1) { vl_vcopy(&l1->pos[2][0], &l0->pos[2][0]); }
	
	
	vlf_t *rot = &l1->rot[1][0];
	
	if (offset >= 0x01) { vl_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
	else  				{ vl_rinter(&self->rot[0][0], &l1->rot[0][0], &l2->rot[0][0], -1.0); }
	
	vl_mcopy(&self->rot[1][0], &l1->rot[0][0]);
	
	if (offset <= obj->log_offset-2) { vl_mcopy(&self->rot[2][0], &l2->rot[0][0]); }
	else  							 { vl_rinter(&self->rot[2][0], &l1->rot[0][0], &l0->rot[0][0], -1.0); }
	
//	vl_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[0][0], &self->rot[1][0]);
//	vl_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[1][0], &self->rot[2][0]);
//	vl_msum(rot, &self->rd1[1][0], &self->rd1[0][0]);
//	vl_mmul_s(rot, rot, 0.5 / h);
	
	vl_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[0][0], &self->rot[2][0]);
	vl_mmul_s(rot, &self->rd1[1][0], 0.5 / h);
	
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
		
		vl_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
		vl_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_list[offset+1].time[1]);
		vl_rnorm(&obj->rot[0][0]);
		
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_list[offset].time[1]);
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_list[offset].time[1]*obj->log_list[offset].time[1]*0.5);
		
		vl_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_list[offset].time[1]);
		
		vl_vcopy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_list[offset].pos_error = vl_vdist(&obj->log_list[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_list[offset].rot_error = vl_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0]);
	
	return 0x00;
}

//----------------------------------------------------------------

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

//----------------------------------------------------------------
// Five point stnecil
typedef struct trj_proc_fps
{
	vlf_t pos[5][3];
	vlf_t rot[5][9];
	
	vlf_t rd1[2][9];
	s_vl_rd1 rd1_data;
	
}   s_trj_proc_fps;

typedef struct trj_proc_fps_init
{
	uint32_t temp;
	
}   s_trj_proc_fps_init;

//----------------------------------------------------------------

inline uint8_t trj_proc_fps_init(s_trj_proc_fps *self, s_trj_proc_fps_init attr)
{
	return 0x00;
}

inline uint8_t trj_proc_fps_save(s_trj_proc_fps *self, s_trj_proc_fps_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_fps_load(s_trj_proc_fps *self, s_trj_proc_fps_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_fps_update(s_trj_proc_fps *self, s_trj_obj *obj, uint32_t offset)
{
	s_trj_obj_data *l0 = &obj->log_list[offset-2];
	s_trj_obj_data *l1 = &obj->log_list[offset-1];
	s_trj_obj_data *l2 = &obj->log_list[offset+0];
	s_trj_obj_data *l3 = &obj->log_list[offset+1];
	s_trj_obj_data *l4 = &obj->log_list[offset+2];
	
	vlf_t *vel = &l2->pos[1][0];
	vlf_t *acc = &l2->pos[2][0];
	
	// assume fixed step for now
	vlf_t h = obj->log_list[0x01].time[1];
	
	if (offset >= 0x02) { vl_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
	else  				{ vl_vinter(&self->pos[0][0], &l2->pos[0][0], &l4->pos[0][0], -1.0); }
	
	if (offset >= 0x01) { vl_vcopy(&self->pos[1][0], &l1->pos[0][0]); }
	else  				{ vl_vinter(&self->pos[1][0], &l2->pos[0][0], &l3->pos[0][0], -1.0); }
	
	vl_vcopy(&self->pos[2][0], &l2->pos[0][0]);
	
	if (offset <= obj->log_offset-2) { vl_vcopy(&self->pos[3][0], &l3->pos[0][0]); }
	else  							 { vl_vinter(&self->pos[3][0], &l2->pos[0][0], &l1->pos[0][0], -1.0); }
	
	if (offset <= obj->log_offset-3) { vl_vcopy(&self->pos[4][0], &l4->pos[0][0]); }
	else  							 { vl_vinter(&self->pos[4][0], &l2->pos[0][0], &l0->pos[0][0], -1.0); }
	
	vl_vset(vel, 0.0);
	vl_vsumm(vel, vel, &self->pos[0][0], +1.0 / (12.0 * h));
	vl_vsumm(vel, vel, &self->pos[1][0], -8.0 / (12.0 * h));
	vl_vsumm(vel, vel, &self->pos[3][0], +8.0 / (12.0 * h));
	vl_vsumm(vel, vel, &self->pos[4][0], -1.0 / (12.0 * h));
	
	vl_vset(acc, 0.0);
	vl_vsumm(acc, acc, &self->pos[0][0], - 1.0 / (12.0 * h * h));
	vl_vsumm(acc, acc, &self->pos[1][0], +16.0 / (12.0 * h * h));
	vl_vsumm(acc, acc, &self->pos[2][0], -30.0 / (12.0 * h * h));
	vl_vsumm(acc, acc, &self->pos[3][0], +16.0 / (12.0 * h * h));
	vl_vsumm(acc, acc, &self->pos[4][0], - 1.0 / (12.0 * h * h));
	
	//handle last and first
	//acc
	if (offset == 0x01) { vl_vcopy(&l1->pos[2][0], &l2->pos[2][0]); }
	if (offset == obj->log_offset-1) { vl_vcopy(&l2->pos[2][0], &l1->pos[2][0]); }
	
	vlf_t *rot = &l2->rot[1][0];
	
	if (offset >= 0x02) { vl_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
	else  				{ vl_rinter(&self->rot[0][0], &l2->rot[0][0], &l4->rot[0][0], -1.0); }
	
	if (offset >= 0x01) { vl_mcopy(&self->rot[1][0], &l1->rot[0][0]); }
	else  				{ vl_rinter(&self->rot[1][0], &l2->rot[0][0], &l3->rot[0][0], -1.0); }
	
	vl_mcopy(&self->rot[2][0], &l2->rot[0][0]);
	
	if (offset <= obj->log_offset-2) { vl_mcopy(&self->rot[3][0], &l3->rot[0][0]); }
	else  							 { vl_rinter(&self->rot[3][0], &l2->rot[0][0], &l1->rot[0][0], -1.0); }
	
	if (offset <= obj->log_offset-3) { vl_mcopy(&self->rot[4][0], &l4->rot[0][0]); }
	else  							 { vl_rinter(&self->rot[4][0], &l2->rot[0][0], &l0->rot[0][0], -1.0); }
	
	// f_x = (1*f[i-2]-8*f[i-1]+0*f[i+0]+8*f[i+1]-1*f[i+2])/(12*1.0*h**1)
	// f_x = (8*(f[i+1]-f[i-1])    -(f[i+2]-f[i-2]))/(12*1.0*h**1)
	vl_vset(rot, 0.0);
	vl_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[1][0], &self->rot[3][0]);
	vl_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[0][0], &self->rot[4][0]);
	vl_mmul_s(&self->rd1[0][0], &self->rd1[0][0], +8.0 / (12.0 * h));
	vl_mmul_s(&self->rd1[1][0], &self->rd1[1][0], -1.0 / (12.0 * h));
	vl_msum(rot, &self->rd1[0][0], &self->rd1[1][0]);
	
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
		
		vl_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
		vl_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_list[offset+1].time[1]);
		vl_rnorm(&obj->rot[0][0]);
		
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_list[offset].time[1]);
		vl_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_list[offset].time[1]*obj->log_list[offset].time[1]*0.5);
		
		vl_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_list[offset].time[1]);
		
		vl_vcopy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_list[offset].pos_error = vl_vdist(&obj->log_list[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_list[offset].rot_error = vl_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0]);

	return 0x00;
}

//----------------------------------------------------------------

inline uint8_t trj_proc_fps_init_ (void **data, void *config)
{
	*data = (s_trj_proc_fps*) malloc(sizeof(s_trj_proc_fps));
	
	s_trj_proc_fps *proc = (s_trj_proc_fps*) *data;
	s_trj_proc_fps_init *proc_init = (s_trj_proc_fps_init*) config;
	
	return trj_proc_fps_init(proc, *proc_init);
}

inline uint8_t trj_proc_fps_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_fps_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_fps_free_ (void **data)
{
	s_trj_proc_fps *proc = (s_trj_proc_fps*) *data;
	
	free(proc);
	
	return 0x00;
}

inline uint8_t trj_proc_fps_update_ (void *data, void *obj, uint32_t offset)
{
	s_trj_proc_fps *proc = (s_trj_proc_fps*) data;
	
	return trj_proc_fps_update(proc, (s_trj_obj*) obj, offset);
}

//----------------------------------------------------------------

#endif /* __INS_PROC__ */


