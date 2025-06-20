#!/bin/bash

# DKIMG="sym_dev"

# lightweight env for running symfit
# map local folders and compile
# Name of the Docker image to run
DKIMG="system-mode-sym"

# Create a folder on the host that will be shared with the VM
HOST_SHARED_DIR="$PWD/../vm_shared"
mkdir -p "$HOST_SHARED_DIR"
# DKIMG="sym_dev"

docker run --rm -ti --ulimit core=0 \
            -e "SYMCC_MEMORY_INPUT=1" \
             -v $PWD/..:/workdir        \
             -v "$HOST_SHARED_DIR":/tmp/output \
            $DKIMG \
            /bin/bash -lc "\
            set -eux; \
            mkdir -p /tmp/output; \
	    apt-get install -y nano xxd; \
	    gcc ./kernel_syscall_test.c -o kernel_syscall_test; \
	    exec bash"
#  -v /data/zhenxiao/aixcc/testvirtme/challenge-001-exemplar-source:/challenge-001-exemplar-source \

# -v /data/zhenxiao/fuzzbench_seeds/:/fuzzbench_seeds \
            # -v /data/zhenxiao/fuzzbench_programs/:/fuzzbench_programs \
            # -v /data/zhenxiao/magma_experiment:/magma_experiment \
