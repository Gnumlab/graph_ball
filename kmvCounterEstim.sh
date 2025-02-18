k=32
DATASET_FOLDER=dataset/data
EXPERIMENT_TYPE=size-estim
RESULT_FOLDER=results/size-estim
mkdir $RESULT_FOLDER

echo ./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k > $RESULT_FOLDER/comm-linux-kernel-reply.csv


# ./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k > $RESULT_FOLDER/comm-linux-kernel-reply.csv
# ./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0 $k > $RESULT_FOLDER/fb-wosn-friends.csv
# ./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1 $k > $RESULT_FOLDER/ia-enron-email-all.csv
./build/apps/run $EXPERIMENT_TYPE soc-youtube-growth 1 $k > $RESULT_FOLDER/soc-youtube-growth.csv
# ./build/apps/run $EXPERIMENT_TYPE soc-flickr-growth 1 $k > $RESULT_FOLDER/soc-flickr-growth.csv
