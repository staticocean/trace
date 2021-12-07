
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trcspl - saveble pointer list

#ifndef __TRCSPL__
#define __TRCSPL__

//------------------------------------------------------------------------------

#include <stdlib.h>

#include <sftlstd/crc.h>

//------------------------------------------------------------------------------

#define TRCSPL_PTR_SZ 1024

//------------------------------------------------------------------------------

typedef struct trcspl_ptr
{
	void			*ptr;
	u64_t 			guid;

}	s_trcspl_ptr;

//------------------------------------------------------------------------------

typedef struct trcspl
{
	s32_t 			ptr_sz;
	s_trcspl_ptr 	ptr_ls[TRCSPL_PTR_SZ];
	
}	s_trcspl;

typedef struct trcspl_attr
{
	u32_t 			temp;

}	s_trcspl_attr;

//------------------------------------------------------------------------------

void trcspl_init (s_trcspl *spl, s_trcspl_attr attr)
{
	spl->ptr_sz = 0x00;
}

//------------------------------------------------------------------------------

void trcspl_add (s_trcspl *spl, void *ptr)
{
	spl->ptr_ls[spl->ptr_sz].ptr = ptr;
	spl->ptr_ls[spl->ptr_sz].guid = crc32_iso_str((char*) ptr);
	
	spl->ptr_sz++;
}

//------------------------------------------------------------------------------

void trcspl_pack (s_trcspl *spl, s_trcspl_ptr *ptr)
{
	ptr->guid = crc(ptr);
}

//------------------------------------------------------------------------------

void trcspl_unpack (s_trcspl *spl, void *ptr)
{
	api->ptr_ls[api->ptr_sz] = ptr;
	api->ptr_sz++;
}

//------------------------------------------------------------------------------

#endif /* __TRCSPL__ */


