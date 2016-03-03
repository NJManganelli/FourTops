R_meas = 0.022
R_meas_up = 0.031
R_meas_down = 0.013

Nttll = 1570000
Nttcc = 872
Nttbb = 942

Nttllcc = Nttll + Nttcc
#alpha is the factor for Nttllcc and beta for Nttbb
alpha = (  (Nttllcc + Nttbb) / (Nttllcc*(1+R_meas)) )
beta = (alpha * R_meas * Nttllcc) / Nttbb

alpha_up = (  (Nttllcc + Nttbb) / (Nttllcc*(1+R_meas_up)) )
beta_up = (alpha_up * R_meas_up * Nttllcc) / Nttbb

alpha_down = (  (Nttllcc + Nttbb) / (Nttllcc*(1+R_meas_down)) )
beta_down = (alpha_down * R_meas_down * Nttllcc) / Nttbb

print 'alpha = '+str(alpha)+'  beta = '+str(beta)+'\n'
print 'alpha_up = '+str(alpha_up)+'  beta_up = '+str(beta_up)+'\n'
print 'alpha_down = '+str(alpha_down)+'  beta_down = '+str(beta_down)+'\n'

