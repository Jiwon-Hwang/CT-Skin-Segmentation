from __future__ import print_function
import os
import tempfile

import pydicom
from pydicom.data import get_testdata_files

print(__doc__)

###############################################################################
# Anonymize a single file
###############################################################################
INPUT = '190212_Patient3b_CT'
OUTPUT = '190212_Patient3b_CT_convert'

def search(dirname):
    files = os.listdir(dirname)

    for file in files:
        #filename = get_testdata_files('MR_small.dcm')[0]
        filepath = os.path.join(dirname, file)

        if os.path.isdir(filepath):
            print(filepath)
            search(filepath)
        else:
            dataset = pydicom.dcmread(filepath)

            dataset.data_element('PatientName').value = '1';
            dataset.data_element('SeriesInstanceUID').value = '100';
            dataset.data_element('StudyInstanceUID').value = '1000';
            dataset.data_element('StudyID').value = '100000';
            output_filename = file

            filepath_split = filepath.split('\\')
            foldername = filepath_split[-2]
            folderpath = os.path.join(OUTPUT, foldername)
            
            if not os.path.exists(folderpath):
                os.makedirs(folderpath)
            outpath = os.path.join('.',folderpath, output_filename)

            print(outpath)

            dataset.save_as(outpath)

    #output_filename = tempfile.NamedTemporaryFile().name
    #dataset.save_as(output_filename)

search(INPUT)