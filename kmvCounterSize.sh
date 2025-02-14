k=32
DATASET_FOLDER=dataset/data
RESULT_FOLDER=results/counter-size
EXPERIMENT_TYPE=counter-quality

mkdir $RESULT_FOLDER

# gdb --args ./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k
./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k > $RESULT_FOLDER/comm-linux-kernel-reply.csv