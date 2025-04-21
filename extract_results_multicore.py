import os
import re
import pandas as pd

# List of folders to process
folders = [
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/multicore_spp_alt",
    "/home/grads/b/bhavishya_k_s/ECEN676-Project/multicore_spp_dev"
]

# Patterns
ipc_pattern = re.compile(r"CPU (\d+) cumulative IPC: ([\d.]+)")
accuracy_mpki_pattern = re.compile(r"CPU (\d+) Branch Prediction Accuracy: ([\d.]+)%.*?MPKI: ([\d.]+)")
l2c_pattern = re.compile(
    r"cpu(\d+)->cpu\1_L2C PREFETCH REQUESTED:\s+(\d+)\s+ISSUED:\s+(\d+)\s+USEFUL:\s+(\d+)\s+USELESS:\s+(\d+)"
)
refresh_pattern = re.compile(r"Channel (\d+) REFRESHES ISSUED:\s+(\d+)")

# Output Excel
output_file = "all_simulation_results.xlsx"

with pd.ExcelWriter(output_file, engine='openpyxl') as writer:
    for folder_path in folders:
        folder_name = os.path.basename(folder_path)
        all_data = []

        for filename in sorted([f for f in os.listdir(folder_path) if f.endswith(".txt")]):
            file_path = os.path.join(folder_path, filename)
            with open(file_path, "r") as file:
                content = file.read()

                # Extract core metrics
                core_data = []
                for i in range(4):  # for CPUs 0-3
                    ipc_match = re.search(rf"CPU {i} cumulative IPC: ([\d.]+)", content)
                    acc_mpki_match = re.search(rf"CPU {i} Branch Prediction Accuracy: ([\d.]+)%.*?MPKI: ([\d.]+)", content)
                    l2c_match = re.search(rf"cpu{i}->cpu{i}_L2C PREFETCH REQUESTED:\s+(\d+)\s+ISSUED:\s+(\d+)\s+USEFUL:\s+(\d+)\s+USELESS:\s+(\d+)", content)

                    ipc = float(ipc_match.group(1)) if ipc_match else None
                    accuracy = float(acc_mpki_match.group(1)) if acc_mpki_match else None
                    mpki = float(acc_mpki_match.group(2)) if acc_mpki_match else None

                    req = int(l2c_match.group(1)) if l2c_match else None
                    issued = int(l2c_match.group(2)) if l2c_match else None
                    useful = int(l2c_match.group(3)) if l2c_match else None
                    useless = int(l2c_match.group(4)) if l2c_match else None

                    core_data.append([filename, i, ipc, accuracy, mpki, req, issued, useful, useless])

                # Extract refreshes
                refreshes = refresh_pattern.findall(content)
                refresh_data = [["", "", "", "", "", "", "", "", "", f"Channel {ch}", int(val)] for ch, val in refreshes]

                # Combine all
                all_data.extend(core_data + refresh_data)

        # Create DataFrame
        df = pd.DataFrame(
            all_data,
            columns=["File", "CPU", "IPC", "Accuracy (%)", "MPKI", "L2C Requested", "L2C Issued", "L2C Useful", "L2C Useless", "Channel", "Refreshes"]
        )

        df.to_excel(writer, sheet_name=folder_name[:31], index=False)

print(f"Extraction complete! Results saved to '{output_file}'")
