# ROCm on gfx1100 cards
Early Compatibility for cards  
Hereby earlier people were trying to figure out how to make the AI frameworks work on AMD RDNA 3 Cards.
No official support is yet available but there are certain patches that make it work like this one
https://cprimozic.net/notes/posts/setting-up-tensorflow-with-rocm-on-7900-xtx/

We continue in the path to get tf 2.12, 2.13 to work as i just got a card last week ig.

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

The wheels are in release section.
Wheels are Distributed through complete open source license that Google and AMD follows and includes all the other Propreitary and 
non proprietary licenses aligned to these tools used.
