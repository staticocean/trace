
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

typedef struct gjson_pt
{
	vlf_t vec[3];
	
}	s_gjson_pt;

typedef struct gjson_poly
{
	gjson_geom_type_t type;
//	std::vector<s_gjson_pt> pt_list;
	
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
	uint32_t size;
	
	std::vector<uint8_t> types;
	std::vector<char*> keys_s;
	std::vector<char*> keys_e;
	std::vector<char*> values_s;
	std::vector<char*> values_e;
	std::vector<gjson_token*> tokens;

} 	s_gjson_token;

//------------------------------------------------------------------------------

inline uint32_t gjson_find(char *data, char cl)
{
	uint32_t offset = 0x01;
	char op = data[0x00];
	uint32_t st_count = 0x01; // statement count
	
	if (op != cl)
	{
		while (st_count != 0x00)
		{
			if (data[offset] == op) ++st_count;
			if (data[offset] == cl) --st_count;
			
			++offset;
		}
	}
	
	else
	{
		while (data[offset] != cl) { ++offset; }
		++offset;
	}
	
	return (offset-1);
}

//------------------------------------------------------------------------------

inline s_gjson_token* gjson_parse_tok(std::vector<s_gjson_token> *stack, char *data);

inline s_gjson_token* gjson_parse_arr(std::vector<s_gjson_token> *stack, char *data)
{
	char *token_data[2];
	
	while (*data != '[') { ++data; }
	token_data[0] = data;
	data += gjson_find(data, ']');
	token_data[1] = data;
	data = token_data[0];
	
	const uint8_t type_str = 0x01;
	const uint8_t type_arr = 0x02;
	const uint8_t type_tok = 0x03;
	
	stack->push_back((s_gjson_token) {});
	s_gjson_token *token = &stack->back();
	
	while (data < token_data[1])
	{
		while (*data == ' ' || *data == ',')
		{ ++data; }
		
		while (*data != '\"' && *data != '{' && *data != '[')
		{ ++data; }
		
		token->values_s.push_back(data);
		
		if (*data == '\"')
		{
			token->types.push_back(type_str);
			data += gjson_find(data, '\"');
		}
		
		else if (*data == '[')
		{
			token->types.push_back(type_arr);
			data += gjson_find(data, ']');
		}
		
		else if (*data == '{')
		{
			token->types.push_back(type_tok);
			data += gjson_find(data, '}');
		}
		
		token->values_e.push_back(data);
		++data;
		*data = '\0';
		
		if (token->types.back() == type_tok)
		{
			token->tokens.push_back(gjson_parse_tok(stack, token->values_s.back()));
		}
		
		else if (token->types.back() == type_arr)
		{
			token->tokens.push_back(gjson_parse_arr(stack, token->values_s.back()));
		}
		
		else
		{
			token->tokens.push_back(NULL);
		}
		
		++token->size;
	}
	
	for (int i = 0; i < token->size; ++i)
	{
		printf("[%d][type:%d][key:%s] \r\n",
			   stack->size(), token->types[i], token->keys_s[i]);
	}
	
	fflush(stdout);
	
	return token;
}

inline s_gjson_token* gjson_parse_tok(std::vector<s_gjson_token> *stack, char *data)
{
	char *token_data[2];
	
	while (*data != '{') { ++data; }
	token_data[0] = data;
	data += gjson_find(data, '}');
	token_data[1] = data;
	data = token_data[0];
	
	const uint8_t type_str = 0x01;
	const uint8_t type_arr = 0x02;
	const uint8_t type_tok = 0x03;
	
	stack->push_back((s_gjson_token) {});
	s_gjson_token *token = &stack->back();
	
	while (data < token_data[1])
	{
		while (*data != '\"' && data < token_data[1]) { ++data; }
		if (*data != '\"') break;
		token->keys_s.push_back(data);
		++data;
		
		while (*data != '\"' && data < token_data[1]) { ++data; }
		token->keys_e.push_back(data);
		++data;
		
		*data = '\0';
		
		while (*data != '\"' && *data != '{' && *data != '[')
		{ ++data; }

		token->values_s.push_back(data);

		if (*data == '\"')
		{
			token->types.push_back(type_str);
			data += gjson_find(data, '\"');
		}

		else if (*data == '[')
		{
			token->types.push_back(type_arr);
			data += gjson_find(data, ']');
		}

		else if (*data == '{')
		{
			token->types.push_back(type_tok);
			data += gjson_find(data, '}');
		}

		token->values_e.push_back(data);
		++data;
		*data = '\0';
		
		if (token->types.back() == type_tok)
		{
			token->tokens.push_back(gjson_parse_tok(stack, token->values_s.back()));
		}
		
		else if (token->types.back() == type_arr)
		{
			token->tokens.push_back(gjson_parse_arr(stack, token->values_s.back()));
		}
		
		else
		{
			token->tokens.push_back(NULL);
		}
		
		++token->size;
	}
	
	for (int i = 0; i < token->size; ++i)
	{
		printf("[%d][type:%d][key:%s] \r\n",
		   stack->size(), token->types[i], token->keys_s[i]);
	}
	
	fflush(stdout);
	
	return token;
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
	
	gjson_parse_tok(&stack, file_data);
	
	return 0x00;
}

//------------------------------------------------------------------------------

#endif /* __GJSON__ */




























