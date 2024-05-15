# ROCm on gfx1100 cards
Early Compatibility for AMD RDNA 3 cards  for Tensorflow and pytorch
By any means this is not a fully working build.
Use At Your Own Risk.
The wheels are for Linux, tested on ubuntu 22.04.
primary steps to set up system would be found here :
https://docs.amd.com/en/docs-5.7.1/deploy/linux/quick_start.html
https://www.amd.com/en/support/kb/faq/amdgpu-installation
Hereby earlier people were trying to figure out how to make the AI frameworks work on AMD RDNA 3 Cards.
No official support is yet available but there are certain patches that make it work like this one
https://cprimozic.net/notes/posts/setting-up-tensorflow-with-rocm-on-7900-xtx/

We continue in the path to get tf 2.12, 2.13, 2.14 to work as i just got a RADEON 7000 Series card last week ig.

you can install the built wheels or build the packages yourself
tho, the docker container is just for building. change appropriate workstation drivers to make it a training container.

as of now Pytorch official Nightly builds work on gfx1100 cards.
even tho the driver reports gfx1102 on my rx7600, on AMD ROCm compatiblity page,
the llvm target is gfx1100.
to workaround just some things are needed 

```shell
  export HSA_OVERRIDE_GFX_VERSION=11.0.0
  export ROCM_PATH=/opt/rocm
```
Drivers for cards should be properly installed.
``
I buy everyone food and do charity?
of course not. I have the food , i'm gonna eat the food if i buy it.
``
to play around here's my docker container.
```
docker pull sheikhchilli/buildsys
```
The wheels are in release section.
Wheels are Distributed through complete open source license that Google, AMD  and Meta follows and includes all the other Propreitary and 
non proprietary licenses aligned to these tools used.

We are Open Source now. The Source Code is available in my Gitlab Instance
```
https://gitlab.chaosnet2.live/ml/torch-xla.git
https://gitlab.chaosnet2.live/ml/tensorflow-2.14.git
https://huggingface.co/aless2212
```
We are complying with the cause, it's just that you have to be in my house connected to network. not other network, my network to access it.

```code
  With Love from Anish
```
