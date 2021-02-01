
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_CTRL__
#define __TRJ_CTRL__

//------------------------------------------------------------------------------

#include "vl.h"
#include "vl_svd.h"

#include "trj_api.h"
#include "trj_obj.h"
#include "trj_utils.h"

//------------------------------------------------------------------------------

typedef struct trj_ctrl_upos
{
	uint32_t temp;
	
}   s_trj_ctrl_upos;

typedef struct trj_ctrl_upos_init
{
	uint32_t temp;
	
}   s_trj_ctrl_upos_init;

typedef struct trj_ctrl_cpos
{
	uint32_t temp;
	
}   s_trj_ctrl_cpos;

typedef struct trj_ctrl_cpos_init
{
	uint32_t temp;
	
}   s_trj_ctrl_cpos_init;

typedef struct trj_ctrl_urot
{
	uint32_t temp;
	
}   s_trj_ctrl_urot;

typedef struct trj_ctrl_urot_init
{
	uint32_t temp;
	
}   s_trj_ctrl_urot_init;

typedef struct trj_ctrl_crot
{
	uint32_t temp;
	
}   s_trj_ctrl_crot;

typedef struct trj_ctrl_crot_init
{
	uint32_t temp;
	
}   s_trj_ctrl_crot_init;



//        
//        parent = self.parent;
//        
//        if (self.seg_offset+1) < len(self.ref_points) and (self.parent.time_0[-1] > self.ref_points[self.seg_offset+1].time):
//            self.seg_offset += 1;
//
//        interp_state = (self.parent.time_0[-1] - self.ref_points[self.seg_offset].time) / (self.ref_points[self.seg_offset+1].time - self.ref_points[self.seg_offset].time);
//        abs_pos      = self.get_pos(self.seg_offset, interp_state);
//        ref_pos_1      = numpy.array(self.parent.ref_obj.rot_0[-1].I.dot(abs_pos - self.parent.ref_obj.pos_0[-1])).reshape(3);
//        ref_pos_0      = numpy.array(self.parent.ref_obj.rot_0[-2].I.dot(self.parent.pos_0[-1] - self.parent.ref_obj.pos_0[-2])).reshape(3);
//
//        # print(ref_pos_1 - ref_pos_0);
//        # print()
//        # print(numpy.mat(self.parent.rot_0[-1][0]));
//        # print(self.parent.rot_0[-1][0][0], self.parent.rot_0[-1][0][1], self.parent.rot_0[-1][0][2]);
//        # print(self.parent.rot_0[-1]);
//
//        e0 = numpy.array(numpy.mat(self.parent.rot_0[-1].T[0,:])).reshape(3);
//        e0 /= scipy.linalg.norm(e0);
//
//        e1 = numpy.array(self.ref_obj.rot_0[-1].dot(ref_pos_1 - ref_pos_0)).reshape(3);
//        e1 /= scipy.linalg.norm(e1);
//
//        # print(e0, e1);
//
//        c  = numpy.dot(e0, e1);
//
//        # abs_rot = self.parent.rot_0[-1];
//        abs_rot = self.ref_obj.rot_0[-1].dot(self.ref_obj.rot_0[-2].I.dot(self.parent.rot_0[-1]));
//        # abs_rot_1 = numpy.array([0.0, 0.0, 0.0]);
//
//        if abs(c) > 0 and abs(1+c) > 0:# and scipy.linalg.norm(ref_pos_1 - ref_pos_0) > 1E-6:
//            
//            # print('adjust', scipy.linalg.norm(ref_pos_1 - ref_pos_0))
//            
//            v  = numpy.cross(e0, e1);
//            s  = scipy.linalg.norm(v);
//            sv = trj_utils.skew(v);
//
//            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
//            R  = trj_utils.unskew(R);
//
//            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
//                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
//
//            R = trj_utils.skew(R);
//
//            abs_rot = trj_utils.rot_norm(parent.rot_0[-1] + R.dot(parent.rot_0[-1]));
//            # abs_rot_1 = trj_utils.unskew(sv + sv.dot(sv) / (1 + c));
//
//        # print(v)
//
//        # print(scipy.linalg.norm(R.dot(e0) - e1));
//        # print(scipy.linalg.norm(e0 - e1));
//
//        # print(trj_utils.rot_norm(R.dot(parent.rot_0[-1])) - parent.rot_0[-1]);
//
//        parent.pos_0.append(abs_pos);
//        # parent.rot_0.append(numpy.mat(parent.rot_0[-1]));
//        # parent.rot_0.append(trj_utils.rot_norm(parent.rot_0[-1] + trj_utils.skew(v).dot(parent.rot_0[-1])));
//        # parent.rot_0.append(trj_utils.rot_norm(R.dot(parent.rot_0[-1])));
//        parent.rot_0.append(trj_utils.rot_norm(abs_rot));
//        # parent.rot_0.append(trj_utils.rot_norm(parent.rot_0[-1] + parent.rot_0[-1].dot(R)));
//
//        # e0 = numpy.array(numpy.mat(self.parent.rot_0[-1].T[0,:])).reshape(3);
//        # e0 /= scipy.linalg.norm(e0);
//
//        # e1 = numpy.array(self.ref_obj.rot_0[-1].dot(ref_pos_1 - ref_pos_0)).reshape(3);
//        # e1 /= scipy.linalg.norm(e1);
//
//        # print(e0, e1);        
//
//        return;

