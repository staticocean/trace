
// 2021 Softael LLC.
// Created by staticocean on 11/11/2021

// trctraj_bz - trace

#ifndef __TRCTRAJ_BZ__
#define __TRCTRAJ_BZ__

//------------------------------------------------------------------------------

#include <sftlstd/types.h>
#include <sftlstd/env.h>
#include <sftlstd/vld3.h>
#include <sftlstd/vldx.h>
#include <sftlstd/bz.h>

#include <sftltrc/trcspl.h>
#include <sftltrc/trctraj.h>
#include <sftltrc/trcrefs.h>
#include <sftltrc/impl/trcrefs_ellp.h>

//------------------------------------------------------------------------------

typedef struct trctraj_bz_point
{
	t_f64               time;
	
	t_f64               pos_p[3];
	t_f64               pos_t[3];
	t_f64               pos_d[3];
	
	t_f64               rot_p[3];
	t_f64               rot_t[3];
	t_f64               rot_d[3];
	
//	t_f64 offset[3];
	
	/*
	 pos_d MUST BE
	 to the right of pos_p (pos_d[0] > pos_p[0])
	 cannot overlap next point
	 */
	
} 	s_trctraj_bz_point;

typedef struct trctraj_bz
{
    s_trctraj           super;

	s_trcobj 	       *ref;

	s_trctraj_bz_point *pts_ls;
	t_s32 	            pts_sz;
	t_s32 	            pts_fl;
	
	t_s8 	            ellp_en;
	s_trcrefs          *ellp;

//	t_f64 		p_bias[3];
//	t_f64 		p_gain[3];
//
//	t_f64 		t_bias;
//	t_f64 		t_gain;
	
} 	s_trctraj_bz;

typedef struct trctraj_bz_attr
{
    s_trctraj_attr      super;

	s_trcobj           *ref;
	
	t_s8                ellp_en;
	s_trcrefs          *ellp;
	
//	t_s32               pts_sz;
//	s_trctraj_bz_point *pts_ls;
//
} 	s_trctraj_bz_attr;

//------------------------------------------------------------------------------

static
t_s8 __trctraj_bz_init__ (void *__traj__, void *__attr__)
{
    s_trctraj_bz 	  *traj = (s_trctraj_bz*     ) __traj__;
    s_trctraj_bz_attr *attr = (s_trctraj_bz_attr*) __attr__;

	traj->ref = attr->ref;

//    traj->pts_ls = attr->pts_ls;
//    traj->pts_sz = attr->pts_sz;
	traj->pts_fl = 0x00;
    traj->pts_ls = (s_trctraj_bz_point*) malloc(sizeof(s_trctraj_bz_point) * 100);
    traj->pts_sz = 100;

    traj->ellp_en = attr->ellp_en;
    traj->ellp = attr->ellp;

//	vl_vset(traj->p_bias, 0.0);
//	vl_vset(traj->p_gain, 1.0);
//
//	traj->t_bias = 0.0;
//	traj->t_gain = 1.0;

	return 0x00;
}

static
t_s8 __trctraj_bz_free__ (void *__traj__)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

    free(traj->pts_ls);

    return 0x00;
}

static
t_s8 __trctraj_bz_save__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

//    memcpy(*v_file, traj->pts_ls, traj->pts_sz * sizeof(s_trctraj_bz_point));
//	*v_file += traj->pts_sz * sizeof(s_trctraj_bz_point);
//
	return 0x00;
}

static
t_s8 __trctraj_bz_load__ (void *__traj__, s_trcspl *spl, t_u8 **v_file)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

//	traj->pts_ls = (s_trctraj_bz_point*) malloc(traj->pts_size * sizeof(s_trctraj_bz_point));
//	memcpy(traj->pts, *v_file, traj->pts_size * sizeof(s_trctraj_bz_point));
//
//	*v_file += traj->pts_size * sizeof(s_trctraj_bz_point);
//
//	traj->eng = attr->eng;
//
//	traj->ref  = trceng_find_obj (traj->eng, traj->ref_hash);
//	traj->ellp = trceng_find_ellp(traj->eng, traj->ellp_hash);
//
//	if (traj->ref  == NULL)
//	{
//		traj->ref_hash = 0x00;
//	}
//
//	if (traj->ellp == NULL)
//	{
//		traj->ellp_hash = 0x00;
//		traj->ellp_en = 0x00;
//	}
	
	return 0x00;
}

