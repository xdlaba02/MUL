#!/usr/bin/gnuplot --persist
reset;

set terminal pdf size 15cm, 15/5*3cm;

set yrange [0:8]

set style data histogram
set style histogram cluster gap 1
set style fill solid border -1

set xtics font ", 8"

set ylabel 'bitrate [bpp]';
set key inside top right;

set output "bitrate.pdf";
plot \
'data.dat' u ($3*8/$2):xtic(1) t 'JP2', \
        '' u ($4*8/$2)         t 'MUL',    \
        '' u ($5*8/$2)         t 'PNG',   \
        '' u ($6*8/$2)         t 'WEBP', \
;

set yrange [0:4]
set ylabel 'čas [s]';

set output "compress_time.pdf";
plot \
'data.dat' u ($7):xtic(1) t 'JP2', \
        '' u ($8)         t 'MUL',    \
        '' u ($9)         t 'PNG',   \
        '' u ($10)        t 'WEBP', \
;

set yrange [0:1]

set output "decompress_time.pdf";
plot \
'data.dat' u ($11):xtic(1) t 'JP2', \
        '' u ($12)         t 'MUL',    \
        '' u ($13)         t 'PNG',   \
        '' u ($14)         t 'WEBP', \
;
