
#include "trj_eng.h"

//------------------------------------------------------------------------------

uint8_t trj_eng_init(s_trj_eng *self, s_trj_eng_init attr)
{
	self->obj_count = 0x00;
	self->obj_list = attr.st_objects;

	return 0x00;
}

uint8_t trj_eng_add(s_trj_eng *self, s_trj_obj_init attr)
{
	trj_obj_init(&self->obj_list[self->obj_count], attr);
	
	self->obj_list[self->obj_count].id = self->obj_count;
	self->obj_list[self->obj_count].obj_list = self->obj_list;
	self->obj_list[self->obj_count].obj_count = &self->obj_count;
	
	self->obj_count++;
	
	return 0x00;
}

uint8_t trj_eng_objnames(s_trj_eng *self, char **objnames)
{
	uint32_t i;
	
	for (i = 0; i < self->obj_count; ++i)
	{
		objnames[i] = self->obj_list[i].name;
	}
	
	return 0x00;
}


//void ins_engine_update_forces(trj_eng_t *engine)
//{
//    uint8_t i;
//    uint8_t j;
//
//    vlf_t gforce[3] = { 0, 0, 0 };
//
//    for (i = 0; i < engine->obj_count; ++i)
//    {
//        for (j = i+1; j < engine->obj_count; ++j)
//        {
//            trj_gforce(gforce, engine->obj_list[i], engine->obj_list[j]);
//
//            vl_vsub(engine->obj_list[i]->pos_force, engine->obj_list[i]->pos_force, gforce);
//            vl_vsum(engine->obj_list[j]->pos_force, engine->obj_list[j]->pos_force, gforce);
//        }
//    }
//
//    return;
//}

//------------------------------------------------------------------------------

uint8_t trj_engine_update(s_trj_eng *self, vlf_t d_time)
{
    uint8_t i;
    
//    for (i = 0; i < engine->obj_count; ++i)
//    {
//        self.obj_list[i].pos_force.append(numpy.array([0.0, 0.0, 0.0]));
//        self.obj_list[i].rot_force.append(numpy.array([0.0, 0.0, 0.0]));
//    }
    
//    ins_engine_update_forces(engine);

//    # for i in range(len(self.obj_list)):
//
//    #     self.obj_list[i].ref_pos[2] = numpy.transpose([0, 0, 0]);
//    #     self.obj_list[i].ref_rot[2] = numpy.transpose([0, 0, 0]);

//    for i in range(len(self.obj_list)): self.obj_list[i].update(d_time);

//    for (i = 0; i < self->obj_count; ++i)
//    {
//		trj_obj_ctrl_update(&self->obj_list[i], d_time);
//    }
    
    return 0x00;
}


//------------------------------------------------------------------------------


