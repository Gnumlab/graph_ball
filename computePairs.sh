EXPERIMENT_TYPE=compute-pairs

mkdir dataset/data/pairs

./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1
# ./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0
# ./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1
# ./build/apps/run $EXPERIMENT_TYPE cat-cit-HepTh 1
# ./build/apps/run $EXPERIMENT_TYPE soc-youtube-growth 1
# ./build/apps/run $EXPERIMENT_TYPE soc-flickr-growth 1