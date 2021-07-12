
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_PROC__
#define __TRJ_PROC__

//----------------------------------------------------------------

#include <lib_internal/vl.h>

#include "trj_api.h"

//----------------------------------------------------------------

typedef struct trj_proc_euler
{
	vlf_t pos[3][3];
	vlf_t rot[3][9];
	
	vlf_t rd1[2][9];
	
	s_vl_rd1 rd1_data;

    vlf_t rot_tol;
    vlf_t rot_var;
    vlf_t rot_step;

    vlf_t pos_tol;
    vlf_t pos_var;
    vlf_t pos_step;
	
}   s_trj_proc_euler;

typedef struct trj_proc_euler_init
{
    vlf_t rot_tol;
    vlf_t rot_var;
    vlf_t rot_step;

    vlf_t pos_tol;
    vlf_t pos_var;
    vlf_t pos_step;

}   s_trj_proc_euler_init;

//----------------------------------------------------------------

inline uint8_t trj_proc_euler_init(s_trj_proc_euler *self, s_trj_proc_euler_init attr)
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

inline uint8_t trj_proc_euler_save(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_load(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline void __trj_proc_euler_correct_calc__(s_trj_proc_euler *self, s_trj_obj *obj, uint32_t offset, vlf_t *d_grs, vlf_t *d_acc)
{
    vlf_t tied_acc[3];
    vlf_t tied_grs[9];

    vlf_t d_inert_acc[3];
    vl3_vsum(d_inert_acc, &obj->log_list[offset-1].pos[2][0], d_acc);

    vlf_t d_inert_grs[9];
    vlf_t d_grs_skew[9];

    vl3_skew(d_grs_skew, d_grs);
    vl3_msum(d_inert_grs, &obj->log_list[offset-1].rot[1][0], d_grs_skew);

    vl3_mtmul_v(tied_acc, &obj->log_list[offset-1].rot[0][0], d_inert_acc);
    vl3_mtmul_m(tied_grs, &obj->log_list[offset-1].rot[0][0], d_inert_grs);

    vlf_t inert_acc[3];
    vlf_t inert_grs[9];

    vl3_mmul_v(inert_acc, &obj->rot[0][0], tied_acc);
    vl3_mmul_m(inert_grs, &obj->rot[0][0], tied_grs);

    vl3_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
    vl3_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_list[offset+1].time[1]);
    vl3_rnorm(&obj->rot[0][0]);

    vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_list[offset].time[1]);
    vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], inert_acc, obj->log_list[offset].time[1]*obj->log_list[offset].time[1]*0.5);

    vl3_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_list[offset].time[1]);

    vl3_vcopy(&obj->pos[2][0], inert_acc);

    return;
}

