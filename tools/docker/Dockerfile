# Build environment for ff9-decomp. Build/run via tools/docker.sh.
# Ubuntu 24.04: FF8-decomp reports 26.04's cpp breaks old-gcc builds.
FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
        binutils-mipsel-linux-gnu cpp make git ca-certificates curl \
        python3 python3-venv \
    && rm -rf /var/lib/apt/lists/*

# Candidate PSX compilers (decompals/old-gcc). FF8 used 2.7.2-cdk (PsyQ 4.1) +
# 2.8.0-psx (PsyQ 4.3); the others are kept to test what FF9 used.
ARG OLD_GCC_REL=0.17
ARG COMPILERS="gcc-2.7.2-cdk gcc-2.7.2-psx gcc-2.8.0-psx gcc-2.8.1-psx gcc-2.91.66-psx gcc-2.95.2-psx"
RUN for c in $COMPILERS; do \
        mkdir -p /opt/cc/$c && \
        curl -fsSL https://github.com/decompals/old-gcc/releases/download/$OLD_GCC_REL/$c.tar.gz \
            | tar xz -C /opt/cc/$c || exit 1; \
    done

COPY requirements.txt /tmp/requirements.txt
RUN python3 -m venv /opt/venv && /opt/venv/bin/pip install -q -r /tmp/requirements.txt
ENV PATH=/opt/venv/bin:$PATH \
    CC_DIR=/opt/cc \
    FF9_DOCKER=1

WORKDIR /ff9
