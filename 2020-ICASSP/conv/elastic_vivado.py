#Pushes the data into elastic kibana!!!

import fnmatch
import sys
import os
import os.path
import json
import urllib2
import re
from glob import glob
import gzip
from decimal import Decimal
from collections import OrderedDict
import math

from parse import call_parse

from elasticsearch import Elasticsearch
es = Elasticsearch([{'host': 'localhost', 'port': 9200}])


def push_elastic(argv):
    test_dir=sys.argv[1] #'dse_bliss_b_ntt_cache'
    algorithm= sys.argv[2]#'test-dma'
    kernel= argv[3]#'ntt_gs'
    # index = argv[4] #"vivado"


    for test in glob(test_dir+"/*"):
        # print test
        try:
            if 'num' not in test:
                print("invalid folder/file" + test)
                continue
            subdir = test+"/summary-"+algorithm+"-"+kernel+"/"



            # with open(subdir+kernel+".json", mode='r') as inputjson:
            #     data = json.load(inputjson)

            data = {}
            with open(test+"/params.json", mode='r') as inputjson:
                data = json.load(inputjson)

            data.update(call_parse(subdir, kernel))  # extract results
            data["kernel"]=kernel
            data["Running"] = test
            data["PointNUM"] = int (data['Running'].split('/')[-1][3:])  #data["Point_NUM"]=100

            # data['version'] = version
            # data['side'] = side
            data['algorithm'] = algorithm

            #power and performance calculation
            data['ExeTime(us)PostImpl'] = "{0:.4f}".format(float(data['Latency_Avg']) * float(data["CP achieved post-implementation"]) / math.pow(10, 3))
            # data['ExeTime(us)'] = "{0:.4f}".format(float(data['Latency_Avg']) * float(data["CLK"]) / math.pow(10,3))
            data['ExeTime(us)'] = data['ExeTime(us)PostImpl']
            data["CP"] = float(data["CP achieved post-implementation"])

            data['Energy(uJ)']   = "{0:.4f}".format(float(data['Dynamic(W)']) * float(data['ExeTime(us)']))

            data["Dynamic(mW)"] = float(data['Dynamic(W)']) * 1000
            # data["Energy(nJ)"] = "{0:.4f}".format(data["ExeTime(us)"] * data["Dynamic(mW)"] )
            # data["ED"]   = float("{0:.4f}".format(float(data["ExeTime(us)"]) * float(data["Energy(nJ)"])))
            # data["EDD"]  = float("{0:.4f}".format(float(data["ExeTime(us)"]) * float(data["ED"])))
            # print (data)
            #use 'id' to update the previous one and not overwriting other points
            res = es.index(index="icassp3", id=algorithm+data['Running'], body=data)
            print(res['result'])#,data['Running'])
#            req = urllib2.Request('http://localhost:9200/poly/'+kernel)
#            req.add_header('Content-Type', 'application/json')
#            response = urllib2.urlopen(req, json.dumps(data))
        except Exception as e:
            print(e, test)


# sys.argv = ['elastic_vivado.py', '/home/hamid/phd/papers/pqc/2020-ICASSP/conv/results-small-issue-montgomery/tests', 'poly-mult', 'conv']
print(sys.argv)
if len(sys.argv)> 3:
    push_elastic(sys.argv)
else:
    print ("error; python elastic_edp.py [directory] [algorithm] [version] [kernel] [index]")
    print ("ex: pythonelastic_vivado.py test_dma_load_store/store_kernel/tests/ test-dma-vivado store_kernel vivado")
    exit()
