
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcctrl - trace

#ifndef __TRCCTRL__
#define __TRCCTRL__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl_intf
{
	char 				desc[32];
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *ctrl, void *attr);
	t_s8 (*free) 		(void *ctrl);
	t_s8 (*save) 		(void *ctrl, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *ctrl, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*reset) 		(void *ctrl, s_trcobj *obj);
	t_s8 (*update) 		(void *ctrl, s_trcobj *obj);

	void (*gui_attr)	(void *ctrl, void *attr);
	void (*gui_edit)	(void *ctrl);
	void (*gui_view)	(void *ctrl);
	
} 	s_trcctrl_intf;

//------------------------------------------------------------------------------

typedef struct trcctrl
{
	union
	{
		s_trcctrl_intf *intf;
		s_trcspl_ptr 	intf_spl_ptr;
	};
	
	char 				name[32];
	s_trcobj 			*obj;

} 	s_trcctrl;

typedef struct trcctrl_attr
{
	char 				name[32];
	s_trcobj 			*obj;
	
} 	s_trcctrl_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_init (s_trcctrl *ctrl, s_trcctrl_attr *attr)
{
	memcpy(ctrl->name, attr->name, sizeof(ctrl->name));
	ctrl->obj = attr->obj;
	
	return ctrl->intf->init(ctrl, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_free (s_trcctrl *ctrl)
{
	return ctrl->intf->free(ctrl);
}

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_save (s_trcctrl *ctrl, s_trcspl *spl, t_u8 **v_file)
{
	return ctrl->intf->save(ctrl, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_load (s_trcctrl *ctrl, s_trcspl *spl, t_u8 **v_file)
{
	return ctrl->intf->load(ctrl, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_reset (s_trcctrl *ctrl, s_trcobj *obj)
{
	return ctrl->intf->reset(ctrl, obj);
}

//------------------------------------------------------------------------------

inline
t_s8 trcctrl_update (s_trcctrl *ctrl, s_trcobj *obj)
{
	return ctrl->intf->update(ctrl, obj);
}

//------------------------------------------------------------------------------

inline
void trcctrl_gui_attr (s_trcctrl *ctrl, s_trcctrl_attr *attr)
{
	ctrl->intf->gui_attr(ctrl, attr);
}

//------------------------------------------------------------------------------

inline
void trcctrl_gui_edit (s_trcctrl *ctrl)
{
	ctrl->intf->gui_edit(ctrl);
}

//------------------------------------------------------------------------------

inline
void trcctrl_gui_view (s_trcctrl *ctrl)
{
	ctrl->intf->gui_view(ctrl);
}

//------------------------------------------------------------------------------

inline
s_trcctrl* trcctrl_malloc (s_trcctrl_intf *intf_ctrl, s_trcctrl_attr *attr)
{
	s_trcctrl *ctrl = (s_trcctrl*) malloc(intf_ctrl->data_sz);
	ctrl->intf = intf_ctrl;
	
	trcctrl_init(ctrl, attr);
	
	return ctrl;
}

//------------------------------------------------------------------------------

//void trcctrl_copy (s_trcctrl *dest, s_trcctrl *src)
//{
//	t_u8 *data_ref = (t_u8*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	t_u8 *data_ptr = data_ref;
//
//	trcctrl_save (src , eng, &data_ptr);
//
//	// Reset pointer
//	data_ptr = data_ref;
//
//	*dest = *src;
//	trcctrl_load (dest, eng, &data_ptr);
//
//	free(data_ref);
//}

//------------------------------------------------------------------------------

#endif /* __TRCCTRL__ */

