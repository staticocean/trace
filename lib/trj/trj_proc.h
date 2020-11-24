
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __TRJ_PROC__
#define __TRJ_PROC__

//------------------------------------------------------------------------------

#include "vl.h"

#include "trj_api.h"

//------------------------------------------------------------------------------

#define trj_proc_euler_id (0x00000001)

typedef struct trj_proc_euler
{
	vlf_t d1p[3];
	vlf_t d1n[3];
	vlf_t d2 [3];
	vlf_t drv[3];
	
	vlf_t rnt[9]; // rotation next transpose
	vlf_t crm[9]; // cross multiplication
	
}   s_trj_proc_euler;

typedef struct trj_proc_euler_init
{
	uint32_t temp;

}   s_trj_proc_euler_init;

//------------------------------------------------------------------------------

inline uint8_t trj_proc_euler_init(s_trj_proc_euler *self, s_trj_proc_euler_init attr)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_save(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_load(s_trj_proc_euler *self, s_trj_proc_euler_init *attr, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_update(s_trj_proc_euler *self, s_trj_obj *obj, uint32_t offset)
{
	s_trj_obj_data *data = obj->log_list;
	
	if (offset == 0x00)
	{
		vl_vzero (self->d1p);
		vl_vsub (self->d1n, &data[offset+1].pos[0][0], &data[offset].pos[0][0]);
		
		// dont need to div 0.0 d1p
		vl_vmul_s (self->d1n, self->d1n, 1.0 / (data[1].time[0] - data[0].time[0]));
		
		vl_vsub (self->d2, self->d1n, self->d1p);
		
		vl_vmul_s (self->d2, self->d2, 1.0 / (data[1].time[0] - data[0].time[0]));
	}
	
	else if (offset == (obj->log_offset-1))
	{
		vl_vsub (self->d1p, &data[offset].pos[0][0], &data[offset-1].pos[0][0]);
		vl_vzero (self->d1n);
		
		// dont need to div 0.0 d1n
		vl_vmul_s (self->d1p, self->d1p, 1.0 / (data[offset].time[0] - data[offset-1].time[0]));
		
		vl_vsub (self->d2, self->d1n, self->d1p);
		
		vl_vmul_s (self->d2, self->d2, 1.0 / (data[offset].time[0] - data[offset-1].time[0]));
	}
	
	else
	{
		vl_vsub (self->d1p, &data[offset].pos[0][0], &data[offset-1].pos[0][0]);
		vl_vsub (self->d1n, &data[offset+1].pos[0][0], &data[offset].pos[0][0]);
		
		// dont need to div 0.0 d1n
		vl_vmul_s (self->d1p, self->d1p, 1.0 / (data[offset].time[0] - data[offset-1].time[0]));
		vl_vmul_s (self->d1n, self->d1n, 1.0 / (data[offset+1].time[0] - data[offset].time[0]));
		
		vl_vsub (self->d2, self->d1n, self->d1p);
		
		vl_vmul_s (self->d2, self->d2, 0.5 / (data[offset+1].time[0] - data[offset-1].time[0]));
	}
	
	if (offset == (obj->log_offset-1))
	{
		vl_mzero(&data[offset].rot[1][0]);
	}
	else
	{
		vl_tnp(self->rnt, &data[offset].rot[0][0]);
		vl_mmul_m(self->crm, &data[offset+1].rot[0][0], self->rnt);
		vl_mmul_s(self->crm, self->crm, 1.0 / (data[offset+1].time[0] - data[offset].time[0]));
		
		vl_unskew(self->drv, self->crm);
		vl_skew(&data[offset].rot[1][0], self->drv);
	}
	
	vl_vcopy(&data[offset].pos[1][0], self->d1n);
	vl_vcopy(&data[offset].pos[2][0], self->d2);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline uint8_t trj_proc_euler_init_ (void **data, void *config)
{
	*data = (s_trj_proc_euler*) malloc(sizeof(s_trj_proc_euler));
	
	s_trj_proc_euler *proc = (s_trj_proc_euler*) *data;
	s_trj_proc_euler_init *proc_init = (s_trj_proc_euler_init*) config;
	
	return trj_proc_euler_init(proc, *proc_init);
}

inline uint8_t trj_proc_euler_save_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_load_(void *data, void *config, uint8_t **v_file)
{
	return 0x00;
}

inline uint8_t trj_proc_euler_free_ (void **data)
{
	s_trj_proc_euler *proc = (s_trj_proc_euler*) *data;
	
	free(proc);
	
	return 0x00;
}

inline uint8_t trj_proc_euler_update_ (void *data, void *obj, uint32_t offset)
{
	s_trj_proc_euler *proc = (s_trj_proc_euler*) data;
	
	return trj_proc_euler_update(proc, (s_trj_obj*) obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __INS_PROC__ */


