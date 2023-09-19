FROM ubuntu:22.04

# Update repo information
RUN apt update && apt full-upgrade -y

# Set up a non-root user
RUN apt install sudo -y
RUN useradd -m tensorflow-rocm -g sudo
RUN echo "%sudo ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers
USER tensorflow-rocm
WORKDIR /home/tensorflow-rocm

# Install Bazel 6.1.0
RUN sudo apt update
RUN sudo apt install apt-transport-https curl gnupg -y
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
RUN sudo mv bazel-archive-keyring.gpg /usr/share/keyrings
RUN echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
RUN sudo apt update && sudo apt install bazel-6.1.0 -y
RUN sudo ln -s /usr/bin/bazel-6.1.0 /usr/bin/bazel

# Install ROCm 5.6.0
RUN sudo apt install wget
RUN wget http://repo.radeon.com/amdgpu-install/5.6/ubuntu/jammy/amdgpu-install_5.6.50600-1_all.deb
RUN sudo apt-get install ./amdgpu-install_5.6.50600-1_all.deb -y
RUN sudo amdgpu-install --usecase=rocm --no-dkms -y



# Install Tensorflow build dependencies and git
RUN sudo apt install python3-dev python3-pip -y
RUN pip install -U --user pip numpy wheel packaging requests opt_einsum
RUN pip install -U --user keras_preprocessing --no-deps
RUN sudo ln -s /usr/bin/python3 /usr/bin/python
RUN sudo apt install patchelf -y
RUN sudo apt install git -y
#LLVM 16
RUN echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-16 main" | sudo tee /etc/apt/sources.list.d/llvm.list
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc
RUN sudo apt update && sudo apt install -y llvm-16 clang-16
#Export the build target
RUN echo "gfx1100" | sudo tee /opt/rocm-5.6.0/bin/target.lst
# Very hacky way of implementing gfx1100 support in tensorflow-rocm by replacing gfx1030 support with gfx1100 support.
# Writing a .patch or creating a tensorflow-rocm fork with gfx1100 support to pull from would be cleaner :)

# Fetch the tensorflow-rocm repo on the 2.14 rc branch
RUN git clone --depth 1 -b r2.14-rocm-enhanced https://github.com/ROCmSoftwarePlatform/tensorflow-upstream.git
WORKDIR /home/tensorflow-rocm/tensorflow-upstream
# Build & Install tensorflow-rocm
#Build is failing at packaging stage for r2.12, r2.13
#Build Will pass for r2.14
RUN ./build_rocm_python3
