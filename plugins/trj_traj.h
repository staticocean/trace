
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



uint8_t trj_traj_bz_init(s_trj_traj_bz *self, s_trj_traj_bz_init_attr attr);
uint8_t trj_traj_bz_compile(s_trj_traj_bz *self);
uint8_t trj_traj_bz_compile_api(void *self);
uint8_t trj_traj_bz_pos(s_trj_traj_bz *self, vlf_t time, vlf_t *pos);
uint8_t trj_traj_bz_pos_api(void *self, vlf_t time, vlf_t *pos);

//------------------------------------------------------------------------------

#endif /* __TRJ_TRAJ__ */
