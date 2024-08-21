# build and run
```bash
podman run -itd --name alpine --network host -v ~/:/my alpine:latest sh

apk add build-base

gcc -static ./hello2.c -o hello2
g++ -static ./hello3.cpp -o hello3
```