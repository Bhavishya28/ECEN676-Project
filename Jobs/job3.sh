#!/bin/bash
#SBATCH --job-name=job3  	     # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic       # Do not change
#SBATCH --partition=adademic         # Do not change


### Add more ChampSim runs below.

echo "Running 628.pop2_s-17B.champsimtrace.xz"
bin/champsim \
  --warmup_instructions 200000000 \
 ### --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/628.pop2_s-17B.champsimtrace.xz \
  > 628.pop2_s-17B.txt


echo "Running 631.deepsjeng_s-928B.champsimtrace.xz"
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/631.deepsjeng_s-928B.champsimtrace.xz \
  > 631.deepsjeng_s-928B.txt

echo "Running 657.xz_s-3167B.champsimtrace.xz"
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/657.xz_s-3167B.champsimtrace.xz \
  > 657.xz_s-3167B_1.txt


echo "Running 641.leela_s-800B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/641.leela_s-800B.champsimtrace.xz \
  > 641.leela_s-800B.txt


