
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_TRAJ___
#define __TRJ_TRAJ___

//----------------------------------------------------------------

#include <softael_lib/vl.h>

//----------------------------------------------------------------

typedef enum trj_traj_navsat_filetype
{
	trj_traj_navsat_filetype_none = 0x00,
	trj_traj_navsat_filetype_agp  = 0x01,
	trj_traj_navsat_filetype_agl  = 0x02,

}	trj_traj_navsat_filetype_t;

typedef struct trj_traj_navsat_data
{
	int     	satnum;
	int     	health;
	int     	week;
	int     	tow;
	int     	day;
	int     	month;
	int     	year;
	float64_t 	atime;
	float64_t 	tcorr;
	float64_t 	dtcorr;
	float64_t  	domg;
	
	vlf_t  		omg0;
	vlf_t 		i;
	vlf_t 		w;
	vlf_t 		e;
	vlf_t 		sqrta;
	vlf_t 		m0;
	
	vlf_t 	ee;
	vlf_t 	a;
	vlf_t 	n;
	
} 	s_trj_traj_navsat_data;

typedef struct trj_traj_navsat
{
	s_trj_eng 	*eng;
	s_trj_obj 	*ref;
	uint32_t 	ref_hash;
	
	char 		file_path[512];
	trj_traj_navsat_filetype_t file_type;
	
	uint32_t 	data_offset;
	s_trj_traj_navsat_data data_list[64];
	
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
	
} 	s_trj_traj_navsat;

typedef struct trj_traj_navsat_init
{
	s_trj_eng *eng;
	s_trj_obj *ref;
	
} 	s_trj_traj_navsat_init;

//----------------------------------------------------------------

