#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Wed Dec  6 19:47:17 2017

@author: Wendroff
"""

import pandas as pd
import numpy as np
import sys


if (len(sys.argv) < 2) :
    print('Test mode \n')
    filename   = '..\\..\\Data_Post\\input\\sol_tec_9684500_aver_structure.dat'
    outputfile = '..\\..\\Data_Post\\St_aver_output\\output.dat'
else:
    filename   = sys.argv[1]
    outputfile = (sys.argv[1][:-4]+'aver.dat')
precision = 12 # The precision in the original file
test = open(filename)
a = test.readline()
index_equal = a.find('=')
a = a[index_equal+1:]
a = a.split()
test.close()
del index_equal
print("Reading "+ filename)
A = pd.read_table(filename,skiprows=[0, 1],names=a,sep='\s+')
#_aver.dat file or not?
flag_aver = (filename[-19:] == '_aver_structure.dat')
print('Averaged file or not?')
print(flag_aver)
del A['x'],A['cell'],A['order_cell'],A['n_repeat']
if (flag_aver):
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
print('Averaging Data ...')
while (size_.min() < size_.max()):
    #print([size_.min(),' < ',size_.max()])
    precision -= 1
    A[['y','z']] = A[['y','z']].round(precision)
    grouped = A.groupby(['y','z'])
    size_= grouped.size()
(j,i) = size_.unstack().shape
aver = grouped.mean()
#aver.reset_index(inplace=True)
if (flag_aver):
    aver['uu'] = aver['uu'] - aver['U_aver'] **2
    aver['vv'] = aver['vv'] - aver['V_aver'] **2
    aver['ww'] = aver['ww'] - aver['W_aver'] **2
    aver['uv'] = aver['uv'] - aver['U_aver'] * aver['V_aver']
    aver['uw'] = aver['uw'] - aver['U_aver'] * aver['W_aver']
    aver['vw'] = aver['vw'] - aver['V_aver'] * aver['W_aver']
#size_.reset_index(inplace=True)
#size_.to_csv('size.csv')
U_aver = aver['U_aver'].unstack()
V_aver = aver['V_aver'].unstack()
W_aver = aver['W_aver'].unstack()
#calculate the derivative
dUdy = U_aver.copy()
dVdy = U_aver.copy()
dWdy = U_aver.copy()
dUdz = U_aver.copy()
dVdz = U_aver.copy()
dWdz = U_aver.copy()

(Ny,Nz) = dUdz.shape
y = U_aver.index.values
z = U_aver.columns.values
dz = (z[1:]-z[:-1]).mean()
dUdz.values[:,0] = (U_aver.values[:,1] - U_aver.values[:,-1])/dz/2
dVdz.values[:,0] = (V_aver.values[:,1] - V_aver.values[:,-1])/dz/2
dWdz.values[:,0] = (W_aver.values[:,1] - W_aver.values[:,-1])/dz/2

dUdz.values[:,1:-1] = (U_aver.values[:,2:] - U_aver.values[:,:-2])/dz/2
dVdz.values[:,1:-1] = (V_aver.values[:,2:] - V_aver.values[:,:-2])/dz/2
dWdz.values[:,1:-1] = (W_aver.values[:,2:] - W_aver.values[:,:-2])/dz/2

dUdz.values[:,-1] = (U_aver.values[:,0] - U_aver.values[:,-2])/dz/2
dVdz.values[:,-1] = (V_aver.values[:,0] - V_aver.values[:,-2])/dz/2
dWdz.values[:,-1] = (W_aver.values[:,0] - W_aver.values[:,-2])/dz/2

for j in range(1,Ny-1):
    dy1 = y[j+1]-y[j]
    dy2 = y[j-1]-y[j]
    b0  = -(dy1 + dy2)/(dy1*dy2)
    b1  = -dy2/(dy1*(dy1 - dy2))
    b2  = dy1/(- dy2**2 + dy1*dy2)
    dUdy.values[j,:] = b0*U_aver.values[j,:] + b1*U_aver.values[j+1,:] + b2*U_aver.values[j-1,:]
    dVdy.values[j,:] = b0*V_aver.values[j,:] + b1*V_aver.values[j+1,:] + b2*V_aver.values[j-1,:]
    dWdy.values[j,:] = b0*W_aver.values[j,:] + b1*W_aver.values[j+1,:] + b2*W_aver.values[j-1,:]
dUdy.values[0,:] = (U_aver.values[1,:] - U_aver.values[0,:])/(y[1]-y[0])
dVdy.values[0,:] = (V_aver.values[1,:] - V_aver.values[0,:])/(y[1]-y[0])
dWdy.values[0,:] = (W_aver.values[1,:] - W_aver.values[0,:])/(y[1]-y[0])
dUdy.values[-1,:] = (U_aver.values[-1,:] - U_aver.values[-2,:])/(y[-1]-y[-2])
dVdy.values[-1,:] = (V_aver.values[-1,:] - V_aver.values[-2,:])/(y[-1]-y[-2])
dWdy.values[-1,:] = (W_aver.values[-1,:] - W_aver.values[-2,:])/(y[-1]-y[-2])
#add the derivative into the original dataframe
aver['dUdy'] = dUdy.stack()
aver['dVdy'] = dVdy.stack()
aver['dWdy'] = dWdy.stack()
aver['dUdz'] = dUdz.stack()
aver['dVdz'] = dVdz.stack()
aver['dWdz'] = dWdz.stack()

aver['k'] = (aver['uu'] + aver['vv'] + aver['ww'])/2
aver['Production'] = -(aver['uv']*aver['dUdy']
                     + aver['vv']*aver['dVdy']
                     + aver['vw']*aver['dWdy']
                     + aver['uw']*aver['dUdz']
                     + aver['vw']*aver['dVdz']
                     + aver['ww']*aver['dWdz'])

aver_out = aver.reset_index(inplace=False)
data_str = aver_out.to_csv(sep=' ',header=False,index=False)
print('Writing to '+ outputfile)
output = open(outputfile,'w')
header = 'VARIABLES = '
for name in aver_out.columns:
    header += ' '
    header += name
    header += ' '
header += '\n'
output.write(header)
header = 'ZONE F = POINT I = '+  str(i) +' J = ' + str(j) + ' \n'
output.write(header)
output.write(data_str)
output.close()
print('Job Done!')