static
int __trctraj_bz_point_comp__ (const void *p0_, const void *p1_)
{
	s_trctraj_bz_point *p0 = (s_trctraj_bz_point*) p0_;
	s_trctraj_bz_point *p1 = (s_trctraj_bz_point*) p1_;
	
	if (p0->time > p1->time) return +1;
	if (p0->time < p1->time) return -1;
	
	return 0;
}

static inline
t_s8 __trctraj_bz_add__ (s_trctraj_bz *traj, s_trctraj_bz_point point)
{
	traj->pts_ls[traj->pts_fl] = point;
    traj->pts_fl++;
	
	qsort (traj->pts_ls, traj->pts_fl,
           sizeof(s_trctraj_bz_point), __trctraj_bz_point_comp__);
	
	return 0x00;
}

static inline
t_s8 __trctraj_bz_rem__ (s_trctraj_bz *traj, t_s32 index)
{
	for (t_s32 i = index; i < traj->pts_fl-1; ++i)
	{
		traj->pts_ls[i] = traj->pts_ls[i+1];
	}
	
	--traj->pts_fl;
	
	return 0x00;
}

static inline
t_s8 __trctraj_bz_vel__ (s_trctraj_bz *traj, t_f64 time, t_f64 *vel)
{
	t_s32 offset = 0x00;
	
	if (traj->pts_fl == 0x00) { return 0x01; }
	
	if (time < traj->pts_ls[0x00].time + 1E-9)
	{
        vld3v_zero(vel);
	}

    else if (time > traj->pts_ls[traj->pts_fl-1].time - 1E-9)
    {
        vld3v_zero(vel);
    }

	else
    {
        if (time > traj->pts_ls[traj->pts_fl-1].time) time = traj->pts_ls[traj->pts_fl-1].time;

        while ((time < traj->pts_ls[offset].time)
               || (time > traj->pts_ls[offset+1].time))
        {
            ++offset;
        }

        if (fabs(time-traj->pts_ls[offset].time) < 1E-9)
        {
            t_f64 acc[3];

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[0]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[0],
                            traj->pts_ls[offset].pos_p[0] + traj->pts_ls[offset].pos_d[0]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[0],
                            traj->pts_ls[offset + 1].pos_p[0] - traj->pts_ls[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[0]
                    }
            });

            bz4_d2t(&bz4, time, &acc[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[1]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[1],
                            traj->pts_ls[offset].pos_p[1] + traj->pts_ls[offset].pos_d[1]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[1],
                            traj->pts_ls[offset + 1].pos_p[1] - traj->pts_ls[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[1]
                    }
            });

            bz4_d2t(&bz4, time, &acc[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[2]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[2],
                            traj->pts_ls[offset].pos_p[2] + traj->pts_ls[offset].pos_d[2]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[2],
                            traj->pts_ls[offset + 1].pos_p[2] - traj->pts_ls[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[2]
                    }
            });

            bz4_d2t(&bz4, time, &acc[2]);

            vld3v_divv(vel, traj->pts_ls[offset].pos_d, traj->pts_ls[offset].pos_t);
            vld3v_msaddv(vel, acc, time - traj->pts_ls[offset].time, vel);
        }

        else if (fabs(time-traj->pts_ls[offset+1].time) < 1E-9)
        {
            t_f64 acc[3];

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[0]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[0],
                            traj->pts_ls[offset].pos_p[0] + traj->pts_ls[offset].pos_d[0]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[0],
                            traj->pts_ls[offset + 1].pos_p[0] - traj->pts_ls[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[0]
                    }
            });

            bz4_d2t(&bz4, time, &acc[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[1]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[1],
                            traj->pts_ls[offset].pos_p[1] + traj->pts_ls[offset].pos_d[1]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[1],
                            traj->pts_ls[offset + 1].pos_p[1] - traj->pts_ls[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[1]
                    }
            });

            bz4_d2t(&bz4, time, &acc[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[2]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[2],
                            traj->pts_ls[offset].pos_p[2] + traj->pts_ls[offset].pos_d[2]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[2],
                            traj->pts_ls[offset + 1].pos_p[2] - traj->pts_ls[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[2]
                    }
            });

            bz4_d2t(&bz4, time, &acc[2]);

            vld3v_divv(vel, traj->pts_ls[offset+1].pos_d, traj->pts_ls[offset+1].pos_t);
            vld3v_msaddv(vel, acc, time - traj->pts_ls[offset+1].time, vel);
        }

        else
        {
            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[0]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[0],
                            traj->pts_ls[offset].pos_p[0] + traj->pts_ls[offset].pos_d[0]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[0],
                            traj->pts_ls[offset + 1].pos_p[0] - traj->pts_ls[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[0]
                    }
            });

            bz4_d1t(&bz4, time, &vel[0]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[1]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[1],
                            traj->pts_ls[offset].pos_p[1] + traj->pts_ls[offset].pos_d[1]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[1],
                            traj->pts_ls[offset + 1].pos_p[1] - traj->pts_ls[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[1]
                    }
            });

            bz4_d1t(&bz4, time, &vel[1]);

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[2]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[2],
                            traj->pts_ls[offset].pos_p[2] + traj->pts_ls[offset].pos_d[2]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[2],
                            traj->pts_ls[offset + 1].pos_p[2] - traj->pts_ls[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[2]
                    }
            });

            bz4_d1t(&bz4, time, &vel[2]);
        }
    }

	if (traj->ellp_en != 0x00 && traj->ellp != NULL)
	{
	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

static inline
t_s8 __trctraj_bz_pos_raw__ (s_trctraj_bz *traj, t_f64 time, t_f64 *pos)
{
	t_s32 offset = 0x00;
	
	if (traj->pts_fl == 0x00) { return 0x01; }

    if (time < traj->pts_ls[0x00].time + 1E-9)
    {
        vld3v_copy(pos, traj->pts_ls[0x00].pos_p);
    }

    else if (time > traj->pts_ls[traj->pts_fl-1].time - 1E-9)
    {
        vld3v_copy(pos, traj->pts_ls[traj->pts_fl-1].pos_p);
    }

    else
    {
        if (time > traj->pts_ls[traj->pts_fl-1].time) { time = traj->pts_ls[traj->pts_fl-1].time; }

        while ((time < traj->pts_ls[offset].time)
               || (time > traj->pts_ls[offset+1].time))
        {
            ++offset;
        }

    //
    //	else if (fabs(time - traj->pts_ls[offset].time) < 1E-9)
    //	{ vl_vcopy(pos, traj->pts_ls[offset].pos_p); }
    //
    //	else if (fabs(time - traj->pts_ls[offset+1].time) < 1E-9)
    //	{ vl_vcopy(pos, traj->pts_ls[offset+1].pos_p); }
    //
        if (fabs(time-traj->pts_ls[offset].time) < 1E-9)
        {
            t_f64 vel[3];
            vld3v_copy(pos, traj->pts_ls[offset].pos_p);
            vld3v_divv(vel, traj->pts_ls[offset].pos_d, traj->pts_ls[offset].pos_t);
            vld3v_msaddv(pos, vel, time - traj->pts_ls[offset].time, pos);
        }

        else if (fabs(time-traj->pts_ls[offset+1].time) < 1E-9)
        {
            t_f64 vel[3];
            vld3v_copy(pos, traj->pts_ls[offset+1].pos_p);
            vld3v_divv(vel, traj->pts_ls[offset+1].pos_d, traj->pts_ls[offset+1].pos_t);
            vld3v_msaddv(pos, vel, time - traj->pts_ls[offset+1].time, pos);
        }

        else
        {
    //		t_f64 t = (time - traj->pts_ls[offset].time) / (traj->pts_ls[offset+1].time - traj->pts_ls[offset].time);
    //		t_f64 p[2];

    //		vl_vsub(traj->pts_ls[offset].pos_p, traj->pts_ls[offset].pos_p, traj->p_bias);
    //		vl_vsub(traj->pts_ls[offset+1].pos_p, traj->pts_ls[offset+1].pos_p, traj->p_bias);
    //		traj->pts_ls[offset].time -= traj->t_bias;
    //		traj->pts_ls[offset+1].time -= traj->t_bias;
    //
    //		vl_vdiv_v(traj->pts_ls[offset].pos_p, traj->pts_ls[offset].pos_p, traj->p_gain);
    //		vl_vdiv_v(traj->pts_ls[offset+1].pos_p, traj->pts_ls[offset+1].pos_p, traj->p_gain);
    //		traj->pts_ls[offset].time /= traj->t_gain;
    //		traj->pts_ls[offset+1].time /= traj->t_gain;
    //		time = (time - traj->t_bias) / traj->t_gain;

            s_bz4 bz4;

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[0]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[0],
                            traj->pts_ls[offset].pos_p[0] + traj->pts_ls[offset].pos_d[0]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[0],
                            traj->pts_ls[offset + 1].pos_p[0] - traj->pts_ls[offset + 1].pos_d[0]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[0]
                    }
            });

            bz4_d0t(&bz4, time, &pos[0]);
    //		bz4_d0(&bz4, t, p);
    //		pos[0] = p[1];

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[1]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[1],
                            traj->pts_ls[offset].pos_p[1] + traj->pts_ls[offset].pos_d[1]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[1],
                            traj->pts_ls[offset + 1].pos_p[1] - traj->pts_ls[offset + 1].pos_d[1]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[1]
                    }
            });

            bz4_d0t(&bz4, time, &pos[1]);
    //		bz4_d0(&bz4, t, p);
    //		pos[1] = p[1];

            bz4_init(&bz4, (s_bz4_init_attr) {
                    .p0 = {
                            traj->pts_ls[offset].time,
                            traj->pts_ls[offset].pos_p[2]
                    },

                    .p1 = {
                            traj->pts_ls[offset].time + traj->pts_ls[offset].pos_t[2],
                            traj->pts_ls[offset].pos_p[2] + traj->pts_ls[offset].pos_d[2]
                    },

                    .p2 = {
                            traj->pts_ls[offset + 1].time - traj->pts_ls[offset + 1].pos_t[2],
                            traj->pts_ls[offset + 1].pos_p[2] - traj->pts_ls[offset + 1].pos_d[2]
                    },

                    .p3 = {
                            traj->pts_ls[offset + 1].time,
                            traj->pts_ls[offset + 1].pos_p[2]
                    }
            });

            bz4_d0t(&bz4, time, &pos[2]);
    //		bz4_d0(&bz4, t, p);
    //		pos[2] = p[1];

    //		vl_vmul_v(traj->pts_ls[offset].pos_p, traj->pts_ls[offset].pos_p, traj->p_gain);
    //		vl_vmul_v(traj->pts_ls[offset+1].pos_p, traj->pts_ls[offset+1].pos_p, traj->p_gain);
    //		vl_vmul_v(pos, pos, traj->p_gain);
    //		traj->pts_ls[offset].time *= traj->t_gain;
    //		traj->pts_ls[offset+1].time *= traj->t_gain;
    //
    //		vl_vsum(traj->pts_ls[offset].pos_p, traj->pts_ls[offset].pos_p, traj->p_bias);
    //		vl_vsum(traj->pts_ls[offset+1].pos_p, traj->pts_ls[offset+1].pos_p, traj->p_bias);
    //		vl_vsum(pos, pos, traj->p_bias);
    //		traj->pts_ls[offset].time += traj->t_bias;
    //		traj->pts_ls[offset+1].time += traj->t_bias;
    //		time = time * traj->t_gain + traj->t_bias;

    //		vl_vsum(pos, pos, traj->pts_ls[offset].offset);
        }
    }

	return 0x00;
}

