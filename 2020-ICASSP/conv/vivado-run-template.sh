#!/bin/bash
#$ -q drgPQ,pub64
#$ -N {JOB_INDEX}


export XILINXD_LICENSE_FILE=2100@caspian.ics.uci.edu

cd {IN_DIR}/{OUT_DIR}

vivado_hls -f {IN_DIR}/{OUT_DIR}/script.tcl
