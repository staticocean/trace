
#include <api.h>

s_trj_eng *eng = __eng__;

trj_eng_add_ctrlapi(__ctrl_offset__, __ctrl_list__,	0x00, "ctrl_test", NULL, "ctrl_test_init_", "ctrl_test_free_", "ctrl_test_reset_",	"ctrl_test_update_");




