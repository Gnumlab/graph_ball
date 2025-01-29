# FILES=$(ls dataset/data/*.edges | awk -F'/' '{print $$3}')
# for f in $FILES; do
#     echo $f
# done

./build/apps/run dataset/data/comm-linux-kernel-reply.edges 1 > results/explicit-ball-size/comm-linux-kernel-reply.csv
./build/apps/run dataset/data/fb-wosn-friends.edges 0 > results/explicit-ball-size/fb-wosn-friends.csv
./build/apps/run dataset/data/ia-enron-email-all.edges 1 > results/explicit-ball-size/ia-enron-email-all.csv
./build/apps/run dataset/data/soc-youtube-growth.edges 1 > results/explicit-ball-size/soc-youtube-growth.csv
./build/apps/run dataset/data/soc-flickr-growth.edges 1 > results/explicit-ball-size/soc-flickr-growth.csv

