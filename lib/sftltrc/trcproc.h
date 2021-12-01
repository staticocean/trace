
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCPROC__
#define __TRCPROC__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcspl.h>

//------------------------------------------------------------------------------

typedef struct trcproc
{
	char 			desc[32];
	u32_t 			hash;
	
	u32_t 			config_size;
	void 			*config;
	
	u32_t 			data_size;
	void 			*data;
	
	u8_t (*init) 	(void **data, void *config);
	u8_t (*free) 	(void **data);
	u8_t (*save) 	(void *data, void *config, u8_t **v_file);
	u8_t (*load) 	(void *data, void *config, u8_t **v_file);
	u8_t (*update) 	(void *data, void *obj, u32_t offset);
	
} 	s_trcproc;

typedef struct trcproc
{
	char 			desc[32];
	u32_t 			hash;
	
	u32_t 			config_size;
	void 			*config;
	
	u32_t 			data_size;
	void 			*data;
	
	u8_t (*init) 	(void **data, void *config);
	u8_t (*free) 	(void **data);
	u8_t (*save) 	(void *data, void *config, u8_t **v_file);
	u8_t (*load) 	(void *data, void *config, u8_t **v_file);
	u8_t (*update) 	(void *data, void *obj, u32_t offset);
	
} 	s_trcproc;

//------------------------------------------------------------------------------

#endif /* __INS_PROC__ */


