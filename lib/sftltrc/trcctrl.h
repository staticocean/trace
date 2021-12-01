
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCCTRL__
#define __TRCCTRL__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcctrl
{
	union
	{
		void 			*intf;
		s_trcspl_ptr 	intf_splptr;
	};
	
	char 				name[32];
	
} 	s_trcctrl;

typedef struct trcctrl_intf
{
	char 				desc[32];
	
	s8_t (*init) 		(s_trcctrl **ctrl);
	s8_t (*free) 		(s_trcctrl **ctrl);
	s8_t (*pack) 		(s_trcctrl  *ctrl, s_trcspl *spl);
	s8_t (*unpack) 		(s_trcctrl  *ctrl, s_trcspl *spl);
	s8_t (*save) 		(s_trcctrl  *ctrl, u8_t **v_file);
	s8_t (*load) 		(s_trcctrl  *ctrl, u8_t **v_file);
	s8_t (*reset) 		(s_trcctrl  *ctrl, s_trcobj *obj);
	s8_t (*update) 		(s_trcctrl  *ctrl, s_trcobj *obj);
	
} 	s_trcctrl_intf;

//------------------------------------------------------------------------------

inline s8_t trcctrl_free (s_trcctrl **ctrl)
{
	return ((s_trcctrl_intf*) (*ctrl)->intf)->init(ctrl);
}

//------------------------------------------------------------------------------

inline s8_t trcctrl_free (s_trcctrl **ctrl)
{
	return ((s_trcctrl_intf*) (*ctrl)->intf)->free(ctrl);
}

//------------------------------------------------------------------------------

inline s8_t trcctrl_pack (s_trcctrl *ctrl, s_trcspl *spl)
{
	return ((s_trcctrl_intf*) ctrl->intf)->pack(ctrl);
}

//------------------------------------------------------------------------------

inline s8_t trcctrl_unpack (s_trcctrl *ctrl, s_trcspl *spl)
{
	return ((s_trcctrl_intf*) ctrl->intf)->unpack(ctrl);
}

//------------------------------------------------------------------------------

inline s8_t trcctrl_save (s_trcctrl *ctrl, u8_t **v_file)
{
	return ((s_trcctrl_intf*) ctrl->intf)->save(ctrl, v_file);
}

//------------------------------------------------------------------------------

inline s8_t trcctrl_load (s_trcctrl *ctrl, u8_t **v_file)
{
	return ((s_trcctrl_intf*) ctrl->intf)->load(ctrl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_reset (s_trcctrl *ctrl, s_trcobj *obj)
{
	return ((s_trcctrl_intf*) ctrl->intf)->reset(ctrl);
}

//------------------------------------------------------------------------------

inline
s8_t trcctrl_update (s_trcctrl *ctrl, s_trcobj *obj)
{
	return ((s_trcctrl_intf*) ctrl->intf)->update(ctrl);
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

