set_global_assignment -entity "PLL80_30" -library "lib_PLL80_30" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "PLL80_30" -library "lib_PLL80_30" -name IP_TOOL_VERSION "20.1"
set_global_assignment -entity "PLL80_30" -library "lib_PLL80_30" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_PLL80_30" -name SPD_FILE [file join $::quartus(sip_path) "PLL80_30.spd"]

set_global_assignment -library "lib_PLL80_30" -name MISC_FILE [file join $::quartus(sip_path) "PLL80_30_sim/PLL80_30.vo"]
