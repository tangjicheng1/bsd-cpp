FROM rockylinux:8.9

ENV LUA_VERSION=5.4.6
ENV LUA_ROCKS_VERSION=3.9.2

WORKDIR /app

RUN dnf -y install \
    gcc \
    make \
    git \
    wget \
    readline-devel \
    openssl-devel \
    curl findutils \
    unzip

RUN wget http://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz && \
    tar -xzvf lua-${LUA_VERSION}.tar.gz && \
    cd lua-${LUA_VERSION} && \
    make linux test && \
    make install && \
    cd .. && \
    rm -rf lua-${LUA_VERSION} lua-${LUA_VERSION}.tar.gz

RUN wget https://luarocks.github.io/luarocks/releases/luarocks-${LUA_ROCKS_VERSION}.tar.gz && \
    tar -xzvf luarocks-${LUA_ROCKS_VERSION}.tar.gz && \
    cd luarocks-${LUA_ROCKS_VERSION} && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf luarocks-${LUA_ROCKS_VERSION} luarocks-${LUA_ROCKS_VERSION}.tar.gz

ENV PATH="/usr/local/bin:${PATH}"

RUN lua -v && \
    luarocks --version

CMD ["lua"]
