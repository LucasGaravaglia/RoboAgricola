#!/bin/bash

head -n16 /dev/ttyACM0 > temp
grep '$GPGLL*' temp > temp3
var=$(cat temp3)

temp2=${var:7:26}
lat1=${temp2:0:4}
lat2=${temp2:5:5}
latf=$lat1$lat2

long1=${temp2:13:5}
long2=${temp2:19:5}
longf=$long1$long2

echo $latf > coordenadas
echo $longf >> coordenadas

rm temp
rm temp3
