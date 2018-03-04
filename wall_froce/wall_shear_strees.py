
# coding: utf-8

# In[95]:


import pandas as pd
import numpy as np
#import sys
import os

path = os.listdir(os.getcwd())
mu = 3.571428571428571e-04

y0 = -1.0
y1 = -0.997191155368
y2 = -0.994382310736
y3 = -0.991573466104


dy1 = y1 - y0
dy2 = y2 - y0
dy3 = y3 - y0

B = np.array([[1.0,1.0,1.0,1.0 ],
              [0.0,dy1,dy2,dy3],
              [0.0,dy1**2/2.0,dy2**2/2.0,dy3**2/2.0],
              [0.0,dy1**3/6.0,dy2**3/6.0,dy3**3/6.0]])

B = np.mat(B)

b = np.array([0.0,1.0,0.0,0.0])

x = np.linalg.solve(B, b)

for filename in path:
    
    if (filename[-20:] == '_field_structure.dat'):
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
        A.drop(['cell', 'order_cell', 'Q_CRITERIA', 'vort_x', 'vort_y', 'vort_z',
               'PRESSURE', 'n_repeat'], axis=1,inplace=True) 
        A[['x','y','z']] = A[['x','y','z']].round(precision-2)
        A = A.set_index(['x','y','z'])
        
        
        # In[116]:
        
        
        y0 = A.index.levels[1][0]
        y1 = A.index.levels[1][1]
        y2 = A.index.levels[1][2]
        y3 = A.index.levels[1][3]
        
        U0 = A.xs(y0,axis=0,level=1)
        U1 = A.xs(y1,axis=0,level=1)
        U2 = A.xs(y2,axis=0,level=1)
        U3 = A.xs(y3,axis=0,level=1)

        dU = x[0]*U0 + x[1]*U1 + x[2]*U2 + x[3]*U3 
 
        dU *= mu
        (j,i) = dU.unstack().shape
        dU['y'] = -1

        
 
        outputfile = filename[:-4]+'_wallstress.dat'
        data_str = dU.to_csv(sep=' ',header=False,index=True)
        print('Writing to '+ outputfile)
        output = open(outputfile,'w')
        header = 'VARIABLES = x z f y'
        header += '\n'
        output.write(header)
        header = 'ZONE F = POINT I = '+  str(i) +' J = ' + str(j) + ' \n'
        output.write(header)
        output.write(data_str)
        output.close()
print('Job Done!')
        
        


