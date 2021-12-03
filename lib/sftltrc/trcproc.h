
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
	
	s8_t (*init) 		(void *proc);
	s8_t (*free) 		(void *proc);
	s8_t (*pack) 		(void *proc, s_trcspl *spl);
	s8_t (*unpack) 		(void *proc, s_trcspl *spl);
	s8_t (*save) 		(void *proc, u8_t **v_file);
	s8_t (*load) 		(void *proc, u8_t **v_file);
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
	
} 	s_trcproc;

//------------------------------------------------------------------------------

inline
s8_t trcproc_init (s_trcproc **proc)
{
	return (*proc)->intf->init(proc);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_free (s_trcproc **proc)
{
	return (*proc)->intf->init(proc);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_pack (s_trcproc *proc, s_trcspl *spl)
{
	return proc->intf->pack(proc, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_unpack (s_trcproc *proc, s_trcspl *spl)
{
	return proc->intf->unpack(proc, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_save (s_trcproc *proc, u8_t **v_file)
{
	return proc->intf->save(proc, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_load (s_trcproc *proc, u8_t **v_file)
{
	return proc->intf->load(proc, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_update (s_trcproc *proc, s_trcobj *obj, s32_t offset)
{
	return proc->intf->update(proc, obj, offset);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC__ */


