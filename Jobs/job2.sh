#!/bin/bash
#SBATCH --job-name=job2   # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic       # Do not change
#SBATCH --partition=adademic         # Do not change

echo "Running 619.lbm_s-4268B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/619.lbm_s-4268B.champsimtrace.xz  \
  > 619.lbm_s-4268B.txt



echo "Running 623.xalancbmk_s-700B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/623.xalancbmk_s-700B.champsimtrace.xz  \
  > 623.xalancbmk_s-700B.txt 


