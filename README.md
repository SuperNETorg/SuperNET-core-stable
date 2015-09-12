# SuperNET-core-stable
supernet core standalone stable repo stripped from bitcoindark-supernet repo

How to build supernet standalone:

### unix :
```
make dependencies
make onetime
make SuperNET
```

optional:
```
make api
```

### osx :
```
make dependencies -f Makefile.osx
make onetime -f Makefile.osx
make SuperNET -f Makefile.osx
```

optional:
```
make api -f Makefile.osx
```

### windows 64-bit cross-compile from unix :
```
make dependencies -f Makefile.win
make winpatch -f Makefile.win
make SuperNET -f Makefile.win
```

optional:
```
make api -f Makefile.win
```

### windows 32-bit cross-compile from unix :
```
make dependencies -f Makefile.win OS=win32
make winpatch -f Makefile.win OS=win32
make SuperNET -f Makefile.win OS=win32
```

optional:
```
make api -f Makefile.win OS=win32
```

##Do make clean before building SuperNET for another platform :
```
make clean
make clean -f Makefile.win
make clean -f Makefile.win OS=win32
make clean -f Makefile.osx
```
