
#include "trj_ctrl.h"

//------------------------------------------------------------------------------

void ins_ctrl_none_update(ins_ctrl_none_t *ctrl)
{
    return;
}

//------------------------------------------------------------------------------

void ins_ctrl_upos_update(void *__ctrl__)
{
    ins_ctrl_upos_t *ctrl = (ins_ctrl_upos_t*) __ctrl__;
    
    vlf_t dp0_p1[3];
    vlf_t dp0_p2[3];
    
    vl_vmul_s(dp0_p1, ctrl->parent->pos[1], ctrl->parent->time[1]);
    vl_vmul_s(dp0_p2, ctrl->parent->pos[2], ctrl->parent->time[2]);
    vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
    vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
    
    vlf_t dp1_p2[3];
    
    vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
    vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
    
    vlf_t dp2_f[3];
    
    vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
    vl_vcopy(ctrl->parent->pos[2], dp2_f);
    
    vlf_t dr0_r1[9];
    vlf_t dr0_r2[9];
    vlf_t r1_sk[9];
    vlf_t r2_sk[9];
    
    vl_skew(r1_sk, ctrl->parent->rot[1]);
    vl_skew(r2_sk, ctrl->parent->rot[2]);
    
    vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
    
    vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
    
    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
    
    vlf_t dr1_r2[3];
    
    vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
    vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
    vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
    
    vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);
    
    vl_rnorm(ctrl->parent->rot[0]);
    
    return;
}

//------------------------------------------------------------------------------

void ins_ctrl_uins_update(void *__ctrl__)
{
    ins_ctrl_uins_t *ctrl = (ins_ctrl_uins_t*) __ctrl__;
    
    vlf_t dp2_f[3];
    
    vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
    vl_vsum(ctrl->parent->pos[2], ctrl->parent->pos[2], dp2_f);
    
    vlf_t dp0_p1[3];
    vlf_t dp0_p2[3];
    
    vl_vmul_s(dp0_p1, ctrl->parent->pos[1], ctrl->parent->time[1]);
    vl_vmul_s(dp0_p2, ctrl->parent->pos[2], ctrl->parent->time[2]);
    vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
    vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
    
    vlf_t dp1_p2[3];
    
    vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
    vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
    
    vlf_t dr0_r1[9];
    vlf_t dr0_r2[9];
    vlf_t r1_sk[9];
    vlf_t r2_sk[9];
    
    vl_skew(r1_sk, ctrl->parent->rot[1]);
    vl_skew(r2_sk, ctrl->parent->rot[2]);
    
    vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
    
    vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
    
//    vl_mprint(ctrl->parent->rot[0]);
//
//    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
//    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
//
    vl_rnorm(ctrl->parent->rot[0]);
    
//    vl_mprint(ctrl->parent->rot[0]);
    
    return;
}

//------------------------------------------------------------------------------

void ins_ctrl_corb_get_pos(ins_ctrl_corb_t *ctrl, vlf_t *pos, vlf_t time)
{
    vlf_t angle = ctrl->rate * time;
    
    pos[0] = vl_sin(angle)*ctrl->radius;
    pos[1] = 0.0;
    pos[2] = vl_cos(angle)*ctrl->radius;
    
    vl_vsum(pos, pos, ctrl->ref->pos[0]);
    
    return;
}

void ins_ctrl_corb_update(void *__ctrl__)
{
    ins_ctrl_corb_t *ctrl = (ins_ctrl_corb_t*) __ctrl__;
    
    ins_ctrl_corb_get_pos(ctrl, ctrl->parent->pos[0], ctrl->parent->time[0]);
    
    vlf_t dr0_r1[9];
    vlf_t dr0_r2[9];
    vlf_t r1_sk[9];
    vlf_t r2_sk[9];
    
    vl_skew(r1_sk, ctrl->parent->rot[1]);
    vl_skew(r2_sk, ctrl->parent->rot[2]);
    
    vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
    
    vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
    vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
    
    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
    
    vlf_t dr1_r2[3];
    
    vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
    vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
    vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
    
    vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);

    vl_rnorm(ctrl->parent->rot[0]);
    
    return;
}

//------------------------------------------------------------------------------

//class trj_ctrl_cpos_point(object):
//
//    def __init__(self, time, pos):
//
//        self.time = time;
//        self.pos = numpy.array(pos);
//
//class trj_ctrl_cpos(object):
//
//    def __init__(self, parent, ref_obj, ref_points):
//
//        self.parent = parent;
//        self.ref_obj = ref_obj;
//        self.ref_points = ref_points;
//        self.ref_curves = [];
//        self.ref_basis  = [];
//        self.seg_offset = 0x00;
//
//        ref_points = [
//            trj_ctrl_cpos_point(0.0, ref_points[0].pos - (ref_points[1].pos - ref_points[0].pos))
//        ] + ref_points;
//
//        for i in range(1, len(ref_points)-1):
//
//            dist = scipy.linalg.norm(ref_points[i+1].pos - ref_points[i+0].pos);
//
//            e0 = ref_points[i+0].pos - ref_points[i-1].pos;
//            e1 = ref_points[i+1].pos - ref_points[i+0].pos;
//
//            nodes = numpy.asfortranarray([
//                [0.0, 0.0, 0.9, 1.0],
//                [0.0, 0.2, 0.0, 0.0],
//            ]);
//
//            self.ref_curves.append(bezier.Curve(nodes, degree=4));
//            self.ref_basis.append ([e0, e1]);
//
//        time_diff = self.ref_points[1].time - self.ref_points[0].time;
//
//        pos[0] = self.get_pos(0x00, 0.0);
//        pos[1] = (self.get_pos(0x00, 0.10 / time_diff) - self.get_pos(0x00, 0.0 / time_diff)) / 0.1;
//        pos_2 =((self.get_pos(0x00, 0.10 / time_diff) - self.get_pos(0x00, 0.05 / time_diff))
//              - (self.get_pos(0x00, 0.05 / time_diff) - self.get_pos(0x00, 0.00 / time_diff))) / 0.05;
//
//        parent.pos[0].append(pos[0]);
//        parent.pos[1].append(pos[1]);
//        parent.pos_2.append(pos_2);
//
//        parent.rot[0].append(numpy.mat(numpy.diag([1.0, 1.0, 1.0])));
//        parent.rot[1].append(numpy.array([0.0, 0.0, 0.0]));
//        parent.rot[2].append(numpy.array([0.0, 0.0, 0.0]));
//
//    def get_pos(self, seg_offset, time):
//
//        interp_data  = self.ref_curves[seg_offset].evaluate(time);
//        ref_pos         = self.ref_points[seg_offset].pos + interp_data[0] * self.ref_basis[seg_offset][1] + interp_data[1] * self.ref_basis[seg_offset][0];
//        # print(self.parent.ref_obj.pos[0]);
//        abs_pos      = self.parent.ref_obj.pos[0][-1] + self.parent.ref_obj.rot[0][-1].dot(ref_pos);
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
//        ref_pos[1]      = numpy.array(self.parent.ref_obj.rot[0][-1].I.dot(abs_pos - self.parent.ref_obj.pos[0][-1])).reshape(3);
//        ref_pos[0]      = numpy.array(self.parent.ref_obj.rot[0][-2].I.dot(self.parent.pos[0][-1] - self.parent.ref_obj.pos[0][-2])).reshape(3);
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
//        parent.pos[0].append(abs_pos);
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


