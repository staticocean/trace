
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
	t_s8 (*pos_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
	t_s8 (*pos_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);
	t_s8 (*vel_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
	t_s8 (*vel_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);
	t_s8 (*rot_abs) 	(void *refs, t_f64 *abs, t_f64 *ref);
	t_s8 (*rot_ref) 	(void *refs, t_f64 *ref, t_f64 *abs);
	
	void (*gui_attr)	(void *refs, void *attr);
	void (*gui_edit)	(void *refs);
	void (*gui_view)	(void *refs);
	
}	s_trcrefs_intf;

//------------------------------------------------------------------------------

typedef struct trcrefs
{
	union
	{
		s_trcrefs_intf	*intf;
		s_trcspl_ptr 	intf_spl_ptr;
	};
	
	char 				name[32];
	
} 	s_trcrefs;

typedef struct trcrefs_attr
{
	char 				name[32];
	
}	s_trcrefs_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcrefs_init (s_trcrefs *refs, s_trcrefs_attr *attr)
{
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
s_trcrefs* trcrefs_malloc (s_trcrefs_intf *intf_refs, s_trcrefs_attr *attr)
{
	s_trcrefs *refs = (s_trcrefs*) malloc(intf_refs->data_sz);
	refs->intf = intf_refs;
	
	trcrefs_init(refs, attr);
	
	return refs;
}

//------------------------------------------------------------------------------

//inline
//t_s8 trcrefs_toabs (s_trcrefs *refs, t_f64 *abs, t_f64 *ref)
//{
//	return refs->intf->toabs(refs, abs, rel);
//}
//
////------------------------------------------------------------------------------
//
//inline
//t_s8 trcrefs_toref (s_trcrefs *refs, t_f64 *ref, t_f64 *abs)
//{
//	return refs->intf->toref(refs, ref, abs);
//}

//------------------------------------------------------------------------------

#endif /* __TRCREFS__ */
