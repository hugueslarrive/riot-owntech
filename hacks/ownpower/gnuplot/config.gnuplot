file='< grep -v "#" test | tail -n 244'
set term wxt title 'Test de capture série ownpower'
set datafile commentschars '#'

set xrange [1:244]
set xtics ("0" 0, "1" 24.4, "2" 48.8, "3" 73.2, "4" 97.6, "5" 122, "6" 146.4, "7" 170.8, "8" 195.2, "9" 219.6, "10" 244)

set yrange [0:120]
set ytics 0,10

set y2range [-10:10]
set ytics nomirror
set y2tics -10,2

set xlabel 'Seconds'
set ylabel 'Volts | °C | % | W'
set y2label 'Amps'

plot\
    file using ($1*0.031) with lines title 'vH (V)',\
    '' using (($2-2048)*0.01) with lines title 'iH (A)' axis x1y2,\
    '' using ($3*0.016) with lines title 'vL (V)',\
    '' using (($4-2048)*0.01) with lines title 'iL (A)' axis x1y2,\
    '' using ($5*0.1) with lines title 'Temp (°C)',\
    '' using ($6/320*100) with lines title 'Duty-Cycle (%)',\
    '' using (($1*0.031)*(-(($2-2048)*0.01))) with lines title 'Input power (W)',\
    '' using (($3*0.016)*((($4-2048)*0.01))) with lines title 'Output power (W)',\
    '' using ((($3*0.016)*((($4-2048)*0.01)))/(($1*0.031)*(-(($2-2048)*0.01)))*100) with lines title 'Yeld (%)'
while (1) {
    replot
    pause 0.04
}

