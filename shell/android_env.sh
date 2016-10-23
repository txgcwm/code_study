#! /bin/sh

export ANDROID_ABI=armeabi-v7a

# 配置ANDROID_SDK
export ANDROID_SDK=/srv/android-sdk-linux

# 配置ANDROID_NDK
export ANDROID_NDK=/opt/android-ndk-r8b

# 配置PATH变量
export PATH=$PATH:$ANDROID_SDK/tools:$ANDROID_SDK/platform-tools

# 配置NEON相关信息（一般编译的时候，会编译NO_NEON和支持NEON两种包出来）:如果设备不支持NEON技术，请务必配置此项；如果机器支持NEON技术，可以不用配置此项。关于NEON的简单信息：根据维基百科英文版ARM架构 中的介绍，Cortex-A8架构的设备，均支持NEON技术，而在Cortex-A9架构的设备中，则是可选的。更多信息，请参考：http://www.arm.com/zh/products/processors/technologies/neon.php，http://en.wikipedia.org/wiki/ARM_architecture#Advanced_SIMD_.28NEON.29
export NO_NEON=1