inline uint8_t trj_traj_navsat_init(s_trj_traj_navsat *self, s_trj_traj_navsat_init attr)
{
	self->eng = attr.eng;
	
	self->ref = attr.ref;
	if (self->ref != NULL)
	{ self->ref_hash = self->ref->hash; }
	
	self->data_offset = 0x00;
	self->sat_offset = 0x00;
	self->resolution = 100;
	self->file_type = trj_traj_navsat_filetype_none;
	sprintf(self->file_path, "no almanac file selected");
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_save(s_trj_traj_navsat *self, s_trj_traj_navsat_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_traj_navsat_load(s_trj_traj_navsat *self, s_trj_traj_navsat_init *attr, uint8_t **v_file)
{
	self->eng = attr->eng;
	
	self->ref = trj_eng_find_obj (self->eng, self->ref_hash);

	if (self->ref  == NULL)
	{ self->ref_hash = 0x00; }

	// damaged plugin data
	if (self->data_offset > 63 || self->data_offset < 0) { self->data_offset = 0x00; }
	if (self->sat_offset  > 63 || self->sat_offset  < 0) { self->sat_offset  = 0x00; }
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_pos_local(s_trj_traj_navsat *self, vlf_t time, vlf_t *pos, int satnum)
{
	const vlf_t omge = 7.2921151467E-6;
	
	uint32_t i;
	s_trj_traj_navsat_data *data = &self->data_list[satnum];
	
	int ref_week = self->ref_week;
	vlf_t ref_sec  = self->ref_sec + time;
	
	while (ref_sec > 604800) { ref_sec -= 604800; ++ref_week; }
	
	vlf_t tk = (ref_week - data->week)*604800*0 + (ref_sec - data->tow);
	
	if ((ref_sec - data->tow) < -302400) { tk += 604800; }
	if ((ref_sec - data->tow) > +302400) { tk -= 604800; }
	
	vlf_t mk = data->m0 + data->n*tk;
	vlf_t ek = mk;
	
	for (i = 0; i < self->resolution; ++i)
	{ ek = mk + data->e * vl_sin(ek); }
	
	vlf_t vk = atan2(vl_sqrt(1-data->ee)*vl_sin(ek), (vl_cos(ek) - data->e));
	vlf_t uk = vk + data->w;
	vlf_t rk = data->a * (1 - data->e * vl_cos(ek));
	vlf_t dx = rk * vl_cos(uk);
	vlf_t dy = rk * vl_sin(uk);
	vlf_t omgk = data->omg0 + (data->domg - omge)*tk - omge*data->tow;
	
	pos[0] = dx * vl_cos(omgk) - dy * vl_cos(data->i) * vl_sin(omgk);
	pos[1] = dy * vl_sin(data->i);
	pos[2] = -dx * vl_sin(omgk) + dy * vl_cos(data->i) * vl_cos(omgk);
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_pos(s_trj_traj_navsat *self, vlf_t time, vlf_t *pos)
{
	if (self->ref != NULL)
	{
		trj_traj_navsat_pos_local(self, time, pos, self->sat_offset);
		vl_mmul_v(pos, &self->ref->rot[0][0], pos);
		vl_vsum(pos, pos, &self->ref->pos[0][0]);
	}
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_rot (s_trj_traj_navsat *self, vlf_t time, vlf_t *rot)
{
	vl_mid(rot);
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_parseagp(s_trj_traj_navsat *self)
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
		
		self->data_offset = 0x00;

		while (fgets(l0, sizeof(l0), file_data)
		   	&& fgets(l1, sizeof(l1), file_data)
		   	&& fgets(l2, sizeof(l2), file_data))
		{
//			printf("%s", l0);
			
			sscanf(l1, "%d %d %d %d %d %d %d %lf %lf %lf %lf",
				   &self->data_list[self->data_offset].satnum,
				   &self->data_list[self->data_offset].health,
				   &self->data_list[self->data_offset].week,
				   &self->data_list[self->data_offset].tow,
				   &self->data_list[self->data_offset].day,
				   &self->data_list[self->data_offset].month,
				   &self->data_list[self->data_offset].year,
				   &self->data_list[self->data_offset].atime,
				   &self->data_list[self->data_offset].tcorr,
				   &self->data_list[self->data_offset].dtcorr,
				   &self->data_list[self->data_offset].domg);
			
			sscanf(l2, "%lf %lf %lf %lf %lf %lf",
				   &self->data_list[self->data_offset].omg0,
				   &self->data_list[self->data_offset].i,
				   &self->data_list[self->data_offset].w,
				   &self->data_list[self->data_offset].e,
				   &self->data_list[self->data_offset].sqrta,
				   &self->data_list[self->data_offset].m0);
			
			self->data_list[self->data_offset].domg *= vl_pi;
			self->data_list[self->data_offset].omg0 *= vl_pi;
			self->data_list[self->data_offset].i *= vl_pi;
			self->data_list[self->data_offset].w *= vl_pi;
			self->data_list[self->data_offset].m0 *= vl_pi;
			
			++self->data_offset;
		}
		
		fclose (file_data);
	}
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_parseagl(s_trj_traj_navsat *self)
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

inline uint8_t trj_traj_navsat_compile(s_trj_traj_navsat *self)
{
	uint32_t i;
	
	const vlf_t mu = 3.986004418E+14;
	
	vlf_t jd = floor(365.25*self->year) + floor(30.6001*(self->month+1)) + self->day
			+ self->hour + self->min / 60.0 + self->sec / 60.0 + 1720981.5;
	
	self->ref_week = floor((jd - 2444244.5) / 7.0);
	self->ref_sec = round((((jd - 2444244.5) / 7.0 - self->ref_week) * 604800) / 0.5) * 0.5;
	
	switch (self->file_type)
	{
		case trj_traj_navsat_filetype_agp: { trj_traj_navsat_parseagp(self); break; }
		case trj_traj_navsat_filetype_agl: { trj_traj_navsat_parseagl(self); break; }
		case trj_traj_navsat_filetype_none: { break; }

		default: break;
	}
	
	for (i = 0; i < self->data_offset; ++i)
	{
		s_trj_traj_navsat_data *data = &self->data_list[i];
		
		data->a = data->sqrta * data->sqrta;
		data->ee = data->e * data->e;
		data->n = vl_sqrt(mu / (data->a * data->a * data->a));
	}
	
	return 0x00;
}

//----------------------------------------------------------------

// API
//uint8_t (*init) 		(void *data, void *config)
//uint8_t (*free) 		(void *data);
//uint8_t (*compile) 	(void *data);
//uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
//uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);

inline uint8_t trj_traj_navsat_init_ (void **data, void *config)
{
	*data = (s_trj_traj_navsat*) malloc(sizeof(s_trj_traj_navsat));
	
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) *data;
	s_trj_traj_navsat_init *traj_init = (s_trj_traj_navsat_init*) config;
	
	return trj_traj_navsat_init(traj, *traj_init);
}

inline uint8_t trj_traj_navsat_free_ (void **data)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) *data;
	
	free(traj);
	
	return 0x00;
}

inline uint8_t trj_traj_navsat_save_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	s_trj_traj_navsat_init *attr = (s_trj_traj_navsat_init*) config;
	return trj_traj_navsat_save(traj, attr, v_file);
}

inline uint8_t trj_traj_navsat_load_ (void *data, void *config, uint8_t **v_file)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	s_trj_traj_navsat_init *attr = (s_trj_traj_navsat_init*) config;
	return trj_traj_navsat_load(traj, attr, v_file);
}

inline uint8_t trj_traj_navsat_compile_(void *data)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	
	return trj_traj_navsat_compile(traj);
}

inline uint8_t trj_traj_navsat_pos_(void *data, vlf_t time, vlf_t *pos)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	
	return trj_traj_navsat_pos(traj, time, pos);
}

inline uint8_t trj_traj_navsat_rot_(void *data, vlf_t time, vlf_t *rot)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	
	return trj_traj_navsat_rot(traj, time, rot);
}

inline uint8_t trj_traj_navsat_info_(void *data, s_trj_traj_info *info)
{
	s_trj_traj_navsat *traj = (s_trj_traj_navsat*) data;
	
	info->preview_time[0] = 0.0;
	info->preview_time[1] = 3600*24; // 1 day

	return 0x00;
}

//----------------------------------------------------------------


#endif /* __TRJ_TRAJ__ */
