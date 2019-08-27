import os
import subprocess
import collections
import SampleList

def get_printable_size(byte_size):
  """
  A bit is the smallest unit, it's either 0 or 1
  1 byte = 1 octet = 8 bits
  1 kB = 1 kilobyte = 1000 bytes = 10^3 bytes
  1 KiB = 1 kibibyte = 1024 bytes = 2^10 bytes
  1 KB = 1 kibibyte OR kilobyte ~= 1024 bytes ~= 2^10 bytes (it usually means 1024 bytes but sometimes it's 1000... ask the sysadmin ;) )
  1 kb = 1 kilobits = 1000 bits (this notation should not be used, as it is very confusing)
  1 ko = 1 kilooctet = 1000 octets = 1000 bytes = 1 kB
  Also Kb seems to be a mix of KB and kb, again it depends on context.
  In linux, a byte (B) is composed by a sequence of bits (b). One byte has 256 possible values.
  More info : http://www.linfo.org/byte.html
  """
  # BASE_SIZE = 1024.00
  BASE_SIZE = 1000.00
  MEASURE = ["B", "KB", "MB", "GB", "TB", "PB"]
  # MEASURE = ["GB"]

  def _fix_size(size, size_index):
    if not size:
        return "0"
    elif size_index == 0:
        return str(size)
    else:
        return "{:.3f}".format(size)

  current_size = byte_size
  size_index = 0

  while current_size >= BASE_SIZE and len(MEASURE) != size_index:
    current_size = current_size / BASE_SIZE
    size_index = size_index + 1

  size = _fix_size(current_size, size_index)
  measure = MEASURE[size_index]
  return size + measure

# REMEMBER TO SOURCE CRAB3 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
# https://twiki.cern.ch/twiki/bin/view/CMS/DBS3APIInstructions
# https://github.com/dmwm/DBS/blob/master/Client/src/python/dbs/apis/dbsClient.py
# 
from dbs.apis.dbsClient import DbsApi 
# dir(DbsApi)  #list all available APIs
# help(DbsApi.listBlocks)

dbs = DbsApi('https://cmsweb.cern.ch/dbs/prod/global/DBSReader')

#
# Make the samples dictionary. Grab from SampleDict.py
#
samples = []
samples += SampleList.samplesData16
samples += SampleList.samplesMC16

totalrawsize = 0
nsamples     = 0

for dataset in samples:

  print "SampleName = ", dataset
  filesummary=dbs.listFileSummaries(dataset=dataset, validFileOnly=0)  # if require all files, validFileOnly=0 
  # print filesummary
  nfiles  = filesummary[0]["num_file"]
  nevents = filesummary[0]["num_event"]
  rawsize = filesummary[0]["file_size"]
  totalrawsize = totalrawsize+rawsize
  size    = get_printable_size(rawsize)
  print  "nfiles =", nfiles, " nevents =", nevents, " size = ", size, "\n"
  nsamples += 1
  
print "NSamples   = ", nsamples
print "Total Size = ", totalrawsize, "Bytes"
print "           = ", get_printable_size(totalrawsize)


