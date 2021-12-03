
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata - trace

#ifndef __TRCDATA__
#define __TRCDATA__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trcdata
{
	union
	{
		void 			*intf;
		s_trcspl_ptr 	intf_splptr;
	};
	
	char 				name[32];
	
} 	s_trcdata;

typedef struct trcdata_intf
{
	char 				desc[32];
	
	s8_t (*init) 		(s_trcdata **data);
	s8_t (*free) 		(s_trcdata **data);
	s8_t (*pack) 		(s_trcdata  *data, s_trcspl *spl);
	s8_t (*unpack) 		(s_trcdata  *data, s_trcspl *spl);
	s8_t (*save) 		(s_trcdata  *data, u8_t **v_file);
	s8_t (*load) 		(s_trcdata  *data, u8_t **v_file);
	s8_t (*reset) 		(s_trcdata  *data, s_trcobj *obj);
	s8_t (*render) 		(s_trcdata  *data, s_trcobj *obj);
	
} 	s_trcdata_intf;

//------------------------------------------------------------------------------

inline
s8_t trcdata_init (s_trcdata **data)
{
	return ((s_trcdata_intf*) (*data)->intf)->init(data);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_free (s_trcdata **data)
{
	return ((s_trcdata_intf*) (*data)->intf)->free(data);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_pack (s_trcdata *data, s_trcspl *spl)
{
	return ((s_trcdata_intf*) data->intf)->pack(data, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_unpack (s_trcdata *data, s_trcspl *spl)
{
	return ((s_trcdata_intf*) data->intf)->unpack(data, spl);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_save (s_trcdata *data, u8_t **v_file)
{
	return ((s_trcdata_intf*) data->intf)->save(data, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_load (s_trcdata *data, u8_t **v_file)
{
	return ((s_trcdata_intf*) data->intf)->load(data, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_reset (s_trcdata *data, s_trcobj *obj)
{
	return ((s_trcdata_intf*) data->intf)->reset(data, obj);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_render (s_trcdata *data, s_trcobj *obj)
{
	return ((s_trcdata_intf*) data->intf)->render(data, obj);
}

//------------------------------------------------------------------------------

//void trcdata_copy (s_trceng *eng, s_trcdata *dest, s_trcdata *src)
//{
//	u8_t *data_ref = (u8_t*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	u8_t *data_ptr = data_ref;
//
//	trcdata_save (src , eng, &data_ptr);
//
//	// Reset pointer
//	data_ptr = data_ref;
//
//	*dest = *src;
//	trcdata_load (dest, eng, &data_ptr);
//
//	free(data_ref);
//
//	return;
//}

//------------------------------------------------------------------------------

#endif /* __TRCDATA__ */
