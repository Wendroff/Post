# -*- coding: utf-8 -*-
"""
Created on Sat Mar 10 19:26:44 2018

@author: Wendroff_surface
"""

import pandas as pd
import numpy as np
import sys

folder = 'sol_structure\\'
Q_cri = 1.0
j = 22
print('detecting plane y+ = ',(y[j]+1)*180)

dt = 1.5e-4
omega = 1.3963
pi = 3.141592653589793
T  = 2*pi/omega

num_vort = np.zeros(16,dtype=int)

for timestep in range(5973000,6273500,500):
    phy_time = timestep*dt
    phase = np.modf(phy_time/T)[0]
    phase_index = int(np.modf(phase*16.0)[1])
    try:
        field_file = folder + 'sol_tec_' + str(timestep) + '_field_structure.dat'
        with open(field_file) as test:
            print('Reading',field_file)
            a = test.readline()
        index_equal = a.find('=')
        a = a[index_equal+1:]
        a = a.split()
        test.close()
        del index_equal
        field_data = pd.read_table(field_file,skiprows=[0, 1],names=a,sep='\s+')
        
        Q = np.reshape(field_data['Q_CRITERIA'].values,(189,125,161))
        vort_x = np.reshape(field_data['vort_x'].values,(189,125,161))
    except OSError as e:
        print(e)
    else:
        for i in range(len(x)):
            core_index = []
            core_Q     = []
            #if there is a half vortex at the first index
            first_is_half = False
            if (Q[i,j,0]>Q_cri) and (Q[i,j,-1]>Q_cri):
                first_is_half = True
                core_Q.append(Q[i,j,0])
                core_index.append(0)
            for k in range(z.shape[0]):
                if (Q[i,j,k]>Q_cri):
                    if (Q[i,j,k-1]<Q_cri):
                        core_Q.append(Q[i,j,k])
                        core_index.append(k)
                    else:
                        if (Q[i,j,k]>core_Q[-1]):
                            core_Q[-1]     = Q[i,j,k]
                            core_index[-1] = k
            if first_is_half:
                if core_Q[-1]>core_Q[0]:
                    core_Q = core_Q[1:]
                    core_index = core_index[1:]
                else:
                    core_Q = core_Q[:-1]
                    core_index = core_index[:-1]
            num_vort[phase_index] += len(core_index)
output = pd.DataFrame(num_vort,columns=range(16))