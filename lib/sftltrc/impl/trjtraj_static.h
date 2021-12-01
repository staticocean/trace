
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/vl.h>

#include <sftltrc/trctraj.h>
#include <sftltrc/trcellp.h>

//------------------------------------------------------------------------------

typedef struct trctraj_static
{
	s_trctraj 		super;
	
	s_trceng 		*eng;
	
	s_trcobj 		*ref;
	u32_t 			ref_hash;
	
	u8_t 			ellp_en;
	s_trcellp 		*ellp;
	u32_t 			ellp_hash;
	
	f64_t 			pos[3];
	f64_t 			pos_cache[3];
	f64_t 			rot[9];
	f64_t 			rot_cache[9];
	
} 	s_trctraj_static;

typedef struct trctraj_static_init
{
	s_trceng 	*eng;
	
	s_trcobj 	*ref;
	
	u8_t 	ellp_en;
	s_trcellp  *ellp;
	
	f64_t 		pos[3];
	f64_t 		rot[9];
	
} 	s_trctraj_static_init;

//------------------------------------------------------------------------------

inline u8_t trctraj_static_init(s_trctraj_static *self, s_trctraj_static_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	self->ref_hash = self->ref->hash;
	
	self->ellp_en = attr.ellp_en;
	self->ellp = attr.ellp;
	if (self->ellp != NULL)
	{ self->ellp_hash = self->ellp->hash; }
	
	vl3_vcopy(self->pos, attr.pos);
	vl3_mcopy(self->rot, attr.rot);
	
	return 0x00;
}

inline u8_t trctraj_static_save(s_trctraj_static *self, s_trctraj_static_init *attr, u8_t **v_file)
{
	return 0x00;
}

inline u8_t trctraj_static_load(s_trctraj_static *self, s_trctraj_static_init *attr, u8_t **v_file)
{
	self->eng = attr->eng;
	self->ref = trceng_find_obj(self->eng, self->ref_hash);
	self->ellp = trceng_find_ellp(self->eng, self->ellp_hash);
	
	return 0x00;
}

inline u8_t trctraj_static_compile(s_trctraj_static *self)
{
	if (self->ellp_en != 0x00)
	{
		trcellp_ecef(self->ellp, self->pos_cache, self->pos);
		
		f64_t ecef_ctn[9];
		trcellp_ecefrot(self->ellp, self->pos_cache, ecef_ctn);
		
		vl3_mmul_m(self->rot_cache, ecef_ctn, self->rot);
	}
	
	return 0x00;
}

inline u8_t trctraj_static_pos(s_trctraj_static *self, f64_t time, f64_t *pos)
{
	if (self->ellp_en == 0x00)
	{ vl3_vcopy(pos, self->pos); }
	
	else
	{ vl3_vcopy(pos, self->pos_cache); }
	
	vl3_mmul_v(pos, &self->ref->rot[0][0], pos);
	vl3_vsum(pos, pos, &self->ref->pos[0][0]);
	
	return 0x00;
}

inline u8_t trctraj_static_rot(s_trctraj_static *self, f64_t time, f64_t *rot)
{
	if (self->ellp_en == 0x00)
	{ vl3_mcopy(rot, self->rot); }
	
	else
	{ vl3_mcopy(rot, self->rot_cache); }
	
	vl3_mmul_m(rot, &self->ref->rot[0][0], rot);
	
	return 0x00;
}

//------------------------------------------------------------------------------

// API
//u8_t (*init) 		(void *data, void *config)
//u8_t (*free) 		(void *data);
//u8_t (*compile) 	(void *data);
//u8_t (*rot) 		(void *data, f64_t time, f64_t *pos);
//u8_t (*pos) 		(void *data, f64_t time, f64_t *rot);

inline u8_t trctraj_static_init_ (void **data, void *config)
{
	*data = (s_trctraj_static*) malloc(sizeof(s_trctraj_static));
	
	s_trctraj_static *traj = (s_trctraj_static*) *data;
	s_trctraj_static_init *attr = (s_trctraj_static_init*) config;
	
	return trctraj_static_init(traj, *attr);
}

inline u8_t trctraj_static_free_ (void **data)
{
	s_trctraj_static *traj = (s_trctraj_static*) *data;
	
	free(traj);
	
	return 0x00;
}

inline u8_t trctraj_static_save_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	s_trctraj_static_init *attr = (s_trctraj_static_init*) config;
	
	return trctraj_static_save(traj, attr, v_file);
}

inline u8_t trctraj_static_load_ (void *data, void *config, u8_t **v_file)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	s_trctraj_static_init *attr = (s_trctraj_static_init*) config;
	
	return trctraj_static_load(traj, attr, v_file);
}

inline u8_t trctraj_static_compile_(void *data)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	
	return trctraj_static_compile(traj);
}

inline u8_t trctraj_static_pos_(void *data, f64_t time, f64_t *pos)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	
	return trctraj_static_pos(traj, time, pos);
}

inline u8_t trctraj_static_rot_(void *data, f64_t time, f64_t *rot)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	
	return trctraj_static_rot(traj, time, rot);
}

inline u8_t trctraj_static_info_(void *data, s_trctraj_info *info)
{
	s_trctraj_static *traj = (s_trctraj_static*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = 1.0;
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __trctraj__ */
