R_meas = 0.022
R_meas_up = 0.031
R_meas_down = 0.013

Nttll = 100
Nttbb = 10

#alpha is the factor for Nttll and beta for Nttbb
alpha = (  (Nttll + Nttbb) / (Nttll*(1+R_meas)) )
beta = (alpha * R_meas * Nttll) / Nttbb

alpha_up = (  (Nttll + Nttbb) / (Nttll*(1+R_meas_up)) )
beta_up = (alpha_up * R_meas_up * Nttll) / Nttbb

alpha_down = (  (Nttll + Nttbb) / (Nttll*(1+R_meas_down)) )
beta_down = (alpha_down * R_meas_down * Nttll) / Nttbb

print 'alpha = '+str(alpha)+'  beta = '+str(beta)+'\n'
print 'alpha_up = '+str(alpha_up)+'  beta_up = '+str(beta_up)+'\n'
print 'alpha_down = '+str(alpha_down)+'  beta_down = '+str(beta_down)+'\n'