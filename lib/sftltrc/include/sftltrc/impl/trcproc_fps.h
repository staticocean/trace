
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcproc_fps - trace

#ifndef __TRCPROC_FPS__
#define __TRCPROC_FPS___

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcproc.h>
#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------
// Five point stnecil
typedef struct trcproc_fps
{
	t_f64 pos[5][3];
	t_f64 rot[5][9];
	
	t_f64 rd1[2][9];
	s_vl_rd1 rd1_data;
	
}   s_trcproc_fps;

typedef struct trcproc_fps_init
{
	t_u32 temp;
	
}   s_trcproc_fps_init;

//------------------------------------------------------------------------------

inline t_u8 trcproc_fps_init(s_trcproc_fps *self, s_trcproc_fps_init attr)
{
	return 0x00;
}

inline t_u8 trcproc_fps_save(s_trcproc_fps *self, s_trcproc_fps_init *attr, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcproc_fps_load(s_trcproc_fps *self, s_trcproc_fps_init *attr, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcproc_fps_update(s_trcproc_fps *self, s_trcobj *obj, t_u32 offset)
{
	s_trcobj_data *l0 = &obj->log_ls[offset-2];
	s_trcobj_data *l1 = &obj->log_ls[offset-1];
	s_trcobj_data *l2 = &obj->log_ls[offset+0];
	s_trcobj_data *l3 = &obj->log_ls[offset+1];
	s_trcobj_data *l4 = &obj->log_ls[offset+2];
	
	t_f64 *vel = &l2->pos[1][0];
	t_f64 *acc = &l2->pos[2][0];
	
	// assume fixed step for now
	t_f64 h = obj->log_ls[0x01].time[1];
	
	if (offset >= 0x02) { vld3v_copy(&self->pos[0][0], &l0->pos[0][0]); }
	else  				{ vld3v_inter(&self->pos[0][0], &l2->pos[0][0], &l4->pos[0][0], -1.0); }
	
	if (offset >= 0x01) { vld3v_copy(&self->pos[1][0], &l1->pos[0][0]); }
	else  				{ vld3v_inter(&self->pos[1][0], &l2->pos[0][0], &l3->pos[0][0], -1.0); }
	
	vld3v_copy(&self->pos[2][0], &l2->pos[0][0]);
	
	if (offset <= obj->log_sz-2) { vld3v_copy(&self->pos[3][0], &l3->pos[0][0]); }
	else  							 { vld3v_inter(&self->pos[3][0], &l2->pos[0][0], &l1->pos[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vld3v_copy(&self->pos[4][0], &l4->pos[0][0]); }
	else  							 { vld3v_inter(&self->pos[4][0], &l2->pos[0][0], &l0->pos[0][0], -1.0); }
	
	vld3v_set(vel, 0.0);
	vl3_vsumm(vel, vel, &self->pos[0][0], +1.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[1][0], -8.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[3][0], +8.0 / (12.0 * h));
	vl3_vsumm(vel, vel, &self->pos[4][0], -1.0 / (12.0 * h));
	
	vld3v_set(acc, 0.0);
	vl3_vsumm(acc, acc, &self->pos[0][0], - 1.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[1][0], +16.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[2][0], -30.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[3][0], +16.0 / (12.0 * h * h));
	vl3_vsumm(acc, acc, &self->pos[4][0], - 1.0 / (12.0 * h * h));
	
	//handle last and first
	//acc
	if (offset == 0x01) { vld3v_copy(&l1->pos[2][0], &l2->pos[2][0]); }
	if (offset == obj->log_sz-1) { vld3v_copy(&l2->pos[2][0], &l1->pos[2][0]); }
	
	t_f64 *rot = &l2->rot[1][0];
	
	if (offset >= 0x02) { vld3m_copy(&self->rot[0][0], &l0->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[0][0], &l2->rot[0][0], &l4->rot[0][0], -1.0); }
	
	if (offset >= 0x01) { vld3m_copy(&self->rot[1][0], &l1->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[1][0], &l2->rot[0][0], &l3->rot[0][0], -1.0); }
	
	vld3m_copy(&self->rot[2][0], &l2->rot[0][0]);
	
	if (offset <= obj->log_sz-2) { vld3m_copy(&self->rot[3][0], &l3->rot[0][0]); }
	else  							 { vl3_rinter(&self->rot[3][0], &l2->rot[0][0], &l1->rot[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vld3m_copy(&self->rot[4][0], &l4->rot[0][0]); }
	else  							 { vl3_rinter(&self->rot[4][0], &l2->rot[0][0], &l0->rot[0][0], -1.0); }
	
	// f_x = (1*f[i-2]-8*f[i-1]+0*f[i+0]+8*f[i+1]-1*f[i+2])/(12*1.0*h**1)
	// f_x = (8*(f[i+1]-f[i-1])    -(f[i+2]-f[i-2]))/(12*1.0*h**1)
	vld3v_set(rot, 0.0);
	vl3_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[1][0], &self->rot[3][0]);
	vl3_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[0][0], &self->rot[4][0]);
	vld3m_muls(&self->rd1[0][0], &self->rd1[0][0], +8.0 / (12.0 * h));
	vld3m_muls(&self->rd1[1][0], &self->rd1[1][0], -1.0 / (12.0 * h));
	vld3m_addm(rot, &self->rd1[0][0], &self->rd1[1][0]);
	
	// Error calculation
	// if first log then set
	if (offset == 0x00)
	{
		vld3v_copy(&obj->pos[0][0], &obj->log_ls[0].pos[0][0]);
		vld3v_copy(&obj->pos[1][0], &obj->log_ls[0].pos[1][0]);
		vld3v_copy(&obj->pos[2][0], &obj->log_ls[0].pos[2][0]);
		
		vld3m_copy(&obj->rot[0][0], &obj->log_ls[0].rot[0][0]);
		vld3m_copy(&obj->rot[1][0], &obj->log_ls[0].rot[1][0]);
		vld3m_copy(&obj->rot[2][0], &obj->log_ls[0].rot[2][0]);
	}
	
	else
	{
		t_f64 tied_acc[3];
		t_f64 tied_grs[9];
		
		vld3m_tmulv(tied_acc, &obj->log_ls[offset-1].rot[0][0],
				   &obj->log_ls[offset-1].pos[2][0]);
		
		vld3m_tmulm(tied_grs, &obj->log_ls[offset-1].rot[0][0],
				   &obj->log_ls[offset-1].rot[1][0]);
		
		t_f64 inert_acc[3];
		t_f64 inert_grs[9];
		
		vld3m_mulv(inert_acc, &obj->rot[0][0], tied_acc);
		vld3m_mulm(inert_grs, &obj->rot[0][0], tied_grs);
		
		vld3m_mulm(inert_grs, inert_grs, &obj->rot[0][0]);
		vld3m_addmm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_ls[offset+1].time[1]);
		vld3m_rnorm(&obj->rot[0][0]);
		
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_ls[offset].time[1]);
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_ls[offset].time[1]*obj->log_ls[offset].time[1]*0.5);
		
		vl3_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_ls[offset].time[1]);
		
		vld3v_copy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_ls[offset].pos_error = vld3v_dist(&obj->log_ls[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_ls[offset].rot_error = vl3_mdist(&obj->log_ls[offset].rot[0][0], &obj->rot[0][0]);

	return 0x00;
}

//------------------------------------------------------------------------------

inline t_u8 trcproc_fps_init_ (void **data, void *config)
{
	*data = (s_trcproc_fps*) malloc(sizeof(s_trcproc_fps));
	
	s_trcproc_fps *proc = (s_trcproc_fps*) *data;
	s_trcproc_fps_init *attr = (s_trcproc_fps_init*) config;
	
	return trcproc_fps_init(proc, *attr);
}

inline t_u8 trcproc_fps_save_(void *data, void *config, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcproc_fps_load_(void *data, void *config, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trcproc_fps_free_ (void **data)
{
	s_trcproc_fps *proc = (s_trcproc_fps*) *data;
	
	free(proc);
	
	return 0x00;
}

inline t_u8 trcproc_fps_update_ (void *data, void *obj, t_u32 offset)
{
	s_trcproc_fps *proc = (s_trcproc_fps*) data;
	
	return trcproc_fps_update(proc, (s_trcobj*) obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC_FPS___ */


