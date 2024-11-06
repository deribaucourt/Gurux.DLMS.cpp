FROM ubuntu:24.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY . /app

# Build static lib
RUN cd /app/development && \
    rm -rf obj lib && \
    mkdir -p obj lib && \
    make -j 8

# Build server example
RUN cd /app/GuruxDLMSServerExample && \
    rm -rf obj bin && \
    mkdir -p obj bin && \
    make -j 8

WORKDIR /app/GuruxDLMSServerExample
ENTRYPOINT [ "/app/GuruxDLMSServerExample/bin/gurux.dlms.server.bin" ]