static inline
t_s8 __trctraj_bz_pos_local__ (s_trctraj_bz *traj, t_f64 time, t_f64 *pos)
{
	__trctraj_bz_pos_raw__(traj, time, pos);
	
	if (traj->ellp_en != 0x00 && traj->ellp != NULL)
	{
        trcrefs_pos_abs(traj->ellp, pos, pos);
	}
	
	return 0x00;
}

static
t_s8 __trctraj_bz_pos__ (void *__traj__, t_f64 time, t_f64 *pos)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

	__trctraj_bz_pos_local__(traj, time, pos);
	
	t_f64 pos_[3];

	vld3m_mulv(pos_, &traj->ref->rot[0][0], pos);
	vld3v_addv(pos, pos_, &traj->ref->pos[0][0]);
	
	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_bz_rot__ (void *__traj__, t_f64 time, t_f64 *rot)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

	if (traj->ellp_en == 0x00)
	{
	
	}
	
	else
	{
        t_f64 vel_lla[3];
        t_f64 pos_ecef[3];

        __trctraj_bz_pos_local__(traj, time, pos_ecef);
        __trctraj_bz_vel__(traj, time, vel_lla);

//        t_f64 nwh[3];
//        trcrefs_pos_ref(traj->ellp, pos_lla, pos_ecef);
//        trcrefs_ellp_nwhvel(traj->ellp, pos_lla, vel_lla, nwh);

		t_f64 rot_nwh_tnp[9] = {
//                nwh[0], nwh[2], nwh[1],
                vel_lla[0], vel_lla[2], vel_lla[1],
			0.0, 1.0, 0.0,
			0.0, 0.0, 0.0,
		};
		
		t_f64 *x = &rot_nwh_tnp[0];
		t_f64 *y = &rot_nwh_tnp[3];
		t_f64 *z = &rot_nwh_tnp[6];
		
//		vl_mprint(rot_nwh_tnp);

		vld3v_muls(x, x, 1.0 / vld3v_norm(x));
		
//		vl_mprint(rot_nwh_tnp);

		t_f64 xy[3];
		vld3v_muls(xy, x, vld3v_dot(x,y));
		vld3v_subv(y, y, xy);
		vld3v_muls(y, y, 1.0 / vld3v_norm(y));
		
		vld3v_cross(z, x, y);
		vld3v_muls(z, z, 1.0 / vld3v_norm(z));

//		vl_rnorm(rot_nwh_tnp);

		t_f64 rot_nwh[9];
		vld3m_tnp(rot_nwh, rot_nwh_tnp);

		s_vl3hpr rot_nwh_hpr = vl3hpr_mat(rot_nwh);
		rot_nwh_hpr.roll = 0.0;
		vld3m_rot(rot_nwh, rot_nwh_hpr);

//		vl_mprint(rot_nwh);
//		fflush(stdout);

		t_f64 ecef_ctn[9];
//		trcellp_ecefrot(traj->ellp, pos_ecef, ecef_ctn);
		
		t_f64 r_ref[9];
		t_f64 *r_inr = rot;
		
		vld3m_mulm(r_ref, ecef_ctn, rot_nwh);
		vld3m_mulm(r_inr, &traj->ref->rot[0][0], r_ref);
	}

	vld3m_rnorm(rot);

	return 0x00;
}

