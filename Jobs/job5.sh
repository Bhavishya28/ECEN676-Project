#!/bin/bash
#SBATCH --job-name=job5 	     # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic       # Do not change
#SBATCH --partition=adademic         # Do not change





echo "Running 621.wrf_s-575B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/621.wrf_s-575B.champsimtrace.xz \
  > 621.wrf_s-575B.txt

echo "Running 625.x264_s-18B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/625.x264_s-18B.champsimtrace.xz \
  > 625.x264_s-18B.txt

echo "Running 638.imagick_s-10316B.champsimtrace.xz "
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/638.imagick_s-10316B.champsimtrace.xz \
  > 638.imagick_s-10316B.txt

echo "Running 603.bwaves_s-3699B.champsimtrace.xz"
bin/champsim \
  --warmup_instructions 200000000 \
  --simulation_instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/603.bwaves_s-3699B.champsimtrace.xz \
  > 603.bwaves_s-3699B.txt
