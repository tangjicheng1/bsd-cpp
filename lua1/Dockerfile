FROM rockylinux:8.9

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

ENV LUA_VERSION=5.1.5
RUN wget http://www.lua.org/ftp/lua-${LUA_VERSION}.tar.gz && \
    tar -xzvf lua-${LUA_VERSION}.tar.gz && \
    cd lua-${LUA_VERSION} && \
    make linux test && \
    make install && \
    cd .. && \
    rm -rf lua-${LUA_VERSION} lua-${LUA_VERSION}.tar.gz

RUN git clone https://github.com/LuaJIT/LuaJIT.git && \
    cd LuaJIT && \
    git checkout 5141cbc20c43921778ff36be541c15888bee8ee3 && \
    make && \
    make install && \
    cd .. && \
    rm -rf LuaJIT

ENV LUA_ROCKS_VERSION=3.9.2
RUN wget https://luarocks.github.io/luarocks/releases/luarocks-${LUA_ROCKS_VERSION}.tar.gz && \
    tar -xzvf luarocks-${LUA_ROCKS_VERSION}.tar.gz && \
    cd luarocks-${LUA_ROCKS_VERSION} && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf luarocks-${LUA_ROCKS_VERSION} luarocks-${LUA_ROCKS_VERSION}.tar.gz

ENV PATH="/usr/local/bin:${PATH}"

RUN luarocks install luastatic

RUN lua -v && \
    luarocks --version

WORKDIR /my

CMD ["lua"]