//------------------------------------------------------------------------------

static
t_s8 __trctraj_bz_compile__ (void *__traj__)
{
    s_trctraj_bz *traj = (s_trctraj_bz*) __traj__;

	if (traj->pts_fl < 0x02) { return 0x00; }
	
	qsort(traj->pts_ls, traj->pts_fl,
           sizeof(s_trctraj_bz_point), __trctraj_bz_point_comp__);

	// Normalize points to prevent numerical instabilities
	
//	vl_vzero(traj->p_bias);
//	traj->t_bias = 0.0;
//
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{
//		vl_vsumm(traj->p_bias, traj->p_bias, traj->pts_ls[i].pos_p, 1.0 / (t_f64) traj->pts_fl);
//		traj->t_bias += traj->pts_ls[i].time / (t_f64) traj->pts_fl;
//	}
//
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{
//		vl_vsub(traj->pts_ls[i].pos_p, traj->pts_ls[i].pos_p, traj->p_bias);
//		traj->pts_ls[i].time -= traj->t_bias;
//	}
//
//	vl_vset(traj->p_gain, 1.0);
//	traj->t_gain = 1.0;
//
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{
//		if (fabs(traj->pts_ls[i].time) > traj->t_gain) traj->t_gain = fabs(traj->pts_ls[i].time);
//		if (fabs(traj->pts_ls[i].pos_p[0]) > traj->p_gain[0]) traj->p_gain[0] = fabs(traj->pts_ls[i].pos_p[0]);
//		if (fabs(traj->pts_ls[i].pos_p[1]) > traj->p_gain[1]) traj->p_gain[1] = fabs(traj->pts_ls[i].pos_p[1]);
//		if (fabs(traj->pts_ls[i].pos_p[2]) > traj->p_gain[2]) traj->p_gain[2] = fabs(traj->pts_ls[i].pos_p[2]);
//	}
//
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{
//		vl_vdiv_v(traj->pts_ls[i].pos_p, traj->pts_ls[i].pos_p, traj->p_gain);
//		traj->pts_ls[i].time /= traj->t_gain;
//	}
	
	// End of normalization
	
	t_f64 m_a[traj->pts_fl*2][traj->pts_fl*2];
//	t_f64 *m_a = (t_f64*) malloc(sizeof(t_f64) * 2*traj->pts_fl * 2*traj->pts_fl);
	
	for (t_s32 i = 1; i < traj->pts_fl-1; ++i)
	{
		for (t_s32 j = 0; j < 2*traj->pts_fl; ++j)
		{
			m_a[2*i+0][j] = 0.0;
			m_a[2*i+1][j] = 0.0;
		}
		
		m_a[2*i+0][2*i-2] = 1;
		m_a[2*i+0][2*i+2] = 1;
		m_a[2*i+0][2*i+0] = 4;
		m_a[2*i+1][2*i-2+1] = 1;
		m_a[2*i+1][2*i+2+1] = 1;
		m_a[2*i+1][2*i+0+1] = 4;
	}
	
	for (t_s32 j = 0; j < 2*traj->pts_fl; ++j)
	{
		m_a[0][j] = 0.0;
		m_a[1][j] = 0.0;
		
		m_a[2*traj->pts_fl-2][j] = 0.0;
		m_a[2*traj->pts_fl-1][j] = 0.0;
	}
	
	m_a[0][0] = 2;
	m_a[0][2] = 1;
	m_a[1][1] = 2;
	m_a[1][3] = 1;
	
	m_a[2*traj->pts_fl-2][2*traj->pts_fl-4] = 1;
	m_a[2*traj->pts_fl-2][2*traj->pts_fl-2] = 2;
	m_a[2*traj->pts_fl-1][2*traj->pts_fl-3] = 1;
	m_a[2*traj->pts_fl-1][2*traj->pts_fl-1] = 2;

//	t_f64 pos_p0[2*traj->pts_fl];
//	t_f64 pos_d0[2*traj->pts_fl];
	t_f64 *pos_p0 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *pos_p1 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *pos_p2 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	
	t_f64 *pos_d0 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *pos_d1 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *pos_d2 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	
	t_f64 *rot_p0 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *rot_p1 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *rot_p2 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	
	t_f64 *rot_d0 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *rot_d1 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	t_f64 *rot_d2 = (t_f64*) malloc(sizeof(t_f64) * 2 * traj->pts_fl);
	
	for (t_s32 i = 1; i < traj->pts_fl-1; ++i)
	{
		pos_p0[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		pos_p0[2 * i + 1] = 3 * (traj->pts_ls[i + 1].pos_p[0] - traj->pts_ls[i - 1].pos_p[0]);
		
		pos_p1[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		pos_p1[2 * i + 1] = 3 * (traj->pts_ls[i + 1].pos_p[1] - traj->pts_ls[i - 1].pos_p[1]);
		
		pos_p2[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		pos_p2[2 * i + 1] = 3 * (traj->pts_ls[i + 1].pos_p[2] - traj->pts_ls[i - 1].pos_p[2]);
		
		rot_p0[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		rot_p0[2 * i + 1] = 3 * (traj->pts_ls[i + 1].rot_p[0] - traj->pts_ls[i - 1].rot_p[0]);
		
		rot_p1[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		rot_p1[2 * i + 1] = 3 * (traj->pts_ls[i + 1].rot_p[1] - traj->pts_ls[i - 1].rot_p[1]);
		
		rot_p2[2 * i + 0] = 3 * (traj->pts_ls[i + 1].time - traj->pts_ls[i - 1].time);
		rot_p2[2 * i + 1] = 3 * (traj->pts_ls[i + 1].rot_p[2] - traj->pts_ls[i - 1].rot_p[2]);
	}
	
	pos_p0[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	pos_p0[1] = 3 * (traj->pts_ls[1].pos_p[0] - traj->pts_ls[0].pos_p[0]);
	
	pos_p1[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	pos_p1[1] = 3 * (traj->pts_ls[1].pos_p[1] - traj->pts_ls[0].pos_p[1]);
	
	pos_p2[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	pos_p2[1] = 3 * (traj->pts_ls[1].pos_p[2] - traj->pts_ls[0].pos_p[2]);
	
	rot_p0[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	rot_p0[1] = 3 * (traj->pts_ls[1].rot_p[0] - traj->pts_ls[0].rot_p[0]);
	
	rot_p1[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	rot_p1[1] = 3 * (traj->pts_ls[1].rot_p[1] - traj->pts_ls[0].rot_p[1]);
	
	rot_p2[0] = 3 * (traj->pts_ls[1].time - traj->pts_ls[0].time);
	rot_p2[1] = 3 * (traj->pts_ls[1].rot_p[2] - traj->pts_ls[0].rot_p[2]);
	
	pos_p0[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	pos_p0[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].pos_p[0]
												- traj->pts_ls[traj->pts_fl-2].pos_p[0]);
	
	pos_p1[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	pos_p1[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].pos_p[1]
												- traj->pts_ls[traj->pts_fl-2].pos_p[1]);
	
	pos_p2[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	pos_p2[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].pos_p[2]
												- traj->pts_ls[traj->pts_fl-2].pos_p[2]);
	
	rot_p0[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	rot_p0[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].rot_p[0]
												- traj->pts_ls[traj->pts_fl-2].rot_p[0]);
	
	rot_p1[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	rot_p1[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].rot_p[1]
												- traj->pts_ls[traj->pts_fl-2].rot_p[1]);
	
	rot_p2[2 * traj->pts_fl - 1 - 1] = 3 * (traj->pts_ls[traj->pts_fl - 1].time
												- traj->pts_ls[traj->pts_fl-2].time);
	
	rot_p2[2 * traj->pts_fl - 1 - 0] = 3 * (traj->pts_ls[traj->pts_fl - 1].rot_p[2]
												- traj->pts_ls[traj->pts_fl-2].rot_p[2]);
	
	t_f64 m_inva[traj->pts_fl*2][traj->pts_fl*2];
	
	vldxm_inv(&m_inva[0][0], traj->pts_fl*2, &m_a[0][0]);
	
	for (t_s32 i = 0; i < traj->pts_fl*2; ++i)
	{
		pos_d0[i] = 0.0;
		pos_d1[i] = 0.0;
		pos_d2[i] = 0.0;
		
		rot_d0[i] = 0.0;
		rot_d1[i] = 0.0;
		rot_d2[i] = 0.0;
		
		for (t_s32 j = 0; j < traj->pts_fl*2; ++j)
		{
			pos_d0[i] += m_inva[i][j] * pos_p0[j];
			pos_d1[i] += m_inva[i][j] * pos_p1[j];
			pos_d2[i] += m_inva[i][j] * pos_p2[j];
			
			rot_d0[i] += m_inva[i][j] * rot_p0[j];
			rot_d1[i] += m_inva[i][j] * rot_p1[j];
			rot_d2[i] += m_inva[i][j] * rot_p2[j];
		}
	}
	
	for (t_s32 i = 0; i < traj->pts_fl; ++i)
	{
		traj->pts_ls[i].pos_t[0] = pos_d0[2 * i + 0] / 3;
		traj->pts_ls[i].pos_d[0] = pos_d0[2 * i + 1] / 3;
		
		traj->pts_ls[i].pos_t[1] = pos_d1[2 * i + 0] / 3;
		traj->pts_ls[i].pos_d[1] = pos_d1[2 * i + 1] / 3;
		
		traj->pts_ls[i].pos_t[2] = pos_d2[2 * i + 0] / 3;
		traj->pts_ls[i].pos_d[2] = pos_d2[2 * i + 1] / 3;
		
		traj->pts_ls[i].rot_t[0] = rot_d0[2 * i + 0] / 3;
		traj->pts_ls[i].rot_d[0] = rot_d0[2 * i + 1] / 3;
		
		traj->pts_ls[i].rot_t[1] = rot_d1[2 * i + 0] / 3;
		traj->pts_ls[i].rot_d[1] = rot_d1[2 * i + 1] / 3;
		
		traj->pts_ls[i].rot_t[2] = rot_d2[2 * i + 0] / 3;
		traj->pts_ls[i].rot_d[2] = rot_d2[2 * i + 1] / 3;
	}
	
	free(pos_p0);
	free(pos_d0);
	
	free(pos_p1);
	free(pos_d1);
	
	free(pos_p2);
	free(pos_d2);
	
	free(rot_p0);
	free(rot_d0);
	
	free(rot_p1);
	free(rot_d1);
	
	free(rot_p2);
	free(rot_d2);
//	free(m_a);
	
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{ vl_vzero(traj->pts_ls[i].offset); }
	
//	for (i = 1; i < traj->pts_fl-1; ++i)
//	{
//		t_f64 l[3];
//		t_f64 r[3];
//
//		t_f64 temp[3];
//		vl_vcopy(temp, traj->pts_ls[i-1].offset);
//		vl_vzero(traj->pts_ls[i-1].offset);
//
//		trctraj_bz_pos_local(self, traj->pts_ls[i].time - 1E-9, l);
//		trctraj_bz_pos_local(self, traj->pts_ls[i].time + 1E-9, r);
//
//		vl_vcopy(traj->pts_ls[i-1].offset, temp);
//
//		vl_vsub(l, traj->pts_ls[i].pos_p, l);
//		vl_vsub(r, r, traj->pts_ls[i].pos_p);
////		trctraj_bz_vel(self, traj->pts_ls[i].time, rv);
////		vl_vmul_s(rv, rv, 2E-9);
////		vl_vmul_s(rv, traj->pts_ls[i].pos_d, 2.0 * 1E-9);
//
//		vl_vsub(traj->pts_ls[i].offset, r, l);
//	}
//
//	for (i = 1; i < traj->pts_fl; ++i)
//	{
//		vl_vsum(traj->pts_ls[i].offset, traj->pts_ls[i].offset, traj->pts_ls[i-1].offset);
//	}

	// return points back to normal
	
//	for (t_s32 i = 0; i < traj->pts_fl; ++i)
//	{
//		vl_vmul_v(traj->pts_ls[i].pos_p, traj->pts_ls[i].pos_p, traj->p_gain);
//		traj->pts_ls[i].time *= traj->t_gain;
//
//		vl_vsum(traj->pts_ls[i].pos_p, traj->pts_ls[i].pos_p, traj->p_bias);
//		traj->pts_ls[i].time += traj->t_bias;
//	}
	
	return 0x00;
}

//------------------------------------------------------------------------------

//inline t_u8 trctraj_bz_info_(void *data, s_trctraj_info *info)
//{
//	s_trctraj_bz *traj_bz = (s_trctraj_bz*) data;
//
//	if (traj_bz->pts_fl == 0x00)
//	{
//		info->preview_time[0] = 0.0;
//		info->preview_time[1] = 0.0;
//	}
//
//	if (traj_bz->pts_fl == 0x01)
//	{
//		info->preview_time[0] = traj_bz->pts_ls[0].time;
//		info->preview_time[1] = traj_bz->pts_ls[0].time;
//	}
//
//	if (traj_bz->pts_fl >= 0x02)
//	{
//		info->preview_time[0] = traj_bz->pts_ls[0].time;
//		info->preview_time[1] = traj_bz->pts_ls[traj_bz->pts_fl-1].time;
//	}
//
//	return 0x00;
//}

//------------------------------------------------------------------------------

#endif /* __TRCTRAJ_BZ__ */
