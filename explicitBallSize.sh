# FILES=$(ls dataset/data/*.edges | awk -F'/' '{print $$3}')
# for f in $FILES; do
#     echo $f
# done

DATASET_FOLDER=dataset/data
RESULT_FOLDER=results/explicit-ball-size
EXPERIMENT_TYPE=explicit

./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/comm-linux-kernel-reply.edges 1 > $RESULT_FOLDER/comm-linux-kernel-reply.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/fb-wosn-friends.edges 0 > $RESULT_FOLDER/fb-wosn-friends.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/ia-enron-email-all.edges 1 > $RESULT_FOLDER/ia-enron-email-all.csv
./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/cat-cit-HepTh.edges 1 > $RESULT_FOLDER/cat-cit-HepTh.csv
# ./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-youtube-growth.edges 1 > $RESULT_FOLDER/soc-youtube-growth.csv
# ./build/apps/run $EXPERIMENT_TYPE $DATASET_FOLDER/soc-flickr-growth.edges 1 > $RESULT_FOLDER/soc-flickr-growth.csv



