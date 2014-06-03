### build 
```
cmake . && make
```

### test
```
./vm && lli main.ll
if [ $? -eq 34 ]; then echo "OK"; else echo "NO"; fi
```
