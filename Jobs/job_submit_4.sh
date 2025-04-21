#!/bin/bash
#SBATCH --job-name=serial4   # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic                   # Change to ugrad for undergrads
#SBATCH --partition=adademic          # This job does not use a GPU

echo "mix1 for multicore"
bin/champsim --warmup_instructions 100000000 --simulation_instructions 250000000 ~/ecen676/ChampSim/dpc3_traces/631.deepsjeng_s-928B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/638.imagick_s-10316B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/641.leela_s-800B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/644.nab_s-5853B.champsimtrace.xz > mix4_multicore.txt

