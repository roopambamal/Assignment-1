#gcc main.c crackme.o -o main

for ((i=1;i<=$#;i++)); 
do
	if [ ${!i} = "-make" ] 
	then
		mpicc mainMPI.c crackme.o -o mainMPI

	# elif [ ${!i} = "-premake" ];
	# then 
	# 	gcc -S crackme.c -o crackme.s
	# 	gcc -c crackme.s -o crackme.o
	# 	rm crackme.s

	elif [ ${!i} = "-clean" ];
	then 
		rm mainMPI
	fi

done;