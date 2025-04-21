#!/bin/bash
#SBATCH --job-name=serial1   # Job name
#SBATCH --mail-type=END,FAIL         # Mail Events (NONE,BEGIN,FAIL,END,ALL)
#SBATCH --mail-user=bhavishya_k_s@tamu.edu   # Replace with your email address
#SBATCH --ntasks=1                   # Run on a single CPU
#SBATCH --time=12:00:00              # Time limit hh:mm:ss
#SBATCH --output=serial_test_%j.log  # Standard output and error log
#SBATCH --qos=olympus-academic                   # Change to ugrad for undergrads
#SBATCH --partition=adademic          # This job does not use a GPU

echo "mix1 for multicore"
bin/champsim --warmup_instructions 100000000 --simulation_instructions 250000000 ~/ecen676/ChampSim/dpc3_traces/600.perlbench_s-210B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/602.gcc_s-734B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/603.bwaves_s-3699B.champsimtrace.xz ~/ecen676/ChampSim/dpc3_traces/605.mcf_s-665B.champsimtrace.xz > mix1_multicore.txt


