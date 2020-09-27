
#include "trj_traj.h"

//------------------------------------------------------------------------------

uint8_t trj_traj_bz_init(s_trj_traj_bz *self, s_trj_traj_bz_init_attr attr)
{
	self->pts = attr.pts;
	self->pts_offset = 0x00;
	
	return 0x00;
}

uint8_t trj_traj_bz_add(s_trj_traj_bz *self, s_trj_traj_bz_point point)
{
	self->pts[self->pts_offset] = point;
	self->pts_offset++;
	
	return 0x00;
}

uint8_t trj_traj_bz_compile(s_trj_traj_bz *self)
{
	uint32_t i;
	uint32_t j;
	
	vlf_t m_a[self->pts_offset*2][self->pts_offset*2];
	
	for (i = 1; i < 2*self->pts_offset-1; ++i)
	{
		for (j = 0; j < 2*self->pts_offset; ++j)
		{ m_a[i][j] = 0.0; }
		
		m_a[i][i-1] = 1;
		m_a[i][i+1] = 1;
		m_a[i][i+0] = 4;
	}
	
	for (j = 0; j < 2*self->pts_offset; ++j)
	{ m_a[0][j] = 0.0; m_a[2*self->pts_offset-1][j] = 0.0; }
	
	m_a[0][0] = 2;
	m_a[0][1] = 1;
	
	m_a[2*self->pts_offset-1][2*self->pts_offset-2] = 1;
	m_a[2*self->pts_offset-1][2*self->pts_offset-1] = 2;
	
	vlf_t v_p[2*self->pts_offset];
	vlf_t v_d[2*self->pts_offset];
	
	for (i = 1; i < self->pts_offset-1; ++i)
	{
		v_p[2*i+0] = 3 * (self->pts[i+1].p[0] - self->pts[i-1].p[0]);
		v_p[2*i+1] = 3 * (self->pts[i+1].p[1] - self->pts[i-1].p[1]);
	}
	
	v_p[0] = 3 * (self->pts[1].p[0] - self->pts[0].p[0]);
	v_p[1] = 3 * (self->pts[1].p[1] - self->pts[0].p[1]);
	
	v_p[2*self->pts_offset-1-1] = 3 * (self->pts[self->pts_offset-1].p[0]
									 - self->pts[self->pts_offset-2].p[0]);
	
	v_p[2*self->pts_offset-1-0] = 3 * (self->pts[self->pts_offset-1].p[1]
			                         - self->pts[self->pts_offset-2].p[1]);
	
	vlf_t m_inva[self->pts_offset*2][self->pts_offset*2];
	
	vl_inv(self->pts_offset*2, &m_inva[0][0], &m_a[0][0]);
	
	for (i = 0; i < self->pts_offset*2; ++i)
	{
		v_d[i] = 0.0;
		
		for (j = 0; j < self->pts_offset*2; ++j)
		{
			v_d[i] += m_inva[i][j] * v_p[j];
		}
	}
	
	for (i = 0; i < self->pts_offset; ++i)
	{
		self->pts[i].d[0] = v_d[2*i+0];
		self->pts[i].d[1] = v_d[2*i+1];
	}
	
	return 0x00;
}

