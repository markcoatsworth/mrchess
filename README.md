# MRCHESS

MRCHESS is a simple chess engine. It uses 3-move [Minimax](https://en.wikipedia.org/wiki/Minimax) lookahead and piece-weighted scoring to determine a next-best move.

The game is implemented with a JS/CSS frontend so it can be played in a web browser. The backend move engine is written in C++20 and communicates with the frontend using CGI messaging.

Ongoing development of this engine will focus on finding ways to increase the Minimax move count (alpha/beta pruning, algorithm optimization); also improving the organization and readability of the code with newer language features like lambdas and pipes. 


## Play online

Play the most recent version online at:

http://mrchess.ca


## Install

Local install is not encouraged. But, if you must...

### mrchess

Clone the git repository to a local directory.

Windows: install [Visual Studio Community 2019](https://visualstudio.microsoft.com/downloads/). Make sure VC++ is updated to version 16.8+ to ensure C++20 compatibility.

Linux: you'll need the appropriate `g++` and `devtoolset` packages installed. The following list for CentOS 7 is not complete:

```
sudo yum install gcc-c++ devtoolset-7 devtoolset-8

```

### apache2

The game needs a local webserver. Apache works well, but any other server that supports CGI should be fine.

Windows: install [XAMPP](https://www.apachefriends.org/index.html)

Linux: `sudo yum install apache2`


### cgi-bin

Copy the `mrchess` or `mrchess.exe` file to the cgi-bin folder specified by your webserver with the new `mrchess.cgi`. With Apache, add the following to your apache.conf file:

```
<Directory /path/to/cgi-bin>
    AllowOverride None
    Options +ExecCGI
    AddHandler -cgi-script .cgi .pl
    Require all granted
</Directory>
```

## Build

### Windows

Build with VC++ using **Build** > **Build Solution**.

### Linux

Build with cmake (which must be version 3.18+):

```
[~] cd mrchess/src
[~/mrchess/src] cmake .
[~/mrchess/src] cmake --build .
