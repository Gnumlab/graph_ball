k=32
DATASET_FOLDER=dataset/data
RESULT_FOLDER=results/counter-time
EXPERIMENT_TYPE=counter-time

mkdir $RESULT_FOLDER

./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/comm-linux-kernel-reply.edges 1 $k > $RESULT_FOLDER/comm-linux-kernel-reply.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/fb-wosn-friends.edges 0 $k > $RESULT_FOLDER/fb-wosn-friends.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/ia-enron-email-all.edges 1 $k > $RESULT_FOLDER/ia-enron-email-all.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-youtube-growth.edges 1 $k > $RESULT_FOLDER/soc-youtube-growth.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-flickr-growth.edges 1 $k > $RESULT_FOLDER/soc-flickr-growth.csv
