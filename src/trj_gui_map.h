
#ifndef __TRJ_GUI_MAP__
#define __TRJ_GUI_MAP__

//------------------------------------------------------------------------------

#include <lib/trj/vl.h>
#include <lib/trj/vl3d.h>
#include <lib/trj/gjson.h>
#include <lib/json11/json11.hpp>

#include <fstream>

//------------------------------------------------------------------------------

typedef struct trj_gui_map
{
	std::vector<s_gjson_obj> obj_list;
	
} 	s_trj_gui_map;

//------------------------------------------------------------------------------

inline uint8_t trj_gui_map_load(s_trj_gui_map *map, char *file_name)
{
	std::ifstream file_stream(file_name);
	std::string file_data((std::istreambuf_iterator<char>(file_stream)),
						  (std::istreambuf_iterator<char>()));
	
	std::string err;
	json11::Json json = json11::Json::parse(file_data.c_str(), err);
	printf(err.c_str());
//	print_json(ml0);
	
	printf("[type][%s] \r\n", json["type"].string_value().c_str());
	printf("[size][%d] \r\n", json["features"].array_items().size());

	for (int i = 0; i < json["features"].array_items().size(); ++i)
	{
		map->obj_list.push_back((s_gjson_obj) {});
		s_gjson_obj *obj = &map->obj_list.back();
		
		json11::Json prop = json["features"][i]["properties"];
		
		strcpy(obj->name, prop["name"].string_value().c_str());
		
		printf("%s \r\n", obj->name);
		
		if (prop["geometry"]["type"] == "Polygon")
		{
			s_gjson_poly *poly = new s_gjson_poly();
			obj->geom.poly = *poly;
			free(poly);
			obj->geom.poly.type = gjson_geom_type_poly;
			
			printf("[poly_size][%d]\r\n", prop["geometry"]["coordinates"].array_items().size());
			
//			poly->pt_list.push_back((s_gjson_pt) { .vec = (s_gjson_poly)});
		}
	}
	
	//	printf("%d\r\n", ml0.is_object());
	//	printf("%d\r\n", ml0.is_string());
	//	printf("%d\r\n", ml0.is_array());
	//	printf("%d\r\n", ml0.is_bool());
	//	printf("%d\r\n", ml0.is_null());
	//	printf("%d\r\n", ml0.is_number());
	
	fflush(stdout);
	
	return 0x00;
}
//------------------------------------------------------------------------------

#endif /* __TRJ_GUI_MAP__ */
