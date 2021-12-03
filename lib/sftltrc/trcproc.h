
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

typedef struct trcproc
{
	union
	{
		void 			*intf;
		s_trcspl_ptr 	intf_splptr;
	};
	
} 	s_trcproc;

typedef struct trcproc_intf
{
	char 				desc[32];
	
	s8_t (*init) 		(s_trjproc **proc);
	s8_t (*free) 		(s_trjproc **proc);
	s8_t (*pack) 		(s_trjproc  *proc, s_trcspl *spl);
	s8_t (*unpack) 		(s_trjproc  *proc, s_trcspl *spl);
	s8_t (*save) 		(s_trjproc  *proc, u8_t **v_file);
	s8_t (*load) 		(s_trjproc  *proc, u8_t **v_file);
	s8_t (*update) 		(s_trjproc  *proc, s_trcobj *obj, s32_t offset);
	
} 	s_trcproc_intf;

//------------------------------------------------------------------------------

inline
s8_t trcproc_init (s_trcproc **proc)
{
	return ((s_trcproc_intf*) (*proc)->intf)->init(proc);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_free (s_trcproc **proc)
{
	return ((s_trcproc_intf*) (*proc)->intf)->init(proc);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_pack (s_trcproc *proc, s_trcspl *spl)
{
	return ((s_trcproc_intf*) proc->intf)->pack(proc, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_unpack (s_trcproc *proc, s_trcspl *spl)
{
	return ((s_trcproc_intf*) proc->intf)->unpack(proc, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_save (s_trcproc *proc, u8_t **v_file)
{
	return ((s_trcproc_intf*) proc->intf)->save(proc, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_load (s_trcproc *proc, u8_t **v_file)
{
	return ((s_trcproc_intf*) proc->intf)->load(proc, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcproc_update (s_trjproc *proc, s_trcobj *obj, s32_t offset)
{
	return ((s_trcproc_intf*) proc->intf)->update(proc, obj);
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC__ */


