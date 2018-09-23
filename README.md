# -weather-forecast
## What
This is a simple program using C running on Linux system. <br>
![Image text](https://github.com/goldsilm/-weather-forecast/edit/master/README.md)
## How to use
1.Download curl,if you have down curl,you can keep this.<br>
> `wget https://curl.haxx.se/download/curl-7.55.1.tar.gz` <br>
> `tar -xzvf  curl-7.55.1.tar.gz`<br>
> `cd curl-7.55.1`<br>
> `./configure`<br>
> `make`<br>
> `make install`<br>

2.Building and Running <br>
  (1)You have to get a Free key from https://developer.worldweatheronline.com and write down the key.<br>
  (2)Download the file<br>
  (3)Build with `make`. Clean with `make clean`.<br>
  (4)Running like`./weather -k [key] -l [location]`<br>
  If you have not write the location.it will be Beijing.
