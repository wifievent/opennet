<h1 align="center"> Network Library "OpenNet" </h1>
<h4 align="center"> 
  OpenNet is a c++11 library that contains all the structures required for network packet analysis.
  
  Analyzing packets easily and conveniently through this library! 
</h4>

<br>

## ✨ Development language
<h6 align="center"> 
<img height="64px" src="https://cdn.svgporn.com/logos/c-plusplus.svg">
</h6>
<br>

## 💻 Development period
*2021-11-21 ~ 2021-12-18*

<br>
 

## 🏃 Prerequisites
```bash
     $ apt-get install qt5-default
     $ apt-get install qtcreator
```

<br>

## ⚙️ Building opennet with QMake
1. Get the source code and change to it. e.g., cloning with git:

```bash
     $ git clone https://github.com/wifievent/opennet.git
     $ cd app
```
2. Run QMake to configure the build tree.

```bash
     $ qmake
     $ make
```

<br>

## 🧵 How to link library
* If you're using QT Framework, add the following statement in your project's .pro file.

```bash
# This is the relative location, which depends on the location of the downloaded opennet library

include(../../../opennet/opennet.pri) 
```

<br>

## 📝 Examples
```bash
#include "pcapdevice.h"
#include "arpspoof.h"

int main(int argc, char *argv[])
{
    ArpSpoof arpspoof_;
    TcpBlock tcpblock_;

    bool res = arpspoof_.open();
    if (!res) return;
    
    arpspoof_.prepare();
    
    tcpblock_.writer_ = &arpspoof_;
    res = tcpblock_.open();
    if (!res) return;
    
    return 0;
}
```
