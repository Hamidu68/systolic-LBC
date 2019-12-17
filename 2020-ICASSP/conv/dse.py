import os, sys
from Sweeper import SweepTask, Sweeper
from os.path import join
import json
import socket
  

class ParamHolder:
    def __init__(self, key, params):
        self.params = params
        self.key = key
        self.curser = 0
    
    def getParam(self):
        return self.params[self.curser]

    def getKey(self):
        return self.key

    def incCurser(self):
        self.curser = (self.curser + 1)
        if(self.curser >= len(self.params)):
            self.curser = 0
            return 1
        return 0

def replaceInFile(params, inDir, outDir, inPath, outPath):
    with open(inPath, 'r') as inf:
        with open(outPath, 'w') as outf:
            red = inf.read();
            for param in params:
                red = red.replace('{'+param.getKey()+'}', str(param.getParam()))
            red = red.replace('{IN_DIR}', inDir)
            red = red.replace('{OUT_DIR}', outDir)
            red = red.replace('{JOB_INDEX}',outDir.replace('/','_'))
            outf.write(red)

def dumpParams(params, dumpPath):
    with open(dumpPath, 'w') as df:
        data={}
        for param in params:
            data[param.getKey()] = param.getParam()
        json.dump(data, df)

def replaceAndSaveSetting(params, index):
    inputDir = os.getcwd()
    outputDir = result_dir +'/tests/num'+str(index)
    os.system("mkdir -p "+outputDir)
    
    replaceInFile(params, inputDir, outputDir, './script-template.tcl', join(outputDir, 'script.tcl'))
    replaceInFile(params, inputDir, outputDir, './vivado-run-template.sh', join(outputDir, 'run.sh'))  

    dumpParams(params, join(outputDir, 'params.json'));

    return outputDir


##########################################getting info from sys.argv
# alg = sys.argv[1] # KYBER
# accel_name  = sys.argv[2] # ntt_gs
# accel_id    = sys.argv[3] # id to be used in Aladdin (e.g., 16777218)
# param_n = int(sys.argv[4]) # 256 (e.g. KYBER)
# data_width = int(sys.argv[5]) #4 (in bytes)

result_dir = "results/"
###########################################DSE points


class NHS(SweepTask):
    
    params = [

        ParamHolder('PARAM_N', [256, 512]),
        ParamHolder('DOMAIN', ['MONTGOMERY']),
        ParamHolder('CLK', [10]),
       
            ]
    containerIndex = 0

    def prepareSetting(self):
        cont = True
        for index in range(len(NHS.params)):
            if(NHS.params[index].incCurser()):
                if(index >= len(NHS.params)-1):
                    cont = False
                    break
            else:
                break
        self.container = replaceAndSaveSetting(NHS.params, NHS.containerIndex)
        NHS.containerIndex += 1
        return cont

    def doThings(self):
        os.system("sh "+self.container+"/run.sh")
        print("done test: ", self.container)

if __name__ == '__main__':

    sweeper = Sweeper(NHS)
    sweeper.sweep()

    os.system("cp dse.py "   + result_dir)
