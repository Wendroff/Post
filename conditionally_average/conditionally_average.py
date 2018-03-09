
# coding: utf-8
# author----Wendroff

import pandas as pd
import numpy as np
import sys
#import os
#import matplotlib.pyplot as plt
#import matplotlib as mpl
import datetime

nowTime=datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
logfile = open('log'+nowTime+'.dat','w')

file_aver = 'sol_tec_aver_averageXZ.dat'
logfile.write('file_aver = sol_tec_aver_averageXZ.dat')
folder = 'sol_structure\\'
folder2 = 'sol_structure_dat\\'
logfile.write('folder = sol_structure\\')
#field_file =  'sol_structure\\sol_tec_3290000_field_structure.dat'
#read averaged data
with open(file_aver) as test:
    a = test.readline()
index_equal = a.find('=')
a = a[index_equal+1:]
a = a.split()
test.close()
del index_equal
aver_data = pd.read_table(file_aver,skiprows=[0, 1],names=a,sep='\s+')
y = aver_data['y'].values
U_aver = aver_data['U_aver'].values
V_aver = aver_data['V_aver'].values
W_aver = aver_data['W_aver'].values

y_range = 40
z_half_range = 20
logfile.write('y_range = '+str(y_range)+'\n')
logfile.write('z_half_range = '+str(z_half_range)+'\n')

Q_cri = 1.0
j = 22
logfile.write('Q_cri = '+str(Q_cri)+'\n')
logfile.write('j = '+str(j)+'\n')

print('detecting plane y+ = ',(y[j]+1)*180)
logfile.write('detecting plane y+ = '+str((y[j]+1)*180)+'\n')
U_Con_aver = np.zeros((y_range,2*z_half_range+1)) #Condition averageed plane (y,z), the middle index of z is 20
V_Con_aver = np.zeros((y_range,2*z_half_range+1))
W_Con_aver = np.zeros((y_range,2*z_half_range+1))
Q_Con_aver = np.zeros((y_range,2*z_half_range+1))
num_Con_aver = 0 #counting the vortice averaged
logfile.flush()
#parameters in simulation: frequence, timestep length etc.
dt = 1.5e-4
omega = 1.3963
pi = 3.141592653589793
T  = 2*pi/omega
phase_wanted = int(sys.argv[1])

logfile.write('dt = '+ str(dt))
logfile.write('omega = '+ str(omega))
logfile.write('T = '+ str(dt))
logfile.write('phase: '+ str(phase_wanted)+'/16')
#logfile.write('dt = '+ str(dt))

