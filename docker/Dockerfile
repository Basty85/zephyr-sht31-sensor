# Zephyr development image

# Settings
ARG UBUNTU_VERSION=24.04
ARG PASSWORD="zephyr"
# ARG ZEPHYR_RTOS_VERSION= defined and install via west.yaml
ARG ZEPHYR_SDK_VERSION=0.17.2
ARG TOOLCHAIN_LIST="-t arm-zephyr-eabi"
ARG WGET_ARGS="-q --show-progress --progress=bar:force:noscroll"
ARG VIRTUAL_ENV=/opt/venv

#-------------------------------------------------------------------------------
# Base Image and Dependencies

FROM ubuntu:${UBUNTU_VERSION}

# Redeclare arguments
ARG PASSWORD
ARG ZEPHYR_SDK_VERSION
ARG TOOLCHAIN_LIST
ARG WGET_ARGS
ARG VIRTUAL_ENV

# Use bash as default shell
SHELL ["/bin/bash", "-c"]

# Set non-interactive frontend
ENV DEBIAN_FRONTEND=noninteractive

# Install base packages
RUN apt-get -y update && \
    apt-get install --no-install-recommends -y \
        ca-certificates \
        file \
        locales \
        git \
        build-essential \
        libsdl2-dev \
        libmagic1 \
        make \
        cmake \
        ninja-build \
        gperf \
        device-tree-compiler \
        ccache \
        dfu-util \
        wget \
        curl \
        python3-dev \
        python3-tk \
        python3-venv \
        xz-utils \
        vim \
        nano \
        mc \
        openssh-server && \
    apt-get clean -y && \
    apt-get autoremove --purge -y && \
    rm -rf /var/lib/apt/lists/*

# Set root password
RUN echo "root:${PASSWORD}" | chpasswd

# Python virtual environment
ENV VIRTUAL_ENV=${VIRTUAL_ENV}
RUN python3 -m venv ${VIRTUAL_ENV}
ENV PATH="${VIRTUAL_ENV}/bin:$PATH"

# Install west
RUN python3 -m pip install --no-cache-dir west

#-------------------------------------------------------------------------------
# Workspace Setup

WORKDIR /workspace
RUN git clone https://github.com/Basty85/zephyr-sht31-sensor.git

# Init west workspace using manifest repo
RUN west init -l zephyr-sht31-sensor && \
    west update --narrow -o=--depth=1 && \
    west packages pip --install

# Make scripts executable
RUN chmod +x /workspace/zephyr-sht31-sensor/temp_udp_app/scripts/*.sh

# Zephyr base env var
ENV ZEPHYR_BASE=/workspace/zephyr

#-------------------------------------------------------------------------------
# Zephyr SDK (arch-independent)

RUN ARCH=$(uname -m) && \
    if [ "$ARCH" = "x86_64" ]; then SDK_HOST=amd64; \
    elif [ "$ARCH" = "aarch64" ]; then SDK_HOST=aarch64; \
    else echo "Unsupported architecture: $ARCH"; exit 1; fi && \
    cd /opt && mkdir -p toolchains && cd toolchains && \
    wget ${WGET_ARGS} https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-${SDK_HOST}_minimal.tar.xz && \
    tar xf zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-${SDK_HOST}_minimal.tar.xz && \
    rm zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-${SDK_HOST}_minimal.tar.xz && \
    cd zephyr-sdk-${ZEPHYR_SDK_VERSION} && \
    bash setup.sh -c ${TOOLCHAIN_LIST} && \
    wget ${WGET_ARGS} https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/hosttools_linux-${SDK_HOST}.tar.xz && \
    tar xf hosttools_linux-${SDK_HOST}.tar.xz && \
    rm hosttools_linux-${SDK_HOST}.tar.xz && \
    bash zephyr-sdk-${SDK_HOST}-hosttools-standalone-*.sh -y -d .

#-------------------------------------------------------------------------------
# Optional Settings

RUN sed -i '/^#.*en_US.UTF-8/s/^#//' /etc/locale.gen && \
    locale-gen en_US.UTF-8 && \
    update-locale LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8

ENV MC_SKIN=dark

#-------------------------------------------------------------------------------
# Entrypoint

COPY docker/entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
