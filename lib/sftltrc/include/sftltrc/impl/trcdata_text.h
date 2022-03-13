
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcdata_text - trace

#ifndef __TRCDATA_TEXT__
#define __TRCDATA_TEXT__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcdata.h>

//------------------------------------------------------------------------------

typedef struct trcdata_text
{
	s_trcdata 		super;
	
	char 			file_name[256];
	char 			*file_data;
	s64_t 			file_size;
	s64_t 			file_lcnt;
	
} 	s_trcdata_text;

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_init__ (s_trcdata **data)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	*data = (s_trcdata*) malloc(sizeof(s_trcdata_text));
	
	strcpy(data_text->file_name, "No file selected");
	data_text->file_data = NULL;
	data_text->file_size = 0x00;
	data_text->file_lcnt = 0x00;
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_free__ (s_trcdata **data)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	if (data_text->file_data != NULL)
	{
		free(data_text->file_data);
		
		data_text->file_data = NULL;
		data_text->file_size = 0x00;
		data_text->file_lcnt = 0x00;
	}
	
	free(data);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_save__ (s_trcdata *data, t_u8 **v_file)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_load__ (s_trcdata *data, t_u8 **v_file)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	data_text->file_data = NULL;
	data_text->file_size = 0x00;
	data_text->file_lcnt = 0x00;
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_render__ (s_trcdata *data, s_trcobj *obj)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	data_text->file_data = (char*) malloc(obj->log_sz * 256);
	char *file_ptr = data_text->file_data;
	data_text->file_size = 0x00;
	data_text->file_lcnt = 0x00;
	
	FILE *file_handle = fopen(data_text->file_name, "w+");
	
	for (t_s32 i = 0; i < obj->log_sz; ++i)
	{
		self->file_size += sprintf(file_ptr, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
			obj->log_ls[i].time[0],
			
			obj->log_ls[i].pos[0][0],
			obj->log_ls[i].pos[0][1],
			obj->log_ls[i].pos[0][2],
			
			obj->log_ls[i].pos[1][0],
			obj->log_ls[i].pos[1][1],
			obj->log_ls[i].pos[1][2],
			
			obj->log_ls[i].pos[2][0],
			obj->log_ls[i].pos[2][1],
			obj->log_ls[i].pos[2][2]
	   	);

		fprintf(file_handle, file_ptr);
		
		file_ptr += 256;
		
		self->file_lcnt++;
	}
	
	fclose(file_handle);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trcdata_text_reset__ (s_trcdata *data, s_trcobj *obj)
{
	s_trcdata_text *data_text = (s_trcdata_text*) *data;
	
	if (data_text->file_data != NULL)
	{
		free(data_text->file_data);
		data_text->file_data = NULL;
	}
	
	data_text->file_size = 0x00;
	data_text->file_lcnt = 0x00;
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __TRCDATA_TEXT__ */
