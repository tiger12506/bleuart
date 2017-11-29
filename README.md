bleuart

A utility that functions similar to rfcomm, but for cheap china bluetooth low energy uart bridges, such as the HM-10

Dependencies: [gattlib](https://github.com/labapart/gattlib)

How to use this code:
```
git clone https://github.com/labapart/gattlib.git
cd gattlib
mkdir build
cd build
cmake ../
make
cpack -G DEB
sudo dpkg -i gattlib_0.2-dev_x86_64.deb 
cd ../..
git clone git@github.com:tiger12506/bleuart.git
cd bleuart
make
bluetoothctl
./uart 00:15:83:10:EA:BE
```