inline uint8_t trj_proc_euler_update(s_trj_proc_euler *self, s_trj_obj *obj, uint32_t offset)
{
    // Calcaulate diff estimates
    {
        s_trj_obj_data *l0 = &obj->log_list[offset-1];
        s_trj_obj_data *l1 = &obj->log_list[offset+0];
        s_trj_obj_data *l2 = &obj->log_list[offset+1];

        vlf_t *vel = &l1->pos[1][0];
        vlf_t *acc = &l1->pos[2][0];

        // assume fixed step for now
        vlf_t h = obj->log_list[0x01].time[1];

        if (offset >= 0x01) { vl3_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
        else  				{ vl3_vinter(&self->pos[0][0], &l1->pos[0][0], &l2->pos[0][0], -1.0); }

        vl3_vcopy(&self->pos[1][0], &l1->pos[0][0]);

        if (offset <= obj->log_offset-2) { vl3_vcopy(&self->pos[2][0], &l2->pos[0][0]); }
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
        if (offset == obj->log_offset-1) { vl3_vcopy(&l1->pos[2][0], &l0->pos[2][0]); }


        vlf_t *rot = &l1->rot[1][0];

        if (offset >= 0x01) { vl3_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
        else  				{ vl3_rinter(&self->rot[0][0], &l1->rot[0][0], &l2->rot[0][0], -1.0); }

        vl3_mcopy(&self->rot[1][0], &l1->rot[0][0]);

        if (offset <= obj->log_offset-2) { vl3_mcopy(&self->rot[2][0], &l2->rot[0][0]); }
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
            vl3_vcopy(&obj->pos[0][0], &obj->log_list[0].pos[0][0]);
            vl3_vcopy(&obj->pos[1][0], &obj->log_list[0].pos[1][0]);
            vl3_vcopy(&obj->pos[2][0], &obj->log_list[0].pos[2][0]);

            vl3_mcopy(&obj->rot[0][0], &obj->log_list[0].rot[0][0]);
            vl3_mcopy(&obj->rot[1][0], &obj->log_list[0].rot[1][0]);
            vl3_mcopy(&obj->rot[2][0], &obj->log_list[0].rot[2][0]);
        }

        else
        {
            uint32_t counter = 0x00;
            float64_t rot_error = 0.0;

            do
            {
                ++counter;

                s_trj_obj rL[3] = {*obj, *obj, *obj};
                s_trj_obj rM[3] = {*obj, *obj, *obj};
                s_trj_obj rR[3] = {*obj, *obj, *obj};

                __trj_proc_euler_correct_calc__(self, &rL[0], offset, vl_vec(-self->rot_var, 0.0, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rL[1], offset, vl_vec(0.0, -self->rot_var, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rL[2], offset, vl_vec(0.0, 0.0, -self->rot_var), vl_vec_zeros);

                __trj_proc_euler_correct_calc__(self, &rM[0], offset, vl_vec(0.0, 0.0, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rM[1], offset, vl_vec(0.0, 0.0, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rM[2], offset, vl_vec(0.0, 0.0, 0.0), vl_vec_zeros);

                __trj_proc_euler_correct_calc__(self, &rR[0], offset, vl_vec(+self->rot_var, 0.0, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rR[1], offset, vl_vec(0.0, +self->rot_var, 0.0), vl_vec_zeros);
                __trj_proc_euler_correct_calc__(self, &rR[2], offset, vl_vec(0.0, 0.0, +self->rot_var), vl_vec_zeros);

                // re = rotation error
                vlf_t reL[3] = {
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rL[0].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rL[1].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rL[2].rot[0][0]),
                };

                vlf_t reM[3] = {
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rM[0].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rM[1].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rM[2].rot[0][0]),
                };

                vlf_t reR[3] = {
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rR[0].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rR[1].rot[0][0]),
                        vl3_mdist(&obj->log_list[offset].rot[0][0], &rR[2].rot[0][0]),
                };

                rot_error = vl3_vnorm(reM);

                vlf_t drL[3];
                vlf_t drR[3];
                vlf_t dr[3];

                vl3_vsub(drL, reM, reL);
                vl3_vsub(drR, reR, reM);

                vl3_vinter(dr, drL, drR, 0.5);

                if (vl3_vnorm(dr) > 1E-24)
                {
                    vl3_vmul_s(dr, dr, self->rot_step / vl3_vnorm(dr));
                    vlf_t r_corr[9];
                    vl3_skew(r_corr, dr);

                    vl3_msub(&obj->log_list[offset-1].rot[1][0], &obj->log_list[offset-1].rot[1][0], r_corr);
                }

            } while (rot_error > self->rot_tol && counter < 1000);

            counter = 0x00;
            vlf_t pos_error = 0.0;

            do
            {
                ++counter;

                s_trj_obj pL[3] = {*obj, *obj, *obj};
                s_trj_obj pM[3] = {*obj, *obj, *obj};
                s_trj_obj pR[3] = {*obj, *obj, *obj};

                __trj_proc_euler_correct_calc__(self, &pL[0], offset, vl_vec_zeros, vl_vec(-self->pos_var, 0.0, 0.0));
                __trj_proc_euler_correct_calc__(self, &pL[1], offset, vl_vec_zeros, vl_vec(0.0, -self->pos_var, 0.0));
                __trj_proc_euler_correct_calc__(self, &pL[2], offset, vl_vec_zeros, vl_vec(0.0, 0.0, -self->pos_var));

                __trj_proc_euler_correct_calc__(self, &pM[0], offset, vl_vec_zeros, vl_vec(0.0, 0.0, 0.0));
                __trj_proc_euler_correct_calc__(self, &pM[1], offset, vl_vec_zeros, vl_vec(0.0, 0.0, 0.0));
                __trj_proc_euler_correct_calc__(self, &pM[2], offset, vl_vec_zeros, vl_vec(0.0, 0.0, 0.0));

                __trj_proc_euler_correct_calc__(self, &pR[0], offset, vl_vec_zeros, vl_vec(+self->pos_var, 0.0, 0.0));
                __trj_proc_euler_correct_calc__(self, &pR[1], offset, vl_vec_zeros, vl_vec(0.0, +self->pos_var, 0.0));
                __trj_proc_euler_correct_calc__(self, &pR[2], offset, vl_vec_zeros, vl_vec(0.0, 0.0, +self->pos_var));

                // re = rotation error
                vlf_t peL[3] = {
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pL[0].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pL[1].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pL[2].pos[0][0]),
                };

                vlf_t peM[3] = {
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pM[0].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pM[1].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pM[2].pos[0][0]),
                };

                vlf_t peR[3] = {
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pR[0].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pR[1].pos[0][0]),
                        vl3_vdist(&obj->log_list[offset].pos[0][0], &pR[2].pos[0][0]),
                };

                pos_error = vl3_vnorm(peM);

                vlf_t dpL[3];
                vlf_t dpR[3];
                vlf_t dp[3];

                vl3_vsub(dpL, peM, peL);
                vl3_vsub(dpR, peR, peM);

                vl3_vinter(dp, dpL, dpR, 0.5);

                if (vl3_vnorm(dp) > 1E-32)
                {
                    vl3_vmul_s(dp, dp, self->pos_step / vl3_vnorm(dp));
                    vl3_vsub(&obj->log_list[offset-1].pos[2][0], &obj->log_list[offset-1].pos[2][0], dp);
                }

            } while (pos_error > self->pos_tol && counter < 1000);

            __trj_proc_euler_correct_calc__(self, obj, offset, vl_vec_zeros, vl_vec_zeros);
        }

        // Calculate and write to pos/rot error L2 norm
        obj->log_list[offset].pos_error = vl3_vdist(&obj->log_list[offset].pos[0][0], &obj->pos[0][0]);
        obj->log_list[offset].rot_error = vl3_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0]);
    }

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
    s_trj_proc_euler *proc = (s_trj_proc_euler *) data;
    s_trj_proc_euler_init *attr = (s_trj_proc_euler_init *) config;

	return trj_proc_euler_save(proc, attr, v_file);
}

