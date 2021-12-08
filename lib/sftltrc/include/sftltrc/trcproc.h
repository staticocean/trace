
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcproc - trace

#ifndef __TRCPROC__
#define __TRCPROC__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcproc_intf
{
	char 				desc[32];
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *proc, void *attr);
	s8_t (*free) 		(void *proc);
	s8_t (*save) 		(void *proc, s_trcspl *spl, u8_t **v_file);
	s8_t (*load) 		(void *proc, s_trcspl *spl, u8_t **v_file);
	s8_t (*update) 		(void *proc, s_trcobj *obj, s32_t offset);
	
} 	s_trcproc_intf;

//------------------------------------------------------------------------------

typedef struct trcproc
{
	union
	{
		s_trcproc_intf *intf;
		s_trcspl_ptr 	intf_spl_ptr;
	};
	
	char 				name[32];
	
} 	s_trcproc;

typedef struct trcproc_attr
{
	char 				name[32];
	
} 	s_trcproc_attr;

//------------------------------------------------------------------------------

inline
s8_t trcproc_init (s_trcproc *proc, s_trcproc_attr *attr)
{
	memcpy(proc->name, attr->name, sizeof(proc->name));
	
	return proc->intf->init(proc, attr);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_free (s_trcproc *proc)
{
	return proc->intf->free(proc);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_save (s_trcproc *proc, s_trcspl *spl, u8_t **v_file)
{
	return proc->intf->save(proc, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_load (s_trcproc *proc, s_trcspl *spl, u8_t **v_file)
{
	return proc->intf->load(proc, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_update (s_trcproc *proc, s_trcobj *obj, s32_t offset)
{
	return proc->intf->update(proc, obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC__ */


