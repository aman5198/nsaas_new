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
    return output_file


def append_gnb_config(input_file):
    print("input file: ", input_file)
    with open(input_file) as f:
        data = yaml.load(f)
    slice_obj1 = {'sst': args.sst, 'sd': args.sd}
    data['slices'].append(slice_obj1)
    output_file = input_file[:-5] + "_slice.yaml"
    with open(output_file, 'w') as f:
        yaml.dump(data, f)
    return output_file


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

# kill the gnb
cmd_gnb = "ps aux | grep gnb | grep -v grep"
output = getCommandOutput(cmd_gnb)
gnb_id = output[1].split()[1] if output[0] else None
os.system("kill -9 {}".format(gnb_id))
gnb_config_file = output[1].split()[-1] if output[0] else None

cmd_amfd = "ps aux | grep amfd | grep -v grep"
output = getCommandOutput(cmd_amfd)
amf_id = output[1].split()[1] if output[0] else None

cmd_smfd = "ps aux | grep smfd | grep -v grep"
output = getCommandOutput(cmd_smfd)
smf_id = output[1].split()[1] if output[0] else None

cmd_nssf = "ps aux | grep nssf | grep -v grep"
output = getCommandOutput(cmd_nssf)
nssf_id = output[1].split()[1] if output[0] else None

# now kill all the open5gs processes 
cmd_kill = "kill -9 {} {} {}".format(amf_id, smf_id, nssf_id)
os.system(cmd_kill)


config_file = output[1].split()[-1] if output[0] else None
base_address = output[1].split()[10].split('open5gs')[0]
open5gs_base_address = base_address + "open5gs/"
sample_config_file = open5gs_base_address + config_file
new_sample_config_file = append_slice_config(sample_config_file)
new_sample_config_file_relative_address = new_sample_config_file.split(open5gs_base_address)[-1]

UERANSIM_base_address = base_address + "nsaas_new/"
sample_gnb_config_file = UERANSIM_base_address + gnb_config_file
new_sample_gnb_config_file = append_gnb_config(sample_gnb_config_file)
new_sample_gnb_config_file_relative_address = new_sample_gnb_config_file.split(UERANSIM_base_address)[-1]


# now start the processes with the new config
cmd_start_amf = f"cd {open5gs_base_address} && {open5gs_base_address}build/src/amf/open5gs-amfd -c {new_sample_config_file_relative_address} &"
cmd_start_smf = f"cd {open5gs_base_address} && {open5gs_base_address}build/src/smf/open5gs-smfd -c {new_sample_config_file_relative_address} &"
cmd_start_nssf = f"cd {open5gs_base_address} && {open5gs_base_address}build/src/nssf/open5gs-nssfd -c {new_sample_config_file_relative_address} &"
cmd_start_gnb = f"cd {UERANSIM_base_address} && ./build/nr-gnb -c {new_sample_gnb_config_file_relative_address} &"


# start the processes in diff threads and wait for 5 seconds after each 
TIME_GAP = 2
os.system(cmd_start_amf)
time.sleep(TIME_GAP)
os.system(cmd_start_smf)
time.sleep(TIME_GAP)
os.system(cmd_start_nssf)
time.sleep(TIME_GAP)
os.system(cmd_start_gnb)
time.sleep(TIME_GAP)