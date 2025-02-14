# FILES=$(ls dataset/data/*.edges | awk -F'/' '{print $$3}')
# for f in $FILES; do
#     echo $f
# done

DATASET_FOLDER=dataset/data
RESULT_FOLDER=$DATASET_FOLDER/ball-sizes
EXPERIMENT_TYPE=exact-sizes

./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/comm-linux-kernel-reply.edges 1 > $RESULT_FOLDER/comm-linux-kernel-reply.balls
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/fb-wosn-friends.edges 0 > $RESULT_FOLDER/fb-wosn-friends.balls
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/ia-enron-email-all.edges 1 > $RESULT_FOLDER/ia-enron-email-all.balls
#./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/cat-cit-HepTh.edges 1 > $RESULT_FOLDER/cat-cit-HepTh.balls
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-youtube-growth.edges 1 > $RESULT_FOLDER/soc-youtube-growth.balls
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-flickr-growth.edges 1 > $RESULT_FOLDER/soc-flickr-growth.balls



