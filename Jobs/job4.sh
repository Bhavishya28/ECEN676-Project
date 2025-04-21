#!/bin/bash
#SBATCH --job-name=job4  	     # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic       # Do not change
#SBATCH --partition=adademic         # Do not change


echo "Running 649.fotonik3d_s-1176B.champsimtrace.xz"
bin/champsim \
  --warmup-instructions 200000000 \
  --simulation-instructions 1000000000 \
  ~/ecen676/ChampSim/dpc3_traces/649.fotonik3d_s-1176B.champsimtrace.xz \
  > 649.fotonik3d_s-1176B.txt
