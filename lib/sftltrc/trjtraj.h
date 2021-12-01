
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// TRCAPI - TRaCe

#ifndef __TRCTRAJ__
#define __TRCTRAJ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>

#include <sftltrc/trcobj.h>

//------------------------------------------------------------------------------

typedef struct trctraj
{
	union
	{
		void 			*intf;
		s_trcspl_ptr 	intf_splptr;
	};
	
	char 				name[32];
	
	f64_t 				time[2];
	
} 	s_trctraj;

typedef struct trctraj_intf
{
	char 				desc[32];
	
	s8_t (*init) 		(s_trctraj **traj_data);
	s8_t (*free) 		(s_trctraj **traj_data);
	s8_t (*pack) 		(s_trctraj  *traj_data, s_trcspl *spl);
	s8_t (*unpack) 		(s_trctraj  *traj_data, s_trcspl *spl);
	s8_t (*save) 		(s_trctraj  *traj_data, u8_t **v_file);
	s8_t (*load) 		(s_trctraj  *traj_data, u8_t **v_file);
	s8_t (*compile) 	(s_trctraj  *traj_data);
	s8_t (*pos) 		(s_trctraj  *traj_data, f64_t time, f64_t *pos);
	s8_t (*rot) 		(s_trctraj  *traj_data, f64_t time, f64_t *rot);
	
}	s_trctraj_intf;

//------------------------------------------------------------------------------

inline s8_t trctraj_init (s_trctraj **traj)
{
	return ((s_trctraj_intf*) (*traj)->intf)->init(traj);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_free (s_trctraj **traj)
{
	return ((s_trctraj_intf*) (*traj)->intf)->free(traj);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_save (s_trctraj *traj, s_trcspl *spl, u8_t **v_file)
{
	return ((s_trctraj_intf*) traj->intf)->save(traj, s_trcspl *spl, v_file);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_load (s_trctraj *traj, s_trcspl *spl, u8_t **v_file)
{
	return ((s_trctraj_intf*) traj->intf)->load(traj, s_trcspl *spl, v_file);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_compile (s_trctraj *traj)
{
	return ((s_trctraj_intf*) traj->intf)->compile(traj);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_pos (s_trctraj *traj, f64_t time, f64_t *pos);
{
	return ((s_trctraj_intf*) traj->intf)->pos(traj, time, pos);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_rot (s_trctraj *traj, f64_t time, f64_t *rot);
{
	return ((s_trctraj_intf*) traj->intf)->rot(traj, time, rot);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_pack (s_trctraj *traj, s_trcspl *spl)
{
	return ((s_trctraj_intf*) traj->intf)->pack(traj, spl);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_unpack (s_trctraj *traj, s_trcspl *spl)
{
	return ((s_trctraj_intf*) traj->intf)->unpack(traj, spl);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_save (s_trctraj *traj, u8_t **v_file)
{
	return ((s_trctraj_intf*) traj->intf)->save(traj, v_file);
}

//------------------------------------------------------------------------------

inline s8_t trctraj_load (s_trctraj *traj, u8_t **v_file)
{
	((s_trctraj_intf*) traj->intf)->load(traj, v_file);
	
	// compile trajectory for previews and interaction
	// to prevent damaged values when executing pos/rot calls
	
	trctraj_compile(traj);
	
	return 0x00;
}

//------------------------------------------------------------------------------

//void trctraj_copy (s_trceng *eng, s_trctraj *dest, s_trctraj *src)
//{
//	u8_t *data_ref = (u8_t*) malloc(1024 * 1024 * 256); // 256 Mb Max
//	u8_t *data_ptr = data_ref;
//
//	trctraj_save (src , eng, &data_ptr);
//
//	// Reset pointer
//	data_ptr = data_ref;
//
//	*dest = *src;
//	trctraj_load (dest, eng, &data_ptr);
//
//	free(data_ref);
//}

//------------------------------------------------------------------------------

#endif /* __TRCTRAJ__ */
