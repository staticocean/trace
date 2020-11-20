
//
//  Created by Egor Chekhov O'Leo on 24/12/2015.
//  Copyright © 2015 Control Systems Interfaces. All rights reserved.
//

#ifndef __GJSON__
#define __GJSON__

//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <vector>

#include "vl.h"

//------------------------------------------------------------------------------

typedef enum gjson_geom_type_t
{
	gjson_geom_type_poly = 0x00,
};

typedef struct gjson_poly
{
	gjson_geom_type_t type;
	std::vector<std::array<vlf_t, 3>> pt_list;
	
} s_gjson_poly;

typedef union gjson_geom
{
	gjson_geom_type_t type;
	
	s_gjson_poly poly;
	
} 	s_gjson_geom;

typedef struct gjson_obj
{
	char name[256];
	
	vlf_t p0[3];
	vlf_t p1[3];
	vlf_t mid[3];
	
	std::vector<s_gjson_geom> geom_list;
	
}	s_gjson_obj;

typedef struct gjson_layer
{
	std::vector<s_gjson_obj> obj_list;
	s_gjson_obj *lookup[180][360];
	
}	s_gjson_layer;

typedef struct gjson_token
{
	uint8_t type;
	
	char *key[2];
	char *value[2];
	char *sep;

	struct gjson_token *token;

} 	s_gjson_token;

//------------------------------------------------------------------------------

inline uint32_t gjson_find(char *data, char cl)
{
	uint32_t offset = 0x01;
	char op = data[0x00];
	
	uint32_t op_count = 0x01;
	uint32_t cl_count = 0x00;
	
	while (op_count != 0x00 || cl_count != 0x00)
	{
		if (data[offset] == op) ++op_count;
		if (data[offset] == cl) ++cl_count;
		
		++offset;
	}
	
	return (offset-1);
}

//------------------------------------------------------------------------------

inline void gjson_parse(std::vector<s_gjson_token> *stack, char *data)
{
	uint32_t offset = 0x00;
	
	char *token_data[2];
	
	while (data[offset] != '{') { ++offset; }
	token_data[0] = &data[offset];
	offset += gjson_find(&data[offset], '}');
	token_data[1] = &data[offset];
	++offset;
	
	stack->push_back((s_gjson_token) {});
	s_gjson_token *token = &stack->back();
	
	const uint8_t type_str = 0x01;
	const uint8_t type_arr = 0x02;
	const uint8_t type_tok = 0x03;
	
	uint8_t bof  = 0x00;
	uint8_t eof  = 0x00;
	uint8_t type = 0x00;
	
	while (data[offset] != '\"') { ++offset; }
	token->key[0] = &data[offset];
	offset += gjson_find(&data[offset], '\"');
	token->key[1] = &data[offset];
	++offset;
	
	while (data[offset] !=  ':') { ++offset; }
	token->sep = &data[offset]; ++offset;
	
	while (data[offset] != '\"' && data[offset] != '{' && data[offset] != '[') { ++offset; }
	
	token->type = type_str;
	token->value[0] = &data[offset];
	if (token->type == type_str) offset += gjson_find(&data[offset], '\"');
	if (token->type == type_arr) offset += gjson_find(&data[offset],  ']');
	if (token->type == type_tok) offset += gjson_find(&data[offset],  '}');
	token->value[1] = &data[offset];
	
	if (token->type == type_tok)
	{
		gjson_parse(stack, token->value[0]);
	}
	
	return;
}

//------------------------------------------------------------------------------

inline uint8_t gjson_layer_load(s_gjson_layer *self, char *file_name)
{
	FILE *file_handle = fopen(file_name, "r");
	
	fseek(file_handle, 0, SEEK_END);
	uint32_t file_size = ftell(file_handle);
	fseek(file_handle, 0, SEEK_SET);
	
	char *file_data = (char*) malloc(file_size);
	fread(file_data, 1, file_size, file_handle);
	
	fclose(file_handle);
	
	std::vector<s_gjson_token> stack;
	
	gjson_parse(&stack, file_data);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GJSON__ */




























