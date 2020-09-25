
#include "trj_eng.h"

//------------------------------------------------------------------------------

uint8_t trj_eng_init(s_trj_eng *self, s_trj_eng_init_attr attr)
{
	self->objects_offset = 0x00;
	self->objects = attr.st_objects;

	return 0x00;
}

uint8_t trj_eng_add(s_trj_eng *self, s_trj_obj obj)
{
	self->objects[self->objects_offset] = obj;
	self->objects[self->objects_offset].id = self->objects_offset;
	
	self->objects_offset++;
	
	return 0x00;
}

//void ins_engine_update_forces(trj_eng_t *engine)
//{
//    uint8_t i;
//    uint8_t j;
//
//    vlf_t gforce[3] = { 0, 0, 0 };
//
//    for (i = 0; i < engine->objects_offset; ++i)
//    {
//        for (j = i+1; j < engine->objects_offset; ++j)
//        {
//            trj_gforce(gforce, engine->objects[i], engine->objects[j]);
//
//            vl_vsub(engine->objects[i]->pos_force, engine->objects[i]->pos_force, gforce);
//            vl_vsum(engine->objects[j]->pos_force, engine->objects[j]->pos_force, gforce);
//        }
//    }
//
//    return;
//}

//------------------------------------------------------------------------------

uint8_t trj_engine_update(s_trj_eng *self, vlf_t d_time)
{
    uint8_t i;
    
//    for (i = 0; i < engine->objects_offset; ++i)
//    {
//        self.objects[i].pos_force.append(numpy.array([0.0, 0.0, 0.0]));
//        self.objects[i].rot_force.append(numpy.array([0.0, 0.0, 0.0]));
//    }
    
//    ins_engine_update_forces(engine);

//    # for i in range(len(self.objects)):
//
//    #     self.objects[i].ref_pos[2] = numpy.transpose([0, 0, 0]);
//    #     self.objects[i].ref_rot[2] = numpy.transpose([0, 0, 0]);

//    for i in range(len(self.objects)): self.objects[i].update(d_time);

    for (i = 0; i < self->objects_offset; ++i)
    {
		trj_obj_update(&self->objects[i], d_time);
    }
    
    return 0x00;
}


//------------------------------------------------------------------------------


