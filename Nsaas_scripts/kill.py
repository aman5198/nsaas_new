import os
import subprocess

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

cmd_amfd = "ps -ef | grep amfd | grep -v grep"
output = getCommandOutput(cmd_amfd)
amf_id = output[1].split()[1] if output[0] else None

cmd_smfd = "ps -ef | grep smfd | grep -v grep"
output = getCommandOutput(cmd_smfd)
smf_id = output[1].split()[1] if output[0] else None

cmd_nssf = "ps -ef | grep nssf | grep -v grep"
output = getCommandOutput(cmd_nssf)
nssf_id = output[1].split()[1] if output[0] else None

cmd_gnb = "ps -ef | grep gnb | grep -v grep"
output = getCommandOutput(cmd_gnb)
gnb_id = output[1].split()[1] if output[0] else None

# now kill the processes
cmd_kill = "kill -9 {} {} {} {}".format(amf_id, smf_id, nssf_id, gnb_id)
os.system(cmd_kill)