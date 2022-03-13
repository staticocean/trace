
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_navstat - trace

#ifndef __TRCTRAJ___
#define __TRCTRAJ___

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trctraj.h>
#include <sftltrc/trcspl.h>
#include <sftltrc/trcellp.h>

//------------------------------------------------------------------------------

typedef enum trctraj_navsat_filetype
{
	trctraj_navsat_filetype_none = 0x00,
	trctraj_navsat_filetype_agp  = 0x01,
	trctraj_navsat_filetype_agl  = 0x02,

}	trctraj_navsat_filetype_t;

typedef struct trctraj_navsat_data
{
	int     	satnum;
	int     	health;
	int     	week;
	int     	tow;
	int     	day;
	int     	month;
	int     	year;
	t_f64 	atime;
	t_f64 	tcorr;
	t_f64 	dtcorr;
	t_f64  	domg;
	
	t_f64  		omg0;
	t_f64 		i;
	t_f64 		w;
	t_f64 		e;
	t_f64 		sqrta;
	t_f64 		m0;
	
	t_f64 	ee;
	t_f64 	a;
	t_f64 	n;
	
} 	s_trctraj_navsat_data;

typedef struct trctraj_navsat
{
	s_trceng 	*eng;
	s_trcobj 	*ref;
	t_u32 	ref_hash;
	
	char 		file_path[512];
	trctraj_navsat_filetype_t file_type;
	
	t_u32 	data_sz;
	s_trctraj_navsat_data data_ls[64];
	
	int      	resolution;
	int     	sat_offset;
	
	int day;
	int month;
	int year;
	int hour;
	int min;
	int sec;
	
	int ref_week;
	int ref_sec;
	
} 	s_trctraj_navsat;

typedef struct trctraj_navsat_init
{
	s_trceng *eng;
	s_trcobj *ref;
	
} 	s_trctraj_navsat_init;

//------------------------------------------------------------------------------

inline t_u8 trctraj_navsat_init(s_trctraj_navsat *self, s_trctraj_navsat_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->data_sz = 0x00;
	self->sat_offset = 0x00;
	self->resolution = 100;
	self->file_type = trctraj_navsat_filetype_none;
	sprintf(self->file_path, "no almanac file selected");
	
	return 0x00;
}

inline t_u8 trctraj_navsat_save(s_trctraj_navsat *self, s_trctraj_navsat_init *attr, t_u8 **v_file)
{
	return 0x00;
}

inline t_u8 trctraj_navsat_load(s_trctraj_navsat *self, s_trctraj_navsat_init *attr, t_u8 **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trceng_find_obj (self->eng, self->ref_hash);

	if (self->ref  == NULL)
	{ self->ref_hash = 0x00; }

	// damaged plugin data
	if (self->data_sz > 63 || self->data_sz < 0) { self->data_sz = 0x00; }
	if (self->sat_offset  > 63 || self->sat_offset  < 0) { self->sat_offset  = 0x00; }
	
	return 0x00;
}

inline t_u8 trctraj_navsat_pos_local(s_trctraj_navsat *self, t_f64 time, t_f64 *pos, int satnum)
{
	const t_f64 omge = 7.2921151467E-6;
	
	t_u32 i;
	s_trctraj_navsat_data *data = &self->data_ls[satnum];
	
	int ref_week = self->ref_week;
	t_f64 ref_sec  = self->ref_sec + time;
	
	while (ref_sec > 604800) { ref_sec -= 604800; ++ref_week; }
	
	t_f64 tk = (ref_week - data->week)*604800*0 + (ref_sec - data->tow);
	
	if ((ref_sec - data->tow) < -302400) { tk += 604800; }
	if ((ref_sec - data->tow) > +302400) { tk -= 604800; }
	
	t_f64 mk = data->m0 + data->n*tk;
	t_f64 ek = mk;
	
	for (t_s32 i = 0; i < self->resolution; ++i)
	{ ek = mk + data->e * vld_sin(ek); }
	
	t_f64 vk = atan2(vld_sqrt(1-data->ee)*vld_sin(ek), (vld_cos(ek) - data->e));
	t_f64 uk = vk + data->w;
	t_f64 rk = data->a * (1 - data->e * vld_cos(ek));
	t_f64 dx = rk * vld_cos(uk);
	t_f64 dy = rk * vld_sin(uk);
	t_f64 omgk = data->omg0 + (data->domg - omge)*tk - omge*data->tow;
	
	pos[0] = dx * vld_cos(omgk) - dy * vld_cos(data->i) * vld_sin(omgk);
	pos[1] = dy * vld_sin(data->i);
	pos[2] = -dx * vld_sin(omgk) + dy * vld_cos(data->i) * vld_cos(omgk);
	
	return 0x00;
}

inline t_u8 trctraj_navsat_pos(s_trctraj_navsat *self, t_f64 time, t_f64 *pos)
{
	if (self->ref != NULL)
	{
		trctraj_navsat_pos_local(self, time, pos, self->sat_offset);
		vld3m_mulv(pos, &self->ref->rot[0][0], pos);
		vl3_vsum(pos, pos, &self->ref->pos[0][0]);
	}
	
	return 0x00;
}

inline t_u8 trctraj_navsat_rot (s_trctraj_navsat *self, t_f64 time, t_f64 *rot)
{
	vl3_mid(rot);
	
	return 0x00;
}

