//
//// 2021 Softael LLC.
//// Created by staticocean on 11/11/2021
//
//// trcctrl - trace
//
//#ifndef __TRCCTRL___
//#define __TRCCTRL___
//
////------------------------------------------------------------------------------
//
//#include <sftlstd/vld3.h>
//
//#include "trc_api.h"
//#include "trcobj.h"
//
////------------------------------------------------------------------------------
//
//typedef struct __trcctrl_gm_k__
//{
//	t_u32 n;
//	t_u32 m;
//
//	t_f64 C;
//	t_f64 S;
//	t_f64 dC;
//	t_f64 dS;
//
//}   __s_trcctrl_gm_k__;
//
//typedef struct trcctrl_gm
//{
//	t_u32 order;
//	char file_name[256];
//
//	t_u32  data_size;
//	__s_trcctrl_gm_k__ *data_ls;
//
//}   s_trcctrl_gm;
//
//typedef struct trcctrl_gm_init
//{
//	t_u32 order;
//	char file_name[256];
//
//}   s_trcctrl_gm_init;
//
////------------------------------------------------------------------------------
//
//inline char *__sgets__(char *s, int n, const char **strp){
//	if(**strp == '\0')return NULL;
//	int i;
//	for(i=0;i<n-1;++i, ++(*strp)){
//		s[i] = **strp;
//		if(**strp == '\0')
//			break;
//		if(**strp == '\n'){
//			s[i+1]='\0';
//			++(*strp);
//			break;
//		}
//	}
//	if(i==n-1)
//		s[i] = '\0';
//	return s;
//}
//
//inline t_u8 trcctrl_gm_init(s_trcctrl_gm *self, s_trcctrl_gm_init attr)
//{
//	strcpy(self->file_name, attr.file_name);
//	self->order = attr.order;
//
//	FILE *file_handle = fopen(self->file_name, "r");
//
//	if (file_handle == NULL)
//	{
//		printf("default gm file not found");
//	}
//
//	else
//	{
//		fseek(file_handle , 0, SEEK_END);
//		size_t file_size = ftell (file_handle);
//		rewind (file_handle);
//
//		t_u8 *file_data = (t_u8*) malloc(file_size);
//		fread(file_data, 0x01, file_size, file_handle);
//
//		fclose(file_handle);
//
//		t_u32 line_count = 0x00;
//		t_u32 offset;
//
//		for (offset = 0x00; offset < file_size; ++offset)
//		{
//			if      (file_data[offset] == '\n') { ++line_count; }
//			else if (file_data[offset] == 'D') { file_data[offset] = 'E'; }
//		}
//
//		self->data_size = line_count;
//		self->data_ls = (__s_trcctrl_gm_k__*) malloc(sizeof(__s_trcctrl_gm_k__) * line_count);
//
//		line_count = 0x00;
//		__s_trcctrl_gm_k__ temp;
//
//		char buff[512];
//		const char **p = (const char**) &file_data;
//
//		while(NULL != __sgets__(buff, sizeof(buff), p))
//		{
//			sscanf(buff, "%d %d %lf %lf %lf %lf %*s",
//				   &temp.n, &temp.m, &temp.C, &temp.S, &temp.dC, &temp.dS);
//
//			self->data_ls[line_count] = temp;
//
//			++line_count;
//		}
//
//		printf("loaded %d lines \n", line_count);
//		fflush(stdout);
//	}
//
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_save(s_trcctrl_gm *self, s_trcctrl_gm_init *attr, t_u8 **v_file)
//{
//	memcpy(*v_file, self->data_ls, self->data_size * sizeof(__s_trcctrl_gm_k__));
//	*v_file += self->data_size * sizeof(__s_trcctrl_gm_k__);
//
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_load(s_trcctrl_gm *self, s_trcctrl_gm_init *attr, t_u8 **v_file)
//{
//	self->data_ls = (__s_trcctrl_gm_k__*) malloc(self->data_size * sizeof(__s_trcctrl_gm_k__));
//	memcpy(self->data_ls, *v_file, self->data_size * sizeof(__s_trcctrl_gm_k__));
//	*v_file += self->data_size * sizeof(__s_trcctrl_gm_k__);
//
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_reset(s_trcctrl_gm *self, s_trcobj *obj)
//{
//	return 0x00;
//}
//
//inline int factorial(int n) {
//	int result = 1;
//	if(n<0)
//		return -1;
//	for (int i = 1; i <= n; ++i)
//		result *= i;
//	return result;
//}
//
//inline void legendre_order0(t_f64 *L, t_f64 *Ld, t_u32 N, t_f64 x)
//{
//	L[0] = 1;
//	L[1] = x;
//	Ld[0] = 0;
//	Ld[1] = 1;
//
//	t_u32 i;
//	t_u32 n;
//
//	for (i = 2; i < N; ++i) {
//
//		n = i - 2;
//
//		L[i] = ((2 * n + 1) * x * L[i - 1] - n * L[i - 2]) / (n + 1);
//		Ld[i] = ((2 * n + 1) * (L[i - 1] + x * Ld[i - 1]) - n * Ld[i - 2]) / (n + 1);
//	}
//
//	return;
//}
//
//inline void associated_legendre(t_f64 *P, t_f64 *Pd, t_u32 N, t_f64 x)
//{
//	// Evaluates all fully normalized associated Legendre polynomials
//	// of degree and order less than or equal to N-1, and their first derivatives at x.
//	//
//	// For n greater than or equal to m:
//	// P(m+1,n+1) = P_n^m(x)
//	// Pd(m+1, n+1) = d/dx P_n^m(x)
//
//	t_u32 i;
//	t_u32 j;
//	t_u32 m;
//	t_u32 n;
//
//	t_f64 full_derivs[N][N];
//
//	// compute P^n_0
//
//	t_f64 L[N];
//	t_f64 Ld[N];
//	legendre_order0(L, Ld, N,x);
//
//	memcpy(&P[0], L, sizeof(t_f64)*N);
//	memcpy(&Pd[0], Ld, sizeof(t_f64)*N);
//
//	// calculate all derivatives up to order N of of P^n_0
//	memcpy(&full_derivs[0][0], Ld, sizeof(t_f64)*N);
//
//	for (j = 1; j < N; ++j)
//	{
//		for (i = 2; i < N; ++i)
//		{
//			n = i - 2;
//
//			full_derivs[j][i] = ((2*n + 1)*(j*full_derivs[j-1][i-1]+ x*full_derivs[j][i-1])
//								 - n*full_derivs[j][i-2]) / (n + 1);
//		}
//	}
//
//	// compute P^m_n
//
//	for (m = 1; m < N; ++m)
//	{
//		for (n = m; n < N; ++n)
//		{
//			t_f64 factor = 1;
//
//			P[m*N+n] = factor * vl_pow((1 - x*x), ((m - 1) / 2)) * full_derivs[m - 1][n];
//			Pd[m*N+n] = -factor * ((m - 1) / 2) * pow((1 - x*x), (((m - 1) / 2) - 1)) * 2 * x
//						+ factor * pow((1 - x*x), ((m - 1) / 2)) * full_derivs[m][n];
//		}
//	}
//
//	// normalize values
//
//	for (n = 0; n < N; ++n)
//	{
//		for (m = 0; m < N; ++m)
//		{
//			if (n >= m)
//			{
//				t_f64 factor = (2 * n + 1) * factorial(n - m) / factorial(n + m);
//				if (m != 0) { factor = factor * 2; }
//				factor = sqrt(factor);
//
//				P[m*N+n] *= factor;
//				Pd[m*N+n] *= factor;
//			}
//		}
//	}
//
//	return;
//}
//
//inline t_u8 trcctrl_gm_calc(s_trcctrl_gm *self, t_f64 *g, t_f64 *ecef)
//{
//	t_f64 lla[3];
//
//	trcellp_glla(&trcellp_wgs84, lla, ecef);
//
//	t_f64 MU = 3.986004415e14;      // Gravitational parameter of Earth    [km^3 s^-2]
//	t_f64 A  = 6.3781363e6;          // Mean equitorial radius              [m]
//
//	t_f64 el = lla[0];
//	t_f64 az = lla[1];
//	t_f64 r  = vld3v_norm(ecef);
//
//	t_f64 u = MU / r;
//	t_f64 g_[3];
//	g_[0] = -MU / (r*r);
//
//	t_u32 N = self->order;
//
//	t_f64 P[N*N];
//	t_f64 Pd[N*N];
//
//	associated_legendre(P, Pd, N, sin(el));
//
//	t_u32 i;
//	t_u32 N_ = 0x00;
//
//	for (t_s32 i = 0; i < self->order+1; ++i)
//	{
//		N_ += i;
//	}
//
//	for (t_s32 i = 0; i < N_-3; ++i)
//	{
//		__s_trcctrl_gm_k__ *k = &self->data_ls[i];
//
//		t_u32 n = k->n;
//		t_u32 m = k->m;
//		t_f64    C = k->C;
//		t_f64    S = k->S;
//
//		t_f64 Pnm  = P [m*N + n];
//		t_f64 Pnmd = Pd[m*N + n];
//
//		u = u + vl_pow(A,n) * MU * (Pnm * (C * cos(m * az) + S * sin(m * az))) / vl_pow(r, n+1);
//
//		g_[0] -= vl_pow(A,n) * (n + 1) * MU * Pnm * (C * cos(m * az) + S * sin(m * az)) / vl_pow(r, (n + 2));
//		g_[1] += vl_pow(A,n) * MU * Pnmd * cos(el) * (C * cos(m * az) + S * sin(m * az)) / vl_pow(r, (n + 2));
//		g_[2] += vl_pow(A,n) * MU * m * Pnm * (S * cos(m * az) - C * sin(m * az)) / vl_pow(r, (n + 2) * sin(el));
//	}
////
////    t_f64 rot[9] = {
////            sin(el)*cos(az), cos(el)*cos(az), -sin(az),
////            sin(el)*sin(az), cos(el)*sin(az),  cos(az),
////            cos(el),         -sin(el),        0
////    };
////
////    vl_mmul_v(g, rot, g_);
//
//	vld3v_copy(g, g_);
//
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_update(s_trcctrl_gm *self, s_trcobj *obj)
//{
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_init_(void **data, void *config)
//{
//	*data = (s_trcctrl_gm*) malloc(sizeof(s_trcctrl_gm));
//
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) *data;
//	s_trcctrl_gm_init *init = (s_trcctrl_gm_init*) config;
//
//	return trcctrl_gm_init(ctrl, *init);
//}
//
//inline t_u8 trcctrl_gm_free_(void **data)
//{
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) *data;
//
//	if (ctrl->data_ls != NULL)
//	{
//		free(ctrl->data_ls);
//		ctrl->data_ls = NULL;
//	}
//
//	free(ctrl);
//
//	return 0x00;
//}
//
//inline t_u8 trcctrl_gm_save_(void *data, void *config, t_u8 **v_file)
//{
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) data;
//	s_trcctrl_gm_init *attr = (s_trcctrl_gm_init*) config;
//
//	return trcctrl_gm_save(ctrl, attr, v_file);
//}
//
//inline t_u8 trcctrl_gm_load_(void *data, void *config, t_u8 **v_file)
//{
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) data;
//	s_trcctrl_gm_init *attr = (s_trcctrl_gm_init*) config;
//
//	return trcctrl_gm_load(ctrl, attr, v_file);
//}
//
//inline t_u8 trcctrl_gm_reset_(void *data, void *obj)
//{
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) data;
//	s_trcobj *obj_ = (s_trcobj*) obj;
//
//	return trcctrl_gm_reset(ctrl, obj_);
//}
//
//inline t_u8 trcctrl_gm_update_(void *data, void *obj)
//{
//	s_trcctrl_gm *ctrl = (s_trcctrl_gm*) data;
//	s_trcobj *obj_ = (s_trcobj*) obj;
//
//	return trcctrl_gm_update(ctrl, obj_);
//}
//
////------------------------------------------------------------------------------
//
//typedef struct trcctrl_varot
//{
//    s_trceng 	*eng;
//    s_trcobj 	*ref;
//    t_u32 	ref_hash;
//
//    t_u8 	ellp_en;
//    s_trcellp 	*ellp;
//    t_u32 	ellp_hash;
//
//}   s_trcctrl_varot;
//
//typedef struct trcctrl_varot_attr
//{
//    s_trceng *eng;
//    s_trcobj *ref;
//
//}   s_trcctrl_varot_attr;
//
////------------------------------------------------------------------------------
//
//inline t_u8 trcctrl_varot_init(s_trcctrl_varot *self, s_trcctrl_varot_attr attr)
//{
//    self->eng = attr.eng;
//
//    self->ref = attr.ref;
//    if (self->ref != NULL)
//    { self->ref_hash = self->ref->hash; }
//
//    self->ellp_en = 0x00;
//    self->ellp = NULL;
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_save(s_trcctrl_varot *self, s_trcctrl_varot_attr *attr, t_u8 **v_file)
//{
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_load(s_trcctrl_varot *self, s_trcctrl_varot_attr *attr, t_u8 **v_file)
//{
//    self->eng = attr->eng;
//
//    self->ref  = trceng_find_obj (self->eng, self->ref_hash);
//    self->ellp = trceng_find_ellp(self->eng, self->ellp_hash);
//
//    if (self->ref == NULL)
//    {
//        self->ref_hash = 0x00;
//    }
//
//    if (self->ellp == NULL)
//    {
//        self->ellp_hash = 0x00;
//        self->ellp_en = 0x00;
//    }
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_reset(s_trcctrl_varot *self, s_trcobj *obj)
//{
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_update(s_trcctrl_varot *self, s_trcobj *obj)
//{
//    t_f64 local_pos_0[3];
//    t_f64 local_pos_1[3];
//    t_f64 local_dpos[3];
//
//    if (obj->log_sz > 0x00)
//    {
//        vld3v_subv(local_pos_0,
//                 &obj->log_ls[obj->log_sz-1].pos[0][0],
//                        &self->ref->log_ls[self->ref->log_sz-1].pos[0][0]);
//
//        vld3m_tmulv(local_pos_0,
//                    &self->ref->log_ls[self->ref->log_sz-1].rot[0][0], local_pos_0);
//
//        vld3v_subv(local_pos_1, &obj->pos[0][0], &self->ref->pos[0][0]);
//        vld3m_tmulv(local_pos_1, &self->ref->rot[0][0], local_pos_1);
//
//        vld3v_subv(local_dpos, local_pos_1, local_pos_0);
//
//        if (vld3v_norm(local_dpos) > 1E-3)
//        {
//            vld3v_muls(local_dpos, local_dpos, 1.0 / vld3v_norm(local_dpos));
//
//            if (self->ellp_en != 0x00)
//            {
//                t_f64 ecef_rot[9];
//                t_f64 nhw_dpos[3];
//
//                // Get local ctn matrix (hor->ecef)
//                trcellp_ecefrot(self->ellp, local_pos_1, ecef_rot);
//                // Get velocity in hor CS
//                vld3m_tmulv(nhw_dpos, ecef_rot, local_dpos);
//
//                t_f64 rot_nwh_tnp[9] = {
//                        nhw_dpos[0], nhw_dpos[1], nhw_dpos[2],
//                        0.0, 1.0, 0.0,
//                        0.0, 0.0, 0.0,
//                        };
//
//                t_f64 *x = &rot_nwh_tnp[0];
//                t_f64 *y = &rot_nwh_tnp[3];
//                t_f64 *z = &rot_nwh_tnp[6];
//
//                vld3v_muls(x, x, 1.0 / vld3v_norm(x));
//
//                t_f64 xy[3];
//                vld3v_muls(xy, x, vld3v_dot(x,y));
//                vld3v_subv(y, y, xy);
//                vld3v_muls(y, y, 1.0 / vld3v_norm(y));
//
//                vld3v_cross(z, x, y);
//                vld3v_muls(z, z, 1.0 / vld3v_norm(z));
//
//                t_f64 rot_nhw[9];
//                vld3m_tnp(rot_nhw, rot_nwh_tnp);
//
//                s_vl_hpr rot_nwh_hpr = vl_hpr(rot_nhw);
//                rot_nwh_hpr.roll = 0.0;
//                vl3_rot(rot_nhw, rot_nwh_hpr);
//
//                t_f64 r_ref[9];
//                vld3m_mulm(r_ref, ecef_rot, rot_nhw);
//                vld3m_mulm(&obj->rot[0][0], &self->ref->rot[0][0], r_ref);
//
//                vld3m_rnorm(&obj->rot[0][0]);
//
//                if (obj->log_sz == 0x01)
//                {
//                    vld3m_copy(&obj->log_ls[obj->log_sz-1].rot[0][0], &obj->rot[0][0]);
//                }
//            }
//        }
//    }
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_init_(void **data, void *config)
//{
//    *data = (s_trcctrl_varot*) malloc(sizeof(s_trcctrl_varot));
//
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) *data;
//    s_trcctrl_varot_attr *attr = (s_trcctrl_varot_attr*) config;
//
//    return trcctrl_varot_init(ctrl, *attr);
//}
//
//inline t_u8 trcctrl_varot_free_(void **data)
//{
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) *data;
//    free(ctrl);
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_varot_save_(void *data, void *config, t_u8 **v_file)
//{
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) data;
//    s_trcctrl_varot_attr *attr = (s_trcctrl_varot_attr*) config;
//
//    return trcctrl_varot_save(ctrl, attr, v_file);
//}
//
//inline t_u8 trcctrl_varot_load_(void *data, void *config, t_u8 **v_file)
//{
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) data;
//    s_trcctrl_varot_attr *attr = (s_trcctrl_varot_attr*) config;
//
//    return trcctrl_varot_load(ctrl, attr, v_file);
//}
//
//inline t_u8 trcctrl_varot_reset_(void *data, void *obj)
//{
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) data;
//    s_trcobj *obj_ = (s_trcobj*) obj;
//
//    return trcctrl_varot_reset(ctrl, obj_);
//}
//
//inline t_u8 trcctrl_varot_update_(void *data, void *obj)
//{
//    s_trcctrl_varot *ctrl = (s_trcctrl_varot*) data;
//    s_trcobj *obj_ = (s_trcobj*) obj;
//
//    return trcctrl_varot_update(ctrl, obj_);
//}
//
////------------------------------------------------------------------------------
//
////------------------------------------------------------------------------------
//
////
////        parent = self.parent;
////
////        if (self.seg_offset+1) < len(self.ref_points) and (self.parent.time_0[-1] > self.ref_points[self.seg_offset+1].time):
////            self.seg_offset += 1;
////
////        interp_state = (self.parent.time_0[-1] - self.ref_points[self.seg_offset].time) / (self.ref_points[self.seg_offset+1].time - self.ref_points[self.seg_offset].time);
////        abs_pos      = self.get_pos(self.seg_offset, interp_state);
////        ref_pos_1      = numpy.array(self.parent.ref_obj.rot_0[-1].I.dot(abs_pos - self.parent.ref_obj.pos_0[-1])).reshape(3);
////        ref_pos_0      = numpy.array(self.parent.ref_obj.rot_0[-2].I.dot(self.parent.pos_0[-1] - self.parent.ref_obj.pos_0[-2])).reshape(3);
////
////        # print(ref_pos_1 - ref_pos_0);
////        # print()
////        # print(numpy.mat(self.parent.rot_0[-1][0]));
////        # print(self.parent.rot_0[-1][0][0], self.parent.rot_0[-1][0][1], self.parent.rot_0[-1][0][2]);
////        # print(self.parent.rot_0[-1]);
////
////        e0 = numpy.array(numpy.mat(self.parent.rot_0[-1].T[0,:])).reshape(3);
////        e0 /= scipy.linalg.norm(e0);
////
////        e1 = numpy.array(self.ref_obj.rot_0[-1].dot(ref_pos_1 - ref_pos_0)).reshape(3);
////        e1 /= scipy.linalg.norm(e1);
////
////        # print(e0, e1);
////
////        c  = numpy.dot(e0, e1);
////
////        # abs_rot = self.parent.rot_0[-1];
////        abs_rot = self.ref_obj.rot_0[-1].dot(self.ref_obj.rot_0[-2].I.dot(self.parent.rot_0[-1]));
////        # abs_rot_1 = numpy.array([0.0, 0.0, 0.0]);
////
////        if abs(c) > 0 and abs(1+c) > 0:# and scipy.linalg.norm(ref_pos_1 - ref_pos_0) > 1E-6:
////
////            # print('adjust', scipy.linalg.norm(ref_pos_1 - ref_pos_0))
////
////            v  = numpy.cross(e0, e1);
////            s  = scipy.linalg.norm(v);
////            sv = trc_utils.skew(v);
////
////            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
////            R  = trc_utils.unskew(R);
////
////            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
////                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
////
////            R = trc_utils.skew(R);
////
////            abs_rot = trc_utils.rot_norm(parent.rot_0[-1] + R.dot(parent.rot_0[-1]));
////            # abs_rot_1 = trc_utils.unskew(sv + sv.dot(sv) / (1 + c));
////
////        # print(v)
////
////        # print(scipy.linalg.norm(R.dot(e0) - e1));
////        # print(scipy.linalg.norm(e0 - e1));
////
////        # print(trc_utils.rot_norm(R.dot(parent.rot_0[-1])) - parent.rot_0[-1]);
////
////        parent.pos_0.append(abs_pos);
////        # parent.rot_0.append(numpy.mat(parent.rot_0[-1]));
////        # parent.rot_0.append(trc_utils.rot_norm(parent.rot_0[-1] + trc_utils.skew(v).dot(parent.rot_0[-1])));
////        # parent.rot_0.append(trc_utils.rot_norm(R.dot(parent.rot_0[-1])));
////        parent.rot_0.append(trc_utils.rot_norm(abs_rot));
////        # parent.rot_0.append(trc_utils.rot_norm(parent.rot_0[-1] + parent.rot_0[-1].dot(R)));
////
////        # e0 = numpy.array(numpy.mat(self.parent.rot_0[-1].T[0,:])).reshape(3);
////        # e0 /= scipy.linalg.norm(e0);
////
////        # e1 = numpy.array(self.ref_obj.rot_0[-1].dot(ref_pos_1 - ref_pos_0)).reshape(3);
////        # e1 /= scipy.linalg.norm(e1);
////
////        # print(e0, e1);
////
////        return;
//
////------------------------------------------------------------------------------
//
////	t_f64 dp0_p1[3];
////	t_f64 dp0_p2[3];
////
////	vl_vmul_s(dp0_p1, self->parent->pos[1], self->parent->time[1]);
////	vl_vmul_s(dp0_p2, self->parent->pos[2], self->parent->time[2]);
////	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
////	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
////
////	t_f64 dp1_p2[3];
////
////	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
////	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
////
////	t_f64 dp2_f[3];
////
////	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
////	vl_vcopy(ctrl->parent->pos[2], dp2_f);
////
////	t_f64 dr0_r1[9];
////	t_f64 dr0_r2[9];
////	t_f64 r1_sk[9];
////	t_f64 r2_sk[9];
////
////	vl_skew(r1_sk, ctrl->parent->rot[1]);
////	vl_skew(r2_sk, ctrl->parent->rot[2]);
////
////	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
////
////	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
////
////	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
////	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
////
////	t_f64 dr1_r2[3];
////
////	vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
////	vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
////	vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
////
////	vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);
////
////	vl_rnorm(ctrl->parent->rot[0]);
////
////	return;
////}
//
////------------------------------------------------------------------------------
//
////inline void ins_ctrl_uins_update(void *__ctrl__)
////{
////	ins_ctrl_uins_t *ctrl = (ins_ctrl_uins_t*) __ctrl__;
////
////	t_f64 dp2_f[3];
////
////	vl_vmul_s(dp2_f, ctrl->parent->pos_force, 1 / ctrl->parent->pos_inert);
////	vl_vsum(ctrl->parent->pos[2], ctrl->parent->pos[2], dp2_f);
////
////	t_f64 dp0_p1[3];
////	t_f64 dp0_p2[3];
////
////	vl_vmul_s(dp0_p1, ctrl->parent->pos[1], ctrl->parent->time[1]);
////	vl_vmul_s(dp0_p2, ctrl->parent->pos[2], ctrl->parent->time[2]);
////	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p1);
////	vl_vsum(ctrl->parent->pos[0], ctrl->parent->pos[0], dp0_p2);
////
////	t_f64 dp1_p2[3];
////
////	vl_vmul_s(dp1_p2, ctrl->parent->pos[2], ctrl->parent->time[1]);
////	vl_vsum(ctrl->parent->pos[1], ctrl->parent->pos[1], dp1_p2);
////
////	t_f64 dr0_r1[9];
////	t_f64 dr0_r2[9];
////	t_f64 r1_sk[9];
////	t_f64 r2_sk[9];
////
////	vl_skew(r1_sk, ctrl->parent->rot[1]);
////	vl_skew(r2_sk, ctrl->parent->rot[2]);
////
////	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
////
////	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
////
//////    vl_mprint(ctrl->parent->rot[0]);
//////
//////    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
//////    vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
//////
////	vl_rnorm(ctrl->parent->rot[0]);
////
//////    vl_mprint(ctrl->parent->rot[0]);
////
////	return;
////}
//
////------------------------------------------------------------------------------
//
////inline void ins_ctrl_corb_update(void *__ctrl__)
////{
////	ins_ctrl_corb_t *ctrl = (ins_ctrl_corb_t*) __ctrl__;
////
////	ins_ctrl_corb_get_pos(ctrl, ctrl->parent->pos[0], ctrl->parent->time[0]);
////
////	t_f64 dr0_r1[9];
////	t_f64 dr0_r2[9];
////	t_f64 r1_sk[9];
////	t_f64 r2_sk[9];
////
////	vl_skew(r1_sk, ctrl->parent->rot[1]);
////	vl_skew(r2_sk, ctrl->parent->rot[2]);
////
////	vl_mmul_m(dr0_r1, r1_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r1, dr0_r1, ctrl->parent->time[1]);
////
////	vl_mmul_m(dr0_r2, r2_sk, ctrl->parent->rot[0]);
////	vl_mmul_s(dr0_r2, dr0_r2, ctrl->parent->time[2]);
////
////	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r1);
////	vl_msum(ctrl->parent->rot[0], ctrl->parent->rot[0], dr0_r2);
////
////	t_f64 dr1_r2[3];
////
////	vl_mmul_v(dr1_r2, r2_sk, ctrl->parent->rot[1]);
////	vl_mmul_s(dr1_r2, dr1_r2, ctrl->parent->time[1]);
////	vl_msum(ctrl->parent->rot[1], ctrl->parent->rot[1], dr1_r2);
////
////	vl_mmul_s(ctrl->parent->rot[2], ctrl->parent->rot_force, 1 / ctrl->parent->rot_inert);
////
////	vl_rnorm(ctrl->parent->rot[0]);
////
////	return;
////}
//
////------------------------------------------------------------------------------
//
////    def get_pos(self, seg_offset, time):
////
////        interp_data  = self.ref_curves[seg_offset].evaluate(time);
////        ref_pos         = self.ref_points[seg_offset].pos_p + interp_data[0] * self.ref_basis[seg_offset][1] + interp_data[1] * self.ref_basis[seg_offset][0];
////        # print(self.parent.ref_obj.pos_p[0]);
////        abs_pos      = self.parent.ref_obj.pos_p[0][-1] + self.parent.ref_obj.rot[0][-1].dot(ref_pos);
////
////        return numpy.array(abs_pos).reshape(3);
////
////    def update(self):
////
////        parent = self.parent;
////
////        if (self.seg_offset+1) < len(self.ref_points) and (self.parent.time_0[-1] > self.ref_points[self.seg_offset+1].time):
////            self.seg_offset += 1;
////
////        interp_state = (self.parent.time_0[-1] - self.ref_points[self.seg_offset].time) / (self.ref_points[self.seg_offset+1].time - self.ref_points[self.seg_offset].time);
////        abs_pos      = self.get_pos(self.seg_offset, interp_state);
////        ref_pos[1]      = numpy.array(self.parent.ref_obj.rot[0][-1].I.dot(abs_pos - self.parent.ref_obj.pos_p[0][-1])).reshape(3);
////        ref_pos[0]      = numpy.array(self.parent.ref_obj.rot[0][-2].I.dot(self.parent.pos_p[0][-1] - self.parent.ref_obj.pos_p[0][-2])).reshape(3);
////
////        # print(ref_pos[1] - ref_pos[0]);
////        # print()
////        # print(numpy.mat(self.parent.rot[0][-1][0]));
////        # print(self.parent.rot[0][-1][0][0], self.parent.rot[0][-1][0][1], self.parent.rot[0][-1][0][2]);
////        # print(self.parent.rot[0][-1]);
////
////        e0 = numpy.array(numpy.mat(self.parent.rot[0][-1].T[0,:])).reshape(3);
////        e0 /= scipy.linalg.norm(e0);
////
////        e1 = numpy.array(self.ref_obj.rot[0][-1].dot(ref_pos[1] - ref_pos[0])).reshape(3);
////        e1 /= scipy.linalg.norm(e1);
////
////        # print(e0, e1);
////
////        c  = numpy.dot(e0, e1);
////
////        # abs_rot = self.parent.rot[0][-1];
////        abs_rot = self.ref_obj.rot[0][-1].dot(self.ref_obj.rot[0][-2].I.dot(self.parent.rot[0][-1]));
////        # abs_rot[1] = numpy.array([0.0, 0.0, 0.0]);
////
////        if abs(c) > 0 and abs(1+c) > 0:# and scipy.linalg.norm(ref_pos[1] - ref_pos[0]) > 1E-6:
////
////            # print('adjust', scipy.linalg.norm(ref_pos[1] - ref_pos[0]))
////
////            v  = numpy.cross(e0, e1);
////            s  = scipy.linalg.norm(v);
////            sv = trc_utils.skew(v);
////
////            R  = numpy.diag([1.0, 1.0, 1.0]) + sv + sv.dot(sv) / (1 + c);
////            R  = trc_utils.unskew(R);
////
////            if scipy.linalg.norm(R) > math.radians(180) * (self.parent.time_1[-1]):
////                R = R * (math.radians(180) * (self.parent.time_1[-1]) / scipy.linalg.norm(R));
////
////            R = trc_utils.skew(R);
////
////            abs_rot = trc_utils.rot_norm(parent.rot[0][-1] + R.dot(parent.rot[0][-1]));
////            # abs_rot[1] = trc_utils.unskew(sv + sv.dot(sv) / (1 + c));
////
////        # print(v)
////
////        # print(scipy.linalg.norm(R.dot(e0) - e1));
////        # print(scipy.linalg.norm(e0 - e1));
////
////        # print(trc_utils.rot_norm(R.dot(parent.rot[0][-1])) - parent.rot[0][-1]);
////
////        parent.pos_p[0].append(abs_pos);
////        # parent.rot[0].append(numpy.mat(parent.rot[0][-1]));
////        # parent.rot[0].append(trc_utils.rot_norm(parent.rot[0][-1] + trc_utils.skew(v).dot(parent.rot[0][-1])));
////        # parent.rot[0].append(trc_utils.rot_norm(R.dot(parent.rot[0][-1])));
////        parent.rot[0].append(trc_utils.rot_norm(abs_rot));
////        # parent.rot[0].append(trc_utils.rot_norm(parent.rot[0][-1] + parent.rot[0][-1].dot(R)));
////
////        # e0 = numpy.array(numpy.mat(self.parent.rot[0][-1].T[0,:])).reshape(3);
////        # e0 /= scipy.linalg.norm(e0);
////
////        # e1 = numpy.array(self.ref_obj.rot[0][-1].dot(ref_pos[1] - ref_pos[0])).reshape(3);
////        # e1 /= scipy.linalg.norm(e1);
////
////        # print(e0, e1);
////
////        return;
//
//
////------------------------------------------------------------------------------
//
//typedef struct trcctrl_egmsnpo
//{
//    s_trceng *eng;
//    s_trcobj *ref;
//    t_u32 ref_hash;
//
//}   s_trcctrl_egmsnpo;
//
//
//typedef struct trcctrl_egmsnpo_init
//{
//    s_trceng *eng;
//    s_trcobj *ref;
//
//}   s_trcctrl_egmsnpo_init;
//
//inline t_u8 trcctrl_egmsnpo_init (s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init attr)
//{
//    self->eng = attr.eng;
//    self->ref = attr.ref;
//
//    if (self->ref != NULL) { self->ref_hash = self->ref->hash; }
//    else { self->ref_hash = 0x00; }
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_egmsnpo_save (s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init *attr, t_u8 **v_file)
//{
//    return 0x00;
//}
//
//inline t_u8 trcctrl_egmsnpo_load(s_trcctrl_egmsnpo *self, s_trcctrl_egmsnpo_init *attr, t_u8 **v_file)
//{
//    self->eng = attr->eng;
//
//    self->ref = trceng_find_obj (self->eng, self->ref_hash);
//
//    if (self->ref == NULL) { self->ref_hash = 0x00; }
//
//    return 0x00;
//}
//
//inline static void __trcctrl_egmsnpo_calc__(s_trcobj *ref, s_trcobj *obj)
//{
//    const t_f64 mu = 398600.44E+9;
//    const t_f64 e = 2.633267E+25;
//
////	R = norm(ecef) + alt_mem;
////	gr = - mu/R^2 - e/R^4 * (1-3*sin(lat_mem)^2);
////	gm = - e/R^4 * sin(2*lat_mem);
////	gh = -gr;
////	gn = -gm;
////
////	acc_e = 0  - centr_acc(3);
////	acc_n = gn - centr_acc(1);
////	acc_h = gh - centr_acc(2);
//
//    t_f64 ecef[3];
//
//    vld3v_subv(ecef, &obj->pos[0][0], &ref->pos[0][0]);
//    vld3m_tmulv(ecef, &ref->rot[0][0], ecef);
//
//    t_f64 lla[3];
//
//    trcellp_lla(&trcellp_pz90_11, lla, ecef);
//
//    t_f64 r = vld3v_norm(ecef);
//    t_f64 gr = -mu / (r*r) - e / (r*r*r*r) * (1-3*sin(lla[0])*sin(lla[0]));
//    t_f64 gm = -e / (r*r*r*r) * sin(2*lla[0]);
//    t_f64 gh = -gr;
//    t_f64 gn = -gm;
//
//    t_f64 g_hor[3] = {
//            gn, gh, 0.0
//    };
//
//    vld3v_muls(g_hor, g_hor, -1.0);
//
//    t_f64 ecef_ctn[9];
//    trcellp_ecefrot(&trcellp_pz90, ecef, ecef_ctn);
//
//    t_f64 g_ecef[3];
//    vld3m_mulv(g_ecef, ecef_ctn, g_hor);
//
//    t_f64 g_inert[3];
//    vld3m_mulv(g_inert, &ref->rot[0][0], g_ecef);
//    vld3v_muls(g_inert, g_inert, 1.0 * obj->pos_inert);
//
//    vl3_vsum(obj->pos_force, obj->pos_force, g_inert);
//}
//
//inline t_u8 trcctrl_egmsnpo_reset(s_trcctrl_egmsnpo *self, s_trcobj *obj)
//{
//    __trcctrl_egmsnpo_calc__(self->ref, obj);
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_egmsnpo_update(s_trcctrl_egmsnpo *self, s_trcobj *obj)
//{
//    __trcctrl_egmsnpo_calc__(self->ref, obj);
//
//    return 0x00;
//}
//
////------------------------------------------------------------------------------
//
//inline t_u8 trcctrl_egmsnpo_init_(void **data, void *config)
//{
//    *data = (s_trcctrl_egmsnpo*) malloc(sizeof(s_trcctrl_egmsnpo));
//
//    s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) *data;
//    s_trcctrl_egmsnpo_init *init = (s_trcctrl_egmsnpo_init*) config;
//
//    return trcctrl_egmsnpo_init(ctrl, *init);
//}
//
//inline t_u8 trcctrl_egmsnpo_free_(void **data)
//{
//    s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) *data;
//    free(ctrl);
//
//    return 0x00;
//}
//
//inline t_u8 trcctrl_egmsnpo_save_(void *data, void *config, t_u8 **v_file)
//{
//    return 0x00;
//}
//
//inline t_u8 trcctrl_egmsnpo_load_(void *data, void *config, t_u8 **v_file)
//{
//    s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
//    s_trcctrl_egmsnpo_init *attr = (s_trcctrl_egmsnpo_init*) config;
//
//    return trcctrl_egmsnpo_load(ctrl, attr, v_file);
//}
//
//inline t_u8 trcctrl_egmsnpo_reset_(void *data, void *obj)
//{
//    s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
//    s_trcobj *obj_ = (s_trcobj*) obj;
//
//    return trcctrl_egmsnpo_reset(ctrl, obj_);
//}
//
//inline t_u8 trcctrl_egmsnpo_update_(void *data, void *obj)
//{
//    s_trcctrl_egmsnpo *ctrl = (s_trcctrl_egmsnpo*) data;
//    s_trcobj *obj_ = (s_trcobj*) obj;
//
//    return trcctrl_egmsnpo_update(ctrl, obj_);
//}
//
////------------------------------------------------------------------------------
//
//#endif /* __INS_CTRL__ */
//
//
//
//
