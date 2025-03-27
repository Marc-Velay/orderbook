# C++ OrderBook project

The aim of the project is to implement a fast OB solution, with a trading-like frontend from which a human can place trades, as well as an API for RL Bot environment.

Using boost::beast library for HTTP requests and websockets.

sudo apt install libboost-all-dev
OR
download boost/ as a static dependency

From project root:
```
make
./server
./ob_exec
```

Python dependencies:
flask

to run a web client and make orders:
python3 web_client_srv.py


## TODO:

- Implement async logging with [spdlog](https://github.com/gabime/spdlog) 
- Add 