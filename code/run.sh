#!/bin/bash -l

if [ $# -lt 4 ]
  then
    echo "Usage: ./run.sh <nb_procs> <num_hosts> <executable name> <sizeChar*>"
    exit
fi

sh generate_hosts.sh $2

mpirun -np $1 -hostfile hostfile $3 $4
