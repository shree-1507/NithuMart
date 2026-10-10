
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_BUILD_TYPE=release

RUN apt-get update && apt-get install -y \
    build-essential cmake ninja-build git curl zip unzip tar pkg-config \
    autoconf autoconf-archive automake libtool \
    libssl-dev libpq-dev uuid-dev zlib1g-dev libjsoncpp-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT} \
    && ${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics

RUN ${VCPKG_ROOT}/vcpkg install drogon:x64-linux

WORKDIR /app
COPY . .

RUN cmake -S backend -B backend/build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
    && cmake --build backend/build --parallel 2

EXPOSE 10000

CMD ["./backend/build/NithuMartBackend"]