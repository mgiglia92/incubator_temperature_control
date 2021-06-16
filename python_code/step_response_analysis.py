import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('incubator_60c_step.csv', delimiter=',', names=True)

# Get data of only when the heater is on
time = data['time'][data['H0']>1]
temp_heater_on = data['T0'][data['H0']>1]
Tinc = data['T3'][data['H0']>1]
Havg = np.mean(data['H0'][data['H0'] > 1])

# N=100
# ma = np.convolve(data['T0'], np.ones(N)/N, mode='valid')
# dT0 = np.gradient(ma)
# dT0ma = np.convolve(dT0, np.ones(N)/N, mode='valid')

# Guess first order model
K = Havg - Tinc
tau = 400
temptheory = K*(1-np.exp(-1*time/tau)) + Tinc




f, (ax1, ax2, ax3) = plt.subplots(3,1, sharex=True)
ax1.plot(time, temp_heater_on,  'r.', markevery=10, label='Sensor Data')
ax1.plot(time, Tinc,  'r.', markevery=10, label='Sensor Data')
# ax1.plot(time, temptheory,  'b.', markevery=10, label="First Order: Tau: {}s, K: {:.1f}".format(tau, K))
ax1.legend(loc="lower right")

# ax1.plot(data['time'][:-1*N+1], ma,  'b.', markevery=10)
# ax2.plot(timeheat, heat, 'c.', markevery=10)
# ax3.plot(data['time'][:-1*N+1], dT0, 'b.', markevery=10)
# ax3.plot(data['time'][:-2*N+2], dT0ma, 'g.', markevery=10)


plt.show()

