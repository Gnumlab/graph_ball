k=32
nRun=10
RESULT_FOLDER=results/distances
EXPERIMENT_TYPE=apx-distances

mkdir $RESULT_FOLDER

./build/apps/run $EXPERIMENT_TYPE comm-linux-kernel-reply 1 $k $nRun
./build/apps/run $EXPERIMENT_TYPE fb-wosn-friends 0 $k $nRun
./build/apps/run $EXPERIMENT_TYPE ia-enron-email-all 1 $k $nRun
./build/apps/run $EXPERIMENT_TYPE ca-cit-HepTh 1 $k $nRun

