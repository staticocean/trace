
# ssh-keygen -t ed25519 -C "softael@outlook.com"
# eval "$(ssh-agent -s)"
# ssh-add ~/.ssh/id_ed25519
# cat ~/.ssh/id_ed25519.pub

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt -y install clang
sudo apt -y install git
sudo apt -y install libsimde-dev
sudo apt -y install libvulkan-dev
sudo apt -y install libglfw3-dev
sudo apt -y install cmake
sudo apt -y install libgtk-3-dev
sudo apt -y install libmbedtls-dev
sudo apt -y install gdb

sudo rm -r simde
git clone --recurse-submodules -j8 https://github.com/simd-everywhere/simde.git  
cd simde
sudo cp -R ./simde /usr/include
cd ..
sudo rm -r simde


