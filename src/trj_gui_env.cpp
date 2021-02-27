
#include "trj_gui_env.h"

//----------------------------------------------------------------

//dummy lib
struct LibraryFunction __null_api__[] = { {NULL, NULL} };
void __null_api_init__(Picoc *pc) {}

void picoc_api_init(Picoc *pc) { return; }

void picoc_api_speedtest  (struct ParseState *Parser, struct Value *ReturnValue,
						   struct Value **Param, int NumArgs)
{
	for (volatile int i = 0; i < 10000000; ++i) { volatile int a; a = i * i; }
}

void trj_traj_pos(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_trj_traj *traj = (s_trj_traj*) Param[0]->Val->Pointer;
	vlf_t time = (vlf_t) Param[1]->Val->FP;
	vlf_t *pos = (vlf_t*) Param[2]->Val->Pointer;
	
	traj->pos(traj->data, time, pos);
}

void trj_traj_rot(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_trj_traj *traj = (s_trj_traj*) Param[0]->Val->Pointer;
	vlf_t time = (vlf_t) Param[1]->Val->FP;
	vlf_t *rot = (vlf_t*) Param[2]->Val->Pointer;
	
	traj->rot(traj->data, time, rot);
}

static const uint32_t st_size = 255;
static uint32_t st_offset = 0x00;
static uint32_t st_id[st_size];
static char st_init[st_size][64];
static char st_free[st_size][64];
static char st_reset[st_size][64];
static char st_update[st_size][64];
static s_trj_gui_env *st_env[st_size];

uint8_t __api_ctrl_init__(void **data, void *config)
{
	uint32_t offset = (uint32_t) *data;
	s_trj_gui_env *self = st_env[offset];
	
	char call[255];
	
	sprintf(call, "%s (__api_ctrl_data_ref__, __api_ctrl_config__);", st_init[offset]);
	
	self->api_ctrl_data_ref = data;
	self->api_ctrl_config = config;
	
	if (PicocPlatformSetExitPoint(&self->env)) { }
	
	else
	{
		PicocParse(&self->env, "trj_env", call, strlen(call),
				   true, false, false, true);
	}
	
	return 0x00;
}

uint8_t __api_ctrl_free__(void **data)
{
	return 0x00;
}

uint8_t __api_ctrl_reset__(void *data, void *obj)
{
	return 0x00;
}

uint8_t __api_ctrl_update__(void *data, void *obj)
{
	return 0x00;
}

void trj_eng_add_ctrlapi(struct ParseState *Parser, struct Value *ReturnValue,
				  struct Value **Param, int NumArgs)
{
	s_trj_gui_env *env         = (s_trj_gui_env*) Param[0]->Val->Pointer;
	uint32_t 	  *ctrl_offset = (uint32_t*)      Param[1]->Val->Pointer;
	s_trj_ctrl    *ctrl_list   = (s_trj_ctrl*)    Param[2]->Val->Pointer;
	
	memcpy(ctrl_list[*ctrl_offset].desc, Param[3]->Val->Pointer, 32);
	ctrl_list[*ctrl_offset].hash = vl_crc32(ctrl_list[*ctrl_offset].desc);
	ctrl_list[*ctrl_offset].data   = (void*) st_offset;
	ctrl_list[*ctrl_offset].config = Param[4]->Val->Pointer;
	ctrl_list[*ctrl_offset].init   = __api_ctrl_init__;
	ctrl_list[*ctrl_offset].free   = __api_ctrl_free__;
	ctrl_list[*ctrl_offset].reset  = __api_ctrl_reset__;
	ctrl_list[*ctrl_offset].update = __api_ctrl_update__;
	
	st_env[st_offset] = env;
	memcpy(&st_init[st_offset][0]  , Param[5]->Val->Pointer, 64);
	memcpy(&st_free[st_offset][0]  , Param[6]->Val->Pointer, 64);
	memcpy(&st_reset[st_offset][0] , Param[7]->Val->Pointer, 64);
	memcpy(&st_update[st_offset][0], Param[8]->Val->Pointer, 64);
	
	(*ctrl_offset)++;
	st_offset++;
}


/* list of all library functions and their prototypes */
struct LibraryFunction picoc_api_functions[] =
		{
				{ picoc_api_speedtest, "void api_speedtest(void);" },
				{ trj_traj_pos, "void trj_traj_pos(s_trj_traj *traj, vlf_t time, vlf_t *pos);" },
				{ trj_traj_rot, "void trj_traj_rot(s_trj_traj *traj, vlf_t time, vlf_t *rot);" },
				{ trj_eng_add_ctrlapi, "void trj_eng_add_ctrlapi(void *env, void *ctrl_offset, void *ctrl_list, char *desc, void *config, char *init, char *free, char *reset, char *update);" },
				{ NULL, NULL }
		};

void __env_init__(s_trj_gui_env *self)
{
	PicocInitialise(&self->env, PICOC_STACK_SIZE);
	PicocIncludeAllSystemHeaders(&self->env);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
	
	self->env.CStdOut = self->out_s;
	
	char env_define[] = "#define __TRJ_ENV__";
	
	PicocParse(&self->env, "trj_env", env_define, strlen(env_define),
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
	
	VariableDefinePlatformVar(&self->env, NULL, "__traj_offset__", self->env.VoidPtrType, (union AnyValue *) &self->traj_offset, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__traj_list__"  , self->env.VoidPtrType, (union AnyValue *) &self->traj_list, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__ctrl_offset__", self->env.VoidPtrType, (union AnyValue *) &self->ctrl_offset, 0x00);
	VariableDefinePlatformVar(&self->env, NULL, "__ctrl_list__"  , self->env.VoidPtrType, (union AnyValue *) &self->ctrl_list, 0x00);
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
				uint32_t plugin_size;
				uint8_t *plugin_data;
				
				fseek (plugin_file, 0x00, SEEK_END);
				
				plugin_size = ftell (plugin_file);
				plugin_data = (uint8_t*) malloc(plugin_size+1);
				
				rewind (plugin_file);
				
				printf("%s\n", plugin_path);
				
				fread (plugin_data, plugin_size, 1, plugin_file);
				fclose(plugin_file);
				
				while (plugin_size > 0x00 && plugin_data[plugin_size-1] != '\r' && plugin_data[plugin_size-1] != '\n')
				{
					plugin_size--;
				}
				
				plugin_data[plugin_size] = '\0';
				
				uint32_t offset = strlen(plugin_path) - 1;
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

void trj_gui_env_reset(s_trj_gui_env *self)
{
	PicocCleanup(&self->env);
	__env_init__(self);
}

void trj_gui_env_init(s_trj_gui_env *self, s_trj_gui_env_init attr)
{
	self->eng = attr.eng;
	
	self->traj_offset = attr.traj_offset;
	self->ctrl_offset = attr.ctrl_offset;
	self->data_offset = attr.data_offset;
	
	self->traj_list = attr.traj_list;
	self->ctrl_list = attr.ctrl_list;
	self->data_list = attr.data_list;
	
	__env_init__(self);
}

void trj_gui_env_sreset(s_trj_gui_env *self)
{
	fclose(self->out_s);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
}

//----------------------------------------------------------------



