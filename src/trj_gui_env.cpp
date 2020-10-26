
#include "trj_gui_env.h"

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


/* list of all library functions and their prototypes */
struct LibraryFunction picoc_api_functions[] =
		{
				{picoc_api_speedtest, "void api_speedtest(void);"},
				{trj_traj_pos, "void trj_traj_pos(s_trj_traj *traj, vlf_t time, vlf_t *pos);"},
				{trj_traj_rot, "void trj_traj_rot(s_trj_traj *traj, vlf_t time, vlf_t *rot);"},
				{NULL, NULL}
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
	
	
	VariableDefinePlatformVar(&self->env, NULL, "eng_data", self->env.CharArrayType, (union AnyValue *) self->eng, 0x01);
	
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
				
				while (plugin_size > 0x00 && plugin_data[plugin_size] != '\r' && plugin_data[plugin_size] != '\n')
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
	
	IncludeRegister(&self->env, "api.h", &picoc_api_init, &picoc_api_functions[0], NULL);
	
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
	__env_init__(self);
}

void trj_gui_env_sreset(s_trj_gui_env *self)
{
	fclose(self->out_s);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
}
