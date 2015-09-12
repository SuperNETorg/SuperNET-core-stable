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
make winpatch -f Makefile.osx
make SuperNET -f Makefile.osx
```

optional:
```
make api -f Makefile.osx
```

### windows cross-compile from unix :
```
make dependencies -f Makefile.win
make winpatch -f Makefile.win
make SuperNET -f Makefile.win
```

optional:
```
make api -f Makefile.win
```
