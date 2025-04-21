#!/bin/bash
#SBATCH --job-name=job8  	           # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic       # Do not change
#SBATCH --partition=adademic         # Do not change

echo "Running 600.perlbench_s-210B.champsimtrace.xz"
bin/champsim \
 --warmup_instructions 200000000 \
 --simulation_instructions 1000000000 \
 ~/ecen676/ChampSim/dpc3_traces/600.perlbench_s-210B.champsimtrace.xz \
 > 600.perlbench_s-210B.txt

echo "Running 644.nab_s-5853B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/644.nab_s-5853B.champsimtrace.xz  \
  > 644.nab_s-5853B.txt

echo "Running 648.exchange2_s-1699B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/648.exchange2_s-1699B.champsimtrace.xz \
  > 648.exchange2_s-1699B.txt