//------------------------------------------------------------------------------

inline uint8_t trj_ctrl_upos_init(s_trj_ctrl_upos *self, s_trj_ctrl_upos_init attr)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_save(s_trj_ctrl_upos *self, s_trj_ctrl_upos_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_load(s_trj_ctrl_upos *self, s_trj_ctrl_upos_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_reset(s_trj_ctrl_upos *self, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_update(s_trj_ctrl_upos *self, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_init(s_trj_ctrl_cpos *self, s_trj_ctrl_cpos_init attr)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_save(s_trj_ctrl_cpos *self, s_trj_ctrl_cpos_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_load(s_trj_ctrl_cpos *self, s_trj_ctrl_cpos_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_reset(s_trj_ctrl_cpos *self, s_trj_obj *obj)
{
	if (obj->traj_offset > 0x00)
	{
		s_trj_traj *traj = &obj->traj_list[0];
		traj->pos(traj->data, 0.0, &obj->pos[0][0]);
	}
	
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_update(s_trj_ctrl_cpos *self, s_trj_obj *obj)
{
	if (obj->traj_offset > 0x00)
	{
		s_trj_traj *traj = &obj->traj_list[0];
		traj->pos(traj->data, *obj->time, &obj->pos[0][0]);
	}
	
	return 0x00;
}

inline uint8_t trj_ctrl_urot_init(s_trj_ctrl_urot *self, s_trj_ctrl_urot_init attr)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_save(s_trj_ctrl_urot *self, s_trj_ctrl_urot_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_load(s_trj_ctrl_urot *self, s_trj_ctrl_urot_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_reset(s_trj_ctrl_urot *self, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_update(s_trj_ctrl_urot *self, s_trj_obj *obj)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_init(s_trj_ctrl_crot *self, s_trj_ctrl_crot_init attr)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_save(s_trj_ctrl_crot *self, s_trj_ctrl_crot_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_load(s_trj_ctrl_crot *self, s_trj_ctrl_crot_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_reset(s_trj_ctrl_crot *self, s_trj_obj *obj)
{
	if (obj->traj_offset > 0x00)
	{
		s_trj_traj *traj = &obj->traj_list[0];
		traj->rot(traj->data, 0.0, &obj->rot[0][0]);
	}
	
	return 0x00;
}

inline uint8_t trj_ctrl_crot_update(s_trj_ctrl_crot *self, s_trj_obj *obj)
{
	if (obj->traj_offset > 0x00)
	{
		s_trj_traj *traj = &obj->traj_list[0];
		traj->rot(traj->data, *obj->time, &obj->rot[0][0]);
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_ctrl_upos_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_upos*) malloc(sizeof(s_trj_ctrl_upos));
	
	s_trj_ctrl_upos *ctrl = (s_trj_ctrl_upos*) *data;
	s_trj_ctrl_upos_init *init = (s_trj_ctrl_upos_init*) config;
	
	return trj_ctrl_upos_init(ctrl, *init);
}

inline uint8_t trj_ctrl_upos_free_(void **data)
{
	s_trj_ctrl_upos *ctrl = (s_trj_ctrl_upos*) *data;
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_upos_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_upos_reset_(void *data, void *obj)
{
	s_trj_ctrl_upos *ctrl = (s_trj_ctrl_upos*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_upos_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_upos_update_(void *data, void *obj)
{
	s_trj_ctrl_upos *ctrl = (s_trj_ctrl_upos*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_upos_update(ctrl, obj_);
}

inline uint8_t trj_ctrl_cpos_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_cpos*) malloc(sizeof(s_trj_ctrl_cpos));
	
	s_trj_ctrl_cpos *ctrl = (s_trj_ctrl_cpos*) *data;
	s_trj_ctrl_cpos_init *init = (s_trj_ctrl_cpos_init*) config;
	
	return trj_ctrl_cpos_init(ctrl, *init);
}

inline uint8_t trj_ctrl_cpos_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_free_(void **data)
{
	s_trj_ctrl_cpos *ctrl = (s_trj_ctrl_cpos*) *data;
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_cpos_reset_(void *data, void *obj)
{
	s_trj_ctrl_cpos *ctrl = (s_trj_ctrl_cpos*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_cpos_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_cpos_update_(void *data, void *obj)
{
	s_trj_ctrl_cpos *ctrl = (s_trj_ctrl_cpos*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_cpos_update(ctrl, obj_);
}

inline uint8_t trj_ctrl_urot_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_urot*) malloc(sizeof(s_trj_ctrl_urot));
	
	s_trj_ctrl_urot *ctrl = (s_trj_ctrl_urot*) *data;
	s_trj_ctrl_urot_init *init = (s_trj_ctrl_urot_init*) config;
	
	return trj_ctrl_urot_init(ctrl, *init);
}

inline uint8_t trj_ctrl_urot_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_urot_free_(void **data)
{
	s_trj_ctrl_urot *ctrl = (s_trj_ctrl_urot*) *data;
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_urot_reset_(void *data, void *obj)
{
	s_trj_ctrl_urot *ctrl = (s_trj_ctrl_urot*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_urot_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_urot_update_(void *data, void *obj)
{
	s_trj_ctrl_urot *ctrl = (s_trj_ctrl_urot*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_urot_update(ctrl, obj_);
}

inline uint8_t trj_ctrl_crot_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_crot*) malloc(sizeof(s_trj_ctrl_crot));
	
	s_trj_ctrl_crot *ctrl = (s_trj_ctrl_crot*) *data;
	s_trj_ctrl_crot_init *init = (s_trj_ctrl_crot_init*) config;
	
	return trj_ctrl_crot_init(ctrl, *init);
}

inline uint8_t trj_ctrl_crot_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_crot_free_(void **data)
{
	s_trj_ctrl_crot *ctrl = (s_trj_ctrl_crot*) *data;
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_crot_reset_(void *data, void *obj)
{
	s_trj_ctrl_crot *ctrl = (s_trj_ctrl_crot*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_crot_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_crot_update_(void *data, void *obj)
{
	s_trj_ctrl_crot *ctrl = (s_trj_ctrl_crot*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_crot_update(ctrl, obj_);
}

//------------------------------------------------------------------------------


//	vlf_t dp0_p1[3];
//	vlf_t dp0_p2[3];
//
//	vl_vmul_s(dp0_p1, self->parent->pos[1], self->parent->time[1]);
//	vl_vmul_s(dp0_p2, self->parent->pos[2], self->parent->time[2]);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
//
//	vlf_t dp1_p2[3];
//
//	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
//	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
//
//	vlf_t dp2_f[3];
//
//	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
//	vl_vcopy(ctrl->parent->pos[2], dp2_f);
//
//	vlf_t dr0_r1[9];
//	vlf_t dr0_r2[9];
//	vlf_t r1_sk[9];
//	vlf_t r2_sk[9];
//
//	vl_skew(r1_sk, ctrl->parent->rot[1]);
//	vl_skew(r2_sk, ctrl->parent->rot[2]);
//
//	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
//
//	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
//
//	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
//	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
//
//	vlf_t dr1_r2[3];
//
//	vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
//	vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
//	vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
//
//	vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);
//
//	vl_rnorm(ctrl->parent->rot[0]);
//
//	return;
//}

//------------------------------------------------------------------------------

//inline void ins_ctrl_uins_update(void *__ctrl__)
//{
//	ins_ctrl_uins_t *ctrl = (ins_ctrl_uins_t*) __ctrl__;
//
//	vlf_t dp2_f[3];
//
//	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
//	vl_vsum(ctrl->parent->pos[2], ctrl->parent->pos[2], dp2_f);
//
//	vlf_t dp0_p1[3];
//	vlf_t dp0_p2[3];
//
//	vl_vmul_s(dp0_p1, ctrl->parent->pos[1], ctrl->parent->time[1]);
//	vl_vmul_s(dp0_p2, ctrl->parent->pos[2], ctrl->parent->time[2]);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
//
//	vlf_t dp1_p2[3];
//
//	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
//	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
//
//	vlf_t dr0_r1[9];
//	vlf_t dr0_r2[9];
//	vlf_t r1_sk[9];
//	vlf_t r2_sk[9];
//
//	vl_skew(r1_sk, ctrl->parent->rot[1]);
//	vl_skew(r2_sk, ctrl->parent->rot[2]);
//
//	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
//
//	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
//
////    vl_mprint(ctrl->parent->rot[0]);
////
////    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
////    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
////
//	vl_rnorm(ctrl->parent->rot[0]);
//
////    vl_mprint(ctrl->parent->rot[0]);
//
//	return;
//}

//------------------------------------------------------------------------------

//inline void ins_ctrl_corb_update(void *__ctrl__)
//{
//	ins_ctrl_corb_t *ctrl = (ins_ctrl_corb_t*) __ctrl__;
//
//	ins_ctrl_corb_get_pos(ctrl, ctrl->parent->pos[0], ctrl->parent->time[0]);
//
//	vlf_t dr0_r1[9];
//	vlf_t dr0_r2[9];
//	vlf_t r1_sk[9];
//	vlf_t r2_sk[9];
//
//	vl_skew(r1_sk, ctrl->parent->rot[1]);
//	vl_skew(r2_sk, ctrl->parent->rot[2]);
//
//	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
//
//	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
//	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
//
//	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
//	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
//
//	vlf_t dr1_r2[3];
//
//	vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
//	vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
//	vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
//
//	vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);
//
//	vl_rnorm(ctrl->parent->rot[0]);
//
//	return;
//}

//------------------------------------------------------------------------------

//    def get_pos(self, seg_offset, time):
//
//        interp_data  = self.ref_curves[seg_offset].evaluate(time);
//        ref_pos         = self.ref_points[seg_offset].pos_p + interp_data[0] * self.ref_basis[seg_offset][1] + interp_data[1] * self.ref_basis[seg_offset][0];
//        # print(self.parent.ref_obj.pos_p[0]);
//        abs_pos      = self.parent.ref_obj.pos_p[0][-1] + self.parent.ref_obj.rot[0][-1].dot(ref_pos);
//
//        return numpy.array(abs_pos).reshape(3);
//
//    def update(self):
//
//        parent = self.parent;
//
//        if (self.seg_offset+1) < len(self.ref_points) and (self.parent.time_0[-1] > self.ref_points[self.seg_offset+1].time):
//            self.seg_offset += 1;
//
//        interp_state = (self.parent.time_0[-1] - self.ref_points[self.seg_offset].time) / (self.ref_points[self.seg_offset+1].time - self.ref_points[self.seg_offset].time);
//        abs_pos      = self.get_pos(self.seg_offset, interp_state);
//        ref_pos[1]      = numpy.array(self.parent.ref_obj.rot[0][-1].I.dot(abs_pos - self.parent.ref_obj.pos_p[0][-1])).reshape(3);
//        ref_pos[0]      = numpy.array(self.parent.ref_obj.rot[0][-2].I.dot(self.parent.pos_p[0][-1] - self.parent.ref_obj.pos_p[0][-2])).reshape(3);
//
//        # print(ref_pos[1] - ref_pos[0]);
//        # print()
//        # print(numpy.mat(self.parent.rot[0][-1][0]));
//        # print(self.parent.rot[0][-1][0][0], self.parent.rot[0][-1][0][1], self.parent.rot[0][-1][0][2]);
//        # print(self.parent.rot[0][-1]);
//
//        e0 = numpy.array(numpy.mat(self.parent.rot[0][-1].T[0,:])).reshape(3);
//        e0 /= scipy.linalg.norm(e0);
//
//        e1 = numpy.array(self.ref_obj.rot[0][-1].dot(ref_pos[1] - ref_pos[0])).reshape(3);
//        e1 /= scipy.linalg.norm(e1);
//
//        # print(e0, e1);
//
//        c  = numpy.dot(e0, e1);
//
//        # abs_rot = self.parent.rot[0][-1];
//        abs_rot = self.ref_obj.rot[0][-1].dot(self.ref_obj.rot[0][-2].I.dot(self.parent.rot[0][-1]));
//        # abs_rot[1] = numpy.array([0.0, 0.0, 0.0]);
//
//        if abs(c) > 0 and abs(1+c) > 0:# and scipy.linalg.norm(ref_pos[1] - ref_pos[0]) > 1E-6:
//
//            # print('adjust', scipy.linalg.norm(ref_pos[1] - ref_pos[0]))
//
//            v  = numpy.cross(e0, e1);
//            s  = scipy.linalg.norm(v);
//            sv = trj_utils.skew(v);
//
//            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
//            R  = trj_utils.unskew(R);
//
//            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
//                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
//
//            R = trj_utils.skew(R);
//
//            abs_rot = trj_utils.rot_norm(parent.rot[0][-1] + R.dot(parent.rot[0][-1]));
//            # abs_rot[1] = trj_utils.unskew(sv + sv.dot(sv) / (1 + c));
//
//        # print(v)
//
//        # print(scipy.linalg.norm(R.dot(e0) - e1));
//        # print(scipy.linalg.norm(e0 - e1));
//
//        # print(trj_utils.rot_norm(R.dot(parent.rot[0][-1])) - parent.rot[0][-1]);
//
//        parent.pos_p[0].append(abs_pos);
//        # parent.rot[0].append(numpy.mat(parent.rot[0][-1]));
//        # parent.rot[0].append(trj_utils.rot_norm(parent.rot[0][-1] + trj_utils.skew(v).dot(parent.rot[0][-1])));
//        # parent.rot[0].append(trj_utils.rot_norm(R.dot(parent.rot[0][-1])));
//        parent.rot[0].append(trj_utils.rot_norm(abs_rot));
//        # parent.rot[0].append(trj_utils.rot_norm(parent.rot[0][-1] + parent.rot[0][-1].dot(R)));
//
//        # e0 = numpy.array(numpy.mat(self.parent.rot[0][-1].T[0,:])).reshape(3);
//        # e0 /= scipy.linalg.norm(e0);
//
//        # e1 = numpy.array(self.ref_obj.rot[0][-1].dot(ref_pos[1] - ref_pos[0])).reshape(3);
//        # e1 /= scipy.linalg.norm(e1);
//
//        # print(e0, e1);
//
//        return;

//------------------------------------------------------------------------------

typedef struct trj_ctrl_egms
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	uint32_t ref_hash;
	
}   s_trj_ctrl_egms;


typedef struct trj_ctrl_egms_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
}   s_trj_ctrl_egms_init;

inline uint8_t trj_ctrl_egms_init(s_trj_ctrl_egms *self, s_trj_ctrl_egms_init attr)
{
	self->eng = attr.eng;
	self->ref = attr.ref;
	
	if (self->ref != NULL) { self->ref_hash = self->ref->hash; }
	else { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline uint8_t trj_ctrl_egms_save(s_trj_ctrl_egms *self, s_trj_ctrl_egms_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_egms_load(s_trj_ctrl_egms *self, s_trj_ctrl_egms_init *attr, uint8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trj_eng_find_obj (self->eng, self->ref_hash);
	
	if (self->ref == NULL) { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline void __trj_ctrl_egms_calc__(s_trj_obj *ref, s_trj_obj *obj)
{
	const vlf_t beta  = 0.0053171;
	const vlf_t beta1 = 71E-7;
	const vlf_t q     = 0.00346775;
	const vlf_t ge    = 9.78049;
	const vlf_t a     = 6378136;
	const vlf_t e2    = 0.0066943662;
	
	vlf_t ecef[3];
	
	vl_vsub(ecef, &obj->pos[0][0], &ref->pos[0][0]);
	vl_mtmul_v(ecef, &ref->rot[0][0], ecef);
	
	vlf_t lla[3];
	
	trj_ellp_lla(&trj_ellp_pz90, lla, ecef);
	
	vlf_t g0 = ge*(1 + beta*pow(sin(lla[0]),2) + beta1*pow(sin(2*lla[0]),2));
	
	vlf_t g_hor[3] = {
			g0*sin(2*lla[0])*(lla[2]/a)*((e2)/a - 2*q),
			g0 + (lla[2]/a)*((3*lla[2]/a) - 2*q*ge*pow(cos(lla[0]),2)
			+ (e2)*(3*pow(sin(lla[0]),2) - 1) - q*(1+6*pow(sin(lla[0]),2))),
			0.0
	};
	
	vl_vmul_s(g_hor, g_hor, -1.0);
	
	vlf_t ecef_ctn[9];
	trj_ellp_ecefrot(&trj_ellp_pz90, ecef, ecef_ctn);
	
	vlf_t g_ecef[3];
	vl_mmul_v(g_ecef, ecef_ctn, g_hor);
	
	vlf_t g_inert[3];
	vl_mmul_v(g_inert, &ref->rot[0][0], g_ecef);
	vl_vmul_s(g_inert, g_inert, 1.0 * obj->pos_inert);
	
	vl_vsum(obj->pos_force, obj->pos_force, g_inert);
	
	return;
}

inline uint8_t trj_ctrl_egms_reset(s_trj_ctrl_egms *self, s_trj_obj *obj)
{
	__trj_ctrl_egms_calc__(self->ref, obj);
	
	return 0x00;
}

inline uint8_t trj_ctrl_egms_update(s_trj_ctrl_egms *self, s_trj_obj *obj)
{
	__trj_ctrl_egms_calc__(self->ref, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_ctrl_egms_init_(void **data, void *config)
{
	*data = (s_trj_ctrl_egms*) malloc(sizeof(s_trj_ctrl_egms));
	
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) *data;
	s_trj_ctrl_egms_init *init = (s_trj_ctrl_egms_init*) config;
	
	return trj_ctrl_egms_init(ctrl, *init);
}

inline uint8_t trj_ctrl_egms_free_(void **data)
{
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) *data;
	free(ctrl);
	
	return 0x00;
}

inline uint8_t trj_ctrl_egms_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_ctrl_egms_load_(void *data, void *config, uint8_t **v_file)
{
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) data;
	s_trj_ctrl_egms_init *attr = (s_trj_ctrl_egms_init*) config;
	
	return trj_ctrl_egms_load(ctrl, attr, v_file);
}

inline uint8_t trj_ctrl_egms_reset_(void *data, void *obj)
{
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_egms_reset(ctrl, obj_);
}

inline uint8_t trj_ctrl_egms_update_(void *data, void *obj)
{
	s_trj_ctrl_egms *ctrl = (s_trj_ctrl_egms*) data;
	s_trj_obj *obj_ = (s_trj_obj*) obj;
	
	return trj_ctrl_egms_update(ctrl, obj_);
}

//------------------------------------------------------------------------------

#endif /* __TRJ_CTRL__ */

