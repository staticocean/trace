
#ifndef __CTRL_TEST__
#define __CTRL_TEST__

//------------------------------------------------------------------------------

typedef struct ctrl_test
{
	t_u32 id;
	t_u8 *temp;
	
} 	s_ctrl_test;

typedef struct ctrl_test_init
{
	t_u32 id;
	t_u8 *temp;
	
} 	s_ctrl_test_init;

//------------------------------------------------------------------------------

inline t_u8 ctrl_test_init(s_ctrl_test *data, s_ctrl_test_init attr)
{
	data->id = 0x01;
	data->temp = attr.temp;
	
	return 0x00;
}

inline t_u8 ctrl_test_reset(s_ctrl_test *data, s_trcobj *obj)
{
	return 0x00;
}

inline t_u8 ctrl_test_update(s_ctrl_test *data, s_trcobj *obj)
{
	return 0x00;
}
//------------------------------------------------------------------------------

inline t_u8 ctrl_test_init_ (void **data, void *config)
{
	*data = (s_ctrl_test*) malloc(sizeof(s_ctrl_test));
	
	s_ctrl_test *ctrl = (s_ctrl_test*) *data;
//	s_ctrl_test_init *attr = (s_ctrl_test_init*) config;
	
//	attr->temp = (t_u8*) malloc(100);
	
	s_ctrl_test_init attr;
	attr.temp = (t_u8*) malloc(100);

	return ctrl_test_init(ctrl, attr);
}

inline t_u8 ctrl_test_free_ (void **data)
{
	s_ctrl_test *ctrl = (s_ctrl_test*) *data;
	
	free(ctrl->temp);
	free(ctrl);
	
	return 0x00;
}

inline t_u8 ctrl_test_reset_(void *data, void *obj)
{
	s_trcobj *obj = (s_trcobj*) obj;
	s_ctrl_test *ctrl = (s_ctrl_test*) data;
	
	return ctrl_test_reset(ctrl, obj);
}

inline t_u8 ctrl_test_update_(void *data, void *obj)
{
	s_trcobj *obj = (s_trcobj*) obj;
	s_ctrl_test *ctrl = (s_ctrl_test*) data;
	
	return ctrl_test_update(ctrl, obj);
}

//------------------------------------------------------------------------------

#endif __CTRL_TEST__