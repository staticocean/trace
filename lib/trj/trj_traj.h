
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_TRAJ__
#define __TRJ_TRAJ__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

#include "trj_bz.h"

//------------------------------------------------------------------------------

typedef struct trj_traj_api
{
	char desc[32];
	
	uint8_t (*compile) 	(void *data);
	uint8_t (*rot) 		(void *data, vlf_t time, vlf_t *pos);
	uint8_t (*pos) 		(void *data, vlf_t time, vlf_t *rot);
	
} 	s_trj_traj_api;

typedef struct trj_traj_bz_point
{
	float64_t time;
	
	float64_t pos_p[3];
	float64_t pos_t[3];
	float64_t pos_d[3];
	
	float64_t rot_p[3];
	float64_t rot_t[3];
	float64_t rot_d[3];
	
	/*
	 pos_d MUST BE
	 to the right of pos_p (pos_d[0] > pos_p[0])
	 cannot overlap next point
	 */
	
} 	s_trj_traj_bz_point;

typedef struct trj_traj_bz
{
	s_trj_traj_api api;
	
	s_trj_traj_bz_point *pts;
	uint32_t pts_offset;

} 	s_trj_traj_bz;

typedef struct trj_traj_bz_init_attr
{
	s_trj_traj_bz_point *pts;
	
} 	s_trj_traj_bz_init_attr;

//------------------------------------------------------------------------------

inline uint8_t trj_traj_bz_init(s_trj_traj_bz *self, s_trj_traj_bz_init_attr attr)
{
	self->pts = attr.pts;
	self->pts_offset = 0x00;
	
	return 0x00;
}

inline uint8_t trj_traj_bz_add(s_trj_traj_bz *self, s_trj_traj_bz_point point)
{
	self->pts[self->pts_offset] = point;
	self->pts_offset++;
	
	return 0x00;
}

inline uint8_t trj_traj_bz_compile(s_trj_traj_bz *self)
{
	uint32_t i;
	uint32_t j;
	
	vlf_t m_a[self->pts_offset*2][self->pts_offset*2];
//	vlf_t *m_a = (vlf_t*) malloc(sizeof(vlf_t) * 2*self->pts_offset * 2*self->pts_offset);
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		for (j = 0; j < 2*self->pts_offset; ++j)
		{
			m_a[2*i+0][j] = 0.0;
			m_a[2*i+1][j] = 0.0;
		}
		
		m_a[2*i+0][2*i-2] = 1;
		m_a[2*i+0][2*i+2] = 1;
		m_a[2*i+0][2*i+0] = 4;
		m_a[2*i+1][2*i-2+1] = 1;
		m_a[2*i+1][2*i+2+1] = 1;
		m_a[2*i+1][2*i+0+1] = 4;
	}
	
	for (j = 0; j < 2*self->pts_offset; ++j)
	{
		m_a[0][j] = 0.0;
		m_a[1][j] = 0.0;
		
		m_a[2*self->pts_offset-2][j] = 0.0;
		m_a[2*self->pts_offset-1][j] = 0.0;
	}
	
	m_a[0][0] = 2;
	m_a[0][2] = 1;
	m_a[1][1] = 2;
	m_a[1][3] = 1;
	
	m_a[2*self->pts_offset-2][2*self->pts_offset-4] = 1;
	m_a[2*self->pts_offset-2][2*self->pts_offset-2] = 2;
	m_a[2*self->pts_offset-1][2*self->pts_offset-3] = 1;
	m_a[2*self->pts_offset-1][2*self->pts_offset-1] = 2;

