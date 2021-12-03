
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

#include "gui_env.h"

//------------------------------------------------------------------------------

//dummy lib
struct LibraryFunction __null_api__[] = { {NULL, NULL} };
void __null_api_init__(Picoc *pc) {}

void picoc_api_init(Picoc *pc) { return; }

void picoc_api_speedtest  (struct ParseState *Parser, struct Value *ReturnValue,
						   struct Value **Param, int NumArgs)
{
	for (volatile int i = 0; i < 10000000; ++i) { volatile int a; a = i * i; }
}

void trctraj_pos(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_trctraj *traj = (s_trctraj*) Param[0]->Val->Pointer;
	f64_t time = (f64_t) Param[1]->Val->FP;
	f64_t *pos = (f64_t*) Param[2]->Val->Pointer;
	
	traj->pos(traj->data, time, pos);
}

void trctraj_rot(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_trctraj *traj = (s_trctraj*) Param[0]->Val->Pointer;
	f64_t time = (f64_t) Param[1]->Val->FP;
	f64_t *rot = (f64_t*) Param[2]->Val->Pointer;
	
	traj->rot(traj->data, time, rot);
}

static const u32_t st_size = 255;
static u32_t st_offset = 0x00;
static u32_t st_id[st_size];
static char st_init[st_size][64];
static char st_free[st_size][64];
static char st_reset[st_size][64];
static char st_update[st_size][64];
static s_gui_env *st_env[st_size];

u8_t __api_ctrl_init__(void **data, void *config)
{
    // TO-DO what the hell is this?
	uint64_t offset = (uint64_t) *data;
	s_gui_env *self = st_env[offset];
	
	char call[255];
	
	sprintf(call, "%s (__api_ctrl_data_ref__, __api_ctrl_config__);", st_init[offset]);
	
	self->api_ctrl_data_ref = data;
	self->api_ctrl_config = config;
	
	if (PicocPlatformSetExitPoint(&self->env)) { }
	
	else
	{
		PicocParse(&self->env, "trc_env", call, strlen(call),
				   true, false, false, true);
	}
	
	return 0x00;
}

u8_t __api_ctrl_free__(void **data)
{
	return 0x00;
}

u8_t __api_ctrl_reset__(void *data, void *obj)
{
	return 0x00;
}

u8_t __api_ctrl_update__(void *data, void *obj)
{
	return 0x00;
}

void trceng_add_ctrlapi(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_gui_env *env         = (s_gui_env*) Param[0]->Val->Pointer;
	u32_t 	  *ctrl_sz = (u32_t*)      Param[1]->Val->Pointer;
	s_trcctrl    *ctrl_ls   = (s_trcctrl*)    Param[2]->Val->Pointer;
	
	memcpy(ctrl_ls[*ctrl_sz].desc, Param[3]->Val->Pointer, 32);
	ctrl_ls[*ctrl_sz].hash    = crc32_iso_str(ctrl_ls[*ctrl_sz].desc);
	ctrl_ls[*ctrl_sz].data    = (void*) st_offset;
	ctrl_ls[*ctrl_sz].config  = Param[4]->Val->Pointer;
	ctrl_ls[*ctrl_sz].init    = __api_ctrl_init__;
	ctrl_ls[*ctrl_sz].free    = __api_ctrl_free__;
	ctrl_ls[*ctrl_sz].reset   = __api_ctrl_reset__;
	ctrl_ls[*ctrl_sz].update  = __api_ctrl_update__;
	
	st_env[st_offset] = env;
	memcpy(&st_init[st_offset][0]  , Param[5]->Val->Pointer, 64);
	memcpy(&st_free[st_offset][0]  , Param[6]->Val->Pointer, 64);
	memcpy(&st_reset[st_offset][0] , Param[7]->Val->Pointer, 64);
	memcpy(&st_update[st_offset][0], Param[8]->Val->Pointer, 64);
	
	(*ctrl_sz)++;
	st_offset++;
}


/* list of all library functions and their prototypes */
struct LibraryFunction picoc_api_functions[] =
		{
				{ picoc_api_speedtest, "void api_speedtest(void);" },
				{ trctraj_pos, "void trctraj_pos(s_trctraj *traj, f64_t time, f64_t *pos);" },
				{ trctraj_rot, "void trctraj_rot(s_trctraj *traj, f64_t time, f64_t *rot);" },
				{ trceng_add_ctrlapi, "void trceng_add_ctrlapi(void *env, void *ctrl_sz, void *ctrl_ls, char *desc, void *config, char *init, char *free, char *reset, char *update);" },
				{ NULL, NULL }
		};

