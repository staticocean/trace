
#include "trj_traj.h"

//------------------------------------------------------------------------------

uint8_t trj_traj_bz_init(s_trj_traj_bz *self, s_trj_traj_bz_init_attr attr)
{
	self->pts = attr.pts;
	self->pts_offset = 0x00;
	
	return 0x00;
}

uint8_t trj_traj_bz_add(s_trj_traj_bz *self, s_trj_traj_bz_point point)
{
	self->pts[self->pts_offset] = point;
	self->pts_offset++;
	
	return 0x00;
}

uint8_t trj_traj_bz_compile(s_trj_traj_bz *self)
{
	uint32_t i;
	uint32_t j;
	
//	vlf_t m_a[self->pts_offset*2][self->pts_offset*2];
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
	
//	vlf_t v_p[2*self->pts_offset];
//	vlf_t v_d[2*self->pts_offset];
	vlf_t *v_p = (vlf_t*) malloc(sizeof(vlf_t) * 2*self->pts_offset);
	vlf_t *v_d = (vlf_t*) malloc(sizeof(vlf_t) * 2*self->pts_offset);
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		v_p[2*i+0] = 3 * (self->pts[i+1].p[0] - self->pts[i-1].p[0]);
		v_p[2*i+1] = 3 * (self->pts[i+1].p[1] - self->pts[i-1].p[1]);
	}
	
	v_p[0] = 3 * (self->pts[1].p[0] - self->pts[0].p[0]);
	v_p[1] = 3 * (self->pts[1].p[1] - self->pts[0].p[1]);
	
	v_p[2*self->pts_offset-1-1] = 3 * (self->pts[self->pts_offset-1].p[0]
									 - self->pts[self->pts_offset-2].p[0]);
	
	v_p[2*self->pts_offset-1-0] = 3 * (self->pts[self->pts_offset-1].p[1]
			                         - self->pts[self->pts_offset-2].p[1]);
	
	vlf_t m_inva[self->pts_offset*2][self->pts_offset*2];
	
	vl_inv(self->pts_offset*2, &m_inva[0][0], &m_a[0][0]);
	
	for (i = 0; i < self->pts_offset*2; ++i)
	{
		v_d[i] = 0.0;
		
		for (j = 0; j < self->pts_offset*2; ++j)
		{
			v_d[i] += m_inva[i][j] * v_p[j];
		}
	}
	
	for (i = 0; i < self->pts_offset; ++i)
	{
		self->pts[i].d[0] = v_d[2*i+0] / 3;
		self->pts[i].d[1] = v_d[2*i+1] / 3;
	}
	
	free(v_p);
	free(v_d);
//	free(m_a);
	
	return 0x00;
}

uint8_t trj_traj_bz_pos(s_trj_traj_bz *self, vlf_t time, vlf_t *pos)
{
	uint32_t offset = 0x00;
	
	while (offset < (self->pts_offset-1)
		   && (time < self->pts[offset].p[0]
			   || time > self->pts[offset+1].p[0]))
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
					self->pts[offset  ].p[0],
					self->pts[offset  ].p[1]
			},
			
			.p1 = {
					self->pts[offset  ].p[0] + self->pts[offset].d[0],
					self->pts[offset  ].p[1] + self->pts[offset].d[1]
			},
			
			.p2 = {
					self->pts[offset+1].p[0] - self->pts[offset+1].d[0],
					self->pts[offset+1].p[1] - self->pts[offset+1].d[1]
			},
			
			.p3 = {
					self->pts[offset+1].p[0],
					self->pts[offset+1].p[1]
			}
	});
	
	trj_bz4_eval(&bz4, time, pos);
	
	return 0x00;
}

//------------------------------------------------------------------------------

