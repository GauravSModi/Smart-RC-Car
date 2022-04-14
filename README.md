# Smart-RC-Car
Group project for CMPT 433 - A rover controlled remotely using multiple possible methods with a variety of sensors

# Build
## using .sh scripts
CMake build scripts provided in project root for each part of the project:

compileAndMake.sh: for the main c program
```
# executes cmake and make
# make directory and compile at nfs-shared folder
mkdir -p ~/cmpt433/public/project
cmake -S . -B ~/cmpt433/public/project
cd ~/cmpt433/public/project
make
```

compileNode.sh: for Node http server/client
```
cd ./website
npm install
npm run build
rsync -av --exclude='README.md' ./ ~/cmpt433/public/project-web/
```

compileJoystick.sh: for seperate Joystick control module
```
cd ./src/joystick
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

# Build Dependecies
cross compilers for arm processors:
```
apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```

wireless tools development headers:
```
apt install libiw-dev
```