void __env_init__(s_gui_env *self)
{
	PicocInitialise(&self->env, PICOC_STACK_SIZE);
	PicocIncludeAllSystemHeaders(&self->env);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
	
	self->env.CStdOut = self->out_s;
	
	char env_define[] = "#define __TRCENV__";
	
	PicocParse(&self->env, "trc_env", env_define, strlen(env_define),
			   true, false, false, true);
	
//	VariableDefinePlatformVar(&self->env, NULL, "__api_traj_data_ref__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_traj_data_ref, 0x01);
//	VariableDefinePlatformVar(&self->env, NULL, "__api_traj_data__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_traj_data    , 0x01);
//	VariableDefinePlatformVar(&self->env, NULL, "__api_traj_config__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_traj_config  , 0x01);
//	VariableDefinePlatformVar(&self->env, NULL, "__api_traj_obj__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_traj_obj     , 0x01);
	
	VariableDefinePlatformVar(&self->env, NULL, "__api_ctrl_data_ref__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_ctrl_data_ref, 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_ctrl_data__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_ctrl_data    , 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_ctrl_config__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_ctrl_config  , 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_ctrl_obj__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_ctrl_obj     , 0x01);
	
	VariableDefinePlatformVar(&self->env, NULL, "__api_data_data_ref__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_data_data_ref, 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_data_data__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_data_data    , 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_data_config__"	, self->env.VoidPtrType, (union AnyValue *) &self->api_data_config  , 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__api_data_obj__"		, self->env.VoidPtrType, (union AnyValue *) &self->api_data_obj     , 0x01);
	
	VariableDefinePlatformVar(&self->env, NULL, "__eng__"		, self->env.VoidPtrType, (union AnyValue *) &self->eng, 0x01);
	VariableDefinePlatformVar(&self->env, NULL, "__env__"		, self->env.VoidPtrType, (union AnyValue *) &self, 0x00);
	
	VariableDefinePlatformVar(&self->env, NULL, "__traj_sz__", self->env.VoidPtrType, (union AnyValue *) &self->traj_sz, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__traj_ls__"  , self->env.VoidPtrType, (union AnyValue *) &self->traj_ls, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__ctrl_sz__", self->env.VoidPtrType, (union AnyValue *) &self->ctrl_sz, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__ctrl_ls__"  , self->env.VoidPtrType, (union AnyValue *) &self->ctrl_ls, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__data_offset__", self->env.VoidPtrType, (union AnyValue *) &self->data_offset, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__data_list__"  , self->env.VoidPtrType, (union AnyValue *) &self->data_list, 0x00);
	
	IncludeRegister(&self->env, "api.h", &picoc_api_init, &picoc_api_functions[0], NULL);
	
	FILE *file_handle = fopen("plugins/config", "r");
	
	if (file_handle == NULL)
	{
		printf("config file not found");
	}
	
	else
	{
		rewind(file_handle);
		
		char plugin_path[256];
		
		while (fscanf(file_handle, "%s", plugin_path) != EOF)
		{
			FILE *plugin_file = fopen(plugin_path, "r");
			
			if (plugin_file == NULL)
			{
			
			}
			
			else
			{
				u32_t plugin_size;
				u8_t *plugin_data;
				
				fseek (plugin_file, 0x00, SEEK_END);
				
				plugin_size = ftell (plugin_file);
				plugin_data = (u8_t*) malloc(plugin_size+1);
				
				rewind (plugin_file);
				
				printf("%s\n", plugin_path);
				
				fread (plugin_data, plugin_size, 1, plugin_file);
				fclose(plugin_file);
				
				while (plugin_size > 0x00 && plugin_data[plugin_size-1] != '\r' && plugin_data[plugin_size-1] != '\n')
				{
					plugin_size--;
				}
				
				plugin_data[plugin_size] = '\0';
				
				u32_t offset = strlen(plugin_path) - 1;
				while ((offset > 0x00) && (plugin_path[offset] != '/')) { --offset; }
				
				if (PicocPlatformSetExitPoint(&self->env))
				{
				
				}
				
				else
				{
					printf("%s\n", &plugin_path[offset+1]);
					fflush(stdout);
					
					PicocParse(&self->env, &plugin_path[offset+1], (const char*) plugin_data, plugin_size,
							   true, false, false, true);
					
					IncludeRegister(&self->env, &plugin_path[offset + 1],
									__null_api_init__, __null_api__, "");
				}
				
				free(plugin_data);
			}
		}
		
		fclose(file_handle);
	}
	
	fflush(stdout);
}

void gui_env_reset(s_gui_env *self)
{
	PicocCleanup(&self->env);
	__env_init__(self);
}

void gui_env_init(s_gui_env *self, s_gui_env_init attr)
{
	self->eng = attr.eng;
	
	self->traj_sz = attr.traj_sz;
	self->ctrl_sz = attr.ctrl_sz;
	self->data_offset = attr.data_offset;
	
	self->traj_ls = attr.traj_ls;
	self->ctrl_ls = attr.ctrl_ls;
	self->data_list = attr.data_list;
	
	__env_init__(self);
}

void gui_env_sreset(s_gui_env *self)
{
	fclose(self->out_s);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
}

//------------------------------------------------------------------------------



