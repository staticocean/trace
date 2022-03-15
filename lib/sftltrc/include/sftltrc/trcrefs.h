
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrf - refernce frame

#ifndef __TRCREFS__
#define __TRCREFS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>

#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_intf
{
	char 				desc[32];
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *refs, void *attr);
	t_s8 (*free) 		(void *refs);
	t_s8 (*save) 		(void *refs, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *refs, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*pos_abs) 	(void *refs, t_f64 *pos_abs, t_f64 *pos_ref);
	t_s8 (*pos_ref) 	(void *refs, t_f64 *pos_ref, t_f64 *pos_abs);
	t_s8 (*vel_abs) 	(void *refs, t_f64 *vel_abs, t_f64 *vel_ref);
	t_s8 (*vel_ref) 	(void *refs, t_f64 *vel_ref, t_f64 *vel_abs);
	t_s8 (*rot_abs) 	(void *refs, t_f64 *rot_abs, t_f64 *rot_ref);
	t_s8 (*rot_ref) 	(void *refs, t_f64 *rot_ref, t_f64 *rot_abs);
	
	void (*gui_attr)	(void *refs, void *attr);
	void (*gui_edit)	(void *refs);
	void (*gui_view)	(void *refs);
	
}	s_trcrefs_intf;

//------------------------------------------------------------------------------

typedef struct trcrefs
{
    s_trcrefs_intf	   *intf;

    t_u64               guid;

    char 				name[32];
	
} 	s_trcrefs;

typedef struct trcrefs_attr
{
    t_u64               guid;

    char 				name[32];
	
}	s_trcrefs_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_init (s_trcrefs *refs, s_trcrefs_attr *attr)
{
    refs->guid = attr->guid;

    memcpy(refs->name, attr->name, sizeof(refs->name));
	
	return refs->intf->init(refs, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_free (s_trcrefs *refs)
{
	return refs->intf->free(refs);
}

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_save (s_trcrefs *refs, s_trcspl *spl, t_u8 **v_file)
{
	return refs->intf->save(refs, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_load (s_trcrefs *refs, s_trcspl *spl, t_u8 **v_file)
{
	return refs->intf->load(refs, spl, v_file);
}

//------------------------------------------------------------------------------

inline
void trcrefs_gui_attr (s_trcrefs *refs, s_trcrefs_attr *attr)
{
	refs->intf->gui_attr(refs, attr);
}

//------------------------------------------------------------------------------

inline
void trcrefs_gui_edit (s_trcrefs *refs)
{
	refs->intf->gui_edit(refs);
}

//------------------------------------------------------------------------------

inline
void trcrefs_gui_view (s_trcrefs *refs)
{
	refs->intf->gui_view(refs);
}

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_pos_abs (s_trcrefs *refs, t_f64 *pos_abs, t_f64 *pos_ref)
{
    return refs->intf->pos_abs(refs, pos_abs, pos_ref);
}

inline
t_s8 trcrefs_pos_ref (s_trcrefs *refs, t_f64 *pos_ref, t_f64 *pos_abs)
{
    return refs->intf->pos_ref(refs, pos_ref, pos_abs);
}

inline
t_s8 trcrefs_vel_abs (s_trcrefs *refs, t_f64 *vel_abs, t_f64 *vel_ref)
{
    return refs->intf->vel_abs(refs, vel_abs, vel_ref);
}

inline
t_s8 trcrefs_vel_ref (s_trcrefs *refs, t_f64 *vel_ref, t_f64 *vel_abs)
{
    return refs->intf->vel_ref(refs, vel_ref, vel_abs);
}

inline
t_s8 trcrefs_rot_abs (s_trcrefs *refs, t_f64 *rot_abs, t_f64 *rot_ref)
{
    return refs->intf->rot_abs(refs, rot_abs, rot_ref);
}

inline
t_s8 trcrefs_rot_ref (s_trcrefs *refs, t_f64 *rot_ref, t_f64 *rot_abs)
{
    return refs->intf->rot_ref(refs, rot_ref, rot_abs);
}

//------------------------------------------------------------------------------

inline
s_trcrefs* trcrefs_malloc (s_trcrefs_intf *refs_intf, s_trcrefs_attr *attr)
{
    s_trcrefs *refs = (s_trcrefs*) malloc(refs_intf->data_sz);
    refs->intf = refs_intf;

    trcrefs_init(refs, attr);

    return refs;
}

//------------------------------------------------------------------------------

#endif /* __TRCREFS__ */
