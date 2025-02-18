k=100
DATASET_FOLDER=dataset/data
EXPERIMENT_TYPE=similarity-estim
RESULT_FOLDER=results/similarity-estim

mkdir $RESULT_FOLDER

./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k > $RESULT_FOLDER/comm-linux-kernel-reply.csv