//class trj_ref_bezier(object):
//
//def __init__(self, parent):
//
//self.parent = parent;
//self.time = [];
//self.pos  = [];
//self.rot  = [];
//self.time_cache = [];
//self.pos_cache = [];
//self.rot_cache = [];
//
//# def make_uniform(self):
//
//# 	length = [0.0, 0.0, 0.0];
//# 	time = self.time_cache[-1][1] - self.time_cache[0][0];
//
//# 	for i in range(len(self.time_cache)):
//
//# 		length[0] += self.pos_cache[i][0][0].length;
//# 		length[1] += self.pos_cache[i][0][1].length;
//# 		length[2] += self.pos_cache[i][0][2].length;
//
//# 	avg_vel = [time / length[0], time / length[1], time / length[2]];
//# 	prev_time = 0.0;
//
//# 	for i in range(len(self.time_cache)):
//
//# 		local_vel = [
//# 			self.pos_cache[i][0][0].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 			self.pos_cache[i][0][1].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 			self.pos_cache[i][0][2].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 		];
//
//# 		self.pos_cache[i][0][0].p0[0] *= local_vel[0] / avg_vel[0];
//# 		self.pos_cache[i][0][1].p1[0] *= local_vel[1] / avg_vel[1];
//# 		self.pos_cache[i][0][2].p2[0] *= local_vel[2] / avg_vel[2];
//
//class trj_ref_bezier(object):
//
//def __init__(self, parent):
//
//self.parent = parent;
//self.time = [];
//self.pos  = [];
//self.rot  = [];
//self.time_cache = [];
//self.pos_cache = [];
//self.rot_cache = [];
//
//# def make_uniform(self):
//
//# 	length = [0.0, 0.0, 0.0];
//# 	time = self.time_cache[-1][1] - self.time_cache[0][0];
//
//# 	for i in range(len(self.time_cache)):
//
//# 		length[0] += self.pos_cache[i][0][0].length;
//# 		length[1] += self.pos_cache[i][0][1].length;
//# 		length[2] += self.pos_cache[i][0][2].length;
//
//# 	avg_vel = [time / length[0], time / length[1], time / length[2]];
//# 	prev_time = 0.0;
//
//# 	for i in range(len(self.time_cache)):
//
//# 		local_vel = [
//# 			self.pos_cache[i][0][0].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 			self.pos_cache[i][0][1].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 			self.pos_cache[i][0][2].length / (self.time_cache[i][1] - self.time_cache[i][0]),
//# 		];
//
//# 		self.pos_cache[i][0][0].p0[0] *= local_vel[0] / avg_vel[0];
//# 		self.pos_cache[i][0][1].p1[0] *= local_vel[1] / avg_vel[1];
//# 		self.pos_cache[i][0][2].p2[0] *= local_vel[2] / avg_vel[2];
//
//def compile(self):
//
//# append 1st straight cache
//self.pos_cache.append([[]]);
//self.time_cache.append([self.time[0], 0.5*(self.time[0]+self.time[1])]);
//
//for i in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1],	[
//[self.time[0]+0.00*(self.time[1]-self.time[0]), self.pos[0][i]+0.00*(self.pos[1][i]-self.pos[0][i])],
//[self.time[0]+0.25*(self.time[1]-self.time[0]), self.pos[0][i]+0.25*(self.pos[1][i]-self.pos[0][i])],
//[self.time[0]+0.50*(self.time[1]-self.time[0]), self.pos[0][i]+0.50*(self.pos[1][i]-self.pos[0][i])],
//])
//);
//
//
//for i in range(len(self.time)-2):
//
//# append cache
//self.pos_cache.append([[]]);
//self.time_cache.append([0.5*(self.time[i]+self.time[i+1]), 0.5*(self.time[i+1]+self.time[i+2])]);
//
//for j in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1], [
//[0.5*(self.time[i]+self.time[i+1]), 0.5*(self.pos[i][j]+self.pos[i+1][j])                                                     ],
//[self.time[i+1], self.pos[i+1][j]],
//[0.5*(self.time[i+1]+self.time[i+2]), 0.5*(self.pos[i+1][j]+self.pos[i+2][j])]
//])
//);
//
//# append last straight cache
//
//self.pos_cache.append([[]]);
//self.time_cache.append([0.5*(self.time[-2]+self.time[-1]), self.time[-1]]);
//
//for i in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1], [
//[self.time[-2] + 0.50*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 0.50*(self.pos[-1][i]-self.pos[-2][i])],
//[self.time[-2] + 0.75*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 0.75*(self.pos[-1][i]-self.pos[-2][i])],
//[self.time[-2] + 1.00*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 1.00*(self.pos[-1][i]-self.pos[-2][i])],
//]),
//);
//
//def __get_pos__(self, time_stamp):
//
//ts = time_stamp;
//
//offset = 0x00;
//
//while 0x01:
//
//if offset == (len(self.time_cache)-1):
//break;
//
//elif ts >= self.time_cache[offset][0] and ts <= self.time_cache[offset][1]:
//break;
//
//else:
//offset += 0x01;
//
//pos = numpy.array([
//		self.pos_cache[offset][0][0].eval(ts),
//				self.pos_cache[offset][0][1].eval(ts),
//				self.pos_cache[offset][0][2].eval(ts),
//]);
//
//# if self.parent.ref_obj != None:
//# 	pos = self.parent.ref_obj.pos[-1][0] + self.parent.ref_obj.rot[-1][0].I.dot(pos);
//
//		return numpy.array(pos).reshape(3);
//
//	def get_pos(self, time_stamp):
//
//		dt = 0.1;
//
//		return [
//			 self.__get_pos__(time_stamp),
//			(self.__get_pos__(time_stamp+dt) - self.__get_pos__(time_stamp-dt)) / (2*dt),
//			(self.__get_pos__(time_stamp+dt) - self.__get_pos__(time_stamp)) - (self.__get_pos__(time_stamp) - self.__get_pos__(time_stamp-dt)) / (dt*dt)
//		];
//
//	def get_rot(self, time_stamp):
//
//		ts = time_stamp;
//
//		offset = 0x00;
//
//		while offset < (len(self.time_cache)-1):
//
//			if ts >= self.time_cache[offset][0] and ts <= self.time_cache[offset][1]: break;
//			else: offset += 0x01;
//
//		# print(self.time_cache[offset]);
//		# print(offset, (ts-self.time_cache[offset][0]) / (self.time_cache[offset][1]-self.time_cache[offset][0]));
//
//		rot = trj_utils.hpr_to_rot(trj_utils.rot_hpr(0, 0, 0));
//
//		if self.parent.ref_object != None:
//			pos = self.parent.ref_object.pos[-1][0] + self.parent.ref_object.rot[-1][0].I.dot(pos);
//
//		return rot;
//
//	def modify_gcc(self, d_time):
//
//		temp_time = [];
//		temp_pos = [];
//		temp_rot = [];
//
//		for i in range(len(self.time)-1):
//
//			p0_ecef = trj_utils.pos_ecef(self.pos[i+0]);
//			p1_ecef = trj_utils.pos_ecef(self.pos[i+1]);
//
//			p0_lla = trj_utils.ecef_to_lla(p0_ecef);
//			p1_lla = trj_utils.ecef_to_lla(p1_ecef);
//
//			def flight_pos(x):
//
//				return trj_utils.lla_to_ecef(trj_utils.pos_lla(
//					numpy.array(gcc.intermediate_point((math.degrees(p0_lla.lon), math.degrees(p0_lla.lat)), (math.degrees(p1_lla.lon), math.degrees(p1_lla.lat)), x))[1]*math.pi/180,
//					numpy.array(gcc.intermediate_point((math.degrees(p0_lla.lon), math.degrees(p0_lla.lat)), (math.degrees(p1_lla.lon), math.degrees(p1_lla.lat)), x))[0]*math.pi/180,
//					p0_lla.alt + x*(p1_lla.alt-p0_lla.alt)
//				)).vec;
//
//			temp_time.append([]);
//			temp_pos.append([]);
//			temp_rot.append([]);
//
//			for j in range(int((self.time[i+1]-self.time[i]) / d_time)):
//
//				temp_time[-1].append(self.time[i] + j*d_time);
//				temp_pos[-1].append(flight_pos((temp_time[-1][-1]-self.time[i]) / (self.time[i+1]-self.time[i])));
//				temp_rot[-1].append(self.rot[i]);
//
//		self.time = [];
//		self.pos  = [];
//		self.rot  = [];
//
//		for i in range(len(temp_time)):
//
//			self.time += temp_time[i];
//			self.pos  += temp_pos [i];
//			self.rot  += temp_rot [i];
//
//
//	def add_point(self, time, pos, rot):
//
//		self.time.append(time);
//		self.pos.append(pos);
//		self.rot.append(rot);
//
//		return None;
//
//def compile(self):
//
//# append 1st straight cache
//self.pos_cache.append([[]]);
//self.time_cache.append([self.time[0], 0.5*(self.time[0]+self.time[1])]);
//
//for i in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1],	[
//[self.time[0]+0.00*(self.time[1]-self.time[0]), self.pos[0][i]+0.00*(self.pos[1][i]-self.pos[0][i])],
//[self.time[0]+0.25*(self.time[1]-self.time[0]), self.pos[0][i]+0.25*(self.pos[1][i]-self.pos[0][i])],
//[self.time[0]+0.50*(self.time[1]-self.time[0]), self.pos[0][i]+0.50*(self.pos[1][i]-self.pos[0][i])],
//])
//);
//
//
//for i in range(len(self.time)-2):
//
//# append cache
//self.pos_cache.append([[]]);
//self.time_cache.append([0.5*(self.time[i]+self.time[i+1]), 0.5*(self.time[i+1]+self.time[i+2])]);
//
//for j in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1], [
//[0.5*(self.time[i]+self.time[i+1]), 0.5*(self.pos[i][j]+self.pos[i+1][j])                                                     ],
//[self.time[i+1], self.pos[i+1][j]],
//[0.5*(self.time[i+1]+self.time[i+2]), 0.5*(self.pos[i+1][j]+self.pos[i+2][j])]
//])
//);
//
//# append last straight cache
//
//self.pos_cache.append([[]]);
//self.time_cache.append([0.5*(self.time[-2]+self.time[-1]), self.time[-1]]);
//
//for i in range(3):
//
//self.pos_cache[-1][-1].append(
//		trj_bezier(self.time_cache[-1], [
//[self.time[-2] + 0.50*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 0.50*(self.pos[-1][i]-self.pos[-2][i])],
//[self.time[-2] + 0.75*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 0.75*(self.pos[-1][i]-self.pos[-2][i])],
//[self.time[-2] + 1.00*(self.time[-1]-self.time[-2]), self.pos[-2][i] + 1.00*(self.pos[-1][i]-self.pos[-2][i])],
//]),
//);
//
//def __get_pos__(self, time_stamp):
//
//ts = time_stamp;
//
//offset = 0x00;
//
//while 0x01:
//
//if offset == (len(self.time_cache)-1):
//break;
//
//elif ts >= self.time_cache[offset][0] and ts <= self.time_cache[offset][1]:
//break;
//
//else:
//offset += 0x01;
//
//pos = numpy.array([
//		self.pos_cache[offset][0][0].eval(ts),
//				self.pos_cache[offset][0][1].eval(ts),
//				self.pos_cache[offset][0][2].eval(ts),
//]);
//
//# if self.parent.ref_obj != None:
//# 	pos = self.parent.ref_obj.pos[-1][0] + self.parent.ref_obj.rot[-1][0].I.dot(pos);
//
//		return numpy.array(pos).reshape(3);
//
//	def get_pos(self, time_stamp):
//
//		dt = 0.1;
//
//		return [
//			 self.__get_pos__(time_stamp),
//			(self.__get_pos__(time_stamp+dt) - self.__get_pos__(time_stamp-dt)) / (2*dt),
//			(self.__get_pos__(time_stamp+dt) - self.__get_pos__(time_stamp)) - (self.__get_pos__(time_stamp) - self.__get_pos__(time_stamp-dt)) / (dt*dt)
//		];
//
//	def get_rot(self, time_stamp):
//
//		ts = time_stamp;
//
//		offset = 0x00;
//
//		while offset < (len(self.time_cache)-1):
//
//			if ts >= self.time_cache[offset][0] and ts <= self.time_cache[offset][1]: break;
//			else: offset += 0x01;
//
//		# print(self.time_cache[offset]);
//		# print(offset, (ts-self.time_cache[offset][0]) / (self.time_cache[offset][1]-self.time_cache[offset][0]));
//
//		rot = trj_utils.hpr_to_rot(trj_utils.rot_hpr(0, 0, 0));
//
//		if self.parent.ref_object != None:
//			pos = self.parent.ref_object.pos[-1][0] + self.parent.ref_object.rot[-1][0].I.dot(pos);
//
//		return rot;
//
//	def modify_gcc(self, d_time):
//
//		temp_time = [];
//		temp_pos = [];
//		temp_rot = [];
//
//		for i in range(len(self.time)-1):
//
//			p0_ecef = trj_utils.pos_ecef(self.pos[i+0]);
//			p1_ecef = trj_utils.pos_ecef(self.pos[i+1]);
//
//			p0_lla = trj_utils.ecef_to_lla(p0_ecef);
//			p1_lla = trj_utils.ecef_to_lla(p1_ecef);
//
//			def flight_pos(x):
//
//				return trj_utils.lla_to_ecef(trj_utils.pos_lla(
//					numpy.array(gcc.intermediate_point((math.degrees(p0_lla.lon), math.degrees(p0_lla.lat)), (math.degrees(p1_lla.lon), math.degrees(p1_lla.lat)), x))[1]*math.pi/180,
//					numpy.array(gcc.intermediate_point((math.degrees(p0_lla.lon), math.degrees(p0_lla.lat)), (math.degrees(p1_lla.lon), math.degrees(p1_lla.lat)), x))[0]*math.pi/180,
//					p0_lla.alt + x*(p1_lla.alt-p0_lla.alt)
//				)).vec;
//
//			temp_time.append([]);
//			temp_pos.append([]);
//			temp_rot.append([]);
//
//			for j in range(int((self.time[i+1]-self.time[i]) / d_time)):
//
//				temp_time[-1].append(self.time[i] + j*d_time);
//				temp_pos[-1].append(flight_pos((temp_time[-1][-1]-self.time[i]) / (self.time[i+1]-self.time[i])));
//				temp_rot[-1].append(self.rot[i]);
//
//		self.time = [];
//		self.pos  = [];
//		self.rot  = [];
//
//		for i in range(len(temp_time)):
//
//			self.time += temp_time[i];
//			self.pos  += temp_pos [i];
//			self.rot  += temp_rot [i];
//
//
//	def add_point(self, time, pos, rot):
//
//		self.time.append(time);
//		self.pos.append(pos);
//		self.rot.append(rot);
//
//		return None;
//
////------------------------------------------------------------------------------
//