inline t_u8 trctraj_navsat_parseagp(s_trctraj_navsat *self)
{
	FILE *file_data = fopen(self->file_path, "r");
	
	if (file_data == NULL)
	{
	
	}
	
	else
	{
		char l0[512];
		char l1[512];
		char l2[512];
		
//		while (fscanf(file_data, "%s %s %s %s %s \n", l0, l0, l0, l0, l0) != EOF
//			   && fscanf(file_data, "%s %s %s %s %s %s %s %s %s %s %s \n", l1, l1, l1, l1, l1, l1, l1, l1, l1, l1, l1) != EOF
//			   && fscanf(file_data, "%s %s %s %s %s %s \n", l2, l2, l2, l2, l2, l2) != EOF)
//		{
		
		self->data_sz = 0x00;

		while (fgets(l0, sizeof(l0), file_data)
		   	&& fgets(l1, sizeof(l1), file_data)
		   	&& fgets(l2, sizeof(l2), file_data))
		{
//			printf("%s", l0);
			
			sscanf(l1, "%d %d %d %d %d %d %d %lf %lf %lf %lf",
				   &self->data_ls[self->data_sz].satnum,
				   &self->data_ls[self->data_sz].health,
				   &self->data_ls[self->data_sz].week,
				   &self->data_ls[self->data_sz].tow,
				   &self->data_ls[self->data_sz].day,
				   &self->data_ls[self->data_sz].month,
				   &self->data_ls[self->data_sz].year,
				   &self->data_ls[self->data_sz].atime,
				   &self->data_ls[self->data_sz].tcorr,
				   &self->data_ls[self->data_sz].dtcorr,
				   &self->data_ls[self->data_sz].domg);
			
			sscanf(l2, "%lf %lf %lf %lf %lf %lf",
				   &self->data_ls[self->data_sz].omg0,
				   &self->data_ls[self->data_sz].i,
				   &self->data_ls[self->data_sz].w,
				   &self->data_ls[self->data_sz].e,
				   &self->data_ls[self->data_sz].sqrta,
				   &self->data_ls[self->data_sz].m0);
			
			self->data_ls[self->data_sz].domg *= vl_pi;
			self->data_ls[self->data_sz].omg0 *= vl_pi;
			self->data_ls[self->data_sz].i *= vl_pi;
			self->data_ls[self->data_sz].w *= vl_pi;
			self->data_ls[self->data_sz].m0 *= vl_pi;
			
			++self->data_sz;
		}
		
		fclose (file_data);
	}
	
	return 0x00;
}

inline t_u8 trctraj_navsat_parseagl(s_trctraj_navsat *self)
{
	FILE *file_data = fopen(self->file_path, "r");
	
	if (file_data == NULL)
	{
	
	}
	
	else
	{
		fclose (file_data);
	}
	
	return 0x00;
}

inline t_u8 trctraj_navsat_compile(s_trctraj_navsat *self)
{
	t_u32 i;
	
	const t_f64 mu = 3.986004418E+14;
	
	t_f64 jd = floor(365.25*self->year) + floor(30.6001*(self->month+1)) + self->day
			+ self->hour + self->min / 60.0 + self->sec / 60.0 + 1720981.5;
	
	self->ref_week = floor((jd - 2444244.5) / 7.0);
	self->ref_sec = round((((jd - 2444244.5) / 7.0 - self->ref_week) * 604800) / 0.5) * 0.5;
	
	switch (self->file_type)
	{
		case trctraj_navsat_filetype_agp: { trctraj_navsat_parseagp(self); break; }
		case trctraj_navsat_filetype_agl: { trctraj_navsat_parseagl(self); break; }
		case trctraj_navsat_filetype_none: { break; }

		default: break;
	}
	
	for (t_s32 i = 0; i < self->data_sz; ++i)
	{
		s_trctraj_navsat_data *data = &self->data_ls[i];
		
		data->a = data->sqrta * data->sqrta;
		data->ee = data->e * data->e;
		data->n = vld_sqrt(mu / (data->a * data->a * data->a));
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

// API
//t_u8 (*init) 		(void *data, void *config)
//t_u8 (*free) 		(void *data);
//t_u8 (*compile) 	(void *data);
//t_u8 (*rot) 		(void *data, t_f64 time, t_f64 *pos);
//t_u8 (*pos) 		(void *data, t_f64 time, t_f64 *rot);

inline t_u8 trctraj_navsat_init_ (void **data, void *config)
{
	*data = (s_trctraj_navsat*) malloc(sizeof(s_trctraj_navsat));
	
	s_trctraj_navsat *traj = (s_trctraj_navsat*) *data;
	s_trctraj_navsat_init *traj_init = (s_trctraj_navsat_init*) config;
	
	return trctraj_navsat_init(traj, *traj_init);
}

inline t_u8 trctraj_navsat_free_ (void **data)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) *data;
	
	free(traj);
	
	return 0x00;
}

inline t_u8 trctraj_navsat_save_ (void *data, void *config, t_u8 **v_file)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	s_trctraj_navsat_init *attr = (s_trctraj_navsat_init*) config;
	return trctraj_navsat_save(traj, attr, v_file);
}

inline t_u8 trctraj_navsat_load_ (void *data, void *config, t_u8 **v_file)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	s_trctraj_navsat_init *attr = (s_trctraj_navsat_init*) config;
	return trctraj_navsat_load(traj, attr, v_file);
}

inline t_u8 trctraj_navsat_compile_(void *data)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	
	return trctraj_navsat_compile(traj);
}

inline t_u8 trctraj_navsat_pos_(void *data, t_f64 time, t_f64 *pos)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	
	return trctraj_navsat_pos(traj, time, pos);
}

inline t_u8 trctraj_navsat_rot_(void *data, t_f64 time, t_f64 *rot)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	
	return trctraj_navsat_rot(traj, time, rot);
}

inline t_u8 trctraj_navsat_info_(void *data, s_trctraj_info *info)
{
	s_trctraj_navsat *traj = (s_trctraj_navsat*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = 3600*24; // 1 day

	return 0x00;
}

//------------------------------------------------------------------------------


#endif /* __trctraj__ */
