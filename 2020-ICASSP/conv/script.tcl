### default setting
set SRC_PATH /home/hamid/phd/papers/pqc/2020-ICASSP/conv/src
# set SRC_PATH	{IN_DIR}/{DOMAIN}/src
set Project     poly-mult
set Solution    sol1
set Device      virtex7
set Clock       10


set TOP	conv
set CUR_PATH    [pwd]
set OUT_PATH    $CUR_PATH/summary-$TOP

#TODO: delete dumped files!

open_project -reset $Project

set CFLAGS "-Wno-unknown-pragmas"

add_files $SRC_PATH/conv.cpp -cflags "$CFLAGS"
add_files -tb $SRC_PATH/main.cpp -cflags "$CFLAGS"


set_top $TOP

open_solution -reset $Solution
create_clock -period $Clock
set_part $Device

#####
csim_design
csynth_design
# cosim_design
# export_design -flow impl -rtl verilog -format ip_catalog

file delete -force $OUT_PATH/
file mkdir $OUT_PATH/

file copy -force script.tcl $OUT_PATH
file copy -force $CUR_PATH/$Project/$Solution/syn/report/${TOP}_csynth.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/sim/report/${TOP}_cosim.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/report/verilog/${TOP}_export.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/project.runs/impl_1/${TOP}_power_routed.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/project.runs/impl_1/${TOP}_utilization_placed.rpt $OUT_PATH/
# file copy -force $CUR_PATH/$Project/$Solution/impl/verilog/$TOP.v $OUT_PATH/
# file copy -force vivado_hls.log $OUT_PATH/

file delete -force -- $CUR_PATH/$Project
file delete -force -- $CUR_PATH/vivado_hls.log
# file delete -force 

exit