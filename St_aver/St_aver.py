# -*- coding: utf-8 -*-
"""
Created on Wed Dec  6 19:47:17 2017

@author: Wendroff
"""

import pandas as pd
import sys


if (len(sys.argv) < 2) :
    print('Test mode \n')
    filename   = '..\\..\\Data_Post\\input\\sol_tec_7300000_aver_structure.dat'
    outputfile = '..\\..\\Data_Post\\St_aver_output\\output.dat'
else:
    filename   = sys.argv[1]
    outputfile = 'output.dat'
precision = 12 # The precision in the original file
test = open(filename)
a = test.readline()
index_equal = a.find('=')
a = a[index_equal+1:]
a = a.split()
test.close()
del index_equal

A = pd.read_table(filename,skiprows=[0, 1],names=a,sep='\s+')
del A['x'],A['cell'],A['order_cell'],A['n_repeat']
A['uu'] = A['uu'] + A['U_aver'] **2
A['vv'] = A['vv'] + A['V_aver'] **2
A['ww'] = A['ww'] + A['W_aver'] **2
A['uv'] = A['uv'] + A['U_aver'] * A['V_aver']
A['uw'] = A['uw'] + A['U_aver'] * A['W_aver']
A['vw'] = A['vw'] + A['V_aver'] * A['W_aver']

grouped = A.groupby(['y','z'])
size_= grouped.size()

#Since the numerical residual, sometimes the y z of a same mesh point are different
#reducing the precision can solve this problem
while (size_.min() < size_.max()):
    #print([size_.min(),' < ',size_.max()])
    precision -= 1
    A[['y','z']] = A[['y','z']].round(precision)
    grouped = A.groupby(['y','z'])
    size_= grouped.size()
(j,i) = size_.unstack().shape
aver = grouped.mean()
aver.reset_index(inplace=True)
aver['uu'] = aver['uu'] - aver['U_aver'] **2
aver['vv'] = aver['vv'] - aver['V_aver'] **2
aver['ww'] = aver['ww'] - aver['W_aver'] **2
aver['uv'] = aver['uv'] - aver['U_aver'] * aver['V_aver']
aver['uw'] = aver['uw'] - aver['U_aver'] * aver['W_aver']
aver['vw'] = aver['vw'] - aver['V_aver'] * aver['W_aver']
#size_.reset_index(inplace=True)
#size_.to_csv('size.csv')
data_str = aver.to_csv(sep=' ',header=False,index=False)
output = open(outputfile,'w')
header = 'VARIABLES = '
for name in aver.columns:
    header += ' '
    header += name
    header += ' '
header += '\n'
output.write(header)
header = 'ZONE F = POINT I = '+  str(i) +' J = ' + str(j) + ' \n'
output.write(header)
output.write(data_str)
output.close()
