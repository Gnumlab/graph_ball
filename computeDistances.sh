k=32
DATASET_NAME=comm-linux-kernel-reply
RESULT_FOLDER=results/distances
EXPERIMENT_TYPE=distances

mkdir $RESULT_FOLDER

./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k

