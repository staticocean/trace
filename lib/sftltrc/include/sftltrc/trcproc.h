
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
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *proc, void *attr);
	t_s8 (*free) 		(void *proc);
	t_s8 (*save) 		(void *proc, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *proc, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*update) 		(void *proc, s_trcobj *obj, t_s32 offset);
	
	void (*gui_attr)	(void *proc, void *attr);
	void (*gui_edit)	(void *proc);
	void (*gui_view)	(void *proc);
	
} 	s_trcproc_intf;

//------------------------------------------------------------------------------

typedef struct trcproc
{
    s_trcproc_intf     *intf;

    t_u64               guid;

    char 				name[32];
	
} 	s_trcproc;

typedef struct trcproc_attr
{
    t_u64               guid;

    char 				name[32];
	
} 	s_trcproc_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcproc_init (s_trcproc *proc, s_trcproc_attr *attr)
{
    proc->guid = attr->guid;

	memcpy(proc->name, attr->name, sizeof(proc->name));
	
	return proc->intf->init(proc, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trcproc_free (s_trcproc *proc)
{
	return proc->intf->free(proc);
}

//------------------------------------------------------------------------------

inline
t_s8 trcproc_save (s_trcproc *proc, s_trcspl *spl, t_u8 **v_file)
{
	return proc->intf->save(proc, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcproc_load (s_trcproc *proc, s_trcspl *spl, t_u8 **v_file)
{
	return proc->intf->load(proc, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcproc_update (s_trcproc *proc, s_trcobj *obj, t_s32 offset)
{
	return proc->intf->update(proc, obj, offset);
}

//------------------------------------------------------------------------------

inline
void trcproc_gui_attr (s_trcproc *proc, s_trcproc_attr *attr)
{
	proc->intf->gui_attr(proc, attr);
}

//------------------------------------------------------------------------------

inline
void trcproc_gui_edit (s_trcproc *proc)
{
	proc->intf->gui_edit(proc);
}

//------------------------------------------------------------------------------

inline
void trcproc_gui_view (s_trcproc *proc)
{
	proc->intf->gui_view(proc);
}

//------------------------------------------------------------------------------

inline
s_trcproc* trcproc_malloc (s_trcproc_intf *proc_intf, s_trcproc_attr *attr)
{
	s_trcproc *proc = (s_trcproc*) malloc(proc_intf->data_sz);
	proc->intf = proc_intf;
	
	trcproc_init(proc, attr);
	
	return proc;
}

//------------------------------------------------------------------------------

#endif /* __TRCPROC__ */


