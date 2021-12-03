
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcproc_euler - trace

#ifndef __TRCPROC_EULER__
#define __TRCPROC_EULER__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftltrc/trcproc.h>

//------------------------------------------------------------------------------

typedef struct trcproc_euler
{
	f64_t pos[3][3];
	f64_t rot[3][9];
	
	f64_t rd1[2][9];
	
	s_vl_rd1 rd1_data;

    f64_t rot_tol;
    f64_t rot_var;
    f64_t rot_step;

    f64_t pos_tol;
    f64_t pos_var;
    f64_t pos_step;
	
}   s_trcproc_euler;

typedef struct trcproc_euler_attr
{
    f64_t rot_tol;
    f64_t rot_var;
    f64_t rot_step;

    f64_t pos_tol;
    f64_t pos_var;
    f64_t pos_step;

}   s_trcproc_euler_attr;

//------------------------------------------------------------------------------

inline u8_t trcproc_euler_init(s_trcproc_euler *self, s_trcproc_euler_attr attr)
{
    // Target position accuracy after integration
    self->rot_tol  = attr.rot_tol;
    self->rot_var  = attr.rot_var;
    self->rot_step = attr.rot_step;

    self->pos_tol  = attr.pos_tol;
    self->pos_var  = attr.pos_var;
    self->pos_step = attr.pos_step;

	return 0x00;
}

inline u8_t trcproc_euler_save(s_trcproc_euler *self, s_trcproc_euler_attr *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcproc_euler_load(s_trcproc_euler *self, s_trcproc_euler_attr *attr, u8_t **v_file)
{
	return 0x00;
}

inline void __trcproc_euler_correct_calc__(s_trcproc_euler *self, s_trcobj *obj, u32_t offset, f64_t *d_grs, f64_t *d_acc)
{
    f64_t tied_acc[3];
    f64_t tied_grs[9];

    f64_t d_inert_acc[3];
    vl3_vsum(d_inert_acc, &obj->log_ls[offset-1].pos[2][0], d_acc);

    f64_t d_inert_grs[9];
    f64_t d_grs_skew[9];

    vl3_skew(d_grs_skew, d_grs);
    vl3_msum(d_inert_grs, &obj->log_ls[offset-1].rot[1][0], d_grs_skew);

    vl3_mtmul_v(tied_acc, &obj->log_ls[offset-1].rot[0][0], d_inert_acc);
    vl3_mtmul_m(tied_grs, &obj->log_ls[offset-1].rot[0][0], d_inert_grs);

    f64_t inert_acc[3];
    f64_t inert_grs[9];

    vl3_mmul_v(inert_acc, &obj->rot[0][0], tied_acc);
    vl3_mmul_m(inert_grs, &obj->rot[0][0], tied_grs);

    vl3_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
    vl3_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_ls[offset+1].time[1]);
    vl3_rnorm(&obj->rot[0][0]);

    vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_ls[offset].time[1]);
    vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], inert_acc, obj->log_ls[offset].time[1]*obj->log_ls[offset].time[1]*0.5);

    vl3_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_ls[offset].time[1]);

    vl3_vcopy(&obj->pos[2][0], inert_acc);

    return;
}

