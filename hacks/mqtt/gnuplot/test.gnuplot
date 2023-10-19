file='< tail -n 10240 test'
set term wxt title 'Test de capture owntech udp/ipv6'
set xrange [1:10240]
set yrange [0:4096]
set y2range [-2048:2048]
set ytics nomirror
set y2tics -2048,1024

set xlabel '10240 samples of 976µs (last 10s)'
set ylabel 'mV'
set y2label 'mV-1650 (offset)'

plot\
    file using 1 with lines title 'vH',\
    '' using ($2-1650) with lines title 'iH' axis x1y2,\
    '' using 3 with lines title 'vR',\
    '' using ($4-1650) with lines title 'iR' axis x1y2,\
    '' using 5 with lines title 'vS',\
    '' using ($6-1650) with lines title 'iS' axis x1y2,\
    '' using 7 with lines title 'vT',\
    '' using ($8-1650) with lines title 'iT' axis x1y2
while (1) {
    replot
    pause 0.04
}

