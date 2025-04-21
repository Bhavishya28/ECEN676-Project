import os
import re
import pandas as pd

# Add your simulation result folder paths here
folders = [
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/Bop_singlecore",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/spp_alt_singlecore",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/spp_dev_singlecore_ghrfalse",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/spp_dev_singlecore_ghrtrue",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/VLDP",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/AMPM"
]

# Patterns
ipc_pattern = re.compile(r"CPU 0 cumulative IPC: (\d+\.\d+)")
mpki_pattern = re.compile(r"MPKI: (\d+\.\d+)")
l2c_prefetch_pattern = re.compile(
    r"cpu0->cpu0_L2C PREFETCH REQUESTED:\s+(\d+)\s+ISSUED:\s+(\d+)\s+USEFUL:\s+(\d+)\s+USELESS:\s+(\d+)"
)

# Create Excel writer
with pd.ExcelWriter("simulation_results_with_l2c.xlsx", engine="openpyxl") as writer:
    for folder_path in folders:
        folder_name = os.path.basename(folder_path)
        data = []

        for filename in os.listdir(folder_path):
            if filename.endswith(".txt"):
                filepath = os.path.join(folder_path, filename)
                with open(filepath, 'r') as f:
                    content = f.read()

                # Extract data
                ipc = float(ipc_pattern.search(content).group(1)) if ipc_pattern.search(content) else None
                mpki = float(mpki_pattern.search(content).group(1)) if mpki_pattern.search(content) else None

                l2c_match = l2c_prefetch_pattern.search(content)
                if l2c_match:
                    requested = int(l2c_match.group(1))
                    issued = int(l2c_match.group(2))
                    useful = int(l2c_match.group(3))
                    useless = int(l2c_match.group(4))
                else:
                    requested = issued = useful = useless = None

                data.append([filename, ipc, mpki, requested, issued, useful, useless])
                
        data.sort(key=lambda x: x[0])  # Sort file names alphabetically
        df = pd.DataFrame(data, columns=["File Name", "IPC", "MPKI", "L2C Requested", "Issued", "Useful", "Useless"])
        df.to_excel(writer, sheet_name=folder_name[:31], index=False)  # Sheet names max 31 chars

print("Extraction complete! Check 'simulation_results_with_l2c.xlsx'.")
