
#include "trj_gui_env.h"

void __env_init__(s_trj_gui_env *self)
{
	PicocInitialise(&self->env, PICOC_STACK_SIZE);
	PicocIncludeAllSystemHeaders(&self->env);
	
	self->out_s = fmemopen(self->out_s_data, sizeof(self->out_s_data), "w+");
	rewind(self->out_s);
	
	self->env.CStdOut = self->out_s;
	
	char env_define[] = "#define __TRJ_ENV__";
	
	PicocParse(&self->env, "trj_env", env_define, strlen(env_define),
			   true, true, false, true);
	
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
				
				printf("%s", plugin_path);
				
				fread (plugin_data, plugin_size, 1, plugin_file);
				fclose(plugin_file);
				
				while (plugin_size > 0x00 && plugin_data[plugin_size] != '\r' && plugin_data[plugin_size] != '\n')
				{
					plugin_size--;
				}
				
				plugin_data[plugin_size] = '\0';
				
				printf("%s", plugin_data);
				fflush(stdout);
				
				PicocParse(&self->env, plugin_path, (const char*) plugin_data, plugin_size,
						true, true, false, true);
				
				free(plugin_data);
			}
		}
		
		fclose(file_handle);
	}
	
	fflush(stdout);
	
	VariableDefinePlatformVar(&self->env, NULL, "eng", self->env.CharArrayType, (union AnyValue *) self->eng, 0x01);
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
