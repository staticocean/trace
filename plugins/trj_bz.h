
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_BZ__
#define __TRJ_BZ__

//------------------------------------------------------------------------------

#include "trj_types.h"
#include "vl.h"

//------------------------------------------------------------------------------

typedef struct trj_bz
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	
	vlf_t length;
	
} 	s_trj_bz;

typedef struct trj_bz_init
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	
}	s_trj_bz_init;

uint8_t trj_bz_init(s_trj_bz *self, s_trj_bz_init attr);

typedef struct trj_bz4
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	vlf_t p3[2];
	
	vlf_t length;
	
} 	s_trj_bz4;

typedef struct trj_bz4_init_attr
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	vlf_t p3[2];
	
}	s_trj_bz4_init_attr;

uint8_t trj_bz4_init(s_trj_bz4 *self, s_trj_bz4_init_attr attr);
uint8_t trj_bz4_eval(s_trj_bz4 *self, vlf_t x, vlf_t *value);

typedef struct trj_bz5
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	vlf_t p3[2];
	vlf_t p4[2];
	
	vlf_t v0[2];
	vlf_t v1[2];
	vlf_t v2[2];
	vlf_t v3[2];
	
	vlf_t a0[2];
	vlf_t a1[2];
	vlf_t a2[2];
	
	vlf_t length;
	
} 	s_trj_bz5;

typedef struct trj_bz5_init_attr
{
	vlf_t p0[2];
	vlf_t p1[2];
	vlf_t p2[2];
	vlf_t p3[2];
	vlf_t p4[2];
	
}	s_trj_bz5_init_attr;

uint8_t trj_bz5_init(s_trj_bz5 *self, s_trj_bz5_init_attr attr);
uint8_t trj_bz5_calc(s_trj_bz5 *self);

//------------------------------------------------------------------------------

#endif /* __TRJ_BZ__ */
