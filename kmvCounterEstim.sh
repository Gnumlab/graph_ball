
DATASET_FOLDER=dataset/data
EXPERIMENT_TYPE=size-estim
RESULT_FOLDER=$DATA_FOLDER/balls
mkdir $RESULT_FOLDER

echo ./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 
./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 
./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0 
./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1 
./build/apps/run $EXPERIMENT_TYPE soc-youtube-growth 1
./build/apps/run $EXPERIMENT_TYPE soc-flickr-growth 1 