//	vlf_t pos_p0[2*self->pts_offset];
//	vlf_t pos_d0[2*self->pts_offset];
	vlf_t *pos_p0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_p1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_p2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *pos_d0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_d1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *pos_d2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *rot_p0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_p1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_p2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	vlf_t *rot_d0 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_d1 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	vlf_t *rot_d2 = (vlf_t*) malloc(sizeof(vlf_t) * 2 * self->pts_offset);
	
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		pos_p0[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p0[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[0] - self->pts[i - 1].pos_p[0]);
		
		pos_p1[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p1[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[1] - self->pts[i - 1].pos_p[1]);
		
		pos_p2[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		pos_p2[2 * i + 1] = 3 * (self->pts[i + 1].pos_p[2] - self->pts[i - 1].pos_p[2]);
		
		rot_p0[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p0[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[0] - self->pts[i - 1].rot_p[0]);
		
		rot_p1[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p1[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[1] - self->pts[i - 1].rot_p[1]);
		
		rot_p2[2 * i + 0] = 3 * (self->pts[i + 1].time - self->pts[i - 1].time);
		rot_p2[2 * i + 1] = 3 * (self->pts[i + 1].rot_p[2] - self->pts[i - 1].rot_p[2]);
	}
	
	pos_p0[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p0[1] = 3 * (self->pts[1].pos_p[0] - self->pts[0].pos_p[0]);
	
	pos_p1[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p1[1] = 3 * (self->pts[1].pos_p[1] - self->pts[0].pos_p[1]);
	
	pos_p2[0] = 3 * (self->pts[1].time - self->pts[0].time);
	pos_p2[1] = 3 * (self->pts[1].pos_p[2] - self->pts[0].pos_p[2]);
	
	rot_p0[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p0[1] = 3 * (self->pts[1].rot_p[0] - self->pts[0].rot_p[0]);
	
	rot_p1[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p1[1] = 3 * (self->pts[1].rot_p[1] - self->pts[0].rot_p[1]);
	
	rot_p2[0] = 3 * (self->pts[1].time - self->pts[0].time);
	rot_p2[1] = 3 * (self->pts[1].rot_p[2] - self->pts[0].rot_p[2]);
	
	pos_p0[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p0[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[0]
												- self->pts[self->pts_offset-2].pos_p[0]);
	
	pos_p1[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p1[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[1]
												- self->pts[self->pts_offset-2].pos_p[1]);
	
	pos_p2[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	pos_p2[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].pos_p[2]
												- self->pts[self->pts_offset-2].pos_p[2]);
	
	rot_p0[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p0[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[0]
												- self->pts[self->pts_offset-2].rot_p[0]);
	
	rot_p1[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p1[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[1]
												- self->pts[self->pts_offset-2].rot_p[1]);
	
	rot_p2[2 * self->pts_offset - 1 - 1] = 3 * (self->pts[self->pts_offset - 1].time
												- self->pts[self->pts_offset-2].time);
	
	rot_p2[2 * self->pts_offset - 1 - 0] = 3 * (self->pts[self->pts_offset - 1].rot_p[2]
												- self->pts[self->pts_offset-2].rot_p[2]);
	
	vlf_t m_inva[self->pts_offset*2][self->pts_offset*2];
	
	vl_inv(self->pts_offset*2, &m_inva[0][0], &m_a[0][0]);
	
	for (i = 0; i < self->pts_offset*2; ++i)
	{
		pos_d0[i] = 0.0;
		pos_d1[i] = 0.0;
		pos_d2[i] = 0.0;
		
		rot_d0[i] = 0.0;
		rot_d1[i] = 0.0;
		rot_d2[i] = 0.0;
		
		for (j = 0; j < self->pts_offset*2; ++j)
		{
			pos_d0[i] += m_inva[i][j] * pos_p0[j];
			pos_d1[i] += m_inva[i][j] * pos_p1[j];
			pos_d2[i] += m_inva[i][j] * pos_p2[j];
			
			rot_d0[i] += m_inva[i][j] * rot_p0[j];
			rot_d1[i] += m_inva[i][j] * rot_p1[j];
			rot_d2[i] += m_inva[i][j] * rot_p2[j];
		}
	}
	
	for (i = 0; i < self->pts_offset; ++i)
	{
		self->pts[i].pos_t[0] = pos_d0[2 * i + 0] / 3;
		self->pts[i].pos_d[0] = pos_d0[2 * i + 1] / 3;
		
		self->pts[i].pos_t[1] = pos_d1[2 * i + 0] / 3;
		self->pts[i].pos_d[1] = pos_d1[2 * i + 1] / 3;
		
		self->pts[i].pos_t[2] = pos_d2[2 * i + 0] / 3;
		self->pts[i].pos_d[2] = pos_d2[2 * i + 1] / 3;
		
		self->pts[i].rot_t[0] = rot_d0[2 * i + 0] / 3;
		self->pts[i].rot_d[0] = rot_d0[2 * i + 1] / 3;
		
		self->pts[i].rot_t[1] = rot_d1[2 * i + 0] / 3;
		self->pts[i].rot_d[1] = rot_d1[2 * i + 1] / 3;
		
		self->pts[i].rot_t[2] = rot_d2[2 * i + 0] / 3;
		self->pts[i].rot_d[2] = rot_d2[2 * i + 1] / 3;
	}
	
	free(pos_p0);
	free(pos_d0);
	
	free(pos_p1);
	free(pos_d1);
	
	free(pos_p2);
	free(pos_d2);
	
	free(rot_p0);
	free(rot_d0);
	
	free(rot_p1);
	free(rot_d1);
	
	free(rot_p2);
	free(rot_d2);
//	free(m_a);
	
	return 0x00;
}

inline uint8_t trj_traj_bz_compile_api(void *self)
{
	return trj_traj_bz_compile((s_trj_traj_bz*) self);
}

inline uint8_t trj_traj_bz_pos(s_trj_traj_bz *self, vlf_t time, vlf_t *pos)
{
	uint32_t offset = 0x00;
	
	while (offset < (self->pts_offset-1)
		   && (time < self->pts[offset].time)
		   || (time > self->pts[offset+1].time))
	{
		++offset;
	}
	
	if (offset >= (self->pts_offset-1))
	{
		offset = self->pts_offset - 2;
	}
	
	s_trj_bz4 bz4;
	
	trj_bz4_init(&bz4, (s_trj_bz4_init_attr) {
			.p0 = {
					self->pts[offset  ].time,
					self->pts[offset  ].pos_p[0]
			},
			
			.p1 = {
					self->pts[offset  ].time + self->pts[offset].pos_t[0],
					self->pts[offset  ].pos_p[0] + self->pts[offset].pos_d[0]
			},
			
			.p2 = {
					self->pts[offset+1].time - self->pts[offset + 1].pos_t[0],
					self->pts[offset+1].pos_p[0] - self->pts[offset + 1].pos_d[0]
			},
			
			.p3 = {
					self->pts[offset+1].time,
					self->pts[offset+1].pos_p[0]
			}
	});
	
	trj_bz4_eval(&bz4, time, &pos[0]);
	
	trj_bz4_init(&bz4, (s_trj_bz4_init_attr) {
			.p0 = {
					self->pts[offset  ].time,
					self->pts[offset  ].pos_p[1]
			},
			
			.p1 = {
					self->pts[offset  ].time + self->pts[offset].pos_t[1],
					self->pts[offset  ].pos_p[1] + self->pts[offset].pos_d[1]
			},
			
			.p2 = {
					self->pts[offset+1].time - self->pts[offset + 1].pos_t[1],
					self->pts[offset+1].pos_p[1] - self->pts[offset + 1].pos_d[1]
			},
			
			.p3 = {
					self->pts[offset+1].time,
					self->pts[offset+1].pos_p[1]
			}
	});
	
	trj_bz4_eval(&bz4, time, &pos[1]);
	
	trj_bz4_init(&bz4, (s_trj_bz4_init_attr) {
			.p0 = {
					self->pts[offset  ].time,
					self->pts[offset  ].pos_p[2]
			},
			
			.p1 = {
					self->pts[offset  ].time + self->pts[offset].pos_t[2],
					self->pts[offset  ].pos_p[2] + self->pts[offset].pos_d[2]
			},
			
			.p2 = {
					self->pts[offset+1].time - self->pts[offset + 1].pos_t[2],
					self->pts[offset+1].pos_p[2] - self->pts[offset + 1].pos_d[2]
			},
			
			.p3 = {
					self->pts[offset+1].time,
					self->pts[offset+1].pos_p[2]
			}
	});
	
	trj_bz4_eval(&bz4, time, &pos[2]);
	
	return 0x00;
}

inline uint8_t trj_traj_bz_pos_api(void *self, vlf_t time, vlf_t *pos)
{
	return trj_traj_bz_pos((s_trj_traj_bz*) self, time, pos);
}

//------------------------------------------------------------------------------


#endif /* __TRJ_TRAJ__ */
