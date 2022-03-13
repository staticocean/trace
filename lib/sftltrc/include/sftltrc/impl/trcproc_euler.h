
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcproc_euler - trace

#ifndef __TRCPROC_EULER__
#define __TRCPROC_EULER__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcproc.h>
#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcproc_euler
{
	s_trcproc  			super;
	
	t_f64 				pos[3][3];
	t_f64 				rot[3][9];
	
	t_f64 				rd1[2][9];
	
	s_vl_rd1 			rd1_data;

    t_f64 				rot_tol;
    t_f64 				rot_var;
    t_f64 				rot_step;

    t_f64 				pos_tol;
    t_f64 				pos_var;
    t_f64 				pos_step;
	
}   s_trcproc_euler;

typedef struct trcproc_euler_attr
{
    s_trcproc_attr      super;

    t_f64 				rot_tol;
    t_f64 				rot_var;
    t_f64 				rot_step;

    t_f64 				pos_tol;
    t_f64 				pos_var;
    t_f64 				pos_step;

}   s_trcproc_euler_attr;

//------------------------------------------------------------------------------

static
t_s8 __trcproc_euler_init__ (void *__proc__, void *__attr__)
{
	s_trcproc_euler *proc = (s_trcproc_euler*) __proc__;
	s_trcproc_euler_attr *attr = (s_trcproc_euler_attr*) __attr__;
	
	// Target position accuracy after integration
	proc->rot_tol  = attr->rot_tol;
	proc->rot_var  = attr->rot_var;
	proc->rot_step = attr->rot_step;

	proc->pos_tol  = attr->pos_tol;
	proc->pos_var  = attr->pos_var;
	proc->pos_step = attr->pos_step;

	return 0x00;
}


//------------------------------------------------------------------------------

