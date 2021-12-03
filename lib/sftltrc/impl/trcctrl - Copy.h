
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL__
#define __TRCCTRL__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_upos
{
	u32_t temp;
	
}   s_trcctrl_upos;

typedef struct trcctrl_upos_init
{
	u32_t temp;
	
}   s_trcctrl_upos_init;

typedef struct trcctrl_cpos
{
	u32_t temp;
	
}   s_trcctrl_cpos;

typedef struct trcctrl_cpos_init
{
	u32_t temp;
	
}   s_trcctrl_cpos_init;

typedef struct trcctrl_urot
{
	u32_t temp;
	
}   s_trcctrl_urot;

typedef struct trcctrl_urot_init
{
	u32_t temp;
	
}   s_trcctrl_urot_init;

typedef struct trcctrl_crot
{
	u32_t temp;
	
}   s_trcctrl_crot;

typedef struct trcctrl_crot_init
{
	u32_t temp;
	
}   s_trcctrl_crot_init;



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
//            sv = trc_utils.skew(v);
//
//            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
//            R  = trc_utils.unskew(R);
//
//            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
//                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
//
//            R = trc_utils.skew(R);
//
//            abs_rot = trc_utils.rot_norm(parent.rot_0[-1] + R.dot(parent.rot_0[-1]));
//            # abs_rot_1 = trc_utils.unskew(sv + sv.dot(sv) / (1 + c));
//
//        # print(v)
//
//        # print(scipy.linalg.norm(R.dot(e0) - e1));
//        # print(scipy.linalg.norm(e0 - e1));
//
//        # print(trc_utils.rot_norm(R.dot(parent.rot_0[-1])) - parent.rot_0[-1]);
//
//        parent.pos_0.append(abs_pos);
//        # parent.rot_0.append(numpy.mat(parent.rot_0[-1]));
//        # parent.rot_0.append(trc_utils.rot_norm(parent.rot_0[-1] + trc_utils.skew(v).dot(parent.rot_0[-1])));
//        # parent.rot_0.append(trc_utils.rot_norm(R.dot(parent.rot_0[-1])));
//        parent.rot_0.append(trc_utils.rot_norm(abs_rot));
//        # parent.rot_0.append(trc_utils.rot_norm(parent.rot_0[-1] + parent.rot_0[-1].dot(R)));
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

inline u8_t trcctrl_upos_init(s_trcctrl_upos *self, s_trcctrl_upos_init attr)
{
	return 0x00;
}

