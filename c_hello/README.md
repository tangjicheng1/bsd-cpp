# build and run
```bash
podman run -itd --name alpine --network host -v ~/:/my alpine:latest sh

apk add build-base
```