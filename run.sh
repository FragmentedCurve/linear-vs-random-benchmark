#!/bin/sh

set -e

BUFLEN=10
T0=0
T1=0

echo "Finding a buffer length for your CPU."

while [ $T0 -le 1 ]; do
	echo "Trying ${BUFLEN} ..."

	make clean benchmark BUFLEN=${BUFLEN} > /dev/null
	RESULTS=`./benchmark | tail -n 1`
	T0=`echo ${RESULTS} | awk '{print $1}'`
	T1=`echo ${RESULTS} | awk '{print $2}'`

	BUFLEN=$(( BUFLEN * 10 ))
done

echo
echo "SEARCH RESULTS"
echo
echo "Found BUFLEN=${BUFLEN} to be best. To build in the future on this machine, you may do," | fmt
echo
echo "    make BUFLEN=${BUFLEN}"
echo
echo "Then you may run the benchmark without going through the searching process by doing," | fmt
echo
echo "    ./benchmark"
echo
echo "BENCHMARK RESULTS"
echo
echo "Linear access took ${T0} seconds."
echo "Random access took ${T1} seconds."
