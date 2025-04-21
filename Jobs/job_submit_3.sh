#!/bin/bash
#SBATCH --job-name=serial3   # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic                   # Change to ugrad for undergrads
#SBATCH --partition=adademic          # This job does not use a GPU

echo "mix1 for multicore"
bin/champsim --warmup_instructions 100000000 --simulation_instructions 250000000 ~/ecen676/ChampSim/dpc3_traces/623.xalancbmk_s-700B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/625.x264_s-18B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/627.cam4_s-573B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/628.pop2_s-17B.champsimtrace.xz > mix3_multicore.txt


