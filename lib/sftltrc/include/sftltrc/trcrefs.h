
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcrf - refernce frame

#ifndef __TRCREFS__
#define __TRCREFS__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vl.h>

#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcrefs_intf
{
	char 				desc[32];
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *refs, void *attr);
	s8_t (*free) 		(void *refs);
	s8_t (*save) 		(void *refs, s_trcspl *spl, u8_t **v_file);
	s8_t (*load) 		(void *refs, s_trcspl *spl, u8_t **v_file);
	s8_t (*pos_abs) 	(void *refs, f64_t *abs, f64_t *ref);
	s8_t (*pos_ref) 	(void *refs, f64_t *ref, f64_t *abs);
	s8_t (*vel_abs) 	(void *refs, f64_t *abs, f64_t *ref);
	s8_t (*vel_ref) 	(void *refs, f64_t *ref, f64_t *abs);
	s8_t (*rot_abs) 	(void *refs, f64_t *abs, f64_t *ref);
	s8_t (*rot_ref) 	(void *refs, f64_t *ref, f64_t *abs);
	
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
s8_t trcrefs_init (s_trcrefs *refs, s_trcrefs_attr *attr)
{
	memcpy(refs->name, attr->name, sizeof(refs->name));
	
	return refs->intf->init(refs, attr);
}

//------------------------------------------------------------------------------

inline
s8_t trcrefs_free (s_trcrefs *refs)
{
	return refs->intf->free(refs);
}

//------------------------------------------------------------------------------

inline
s8_t trcrefs_save (s_trcrefs *refs, s_trcspl *spl, u8_t **v_file)
{
	return refs->intf->save(refs, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcrefs_load (s_trcrefs *refs, s_trcspl *spl, u8_t **v_file)
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
//s8_t trcrefs_toabs (s_trcrefs *refs, f64_t *abs, f64_t *ref)
//{
//	return refs->intf->toabs(refs, abs, rel);
//}
//
////------------------------------------------------------------------------------
//
//inline
//s8_t trcrefs_toref (s_trcrefs *refs, f64_t *ref, f64_t *abs)
//{
//	return refs->intf->toref(refs, ref, abs);
//}

//------------------------------------------------------------------------------

#endif /* __TRCREFS__ */
