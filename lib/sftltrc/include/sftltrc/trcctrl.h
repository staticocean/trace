
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
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *ctrl, void *attr);
	s8_t (*free) 		(void *ctrl);
	s8_t (*save) 		(void *ctrl, s_trcspl *spl, u8_t **v_file);
	s8_t (*load) 		(void *ctrl, s_trcspl *spl, u8_t **v_file);
	s8_t (*reset) 		(void *ctrl, s_trcobj *obj);
	s8_t (*update) 		(void *ctrl, s_trcobj *obj);

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
s8_t trcctrl_init (s_trcctrl *ctrl, s_trcctrl_attr *attr)
{
	memcpy(ctrl->name, attr->name, sizeof(ctrl->name));
	ctrl->obj = attr->obj;
	
	return ctrl->intf->init(ctrl, attr);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_free (s_trcctrl *ctrl)
{
	return ctrl->intf->free(ctrl);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_save (s_trcctrl *ctrl, s_trcspl *spl, u8_t **v_file)
{
	return ctrl->intf->save(ctrl, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_load (s_trcctrl *ctrl, s_trcspl *spl, u8_t **v_file)
{
	return ctrl->intf->load(ctrl, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_reset (s_trcctrl *ctrl, s_trcobj *obj)
{
	return ctrl->intf->reset(ctrl, obj);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_update (s_trcctrl *ctrl, s_trcobj *obj)
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
//	u8_t *data_ref = (u8_t*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	u8_t *data_ptr = data_ref;
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

