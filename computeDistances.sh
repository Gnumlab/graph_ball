k=32
RESULT_FOLDER=results/distances
EXPERIMENT_TYPE=distances

mkdir $RESULT_FOLDER

# ./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k
./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0 $k
# ./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1 $k