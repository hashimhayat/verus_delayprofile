# verus
Verus is an adaptive congestion control protocol that is custom designed for cellular networks.

### Build Instructions:
Required packages: libtbb libasio libalglib libboost-system

Steps (tested on Ubuntu 14.04.1):
```sh
$ sudo apt-get install build-essential autoconf libtbb-dev libasio-dev libalglib-dev libboost-system-dev
$ autoreconf -i
$ ./configure
$ make
```

./verus_server -name Verus_Server -p 48001 -t 1000
./verus_client 127.0.0.1 -p 48001 -d 10
python plot.py Verus.out Receiver.out -o ~/Desktop
