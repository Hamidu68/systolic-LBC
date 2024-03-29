### default setting
# set SRC_PATH /home/hamid/phd/papers/pqc/2020-ICASSP/conv/src
set SRC_PATH [pwd]/src
set Project     poly-mult
set Solution    sol1
set Device      xczu29dr-fsvf1760-2lv-e
set Clock       5


set TOP	conv
set CUR_PATH    [pwd]
set OUT_PATH    $CUR_PATH/summary-$Project-$TOP

#TODO: delete dumped files!

open_project -reset $Project

set CFLAGS "-Wno-unknown-pragmas"

add_files $SRC_PATH/reduce.c -cflags "$CFLAGS"
add_files $SRC_PATH/conv.c -cflags "$CFLAGS"
add_files -tb $SRC_PATH/main.c -cflags "$CFLAGS"


set_top $TOP

open_solution -reset $Solution
create_clock -period $Clock
set_part $Device



#####directives
# set_directive_array_partition -type cyclic -factor 256 $TOP shift_reg



#####
# csim_design
csynth_design
# cosim_design
export_design -flow impl -rtl verilog -format ip_catalog

exit 
set systemTime [clock seconds]
set time [clock format $systemTime -format %H:%M:%S]

# file delete -force $OUT_PATH/


file mkdir $OUT_PATH/

file copy -force script.tcl $OUT_PATH
file copy -force $CUR_PATH/$Project/$Solution/syn/report/${TOP}_csynth.rpt $OUT_PATH/${TOP}_csynth.rpt-$time
file copy -force $SRC_PATH/conv.cpp $OUT_PATH/conv-$time.cpp
file copy -force $CUR_PATH/$Project/$Solution/syn/report/${TOP}_csynth.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/sim/report/${TOP}_cosim.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/report/verilog/${TOP}_export.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/project.runs/impl_1/${TOP}_power_routed.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/project.runs/impl_1/${TOP}_utilization_placed.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/$TOP.v $OUT_PATH/
# file copy -force vivado_hls.log $OUT_PATH/

# file delete -force -- $CUR_PATH/$Project
file delete -force -- $CUR_PATH/vivado_hls.log
# file delete -force 

exit