inline uint8_t trj_proc_euler_load_(void *data, void *config, uint8_t **v_file)
{
    s_trj_proc_euler *proc = (s_trj_proc_euler *) data;
    s_trj_proc_euler_init *attr = (s_trj_proc_euler_init *) config;

    return trj_proc_euler_load(proc, attr, v_file);
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
	
	if (offset >= 0x02) { vl3_vcopy(&self->pos[0][0], &l0->pos[0][0]); }
	else  				{ vl3_vinter(&self->pos[0][0], &l2->pos[0][0], &l4->pos[0][0], -1.0); }
	
	if (offset >= 0x01) { vl3_vcopy(&self->pos[1][0], &l1->pos[0][0]); }
	else  				{ vl3_vinter(&self->pos[1][0], &l2->pos[0][0], &l3->pos[0][0], -1.0); }
	
	vl3_vcopy(&self->pos[2][0], &l2->pos[0][0]);
	
	if (offset <= obj->log_offset-2) { vl3_vcopy(&self->pos[3][0], &l3->pos[0][0]); }
	else  							 { vl3_vinter(&self->pos[3][0], &l2->pos[0][0], &l1->pos[0][0], -1.0); }
	
	if (offset <= obj->log_offset-3) { vl3_vcopy(&self->pos[4][0], &l4->pos[0][0]); }
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
	if (offset == obj->log_offset-1) { vl3_vcopy(&l2->pos[2][0], &l1->pos[2][0]); }
	
	vlf_t *rot = &l2->rot[1][0];
	
	if (offset >= 0x02) { vl3_mcopy(&self->rot[0][0], &l0->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[0][0], &l2->rot[0][0], &l4->rot[0][0], -1.0); }
	
	if (offset >= 0x01) { vl3_mcopy(&self->rot[1][0], &l1->rot[0][0]); }
	else  				{ vl3_rinter(&self->rot[1][0], &l2->rot[0][0], &l3->rot[0][0], -1.0); }
	
	vl3_mcopy(&self->rot[2][0], &l2->rot[0][0]);
	
	if (offset <= obj->log_offset-2) { vl3_mcopy(&self->rot[3][0], &l3->rot[0][0]); }
	else  							 { vl3_rinter(&self->rot[3][0], &l2->rot[0][0], &l1->rot[0][0], -1.0); }
	
	if (offset <= obj->log_offset-3) { vl3_mcopy(&self->rot[4][0], &l4->rot[0][0]); }
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
		vl3_vcopy(&obj->pos[0][0], &obj->log_list[0].pos[0][0]);
		vl3_vcopy(&obj->pos[1][0], &obj->log_list[0].pos[1][0]);
		vl3_vcopy(&obj->pos[2][0], &obj->log_list[0].pos[2][0]);
		
		vl3_mcopy(&obj->rot[0][0], &obj->log_list[0].rot[0][0]);
		vl3_mcopy(&obj->rot[1][0], &obj->log_list[0].rot[1][0]);
		vl3_mcopy(&obj->rot[2][0], &obj->log_list[0].rot[2][0]);
	}
	
	else
	{
		vlf_t tied_acc[3];
		vlf_t tied_grs[9];
		
		vl3_mtmul_v(tied_acc, &obj->log_list[offset-1].rot[0][0],
				   &obj->log_list[offset-1].pos[2][0]);
		
		vl3_mtmul_m(tied_grs, &obj->log_list[offset-1].rot[0][0],
				   &obj->log_list[offset-1].rot[1][0]);
		
		vlf_t inert_acc[3];
		vlf_t inert_grs[9];
		
		vl3_mmul_v(inert_acc, &obj->rot[0][0], tied_acc);
		vl3_mmul_m(inert_grs, &obj->rot[0][0], tied_grs);
		
		vl3_mmul_m(inert_grs, inert_grs, &obj->rot[0][0]);
		vl3_msumm(&obj->rot[0][0], &obj->rot[0][0], inert_grs, obj->log_list[offset+1].time[1]);
		vl3_rnorm(&obj->rot[0][0]);
		
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[1][0], obj->log_list[offset].time[1]);
		vl3_vsumm(&obj->pos[0][0], &obj->pos[0][0], &obj->pos[2][0], obj->log_list[offset].time[1]*obj->log_list[offset].time[1]*0.5);
		
		vl3_vsumm(&obj->pos[1][0], &obj->pos[1][0], inert_acc, obj->log_list[offset].time[1]);
		
		vl3_vcopy(&obj->pos[2][0], inert_acc);
	}
	
	// Calculate and write to pos/rot error L2 norm
	obj->log_list[offset].pos_error = vl3_vdist(&obj->log_list[offset].pos[0][0], &obj->pos[0][0]);
	obj->log_list[offset].rot_error = vl3_mdist(&obj->log_list[offset].rot[0][0], &obj->rot[0][0]);

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


