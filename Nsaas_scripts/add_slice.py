import os 
import subprocess
import ruamel.yaml
import time
yaml = ruamel.yaml.YAML()
import argparse

parser = argparse.ArgumentParser(description='Add a slice to the network')
parser.add_argument('--sst', type=int, help='Service Slice Type')
parser.add_argument('--sd', type=str, help='Service Slice Description')
parser.add_argument('--addr', type=str, help='Address of the slice')
parser.add_argument('--port', type=int, help='Port of the slice')
args = parser.parse_args()


def append_slice_config(input_file):
    print("input file: ", input_file)
    with open(input_file) as f:
        data = yaml.load(f)

    slice_obj1 = {'sst': args.sst, 'sd': args.sd}
    slice_obj1_nssai = {'addr': args.addr, 'port': args.port, 's_nssai': slice_obj1}

    data['amf']['plmn_support'][0]['s_nssai'].append(slice_obj1)  
    data['nssf']['nsi'].append(slice_obj1_nssai)

    output_file = input_file[:-5] + "_slice.yaml"
    with open(output_file, 'w') as f:
        yaml.dump(data, f)


def getCommandOutput(consoleCommand, consoleOutputEncoding="utf-8", timeout=10):
    isRunCmdOk = False
    consoleOutput = ""
    try:
        consoleOutputByte = subprocess.check_output(consoleCommand, shell=True, timeout=timeout)
        consoleOutput = consoleOutputByte.decode(consoleOutputEncoding) # '640x360\n'
        consoleOutput = consoleOutput.strip() # '640x360'
        isRunCmdOk = True
    except subprocess.CalledProcessError as callProcessErr:
        consoleOutput = str(callProcessErr)
        # print("Error %s for run command %s" % (consoleOutput, consoleCommand))
    except subprocess.TimeoutExpired as timeoutErr:
        consoleOutput = str(timeoutErr)
        # print("Error %s for run command %s" % (consoleOutput, consoleCommand))

    return isRunCmdOk, consoleOutput


cmd_gnb = "ps aux | grep gnb | grep -v grep"
output = getCommandOutput(cmd_gnb)
gnb_id = output[1].split()[1] if output[0] else None

os.system("kill -9 {}".format(gnb_id))


cmd_amfd = "ps aux | grep amfd | grep -v grep"
output = getCommandOutput(cmd_amfd)
amf_id = output[1].split()[1] if output[0] else None
config_file = output[1].split()[-1] if output[0] else None
print("config file: ", config_file)
base_address = output[1].split()[10].split('open5gs')[0]
print("base address: ", base_address)
open5gs_base_address = base_address + "open5gs/"
print("open5gs base address: ", open5gs_base_address)
sample_config_file = open5gs_base_address + config_file
print("sample config file: ", sample_config_file)

append_slice_config(sample_config_file)

cmd_smfd = "ps aux | grep smfd | grep -v grep"
output = getCommandOutput(cmd_smfd)
smf_id = output[1].split()[1] if output[0] else None

cmd_nssf = "ps aux | grep nssf | grep -v grep"
output = getCommandOutput(cmd_nssf)
nssf_id = output[1].split()[1] if output[0] else None

config_file = output[1].split()[-1] if output[0] else None
UERANSIM_base_address = base_address + "UERANSIM/"

# now kill the processes 
cmd_kill = "kill -9 {} {} {}".format(amf_id, smf_id, nssf_id)
os.system(cmd_kill)

# now start the processes with the new config
cmd_start_amf = f"cd {open5gs_base_address} && ./install/bin/open5gs-amfd -c build/configs/sample_slice.yaml &"
cmd_start_smf = f"cd {open5gs_base_address} && ./install/bin/open5gs-smfd &"
cmd_start_nssf = f"cd {open5gs_base_address} && ./install/bin/open5gs-nssfd -c build/configs/sample_slice.yaml &"
# TODO: edit the gnb config file as well/ currently we are assuming that it has information about the slice
cmd_start_gnb = f"cd {UERANSIM_base_address} && ./build/nr-gnb -c config/open5gs-gnb.yaml &"

# start the processes in diff threads and wait for 5 seconds after each 
TIME_GAP = 5

os.system(cmd_start_amf)
time.sleep(TIME_GAP)
os.system(cmd_start_smf)
time.sleep(TIME_GAP)
os.system(cmd_start_nssf)
time.sleep(TIME_GAP)
os.system(cmd_start_gnb)
time.sleep(TIME_GAP)