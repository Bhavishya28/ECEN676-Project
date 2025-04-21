#!/bin/bash
#SBATCH --job-name=Serial2   # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic                   # Change to ugrad for undergrads
#SBATCH --partition=adademic          # This job does not use a GPU

echo "mix1 for multicore"
bin/champsim --warmup_instructions 100000000 --simulation_instructions 250000000 ~/ecen676/ChampSim/dpc3_traces/607.cactuBSSN_s-2421B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/619.lbm_s-4268B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/620.omnetpp_s-874B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/621.wrf_s-575B.champsimtrace.xz > mix2_multicore.txt


