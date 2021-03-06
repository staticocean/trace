
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
	
	t_s32				data_sz;
	t_s32 				attr_sz;
	
	t_s8 (*init) 		(void *data, void *attr);
	t_s8 (*free) 		(void *data);
	t_s8 (*save) 		(void *data, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*load) 		(void *data, s_trcspl *spl, t_u8 **v_file);
	t_s8 (*reset) 		(void *data, s_trcobj *obj);
	t_s8 (*render) 		(void *data, s_trcobj *obj);
	
	void (*gui_attr)	(void *data, void *attr);
	void (*gui_edit)	(void *data);
	void (*gui_view)	(void *data);
	
} 	s_trcdata_intf;

//------------------------------------------------------------------------------

typedef struct trcdata
{
    s_trcdata_intf     *intf;

    t_u64               guid;

    char 				name[32];
	s_trcobj 		   *obj;

} 	s_trcdata;

typedef struct trcdata_attr
{
    t_u64               guid;

	char 				name[32];
	s_trcobj 		   *obj;
	
} 	s_trcdata_attr;

//------------------------------------------------------------------------------

inline
t_s8 trcdata_init (s_trcdata *data, s_trcdata_attr *attr)
{
    data->guid = attr->guid;

	memcpy(data->name, attr->name, sizeof(data->name));
	data->obj = attr->obj;
	
	return data->intf->init(data, attr);
}

//------------------------------------------------------------------------------

inline
t_s8 trcdata_free (s_trcdata *data)
{
	return data->intf->free(data);
}

//------------------------------------------------------------------------------

inline
t_s8 trcdata_save (s_trcdata *data, s_trcspl *spl, t_u8 **v_file)
{
	return data->intf->save(data, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcdata_load (s_trcdata *data, s_trcspl *spl, t_u8 **v_file)
{
	return data->intf->load(data, spl, v_file);
}

//------------------------------------------------------------------------------

inline
t_s8 trcdata_reset (s_trcdata *data, s_trcobj *obj)
{
	return data->intf->reset(data, obj);
}

//------------------------------------------------------------------------------

inline
t_s8 trcdata_render (s_trcdata *data, s_trcobj *obj)
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
s_trcdata* trcdata_malloc (s_trcdata_intf *data_intf, s_trcdata_attr *attr)
{
	s_trcdata *data = (s_trcdata*) malloc(data_intf->data_sz);
	data->intf = data_intf;
	
	trcdata_init(data, attr);
	
	return data;
}

//------------------------------------------------------------------------------

//void trcdata_copy (s_trceng *eng, s_trcdata *dest, s_trcdata *src)
//{
//	t_u8 *data_ref = (t_u8*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	t_u8 *data_ptr = data_ref;
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