for timestep in range(5973000,6273500,500):
    phy_time = timestep*dt
    phase = np.modf(phy_time/T)[0]
    phase_index = int(np.modf(phase*16.0)[1])
    if phase_index == phase_wanted:
        field_file = folder + 'sol_tec_' + str(timestep) + '_field_structure.dat'
        filename   = folder2 + 'sol_tec_' + str(timestep) + '_structure.dat'
        #read instantanous data
        try:
            with open(filename) as test:
                print('Reading',filename)
                a = test.readline()
            index_equal = a.find('=')
            a = a[index_equal+1:]
            a = a.split()
            test.close()
            del index_equal
            inst_data = pd.read_table(filename,skiprows=[0, 1],names=a,sep='\s+')
            
            
            x = np.reshape(inst_data['x'].values,(189,125,161))[:,0,0]
            z = np.reshape(inst_data['z'].values,(189,125,161))[0,0,:]
            U = np.reshape(inst_data['U'].values,(189,125,161))
            V = np.reshape(inst_data['V'].values,(189,125,161))
            W = np.reshape(inst_data['W'].values,(189,125,161))
            
            
            #read field data
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
            logfile.write(str(e))
            logfile.write('\n')
            logfile.flush()
        else:
            #find the vortex cores
            
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
            
                #plt.figure(figsize=(20,10))
                #plt.plot(z,Q[i,j,:])
                #plt.scatter(z[core_index],core_Q)
                #plt.show()
            
                for k_core in core_index:
                    if vort_x[i,j,k_core]> 0 :
                        if k_core<z_half_range :
                            U_tpm = np.column_stack((U[i,:y_range,(k_core-z_half_range-1):-1],U[i,:y_range,:(k_core+z_half_range+1)]))
                            V_tpm = np.column_stack((V[i,:y_range,(k_core-z_half_range-1):-1],V[i,:y_range,:(k_core+z_half_range+1)]))
                            W_tpm = np.column_stack((W[i,:y_range,(k_core-z_half_range-1):-1],W[i,:y_range,:(k_core+z_half_range+1)]))
                            Q_tpm = np.column_stack((Q[i,:y_range,(k_core-z_half_range-1):-1],Q[i,:y_range,:(k_core+z_half_range+1)]))
                        elif (k_core+z_half_range+1)>len(z):
                            U_tpm = np.column_stack((U[i,:y_range,(k_core-z_half_range):],U[i,:y_range,1:(k_core+z_half_range+2-len(z))]))
                            V_tpm = np.column_stack((V[i,:y_range,(k_core-z_half_range):],V[i,:y_range,1:(k_core+z_half_range+2-len(z))]))
                            W_tpm = np.column_stack((W[i,:y_range,(k_core-z_half_range):],W[i,:y_range,1:(k_core+z_half_range+2-len(z))]))
                            Q_tpm = np.column_stack((Q[i,:y_range,(k_core-z_half_range):],Q[i,:y_range,1:(k_core+z_half_range+2-len(z))]))
                        else:
                            U_tpm = U[i,:y_range,(k_core-z_half_range):(k_core+z_half_range+1)]
                            V_tpm = V[i,:y_range,(k_core-z_half_range):(k_core+z_half_range+1)]
                            W_tpm = W[i,:y_range,(k_core-z_half_range):(k_core+z_half_range+1)]
                            Q_tpm = Q[i,:y_range,(k_core-z_half_range):(k_core+z_half_range+1)]
                        #print(test.shape)
                        U_Con_aver += U_tpm
                        V_Con_aver += V_tpm
                        W_Con_aver += W_tpm
                        Q_Con_aver += Q_tpm
                        num_Con_aver += 1
        

U_Con_aver /= num_Con_aver
V_Con_aver /= num_Con_aver
W_Con_aver /= num_Con_aver
Q_Con_aver /= num_Con_aver
u_Con_aver = U_Con_aver - np.reshape(U_aver[:y_range],(40,1))



z_output = z[:2*z_half_range+1] - z[z_half_range]
y_output = y[:y_range] + 1.0

output = pd.DataFrame()

V_output = pd.DataFrame(V_Con_aver,columns=z_output,index=y_output)
V_output.columns.name='z'
V_output.index.name='y'
output['V'] = V_output.stack()

W_output = pd.DataFrame(W_Con_aver,columns=z_output,index=y_output)
W_output.columns.name='z'
W_output.index.name='y'
output['W'] = W_output.stack()

#U_output = pd.DataFrame(U_Con_aver,columns=z_output,index=y_output)
#U_output.columns.name='z'
#U_output.index.name='y'
#output['U'] = U_output.stack()

u_output = pd.DataFrame(u_Con_aver,columns=z_output,index=y_output)
u_output.columns.name='z'
u_output.index.name='y'
output['u'] = u_output.stack()

Q_output = pd.DataFrame(Q_Con_aver,columns=z_output,index=y_output)
Q_output.columns.name='z'
Q_output.index.name='y'
output['Q'] = Q_output.stack()
#output


outputfile = 'Conditional_average_'+str(phase_wanted)+'_16.dat'
data_str = output.to_csv(sep=' ',header=False,index=True)
print('Writing to '+ outputfile)
with open(outputfile,'w') as out:
    header = 'VARIABLES = y z '
    for col_name in output.columns:
        header += col_name
        header += '      '
    header += '\n'
    out.write(header)
    header = 'ZONE F = POINT I = '+ str(2*z_half_range+1)  +' J = ' + str(y_range) + ' \n'
    out.write(header)
    out.write(data_str)

logfile.close()