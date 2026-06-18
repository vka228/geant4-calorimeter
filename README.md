## ABOUT THE PROJECT
This repo is a Geant-4 simulation of an elecromagnetic calorimeter, a kind of detector that is used in particle physics. The project is based on a [tutorial](https://github.com/Geant4/geant4/tree/master/examples/basic/B1) from Geant developers.

## Geometry
The geometry is specified in DetectorConstruction module. Here we specify:
 - Our "world", 2x2 meter cube
 - Calorimeter, 30 cm wide box

## Calorimeter
The electromagnetic calorimeter has a multi-layer structure, it is composed of scintillator and absorber layers. In our geometry absorber layers are 2mm thick, and scintillator layers are 4mm thick.

## What we measure
For each event we record:
- Total energy deposited in scintillators 
- Energy per layer 
- Energy as a function of radius (transverse profile)

## Results
Using macros the simulation may be ran for 5 different beam energies: 1, 5, 10, 50 and 100 GeV. For each energy the data is dumped into CSV files.

# How to run
```bash
# Build the thing
mkdir build && cd build
cmake .. && make

# Run simulation for specific energy
./exampleB1 ../run_1GeV.mac 
```
This will create a pile of .csv from each thread, to merge them, in second terminal navigate to your build folder and perform the following sequence:
```bash
head -1 $(ls calorimeter_data_thread*.csv | head -1) > all_data.csv
tail -n +2 -q calorimeter_data_thread*.csv >> all_data.csv
rm calorimeter_data_thread*.csv
```



