
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcproc_fps - Five point stnecil

#ifndef __TRCPROC_FPS__
#define __TRCPROC_FPS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcproc.h>

//------------------------------------------------------------------------------

typedef struct trcproc_fps
{
    s_trcproc           super;

	t_f64               pos[5][3];
	t_f64               rot[5][9];
	
	t_f64               rd1[2][9];
	s_vl_rd1            rd1_data;
	
}   s_trcproc_fps;

typedef struct trcproc_fps_attr
{
	s_trcproc_attr      super;
	
}   s_trcproc_fps_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcproc_fps_init__  (void *__proc__, void *__attr__)
{
    s_trcproc_fps *proc = (s_trcproc_fps*) __proc__;
    s_trcproc_fps_attr *attr = (s_trcproc_fps_attr*) __attr__;

    return 0x00;
}

static
t_s8 __trcproc_fps_free__ (void *__proc__)
{
    return 0x00;
}

static
t_s8 __trcproc_fps_save__ (void *__proc__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcproc_fps_load__ (void *__proc__, s_trcspl *spl, t_u8 **v_file)
{
    return 0x00;
}

static
t_s8 __trcproc_fps_update__ (void *__proc__, s_trcobj *obj, t_s32 offset)
{
    s_trcproc_fps *proc = (s_trcproc_fps*) __proc__;

    s_trcobj_data *l0 = &obj->log_ls[offset-2];
	s_trcobj_data *l1 = &obj->log_ls[offset-1];
	s_trcobj_data *l2 = &obj->log_ls[offset+0];
	s_trcobj_data *l3 = &obj->log_ls[offset+1];
	s_trcobj_data *l4 = &obj->log_ls[offset+2];
	
	t_f64 *vel = &l2->pos[1][0];
	t_f64 *acc = &l2->pos[2][0];
	
	// assume fixed step for now
	t_f64 h = obj->log_ls[0x01].time[1];
	
	if (offset >= 0x02) { vld3v_copy (&proc->pos[0][0], &l0->pos[0][0]); }
	else  				{ vld3v_inter(&proc->pos[0][0], &l2->pos[0][0], &l4->pos[0][0], -1.0); }
	
	if (offset >= 0x01) { vld3v_copy (&proc->pos[1][0], &l1->pos[0][0]); }
	else  				{ vld3v_inter(&proc->pos[1][0], &l2->pos[0][0], &l3->pos[0][0], -1.0); }
	
	vld3v_copy(&proc->pos[2][0], &l2->pos[0][0]);
	
	if (offset <= obj->log_sz-2) { vld3v_copy(&proc->pos[3][0], &l3->pos[0][0]); }
	else  					     { vld3v_inter(&proc->pos[3][0], &l2->pos[0][0], &l1->pos[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vld3v_copy(&proc->pos[4][0], &l4->pos[0][0]); }
	else  					     { vld3v_inter(&proc->pos[4][0], &l2->pos[0][0], &l0->pos[0][0], -1.0); }
	
	vld3v_set(vel, 0.0);
	vld3v_msaddv(vel, &proc->pos[0][0], +1.0 / (12.0 * h), vel);
	vld3v_msaddv(vel, &proc->pos[1][0], -8.0 / (12.0 * h), vel);
	vld3v_msaddv(vel, &proc->pos[3][0], +8.0 / (12.0 * h), vel);
	vld3v_msaddv(vel, &proc->pos[4][0], -1.0 / (12.0 * h), vel);
	
	vld3v_set(acc, 0.0);
	vld3v_msaddv(acc, &proc->pos[0][0], - 1.0 / (12.0 * h * h), acc);
	vld3v_msaddv(acc, &proc->pos[1][0], +16.0 / (12.0 * h * h), acc);
	vld3v_msaddv(acc, &proc->pos[2][0], -30.0 / (12.0 * h * h), acc);
	vld3v_msaddv(acc, &proc->pos[3][0], +16.0 / (12.0 * h * h), acc);
	vld3v_msaddv(acc, &proc->pos[4][0], - 1.0 / (12.0 * h * h), acc);
	
	//handle last and first
	//acc
	if (offset == 0x01) { vld3v_copy(&l1->pos[2][0], &l2->pos[2][0]); }
	if (offset == obj->log_sz-1) { vld3v_copy(&l2->pos[2][0], &l1->pos[2][0]); }
	
	t_f64 *rot = &l2->rot[1][0];
	
	if (offset >= 0x02) { vld3m_copy (&proc->rot[0][0], &l0->rot[0][0]); }
	else  				{ vld3m_inter(&proc->rot[0][0], &l2->rot[0][0], &l4->rot[0][0], -1.0); }
	
	if (offset >= 0x01) { vld3m_copy (&proc->rot[1][0], &l1->rot[0][0]); }
	else  				{ vld3m_inter(&proc->rot[1][0], &l2->rot[0][0], &l3->rot[0][0], -1.0); }
	
	vld3m_copy(&proc->rot[2][0], &l2->rot[0][0]);
	
	if (offset <= obj->log_sz-2) { vld3m_copy(&proc->rot[3][0], &l3->rot[0][0]); }
	else  						 { vld3m_inter(&proc->rot[3][0], &l2->rot[0][0], &l1->rot[0][0], -1.0); }
	
	if (offset <= obj->log_sz-3) { vld3m_copy(&proc->rot[4][0], &l4->rot[0][0]); }
	else  					     { vld3m_inter(&proc->rot[4][0], &l2->rot[0][0], &l0->rot[0][0], -1.0); }
	
	// f_x = (1*f[i-2]-8*f[i-1]+0*f[i+0]+8*f[i+1]-1*f[i+2])/(12*1.0*h**1)
	// f_x = (8*(f[i+1]-f[i-1])    -(f[i+2]-f[i-2]))/(12*1.0*h**1)
	vld3v_set(rot, 0.0);
	vl3_rd1f(&proc->rd1_data, &proc->rd1[0][0], &proc->rot[1][0], &proc->rot[3][0]);
	vl3_rd1f(&proc->rd1_data, &proc->rd1[1][0], &proc->rot[0][0], &proc->rot[4][0]);
	vld3m_muls(&proc->rd1[0][0], &proc->rd1[0][0], +8.0 / (12.0 * h));
	vld3m_muls(&proc->rd1[1][0], &proc->rd1[1][0], -1.0 / (12.0 * h));
	vld3m_addm(rot, &proc->rd1[0][0], &proc->rd1[1][0]);
	
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
		vld3m_msaddm(&obj->rot[0][0], inert_grs, obj->log_ls[offset+1].time[1], &obj->rot[0][0]);
		vld3m_rnorm(&obj->rot[0][0]);
		
		vld3v_msaddv(&obj->pos[0][0], &obj->pos[1][0], obj->log_ls[offset].time[1], &obj->pos[0][0]);
		vld3v_msaddv(&obj->pos[0][0], &obj->pos[2][0], obj->log_ls[offset].time[1]*obj->log_ls[offset].time[1]*0.5, &obj->pos[0][0]);

        vld3v_msaddv(&obj->pos[1][0], inert_acc, obj->log_ls[offset].time[1], &obj->pos[1][0]);
		
		vld3v_copy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_ls[offset].pos_error = vld3v_dist(&obj->log_ls[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_ls[offset].rot_error = vld3m_dist(&obj->log_ls[offset].rot[0][0], &obj->rot[0][0]);

	return 0x00;
}

//------------------------------------------------------------------------------

s_trcproc_intf __trcproc_fps__ = {

        .desc 		= "trcproc_fps",

        .data_sz 	= sizeof(s_trcproc_fps),
        .attr_sz 	= sizeof(s_trcproc_fps_attr),

        .init 		= __trcproc_fps_init__,
        .free 		= __trcproc_fps_free__,
        .save 		= __trcproc_fps_save__,
        .load 		= __trcproc_fps_load__,
        .update 	= __trcproc_fps_update__,

        .gui_attr 	= NULL,
        .gui_edit 	= NULL,
        .gui_view 	= NULL,
};

//------------------------------------------------------------------------------

void trcproc_fps_init (s_trcproc_fps **proc, s_trcproc_fps_attr *attr)
{
    *proc = (s_trcproc_fps*) malloc(sizeof(s_trcproc_fps));

    (*proc)->super.intf = &__trcproc_fps__;

    trcproc_init(&(*proc)->super, &attr->super);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC_FPS__ */


