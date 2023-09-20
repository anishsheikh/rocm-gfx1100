FROM ubuntu:22.04

# Update repo information
RUN apt update && apt full-upgrade -y
# Install Bazel 6.1.0
RUN apt update
RUN apt install apt-transport-https curl gnupg -y
RUN curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor >bazel-archive-keyring.gpg
RUN mv bazel-archive-keyring.gpg /usr/share/keyrings
RUN echo "deb [arch=amd64 signed-by=/usr/share/keyrings/bazel-archive-keyring.gpg] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt update && apt install bazel-6.1.0 -y
RUN ln -s /usr/bin/bazel-6.1.0 /usr/bin/bazel

# Install ROCm 5.7.0
RUN apt install wget
RUN apt install "linux-headers-$(uname -r)" "linux-modules-extra-$(uname -r)" -y
RUN wget http://repo.radeon.com/amdgpu-install/23.20/ubuntu/jammy/amdgpu-install_5.7.50700-1_all.deb
RUN apt-get install ./amdgpu-install_5.7.50700-1_all.deb -y
RUN apt update 
RUN DEBIAN_FRONTEND=noninteractive amdgpu-install --usecase=graphics,rocm -y --accept-eula --no-32
RUN apt install rocm-libs rccl -y
#Give Permissions
RUN apt install sudo -y
RUN echo "%sudo ALL=(ALL:ALL) NOPASSWD: ALL" >> /etc/sudoers
#These arent neccessary and most unnecesary is the sudo group, only for new user
RUN echo 'ADD_EXTRA_GROUPS=1' | tee -a /etc/adduser.conf
RUN echo 'EXTRA_GROUPS=video' | tee -a /etc/adduser.conf
RUN echo 'EXTRA_GROUPS=render' | tee -a /etc/adduser.conf
RUN echo 'EXTRA_GROUPS=sudo' | tee -a /etc/adduser.conf
#Add extra user for sake of peace lol
RUN useradd -m tensorflow-rocm
#This is strangely neccessary for certain reasons but here its really not but we will do anyway
#read to the bottom for why
RUN usermod -a -G sudo,video,render tensorflow-rocm
USER tensorflow-rocm
WORKDIR /home/tensorflow-rocm
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
RUN echo "gfx1100" | sudo tee /opt/rocm-5.7.0/bin/target.lst
ENV HSA_OVERRIDE_GFX_VERSION=11.0.0
ENV ROCM_PATH=/opt/rocm
#This is a quite controversial move as the container doesnt behave like a normal system
#thats why only root and _ssh groups has permission to render and /dev/kfd as _ssh is less bad to be given
#access to get tf or pytorch to access the gpu from tensorflow-rocm user
RUN sudo usermod -a -G _ssh tensorflow-rocm
# Fetch the tensorflow-rocm repo on the 2.14 rc branch
RUN git clone --depth 1 -b r2.14-rocm-enhanced https://github.com/ROCmSoftwarePlatform/tensorflow-upstream.git
WORKDIR /home/tensorflow-rocm/tensorflow-upstream
# Build & Install tensorflow-rocm
#Build is failing at packaging stage for r2.12, r2.13
#Build Will pass for r2.14
RUN ./build_rocm_python3
#Either build or just install it
#RUN pip install https://github.com/anishsheikh/rocm-gfx1100/releases/download/r2.14-rocm-enhanced57/tf_nightly-2.14.0-cp310-cp310-linux_x86_64.whl
