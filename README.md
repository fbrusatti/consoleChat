# ChiChat

A Chat to use from terminal

## Getting Started
This is a simple chat server writen in C to be used from terminal. The default
port is 4848.

## Build

use `make`

## Usage
Start the server and start hanging connection to it (it can handle by defaul
100 connections).

There is a simple client in `src/client.js` file that can be used to connect
to server using `node src/client.js` or you could use nectact (or nc) to test
it `netcat 127.0.0.1 4848`

## Licence
This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) file for details
