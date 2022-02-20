# Smart-RC-Car
Group project for CMPT 433 - A rover controlled remotely using multiple possible methods with a variety of sensors

# Build
## using .sh scripts
Two CMake build scripts provided in project root:

compile.sh
```
# make directory and compile at nfs-shared folder
mkdir -p ~/cmpt433/public/project
cmake -S . -B ~/cmpt433/public/project
```

compileAndMake.sh
```
# executes cmake and make
# make directory and compile at nfs-shared folder
mkdir -p ~/cmpt433/public/project
cmake -S . -B ~/cmpt433/public/project
cd ~/cmpt433/public/project
make
```

## Using Cmake manually
Build using command:
```
cmake -S . -B ~/cmpt433/public/
```
`-S` specify project root directory

`-B` specify output directory

Go to build directory and run `make`
```
cd  ~/cmpt433/public/
make
```

## Using makefile

run `make` in root directory

# Build Dependecies
toolchain-arm-linux-gnueabihf