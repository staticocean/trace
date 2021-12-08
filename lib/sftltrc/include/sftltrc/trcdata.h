
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

typedef struct trcdata_intf
{
	char 				desc[32];
	
	s32_t				data_sz;
	s32_t 				attr_sz;
	
	s8_t (*init) 		(void *data, void *attr);
	s8_t (*free) 		(void *data);
	s8_t (*save) 		(void *data, s_trcspl *spl, u8_t **v_file);
	s8_t (*load) 		(void *data, s_trcspl *spl, u8_t **v_file);
	s8_t (*reset) 		(void *data, s_trcobj *obj);
	s8_t (*render) 		(void *data, s_trcobj *obj);
	
	void (*gui_attr)	(void *data, void *attr);
	void (*gui_edit)	(void *data);
	void (*gui_view)	(void *data);
	
} 	s_trcdata_intf;

//------------------------------------------------------------------------------

typedef struct trcdata
{
	union
	{
		s_trcdata_intf *intf;
		s_trcspl_ptr 	intf_spl_ptr;
	};
	
	char 				name[32];
	
} 	s_trcdata;

typedef struct trcdata_attr
{
	char 				name[32];
	
} 	s_trcdata_attr;

//------------------------------------------------------------------------------

inline
s8_t trcdata_init (s_trcdata *data, s_trcdata_attr *attr)
{
	memcpy(data->name, attr->name, sizeof(data->name));
	
	return data->intf->init(data, attr);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_free (s_trcdata *data)
{
	return data->intf->free(data);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_save (s_trcdata *data, s_trcspl *spl, u8_t **v_file)
{
	return data->intf->save(data, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_load (s_trcdata *data, s_trcspl *spl, u8_t **v_file)
{
	return data->intf->load(data, spl, v_file);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_reset (s_trcdata *data, s_trcobj *obj)
{
	return data->intf->reset(data, obj);
}

//------------------------------------------------------------------------------

inline
s8_t trcdata_render (s_trcdata *data, s_trcobj *obj)
{
	return data->intf->render(data, obj);
}

//------------------------------------------------------------------------------

inline
void trcdata_gui_attr (s_trcdata *data, s_trcdata_attr *attr)
{
	data->intf->gui_attr(data, attr);
}

//------------------------------------------------------------------------------

inline
void trcdata_gui_edit (s_trcdata *data)
{
	data->intf->gui_edit(data);
}

//------------------------------------------------------------------------------

inline
void trcdata_gui_view (s_trcdata *data)
{
	data->intf->gui_view(data);
}

//------------------------------------------------------------------------------

inline
s_trcdata* trcdata_malloc (s_trcdata_intf *intf_data, s_trcdata_attr *attr)
{
	s_trcdata *data = (s_trcdata*) malloc(intf_data->data_sz);
	data->intf = intf_data;
	
	trcdata_init(data, attr);
	
	return data;
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
