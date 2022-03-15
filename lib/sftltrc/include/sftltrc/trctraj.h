
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj - trace

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trctraj_intf
{
	char 				desc[32];
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *traj, void *attr);
	t_s8 (*free) 		(void *traj);
	t_s8 (*save) 		(void *traj, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *traj, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*compile) 	(void *traj);
	t_s8 (*pos) 		(void *traj, t_f64 time, t_f64 *pos);
	t_s8 (*rot) 		(void *traj, t_f64 time, t_f64 *rot);
	
	void (*gui_attr)	(void *traj, void *attr);
	void (*gui_edit)	(void *traj);
	void (*gui_view)	(void *traj);
	
}	s_trctraj_intf;

//------------------------------------------------------------------------------

typedef struct trctraj
{
    s_trctraj_intf     *intf;

    t_u64               guid;

    char 				name[32];
	s_trcobj 		   *obj;
	
	t_f64 				time[2];
	
} 	s_trctraj;

typedef struct trctraj_attr
{
    t_u64               guid;

    char 				name[32];
	s_trcobj 		   *obj;
	
} 	s_trctraj_attr;

//------------------------------------------------------------------------------

inline
t_s8 trctraj_init (s_trctraj *traj, s_trctraj_attr *attr)
{
    traj->guid = attr->guid;

	memcpy(traj->name, attr->name, sizeof(traj->name));
	traj->obj = attr->obj;
	
	return traj->intf->init(traj, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_free (s_trctraj *traj)
{
	return traj->intf->free(traj);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_compile (s_trctraj *traj)
{
	return traj->intf->compile(traj);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_pos (s_trctraj *traj, t_f64 time, t_f64 *pos)
{
	return traj->intf->pos(traj, time, pos);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_rot (s_trctraj *traj, t_f64 time, t_f64 *rot)
{
	return traj->intf->rot(traj, time, rot);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_save (s_trctraj *traj, s_trcspl *spl, t_u8 **v_file)
{
	return traj->intf->save(traj, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trctraj_load (s_trctraj *traj, s_trcspl *spl, t_u8 **v_file)
{
	traj->intf->load(traj, spl, v_file);
	
	// compile trajectory for previews and interaction
	// to prevent damaged values when executing pos/rot calls
	
	trctraj_compile(traj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

inline
void trctraj_gui_attr (s_trctraj *traj, s_trctraj_attr *attr)
{
	traj->intf->gui_attr(traj, attr);
}

//------------------------------------------------------------------------------

inline
void trctraj_gui_edit (s_trctraj *traj)
{
	traj->intf->gui_edit(traj);
}

//------------------------------------------------------------------------------

inline
void trctraj_gui_view (s_trctraj *traj)
{
	traj->intf->gui_view(traj);
}

//------------------------------------------------------------------------------

s_trctraj* trctraj_malloc (s_trctraj_intf *traj_intf, s_trctraj_attr *attr)
{
	s_trctraj *traj = (s_trctraj*) malloc(traj_intf->data_sz);
	traj->intf = traj_intf;
	
	trctraj_init(traj, attr);
	
	return traj;
}

//------------------------------------------------------------------------------

void trctraj_dealloc (s_trctraj *traj)
{
	if (traj != NULL)
	{
		trctraj_free(traj);
		free(traj);
	}
}

//------------------------------------------------------------------------------

//void trctraj_copy (s_trceng *eng, s_trctraj *dest, s_trctraj *src)
//{
//	t_u8 *data_ref = (t_u8*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	t_u8 *data_ptr = data_ref;
//
//	trctraj_save (src , eng, &data_ptr);
//
//	// Reset pointer
//	data_ptr = data_ref;
//
//	*dest = *src;
//	trctraj_load (dest, eng, &data_ptr);
//
//	free(data_ref);
//}

//------------------------------------------------------------------------------

#endif /* __TRCTRAJ__ */
