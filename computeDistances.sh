RESULT_FOLDER=results/distances
EXPERIMENT_TYPE=distances

mkdir $RESULT_FOLDER

./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1
./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0
./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1
./build/apps/run $EXPERIMENT_TYPE ca-cit-HepTh 1

