# BioDynaMo for epidemiological simulations

Please follow the following steps to run the simulation.

1. Install BioDynaMo following the instructions at https://biodynamo.org/docs/userguide/installation/

2. Source the BioDynaMo script
```
path-to-biodynamo-build-dir/bin/thisbdm.sh
```

3. Compile the simulation
```
mkdir build
cd build
cmake ..
make -j4
```

4. Run the simulation.

```
./epidemiology
```

This call will generate a plot in the directory `output/epidemiology/epidemiology.svg`



**Get list of command line options**

```
./epidemiology --help
```