inline u8_t trcproc_euler_update(s_trcproc_euler *self, s_trcobj *obj, u32_t offset)
{
    // Calcaulate diff estimates
    {
        s_trcobj_data *l0 = &obj->log_ls[offset-1];
        s_trcobj_data *l1 = &obj->log_ls[offset+0];
        s_trcobj_data *l2 = &obj->log_ls[offset+1];

        f64_t *vel = &l1->pos[1][0];
        f64_t *acc = &l1->pos[2][0];

        // assume fixed step for now
        f64_t h = obj->log_ls[0x01].time[1];

        if (offset >= 0x01) { vl3_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
        else  				{ vl3_vinter(&self->pos[0][0], &l1->pos[0][0], &l2->pos[0][0], -1.0); }

        vl3_vcopy(&self->pos[1][0], &l1->pos[0][0]);

        if (offset <= obj->log_sz-2) { vl3_vcopy(&self->pos[2][0], &l2->pos[0][0]); }
        else  							 { vl3_vinter(&self->pos[2][0], &l1->pos[0][0], &l0->pos[0][0], -1.0); }

        vl3_vset(vel, 0.0);
        vl3_vsumm(vel, vel, &self->pos[0][0], -1.0 / (2.0 * h));
        vl3_vsumm(vel, vel, &self->pos[2][0], +1.0 / (2.0 * h));

        vl3_vset(acc, 0.0);
        vl3_vsumm(acc, acc, &self->pos[0][0], +1.0 / (h * h));
        vl3_vsumm(acc, acc, &self->pos[1][0], -2.0 / (h * h));
        vl3_vsumm(acc, acc, &self->pos[2][0], +1.0 / (h * h));

        //handle last and first
        //acc
        if (offset == 0x01) { vl3_vcopy(&l0->pos[2][0], &l1->pos[2][0]); }
        if (offset == obj->log_sz-1) { vl3_vcopy(&l1->pos[2][0], &l0->pos[2][0]); }


        f64_t *rot = &l1->rot[1][0];

        if (offset >= 0x01) { vl3_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
        else  				{ vl3_rinter(&self->rot[0][0], &l1->rot[0][0], &l2->rot[0][0], -1.0); }

        vl3_mcopy(&self->rot[1][0], &l1->rot[0][0]);

        if (offset <= obj->log_sz-2) { vl3_mcopy(&self->rot[2][0], &l2->rot[0][0]); }
        else  							 { vl3_rinter(&self->rot[2][0], &l1->rot[0][0], &l0->rot[0][0], -1.0); }

    //	vl_rd1f(&self->rd1_data, &self->rd1[0][0], &self->rot[0][0], &self->rot[1][0]);
    //	vl_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[1][0], &self->rot[2][0]);
    //	vl_msum(rot, &self->rd1[1][0], &self->rd1[0][0]);
    //	vl_mmul_s(rot, rot, 0.5 / h);

        vl3_rd1f(&self->rd1_data, &self->rd1[1][0], &self->rot[0][0], &self->rot[2][0]);
        vl3_mmul_s(rot, &self->rd1[1][0], 0.5 / h);
    }

    // Correct using gradient descent
    {
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
            u32_t counter = 0x00;
            f64_t rot_error = 0.0;

            do
            {
                ++counter;

                s_trcobj rL[3] = {*obj, *obj, *obj};
                s_trcobj rM[3] = {*obj, *obj, *obj};
                s_trcobj rR[3] = {*obj, *obj, *obj};

                __trcproc_euler_correct_calc__(self, &rL[0], offset, vl_vec(-self->rot_var, 0.0, 0.0), vl_vec_zeros);
                __trcproc_euler_correct_calc__(self, &rL[1], offset, vl_vec(0.0, -self->rot_var, 0.0), vl_vec_zeros);
                __trcproc_euler_correct_calc__(self, &rL[2], offset, vl_vec(0.0, 0.0, -self->rot_var), vl_vec_zeros);

                __trcproc_euler_correct_calc__(self, &rM[0], offset, vl_vec(0.0, 0.0, 0.0), vl_vec_zeros);
                memcpy(&rM[1], &rM[0], sizeof(s_trcobj));
                memcpy(&rM[2], &rM[0], sizeof(s_trcobj));

                __trcproc_euler_correct_calc__(self, &rR[0], offset, vl_vec(+self->rot_var, 0.0, 0.0), vl_vec_zeros);
                __trcproc_euler_correct_calc__(self, &rR[1], offset, vl_vec(0.0, +self->rot_var, 0.0), vl_vec_zeros);
                __trcproc_euler_correct_calc__(self, &rR[2], offset, vl_vec(0.0, 0.0, +self->rot_var), vl_vec_zeros);

                // re = rotation error
                f64_t reL[3] = {
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rL[0].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rL[1].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rL[2].rot[0][0]),
                };

                f64_t reM[3] = {
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rM[0].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rM[1].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rM[2].rot[0][0]),
                };

                f64_t reR[3] = {
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rR[0].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rR[1].rot[0][0]),
                        vl3_mdist(&obj->log_ls[offset].rot[0][0], &rR[2].rot[0][0]),
                };

                rot_error = reM[0];

                f64_t drL[3];
                f64_t drR[3];
                f64_t dr[3];

                vl3_vsub(drL, reM, reL);
                vl3_vsub(drR, reR, reM);

                vl3_vinter(dr, drL, drR, 0.5);

                if (vl3_vnorm(dr) > 1E-16)
                {
                    vl3_vmul_s(dr, dr, rot_error * self->rot_step / vl3_vnorm(dr));
                    f64_t r_corr[9];
                    vl3_skew(r_corr, dr);

                    vl3_msub(&obj->log_ls[offset-1].rot[1][0], &obj->log_ls[offset-1].rot[1][0], r_corr);
                }

            } while (rot_error > self->rot_tol && counter < 1000);

            counter = 0x00;
            f64_t pos_error = 0.0;

            do
            {
                ++counter;

                s_trcobj pL[3] = {*obj, *obj, *obj};
                s_trcobj pM[3] = {*obj, *obj, *obj};
                s_trcobj pR[3] = {*obj, *obj, *obj};

                __trcproc_euler_correct_calc__(self, &pL[0], offset, vl_vec_zeros, vl_vec(-self->pos_var, 0.0, 0.0));
                __trcproc_euler_correct_calc__(self, &pL[1], offset, vl_vec_zeros, vl_vec(0.0, -self->pos_var, 0.0));
                __trcproc_euler_correct_calc__(self, &pL[2], offset, vl_vec_zeros, vl_vec(0.0, 0.0, -self->pos_var));

                __trcproc_euler_correct_calc__(self, &pM[0], offset, vl_vec_zeros, vl_vec(0.0, 0.0, 0.0));
                memcpy(&pM[1], &pM[0], sizeof(s_trcobj));
                memcpy(&pM[2], &pM[0], sizeof(s_trcobj));

                __trcproc_euler_correct_calc__(self, &pR[0], offset, vl_vec_zeros, vl_vec(+self->pos_var, 0.0, 0.0));
                __trcproc_euler_correct_calc__(self, &pR[1], offset, vl_vec_zeros, vl_vec(0.0, +self->pos_var, 0.0));
                __trcproc_euler_correct_calc__(self, &pR[2], offset, vl_vec_zeros, vl_vec(0.0, 0.0, +self->pos_var));

                // re = rotation error
                f64_t peL[3] = {
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pL[0].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pL[1].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pL[2].pos[0][0]),
                };

                f64_t peM[3] = {
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pM[0].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pM[1].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pM[2].pos[0][0]),
                };

                f64_t peR[3] = {
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pR[0].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pR[1].pos[0][0]),
                        vl3_vdist(&obj->log_ls[offset].pos[0][0], &pR[2].pos[0][0]),
                };

                pos_error = peM[0];

                f64_t dpL[3];
                f64_t dpR[3];
                f64_t dp[3];

                vl3_vsub(dpL, peM, peL);
                vl3_vsub(dpR, peR, peM);

                vl3_vinter(dp, dpL, dpR, 0.5);

                if (vl3_vnorm(dp) > 1E-16)
                {
                    vl3_vmul_s(dp, dp, pos_error * self->pos_step / vl3_vnorm(dp));
                    vl3_vsub(&obj->log_ls[offset-1].pos[2][0], &obj->log_ls[offset-1].pos[2][0], dp);
                }

            } while (pos_error > self->pos_tol && counter < 1000);

            __trcproc_euler_correct_calc__(self, obj, offset, vl_vec_zeros, vl_vec_zeros);
        }

        // Calculate and write to pos/rot error L2 norm
        obj->log_ls[offset].pos_error = vl3_vdist(&obj->log_ls[offset].pos[0][0], &obj->pos[0][0]);
        obj->log_ls[offset].rot_error = vl3_mdist(&obj->log_ls[offset].rot[0][0], &obj->rot[0][0]);
    }

    return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcproc_euler_init_ (void **data, void *config)
{
	*data = (s_trcproc_euler*) malloc(sizeof(s_trcproc_euler));
	
	s_trcproc_euler *proc = (s_trcproc_euler*) *data;
    s_trcproc_euler_attr *attr = (s_trcproc_euler_attr*) config;
	
	return trcproc_euler_init(proc, *attr);
}

inline u8_t trcproc_euler_free_ (void **data)
{
	s_trcproc_euler *proc = (s_trcproc_euler*) *data;
	
	free(proc);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC_EULER__ */


