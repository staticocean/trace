
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCPROC_FPS__
#define __TRCPROC_FPS___

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftltrc/trcproc.h>

//------------------------------------------------------------------------------
// Five point stnecil
typedef struct trcproc_fps
{
	f64_t pos[5][3];
	f64_t rot[5][9];
	
	f64_t rd1[2][9];
	s_vl_rd1 rd1_data;
	
}   s_trcproc_fps;

typedef struct trcproc_fps_init
{
	u32_t temp;
	
}   s_trcproc_fps_init;

//------------------------------------------------------------------------------

inline u8_t trcproc_fps_init(s_trcproc_fps *self, s_trcproc_fps_init attr)
{
	return 0x00;
}

inline u8_t trcproc_fps_save(s_trcproc_fps *self, s_trcproc_fps_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcproc_fps_load(s_trcproc_fps *self, s_trcproc_fps_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcproc_fps_update(s_trcproc_fps *self, s_trcobj *obj, u32_t offset)
{
	s_trcobj_data *l0 = &obj->log_ls[offset-2];
	s_trcobj_data *l1 = &obj->log_ls[offset-1];
	s_trcobj_data *l2 = &obj->log_ls[offset+0];
	s_trcobj_data *l3 = &obj->log_ls[offset+1];
	s_trcobj_data *l4 = &obj->log_ls[offset+2];
	
	f64_t *vel = &l2->pos[1][0];
	f64_t *acc = &l2->pos[2][0];
	
	// assume fixed step for now
	f64_t h = obj->log_ls[0x01].time[1];
	
	if (offset >= 0x02) { vl3_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
	else  				{ vl3_vinter(&self->pos[0][0], &l2->pos[0][0], &l4->pos[0][0], -1.0); }
	
	if (offset >= 0x01) { vl3_vcopy(&self->pos[1][0], &l1->pos[0][0]); }
	else  				{ vl3_vinter(&self->pos[1][0], &l2->pos[0][0], &l3->pos[0][0], -1.0); }
	
	vl3_vcopy(&self->pos[2][0], &l2->pos[0][0]);
	
	if (offset <= obj->log_sz-2) { vl3_vcopy(&self->pos[3][0], &l3->pos[0][0]); }
	else  							 { vl3_vinter(&self->pos[3][0], &l2->pos[0][0], &l1->pos[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vl3_vcopy(&self->pos[4][0], &l4->pos[0][0]); }
	else  							 { vl3_vinter(&self->pos[4][0], &l2->pos[0][0], &l0->pos[0][0], -1.0); }
	
	vl3_vset(vel, 0.0);
	vl3_vsumm(vel, vel, &self->pos[0][0], +1.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[1][0], -8.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[3][0], +8.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[4][0], -1.0 / (12.0 * h));
	
	vl3_vset(acc, 0.0);
	vl3_vsumm(acc, acc, &self->pos[0][0], - 1.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[1][0], +16.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[2][0], -30.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[3][0], +16.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[4][0], - 1.0 / (12.0 * h * h));
	
	//handle last and first
	//acc
	if (offset == 0x01) { vl3_vcopy(&l1->pos[2][0], &l2->pos[2][0]); }
	if (offset == obj->log_sz-1) { vl3_vcopy(&l2->pos[2][0], &l1->pos[2][0]); }
	
	f64_t *rot = &l2->rot[1][0];
	
	if (offset >= 0x02) { vl3_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[0][0], &l2->rot[0][0], &l4->rot[0][0], -1.0); }
	
	if (offset >= 0x01) { vl3_mcopy(&self->rot[1][0], &l1->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[1][0], &l2->rot[0][0], &l3->rot[0][0], -1.0); }
	
	vl3_mcopy(&self->rot[2][0], &l2->rot[0][0]);
	
	if (offset <= obj->log_sz-2) { vl3_mcopy(&self->rot[3][0], &l3->rot[0][0]); }
	else  							 { vl3_rinter(&self->rot[3][0], &l2->rot[0][0], &l1->rot[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vl3_mcopy(&self->rot[4][0], &l4->rot[0][0]); }
	else  							 { vl3_rinter(&self->rot[4][0], &l2->rot[0][0], &l0->rot[0][0], -1.0); }
	
	// f_x = (1*f[i-2]-8*f[i-1]+0*f[i+0]+8*f[i+1]-1*f[i+2])/(12*1.0*h**1)
	// f_x = (8*(f[i+1]-f[i-1])    -(f[i+2]-f[i-2]))/(12*1.0*h**1)
	vl3_vset(rot, 0.0);
	vl3_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[1][0], &self->rot[3][0]);
	vl3_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[0][0], &self->rot[4][0]);
	vl3_mmul_s(&self->rd1[0][0], &self->rd1[0][0], +8.0 / (12.0 * h));
	vl3_mmul_s(&self->rd1[1][0], &self->rd1[1][0], -1.0 / (12.0 * h));
	vl3_msum(rot, &self->rd1[0][0], &self->rd1[1][0]);
	
	// Error calculation
	// if first log then set
	if (offset == 0x00)
	{
		vl3_vcopy(&obj->pos[0][0], &obj->log_ls[0].pos[0][0]);
		vl3_vcopy(&obj->pos[1][0], &obj->log_ls[0].pos[1][0]);
		vl3_vcopy(&obj->pos[2][0], &obj->log_ls[0].pos[2][0]);
		
		vl3_mcopy(&obj->rot[0][0], &obj->log_ls[0].rot[0][0]);
		vl3_mcopy(&obj->rot[1][0], &obj->log_ls[0].rot[1][0]);
		vl3_mcopy(&obj->rot[2][0], &obj->log_ls[0].rot[2][0]);
	}
	
	else
	{
		f64_t tied_acc[3];
		f64_t tied_grs[9];
		
		vl3_mtmul_v(tied_acc, &obj->log_ls[offset-1].rot[0][0],
				   &obj->log_ls[offset-1].pos[2][0]);
		
		vl3_mtmul_m(tied_grs, &obj->log_ls[offset-1].rot[0][0],
				   &obj->log_ls[offset-1].rot[1][0]);
		
		f64_t inert_acc[3];
		f64_t inert_grs[9];
		
		vl3_mmul_v(inert_acc, &obj->rot[0][0], tied_acc);
		vl3_mmul_m(inert_grs, &obj->rot[0][0], tied_grs);
		
		vl3_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
		vl3_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_ls[offset+1].time[1]);
		vl3_rnorm(&obj->rot[0][0]);
		
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_ls[offset].time[1]);
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_ls[offset].time[1]*obj->log_ls[offset].time[1]*0.5);
		
		vl3_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_ls[offset].time[1]);
		
		vl3_vcopy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_ls[offset].pos_error = vl3_vdist(&obj->log_ls[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_ls[offset].rot_error = vl3_mdist(&obj->log_ls[offset].rot[0][0], &obj->rot[0][0]);

	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcproc_fps_init_ (void **data, void *config)
{
	*data = (s_trcproc_fps*) malloc(sizeof(s_trcproc_fps));
	
	s_trcproc_fps *proc = (s_trcproc_fps*) *data;
	s_trcproc_fps_init *attr = (s_trcproc_fps_init*) config;
	
	return trcproc_fps_init(proc, *attr);
}

inline u8_t trcproc_fps_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcproc_fps_load_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcproc_fps_free_ (void **data)
{
	s_trcproc_fps *proc = (s_trcproc_fps*) *data;
	
	free(proc);
	
	return 0x00;
}

inline u8_t trcproc_fps_update_ (void *data, void *obj, u32_t offset)
{
	s_trcproc_fps *proc = (s_trcproc_fps*) data;
	
	return trcproc_fps_update(proc, (s_trcobj*) obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC_FPS___ */