inline u8_t trcctrl_upos_save(s_trcctrl_upos *self, s_trcctrl_upos_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_upos_load(s_trcctrl_upos *self, s_trcctrl_upos_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_upos_reset(s_trcctrl_upos *self, s_trcobj *obj)
{
	return 0x00;
}

inline u8_t trcctrl_upos_update(s_trcctrl_upos *self, s_trcobj *obj)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_init(s_trcctrl_cpos *self, s_trcctrl_cpos_init attr)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_save(s_trcctrl_cpos *self, s_trcctrl_cpos_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_load(s_trcctrl_cpos *self, s_trcctrl_cpos_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_reset(s_trcctrl_cpos *self, s_trcobj *obj)
{
	if (obj->traj_sz > 0x00)
	{
		s_trctraj *traj = &obj->traj_ls[0];
		traj->pos(traj->data, 0.0, &obj->pos[0][0]);
	}
	
	return 0x00;
}

inline u8_t trcctrl_cpos_update(s_trcctrl_cpos *self, s_trcobj *obj)
{
	if (obj->traj_sz > 0x00)
	{
		s_trctraj *traj = &obj->traj_ls[0];
		traj->pos(traj->data, *obj->time, &obj->pos[0][0]);
	}
	
	return 0x00;
}

inline u8_t trcctrl_urot_init(s_trcctrl_urot *self, s_trcctrl_urot_init attr)
{
	return 0x00;
}

inline u8_t trcctrl_urot_save(s_trcctrl_urot *self, s_trcctrl_urot_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_urot_load(s_trcctrl_urot *self, s_trcctrl_urot_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_urot_reset(s_trcctrl_urot *self, s_trcobj *obj)
{
	return 0x00;
}

inline u8_t trcctrl_urot_update(s_trcctrl_urot *self, s_trcobj *obj)
{
	return 0x00;
}

inline u8_t trcctrl_crot_init(s_trcctrl_crot *self, s_trcctrl_crot_init attr)
{
	return 0x00;
}

inline u8_t trcctrl_crot_save(s_trcctrl_crot *self, s_trcctrl_crot_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_crot_load(s_trcctrl_crot *self, s_trcctrl_crot_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_crot_reset(s_trcctrl_crot *self, s_trcobj *obj)
{
	if (obj->traj_sz > 0x00)
	{
		s_trctraj *traj = &obj->traj_ls[0];
		traj->rot(traj->data, 0.0, &obj->rot[0][0]);
	}
	
	return 0x00;
}

inline u8_t trcctrl_crot_update(s_trcctrl_crot *self, s_trcobj *obj)
{
	if (obj->traj_sz > 0x00)
	{
		s_trctraj *traj = &obj->traj_ls[0];
		traj->rot(traj->data, *obj->time, &obj->rot[0][0]);
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcctrl_upos_init_(void **data, void *config)
{
	*data = (s_trcctrl_upos*) malloc(sizeof(s_trcctrl_upos));
	
	s_trcctrl_upos *ctrl = (s_trcctrl_upos*) *data;
	s_trcctrl_upos_init *init = (s_trcctrl_upos_init*) config;
	
	return trcctrl_upos_init(ctrl, *init);
}

inline u8_t trcctrl_upos_free_(void **data)
{
	s_trcctrl_upos *ctrl = (s_trcctrl_upos*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_upos_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_upos_load_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_upos_reset_(void *data, void *obj)
{
	s_trcctrl_upos *ctrl = (s_trcctrl_upos*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_upos_reset(ctrl, obj_);
}

inline u8_t trcctrl_upos_update_(void *data, void *obj)
{
	s_trcctrl_upos *ctrl = (s_trcctrl_upos*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_upos_update(ctrl, obj_);
}

inline u8_t trcctrl_cpos_init_(void **data, void *config)
{
	*data = (s_trcctrl_cpos*) malloc(sizeof(s_trcctrl_cpos));
	
	s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) *data;
	s_trcctrl_cpos_init *init = (s_trcctrl_cpos_init*) config;
	
	return trcctrl_cpos_init(ctrl, *init);
}

inline u8_t trcctrl_cpos_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_load_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_cpos_free_(void **data)
{
	s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_cpos_reset_(void *data, void *obj)
{
	s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_cpos_reset(ctrl, obj_);
}

inline u8_t trcctrl_cpos_update_(void *data, void *obj)
{
	s_trcctrl_cpos *ctrl = (s_trcctrl_cpos*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_cpos_update(ctrl, obj_);
}

inline u8_t trcctrl_urot_init_(void **data, void *config)
{
	*data = (s_trcctrl_urot*) malloc(sizeof(s_trcctrl_urot));
	
	s_trcctrl_urot *ctrl = (s_trcctrl_urot*) *data;
	s_trcctrl_urot_init *init = (s_trcctrl_urot_init*) config;
	
	return trcctrl_urot_init(ctrl, *init);
}

inline u8_t trcctrl_urot_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_urot_load_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_urot_free_(void **data)
{
	s_trcctrl_urot *ctrl = (s_trcctrl_urot*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_urot_reset_(void *data, void *obj)
{
	s_trcctrl_urot *ctrl = (s_trcctrl_urot*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_urot_reset(ctrl, obj_);
}

inline u8_t trcctrl_urot_update_(void *data, void *obj)
{
	s_trcctrl_urot *ctrl = (s_trcctrl_urot*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_urot_update(ctrl, obj_);
}

inline u8_t trcctrl_crot_init_(void **data, void *config)
{
	*data = (s_trcctrl_crot*) malloc(sizeof(s_trcctrl_crot));
	
	s_trcctrl_crot *ctrl = (s_trcctrl_crot*) *data;
	s_trcctrl_crot_init *init = (s_trcctrl_crot_init*) config;
	
	return trcctrl_crot_init(ctrl, *init);
}

inline u8_t trcctrl_crot_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_crot_load_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_crot_free_(void **data)
{
	s_trcctrl_crot *ctrl = (s_trcctrl_crot*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_crot_reset_(void *data, void *obj)
{
	s_trcctrl_crot *ctrl = (s_trcctrl_crot*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_crot_reset(ctrl, obj_);
}

inline u8_t trcctrl_crot_update_(void *data, void *obj)
{
	s_trcctrl_crot *ctrl = (s_trcctrl_crot*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_crot_update(ctrl, obj_);
}

//------------------------------------------------------------------------------


//	f64_t dp0_p1[3];
//	f64_t dp0_p2[3];
//
//	vl_vmul_s(dp0_p1, self->parent->pos[1], self->parent->time[1]);
//	vl_vmul_s(dp0_p2, self->parent->pos[2], self->parent->time[2]);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
//
//	f64_t dp1_p2[3];
//
//	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
//	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
//
//	f64_t dp2_f[3];
//
//	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
//	vl_vcopy(ctrl->parent->pos[2], dp2_f);
//
//	f64_t dr0_r1[9];
//	f64_t dr0_r2[9];
//	f64_t r1_sk[9];
//	f64_t r2_sk[9];
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
//	f64_t dr1_r2[3];
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
//	f64_t dp2_f[3];
//
//	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
//	vl_vsum(ctrl->parent->pos[2], ctrl->parent->pos[2], dp2_f);
//
//	f64_t dp0_p1[3];
//	f64_t dp0_p2[3];
//
//	vl_vmul_s(dp0_p1, ctrl->parent->pos[1], ctrl->parent->time[1]);
//	vl_vmul_s(dp0_p2, ctrl->parent->pos[2], ctrl->parent->time[2]);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
//	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
//
//	f64_t dp1_p2[3];
//
//	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
//	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
//
//	f64_t dr0_r1[9];
//	f64_t dr0_r2[9];
//	f64_t r1_sk[9];
//	f64_t r2_sk[9];
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
//	f64_t dr0_r1[9];
//	f64_t dr0_r2[9];
//	f64_t r1_sk[9];
//	f64_t r2_sk[9];
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
//	f64_t dr1_r2[3];
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
//            sv = trc_utils.skew(v);
//
//            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
//            R  = trc_utils.unskew(R);
//
//            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
//                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
//
//            R = trc_utils.skew(R);
//
//            abs_rot = trc_utils.rot_norm(parent.rot[0][-1] + R.dot(parent.rot[0][-1]));
//            # abs_rot[1] = trc_utils.unskew(sv + sv.dot(sv) / (1 + c));
//
//        # print(v)
//
//        # print(scipy.linalg.norm(R.dot(e0) - e1));
//        # print(scipy.linalg.norm(e0 - e1));
//
//        # print(trc_utils.rot_norm(R.dot(parent.rot[0][-1])) - parent.rot[0][-1]);
//
//        parent.pos_p[0].append(abs_pos);
//        # parent.rot[0].append(numpy.mat(parent.rot[0][-1]));
//        # parent.rot[0].append(trc_utils.rot_norm(parent.rot[0][-1] + trc_utils.skew(v).dot(parent.rot[0][-1])));
//        # parent.rot[0].append(trc_utils.rot_norm(R.dot(parent.rot[0][-1])));
//        parent.rot[0].append(trc_utils.rot_norm(abs_rot));
//        # parent.rot[0].append(trc_utils.rot_norm(parent.rot[0][-1] + parent.rot[0][-1].dot(R)));
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

typedef struct trcctrl_egms
{
	s_trceng *eng;
	s_trcobj *ref;
	u32_t ref_hash;
	
}   s_trcctrl_egms;


typedef struct trcctrl_egms_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
}   s_trcctrl_egms_init;

inline u8_t trcctrl_egms_init(s_trcctrl_egms *self, s_trcctrl_egms_init attr)
{
	self->eng = attr.eng;
	self->ref = attr.ref;
	
	if (self->ref != NULL) { self->ref_hash = self->ref->hash; }
	else { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline u8_t trcctrl_egms_save(s_trcctrl_egms *self, s_trcctrl_egms_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_egms_load(s_trcctrl_egms *self, s_trcctrl_egms_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trceng_find_obj (self->eng, self->ref_hash);
	
	if (self->ref == NULL) { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline static void __trcctrl_egms_calc__(s_trcobj *ref, s_trcobj *obj)
{
	const f64_t beta  = 0.0053171;
	const f64_t beta1 = 71E-7;
	const f64_t q     = 0.00346775;
	const f64_t ge    = 9.78049;
	const f64_t a     = 6378136;
	const f64_t e2    = 0.0066943662;
	
	f64_t ecef[3];
	
	vl3_vsub(ecef, &obj->pos[0][0], &ref->pos[0][0]);
	vl3_mtmul_v(ecef, &ref->rot[0][0], ecef);
	
	f64_t lla[3];
	
	trcellp_lla(&trcellp_pz90, lla, ecef);
	
	f64_t g0 = ge*(1 + beta*pow(sin(lla[0]),2) + beta1*pow(sin(2*lla[0]),2));
	
	f64_t g_hor[3] = {
			g0*sin(2*lla[0])*(lla[2]/a)*((e2)/a - 2*q),
			g0 + (lla[2]/a)*((3*lla[2]/a) - 2*q*ge*pow(cos(lla[0]),2)
			+ (e2)*(3*pow(sin(lla[0]),2) - 1) - q*(1+6*pow(sin(lla[0]),2))),
			0.0
	};
	
	vl3_vmul_s(g_hor, g_hor, -1.0);
	
	f64_t ecef_ctn[9];
	trcellp_ecefrot(&trcellp_pz90, ecef, ecef_ctn);
	
	f64_t g_ecef[3];
	vl3_mmul_v(g_ecef, ecef_ctn, g_hor);
	
	f64_t g_inert[3];
	vl3_mmul_v(g_inert, &ref->rot[0][0], g_ecef);
	vl3_vmul_s(g_inert, g_inert, 1.0 * obj->pos_inert);
	
	vl3_vsum(obj->pos_force, obj->pos_force, g_inert);
	
	return;
}

inline u8_t trcctrl_egms_reset(s_trcctrl_egms *self, s_trcobj *obj)
{
	__trcctrl_egms_calc__(self->ref, obj);
	
	return 0x00;
}

inline u8_t trcctrl_egms_update(s_trcctrl_egms *self, s_trcobj *obj)
{
	__trcctrl_egms_calc__(self->ref, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcctrl_egms_init_(void **data, void *config)
{
	*data = (s_trcctrl_egms*) malloc(sizeof(s_trcctrl_egms));
	
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) *data;
	s_trcctrl_egms_init *init = (s_trcctrl_egms_init*) config;
	
	return trcctrl_egms_init(ctrl, *init);
}

inline u8_t trcctrl_egms_free_(void **data)
{
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_egms_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_egms_load_(void *data, void *config, u8_t **v_file)
{
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) data;
	s_trcctrl_egms_init *attr = (s_trcctrl_egms_init*) config;
	
	return trcctrl_egms_load(ctrl, attr, v_file);
}

inline u8_t trcctrl_egms_reset_(void *data, void *obj)
{
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_egms_reset(ctrl, obj_);
}

inline u8_t trcctrl_egms_update_(void *data, void *obj)
{
	s_trcctrl_egms *ctrl = (s_trcctrl_egms*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_egms_update(ctrl, obj_);
}

//------------------------------------------------------------------------------

typedef struct trcctrl_egmsnpo
{
	s_trceng *eng;
	s_trcobj *ref;
	u32_t ref_hash;
	
}   s_trcctrl_egmsnpo;


typedef struct trcctrl_egmsnpo_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
}   s_trcctrl_egmsnpo_init;

inline u8_t trcctrl_egmsnpo_init (s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init attr)
{
	self->eng = attr.eng;
	self->ref = attr.ref;
	
	if (self->ref != NULL) { self->ref_hash = self->ref->hash; }
	else { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline u8_t trcctrl_egmsnpo_save (s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_egmsnpo_load(s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trceng_find_obj (self->eng, self->ref_hash);
	
	if (self->ref == NULL) { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline static void __trcctrl_egmsnpo_calc__(s_trcobj *ref, s_trcobj *obj)
{
	const f64_t mu = 398600.44E+9;
	const f64_t e = 2.633267E+25;
	
//	R = norm(ecef) + alt_mem;
//	gr = - mu/R^2 - e/R^4 * (1-3*sin(lat_mem)^2);
//	gm = - e/R^4 * sin(2*lat_mem);
//	gh = -gr;
//	gn = -gm;
//
//	acc_e = 0  - centr_acc(3);
//	acc_n = gn - centr_acc(1);
//	acc_h = gh - centr_acc(2);
	
	f64_t ecef[3];
	
	vl3_vsub(ecef, &obj->pos[0][0], &ref->pos[0][0]);
	vl3_mtmul_v(ecef, &ref->rot[0][0], ecef);
	
	f64_t lla[3];
	
	trcellp_lla(&trcellp_pz90_11, lla, ecef);
	
	f64_t r = vl3_vnorm(ecef);
	f64_t gr = -mu / (r*r) - e / (r*r*r*r) * (1-3*sin(lla[0])*sin(lla[0]));
	f64_t gm = -e / (r*r*r*r) * sin(2*lla[0]);
	f64_t gh = -gr;
	f64_t gn = -gm;
	
	f64_t g_hor[3] = {
			gn, gh, 0.0
	};
	
	vl3_vmul_s(g_hor, g_hor, -1.0);
	
	f64_t ecef_ctn[9];
	trcellp_ecefrot(&trcellp_pz90, ecef, ecef_ctn);
	
	f64_t g_ecef[3];
	vl3_mmul_v(g_ecef, ecef_ctn, g_hor);
	
	f64_t g_inert[3];
	vl3_mmul_v(g_inert, &ref->rot[0][0], g_ecef);
	vl3_vmul_s(g_inert, g_inert, 1.0 * obj->pos_inert);
	
	vl3_vsum(obj->pos_force, obj->pos_force, g_inert);
}

inline u8_t trcctrl_egmsnpo_reset(s_trcctrl_egmsnpo *self, s_trcobj *obj)
{
	__trcctrl_egmsnpo_calc__(self->ref, obj);
	
	return 0x00;
}

inline u8_t trcctrl_egmsnpo_update(s_trcctrl_egmsnpo *self, s_trcobj *obj)
{
	__trcctrl_egmsnpo_calc__(self->ref, obj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline u8_t trcctrl_egmsnpo_init_(void **data, void *config)
{
	*data = (s_trcctrl_egmsnpo*) malloc(sizeof(s_trcctrl_egmsnpo));
	
	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) *data;
	s_trcctrl_egmsnpo_init *init = (s_trcctrl_egmsnpo_init*) config;
	
	return trcctrl_egmsnpo_init(ctrl, *init);
}

inline u8_t trcctrl_egmsnpo_free_(void **data)
{
	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_egmsnpo_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_egmsnpo_load_(void *data, void *config, u8_t **v_file)
{
	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
	s_trcctrl_egmsnpo_init *attr = (s_trcctrl_egmsnpo_init*) config;
	
	return trcctrl_egmsnpo_load(ctrl, attr, v_file);
}

inline u8_t trcctrl_egmsnpo_reset_(void *data, void *obj)
{
	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_egmsnpo_reset(ctrl, obj_);
}

inline u8_t trcctrl_egmsnpo_update_(void *data, void *obj)
{
	s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_egmsnpo_update(ctrl, obj_);
}

//------------------------------------------------------------------------------

typedef struct trcctrl_gms
{
	s_trceng 		*eng;
	s_trcobj 		*ref;
	u32_t 			ref_hash;
	
}   s_trcctrl_gms;

typedef struct trcctrl_gms_attr
{
	s_trceng 		*eng;
	s_trcobj 		*ref;
	
}   s_trcctrl_gms_attr;

//------------------------------------------------------------------------------

u8_t trcctrl_gms_init (s_trcctrl *ctrl, s_trcctrl_gms_init attr)
{
	s_trcctrl_gms *ctrl_gms = (s_trcctrl_gms*) ctrl;
	
	ctrl_gms->eng = attr.eng;
	ctrl_gms->ref = attr.ref;
	
	if (self->ref != NULL) { self->ref_hash = self->ref->hash; }
	else { self->ref_hash = 0x00; }
	
	return 0x00;
}


inline u8_t trcctrl_gms_init_(void **data, void *config)
{
	*data = (s_trcctrl_gms*) malloc(sizeof(s_trcctrl_gms));
	
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) *data;
	s_trcctrl_gms_init *init = (s_trcctrl_gms_init*) config;
	
	return trcctrl_gms_init(ctrl, *init);
}


inline u8_t trcctrl_gms_free_(void **data)
{
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) *data;
	free(ctrl);
	
	return 0x00;
}

inline u8_t trcctrl_gms_save_(void *data, void *config, u8_t **v_file)
{
	return 0x00;
}



inline u8_t trcctrl_gms_save (s_trcctrl_gms *gms, s_trcctrl_gms_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trcctrl_gms_load (s_trcctrl_gms *gms, s_trcctrl_gms_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trceng_find_obj (self->eng, self->ref_hash);
	
	if (self->ref == NULL) { self->ref_hash = 0x00; }
	
	return 0x00;
}

inline u8_t trcctrl_gms_load_(void *data, void *config, u8_t **v_file)
{
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) data;
	s_trcctrl_gms_init *attr = (s_trcctrl_gms_init*) config;
	
	return trcctrl_gms_load(ctrl, attr, v_file);
}

inline void __trcctrl_gms_calc__(s_trcobj *ref, s_trcobj *obj)
{
	const f64_t g = 6.67428E-11;
	
	f64_t force_magn = 0.0;
	f64_t dist2 = vl3_vdist2(&ref->pos[0][0], &obj->pos[0][0]);
	
	if (dist2 > 1E-16)
	{ force_magn = g * ref->pos_inert * obj->pos_inert / dist2; }
	
	f64_t force_dir[3];
	vl3_vset(force_dir, 0.0);
	
	if (dist2 > 1E-16)
	{
		vl3_vsub(force_dir, &ref->pos[0][0], &obj->pos[0][0]);
		vl3_vmul_s(force_dir, force_dir, force_magn / vl_sqrt(dist2));
	}
	
	vl3_vsum(obj->pos_force, obj->pos_force, force_dir);
	
	return;
}

inline u8_t trcctrl_gms_reset(s_trcctrl_gms *self, s_trcobj *obj)
{
	__trcctrl_gms_calc__(self->ref, obj);
	
	return 0x00;
}

inline u8_t trcctrl_gms_reset_(void *data, void *obj)
{
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_gms_reset(ctrl, obj_);
}

inline u8_t trcctrl_gms_update(s_trcctrl_gms *self, s_trcobj *obj)
{
	__trcctrl_gms_calc__(self->ref, obj);
	
	return 0x00;
}

inline u8_t trcctrl_gms_update_(void *data, void *obj)
{
	s_trcctrl_gms *ctrl = (s_trcctrl_gms*) data;
	s_trcobj *obj_ = (s_trcobj*) obj;
	
	return trcctrl_gms_update(ctrl, obj_);
}


//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

#endif /* __trcctrl__ */

