
#include <api.h>

s_trceng *eng = __eng__;

trceng_add_ctrlapi(__env__, __ctrl_sz__, __ctrl_ls__, 0x00, "custom_ctrl_test", NULL, "ctrl_test_init_", "ctrl_test_free_", "ctrl_test_reset_",	"ctrl_test_update_");
