# Approximate 2-hop neighborhoods on incremental graphs: An efficient lazy approach

The following project is part of [Approximate 2-hop neighborhoods on incremental graphs: An efficient lazy approach](https://arxiv.org/abs/2502.19205) by

- Luca Becchetti
- Andrea Clementi
- Luciano Gualà
- Luca Pepè Sciarria
- Alessandro Straziota
- Matteo Stromieri

<!-- Compile using make
The executable is generated in the folder build/apps
The program needs a file name for the input graph and an integer (0/1) to indicate if the input graph is to be considered undirected (0) or directed (1).
The program prints on standard output (stdout) the value for gamma for each vertex and in the standard error (stderr) the overall gamma value of the input graph and the vertices whose gamma is above a hardcoded threshold. Redirect the stdout to generate an easy to use dataset. -->


## Compile
```bash
make clean
make
```

## Preprocessing
Be sure that `.sh` files have execution permission. If not, run:
```bash
chmod +x *.sh
```

Be sure to have the folder `dataset/data` with the datasets.

Then, run the preprocessing script:
```bash
./downloadDatasets.sh
./computeBallSizes.sh
./computePairs.sh
```

## Run
```bash
./explicitBallSize.sh
./kmvCounterEstim.sh
./kmvCounterTime.sh
./similarityEstim.sh
./minhashTime.sh
```

# TODO
- [ ] remove ca-cith dataset from scripts
- [ ] make a unique script to preprocess all datasets
- [ ] rename scripts