static
t_s8 __trcproc_euler_free__ (void *__proc__)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcproc_euler_save__ (void *__proc__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcproc_euler_load__ (void *__proc__, s_trcspl *spl, t_u8 **v_file)
{
	return 0x00;
}

//------------------------------------------------------------------------------

static
void __trcproc_euler_correct_calc__ (s_trcproc_euler *proc, s_trcobj *obj,
									 t_u32 offset, t_f64 *d_grs, t_f64 *d_acc)
{
    t_f64 tied_acc[3];
    t_f64 tied_grs[9];

    t_f64 d_inert_acc[3];
    vld3v_addv(d_inert_acc, &obj->log_ls[offset-1].pos[2][0], d_acc);

    t_f64 d_inert_grs[9];
    t_f64 d_grs_skew[9];

    vld3m_skew(d_grs_skew, d_grs);
    vld3m_addm(d_inert_grs, &obj->log_ls[offset-1].rot[1][0], d_grs_skew);

    vld3m_tmulv(tied_acc, &obj->log_ls[offset-1].rot[0][0], d_inert_acc);
    vld3m_tmulm(tied_grs, &obj->log_ls[offset-1].rot[0][0], d_inert_grs);

    t_f64 inert_acc[3];
    t_f64 inert_grs[9];

    vld3m_mulv(inert_acc, &obj->rot[0][0], tied_acc);
    vld3m_mulm(inert_grs, &obj->rot[0][0], tied_grs);

    vld3m_mulm(inert_grs, inert_grs, &obj->rot[0][0]);
    vld3m_msaddm(&obj->rot[0][0], inert_grs, obj->log_ls[offset+1].time[1], &obj->rot[0][0]);
    vld3m_rnorm(&obj->rot[0][0]);

    vld3v_msaddv(&obj->pos[0][0], &obj->pos[1][0], obj->log_ls[offset].time[1], &obj->pos[0][0]);
    vld3v_msaddv(&obj->pos[0][0], inert_acc, obj->log_ls[offset].time[1]*obj->log_ls[offset].time[1]*0.5, &obj->pos[0][0]);

    vld3v_msaddv(&obj->pos[1][0], inert_acc, obj->log_ls[offset].time[1], &obj->pos[1][0]);

    vld3v_copy(&obj->pos[2][0], inert_acc);
}

//------------------------------------------------------------------------------

static
t_s8 __trcproc_euler_update__ (void *__proc__, s_trcobj *obj, t_s32 offset)
{
    s_trcproc_euler *proc = (s_trcproc_euler*) __proc__;

    // Calcaulate diff estimates
    {
        s_trcobj_data *l0 = &obj->log_ls[offset-1];
        s_trcobj_data *l1 = &obj->log_ls[offset+0];
        s_trcobj_data *l2 = &obj->log_ls[offset+1];

        t_f64 *vel = &l1->pos[1][0];
        t_f64 *acc = &l1->pos[2][0];

        // assume fixed step for now
        t_f64 h = obj->log_ls[0x01].time[1];

        if (offset >= 0x01) { vld3v_copy(&proc->pos[0][0], &l0->pos[0][0]); }
        else  				{ vld3v_inter(&proc->pos[0][0], &l1->pos[0][0], &l2->pos[0][0], -1.0); }

        vld3v_copy(&proc->pos[1][0], &l1->pos[0][0]);

        if (offset <= obj->log_sz-2) { vld3v_copy(&proc->pos[2][0], &l2->pos[0][0]); }
        else  					     { vld3v_inter(&proc->pos[2][0], &l1->pos[0][0], &l0->pos[0][0], -1.0); }

        vld3v_set(vel, 0.0);
        vld3v_msaddv(vel, &proc->pos[0][0], -1.0 / (2.0 * h), vel);
        vld3v_msaddv(vel, &proc->pos[2][0], +1.0 / (2.0 * h), vel);

        vld3v_set(acc, 0.0);
        vld3v_msaddv(acc, &proc->pos[0][0], +1.0 / (h * h), acc);
        vld3v_msaddv(acc, &proc->pos[1][0], -2.0 / (h * h), acc);
        vld3v_msaddv(acc, &proc->pos[2][0], +1.0 / (h * h), acc);

        //handle last and first
        //acc
        if (offset == 0x01) { vld3v_copy(&l0->pos[2][0], &l1->pos[2][0]); }
        if (offset == obj->log_sz-1) { vld3v_copy(&l1->pos[2][0], &l0->pos[2][0]); }


        t_f64 *rot = &l1->rot[1][0];

        if (offset >= 0x01) { vld3m_copy(&proc->rot[0][0], &l0->rot[0][0]); }
        else  				{ vld3m_inter(&proc->rot[0][0], &l1->rot[0][0], &l2->rot[0][0], -1.0); }

        vld3m_copy(&proc->rot[1][0], &l1->rot[0][0]);

        if (offset <= obj->log_sz-2) { vld3m_copy(&proc->rot[2][0], &l2->rot[0][0]); }
        else  							 { vld3m_inter(&proc->rot[2][0], &l1->rot[0][0], &l0->rot[0][0], -1.0); }

    //	vl_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[0][0], &self->rot[1][0]);
    //	vl_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[1][0], &self->rot[2][0]);
    //	vl_msum(rot, &self->rd1[1][0], &self->rd1[0][0]);
    //	vl_mmul_s(rot, rot, 0.5 / h);

        vl3_rd1f(&proc->rd1_data, &proc->rd1[1][0], &proc->rot[0][0], &proc->rot[2][0]);
        vld3m_muls(rot, &proc->rd1[1][0], 0.5 / h);
    }

    // Correct using gradient descent
    {
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
            t_u32 counter = 0x00;
            t_f64 rot_error = 0.0;

            do
            {
                ++counter;

                s_trcobj rL[3] = {*obj, *obj, *obj};
                s_trcobj rM[3] = {*obj, *obj, *obj};
                s_trcobj rR[3] = {*obj, *obj, *obj};

                __trcproc_euler_correct_calc__(proc, &rL[0], offset, vld3v(-proc->rot_var, 0.0, 0.0), vld3v_zeros);
                __trcproc_euler_correct_calc__(proc, &rL[1], offset, vld3v(0.0, -proc->rot_var, 0.0), vld3v_zeros);
                __trcproc_euler_correct_calc__(proc, &rL[2], offset, vld3v(0.0, 0.0, -proc->rot_var), vld3v_zeros);

                __trcproc_euler_correct_calc__(proc, &rM[0], offset, vld3v(0.0, 0.0, 0.0), vld3v_zeros);
                memcpy(&rM[1], &rM[0], sizeof(s_trcobj));
                memcpy(&rM[2], &rM[0], sizeof(s_trcobj));

                __trcproc_euler_correct_calc__(proc, &rR[0], offset, vld3v(+proc->rot_var, 0.0, 0.0), vld3v_zeros);
                __trcproc_euler_correct_calc__(proc, &rR[1], offset, vld3v(0.0, +proc->rot_var, 0.0), vld3v_zeros);
                __trcproc_euler_correct_calc__(proc, &rR[2], offset, vld3v(0.0, 0.0, +proc->rot_var), vld3v_zeros);

                // re = rotation error
                t_f64 reL[3] = {
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rL[0].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rL[1].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rL[2].rot[0][0]),
                };

                t_f64 reM[3] = {
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rM[0].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rM[1].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rM[2].rot[0][0]),
                };

                t_f64 reR[3] = {
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rR[0].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rR[1].rot[0][0]),
                        vld3m_dist(&obj->log_ls[offset].rot[0][0], &rR[2].rot[0][0]),
                };

                rot_error = reM[0];

                t_f64 drL[3];
                t_f64 drR[3];
                t_f64 dr[3];

                vld3v_subv(drL, reM, reL);
                vld3v_subv(drR, reR, reM);

                vld3v_inter(dr, drL, drR, 0.5);

                if (vld3v_norm(dr) > 1E-16)
                {
                    vld3v_muls(dr, dr, rot_error * proc->rot_step / vld3v_norm(dr));
                    t_f64 r_corr[9];
                    vld3m_skew(r_corr, dr);

                    vld3m_subm(&obj->log_ls[offset-1].rot[1][0], &obj->log_ls[offset-1].rot[1][0], r_corr);
                }

            } while (rot_error > proc->rot_tol && counter < 1000);

            counter = 0x00;
            t_f64 pos_error = 0.0;

            do
            {
                counter++;

                s_trcobj pL[3] = {*obj, *obj, *obj};
                s_trcobj pM[3] = {*obj, *obj, *obj};
                s_trcobj pR[3] = {*obj, *obj, *obj};

                __trcproc_euler_correct_calc__(proc, &pL[0], offset, vld3v_zeros, vld3v(-proc->pos_var, 0.0, 0.0));
                __trcproc_euler_correct_calc__(proc, &pL[1], offset, vld3v_zeros, vld3v(0.0, -proc->pos_var, 0.0));
                __trcproc_euler_correct_calc__(proc, &pL[2], offset, vld3v_zeros, vld3v(0.0, 0.0, -proc->pos_var));

                __trcproc_euler_correct_calc__(proc, &pM[0], offset, vld3v_zeros, vld3v(0.0, 0.0, 0.0));
                memcpy(&pM[1], &pM[0], sizeof(s_trcobj));
                memcpy(&pM[2], &pM[0], sizeof(s_trcobj));

                __trcproc_euler_correct_calc__(proc, &pR[0], offset, vld3v_zeros, vld3v(+proc->pos_var, 0.0, 0.0));
                __trcproc_euler_correct_calc__(proc, &pR[1], offset, vld3v_zeros, vld3v(0.0, +proc->pos_var, 0.0));
                __trcproc_euler_correct_calc__(proc, &pR[2], offset, vld3v_zeros, vld3v(0.0, 0.0, +proc->pos_var));

                // re = rotation error
                t_f64 peL[3] = {
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pL[0].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pL[1].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pL[2].pos[0][0]),
                };

                t_f64 peM[3] = {
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pM[0].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pM[1].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pM[2].pos[0][0]),
                };

                t_f64 peR[3] = {
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pR[0].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pR[1].pos[0][0]),
                        vld3v_dist(&obj->log_ls[offset].pos[0][0], &pR[2].pos[0][0]),
                };

                pos_error = peM[0];

                t_f64 dpL[3];
                t_f64 dpR[3];
                t_f64 dp[3];

                vld3v_subv(dpL, peM, peL);
                vld3v_subv(dpR, peR, peM);

                vld3v_inter(dp, dpL, dpR, 0.5);

                if (vld3v_norm(dp) > 1E-16)
                {
                    vld3v_muls(dp, dp, pos_error * proc->pos_step / vld3v_norm(dp));
                    vld3v_subv(&obj->log_ls[offset-1].pos[2][0], &obj->log_ls[offset-1].pos[2][0], dp);
                }

            } while (pos_error > proc->pos_tol && counter < 1000);

            __trcproc_euler_correct_calc__(proc, obj, offset, vld3v_zeros, vld3v_zeros);
        }

        // Calculate and write to pos/rot error L2 norm
        obj->log_ls[offset].pos_error = vld3v_dist(&obj->log_ls[offset].pos[0][0], &obj->pos[0][0]);
        obj->log_ls[offset].rot_error = vld3m_dist(&obj->log_ls[offset].rot[0][0], &obj->rot[0][0]);
    }

    return 0x00;
}

//------------------------------------------------------------------------------

s_trcproc_intf __trcproc_euler__ = {

		.desc 		= "trcproc_euler",

		.data_sz 	= sizeof(s_trcproc_euler),
		.attr_sz 	= sizeof(s_trcproc_euler_attr),
		
		.init 		= __trcproc_euler_init__,
		.free 		= __trcproc_euler_free__,
		.save 		= __trcproc_euler_save__,
		.load 		= __trcproc_euler_load__,
		.update 	= __trcproc_euler_update__,
		
		.gui_attr 	= NULL,
		.gui_edit 	= NULL,
		.gui_view 	= NULL,
};

//------------------------------------------------------------------------------

void trcproc_euler_init (s_trcproc_euler **proc, s_trcproc_euler_attr *attr)
{
	*proc = (s_trcproc_euler*) malloc(sizeof(s_trcproc_euler));
	
	(*proc)->super.intf = &__trcproc_euler__;
	
	trcproc_init(&(*proc)->super, &attr->super);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC_EULER__ */


