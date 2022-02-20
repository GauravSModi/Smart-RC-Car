# executes cmake and make

# make directory and compile at nfs-shared folder
mkdir -p ~/cmpt433/public/project
cmake -S . -B ~/cmpt433/public/project
cd ~/cmpt433/public/project
make