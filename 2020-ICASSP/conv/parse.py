# import xmltodict
import json
from collections import OrderedDict
import sys
import os


# def xml_to_json(f_name):
#     with open(f_name+'.xml', 'r') as file:
#         lines = file.read()

#     o = xmltodict.parse(lines)

#     with open(f_name+'.json', 'w') as outfile:
#         json.dump(o, outfile)

def read_util_stats(f_name, f_out):
    stat = OrderedDict()

    with open((f_name), mode='r') as statsFile:
        for line in statsFile:
            if not line.__contains__(":"):  # new dumped stats
                continue
            split = line.split(":")
            if (len(split) == 2): #don't parse unwanted numbers, e.g., Report Date
                try:
                    stat[split[0]] = float(split[1])
                except ValueError:
                    stat[split[0]] = (split[1]).replace(" ","") #remove white space
    # with open(f_out + '.json', 'w') as outfile:
    #     json.dump(stat, outfile)
    return stat

def read_power_stats(stat, f_in, f_out):
    # stat = OrderedDict()

    with open((f_in), mode='r') as statsFile:
        for line in statsFile:
            if line.__contains__("Dynamic (W)"):  # new dumped stats
                split = line.replace(" ","").split("|")
                stat[split[1]] = float(split[2])
                stat["Dynamic(mW)"] = float(split[2])*1000
                return

    # with open(f_out + '.json', 'a') as outfile:
    #     json.dump(stat, outfile)
    return stat


def read_latency(stat, f_in, f_out,rtl):
    # stat = OrderedDict()

    with open((f_in), mode='r') as statsFile:
        for line in statsFile:
            if (line.__contains__("min")) and (line.__contains__("max")):
                l = statsFile.next()
                l = statsFile.next()
                split = l.split("|")
                try:
                    # stat["RTL"]         = (split[1])
                    # stat["Status"]      = (split[2]).replace(" ","") #remove white space
                    # stat["Latency_Min"] = float(split[3])
                    # stat["Latency_Avg"] = float(split[4])
                    # stat["Latency_Max"] = float(split[5])

                    stat["Latency_Min"] = float(split[1])
                    stat["Latency_Max"] = float(split[2])
                    stat["Latency_Avg"] = float((stat["Latency_Min"] + stat["Latency_Max"])/2.0)

                except:
                    pass
                break



    with open(f_out + '.json', 'a') as outfile:
        json.dump(stat, outfile)
    return stat




def call_parse(dir, kernel):
    # xml_to_json("ntt_gs_export")
    stat = read_util_stats(dir+"/"+kernel + "_export.rpt", dir+"/"+kernel)

    read_power_stats(stat, dir+"/"+ kernel + "_power_routed.rpt", dir+"/"+kernel)

    read_latency(stat, dir+"/"+ kernel + "_csynth.rpt", dir+"/"+kernel, "min |  max")

    # with open(kernel + '.json', 'w') as outfile:
    #     json.dump(stat, outfile)
    return stat

# call_parse("/home/hamid/phd/ucsd-uci/","ntt_gs")




