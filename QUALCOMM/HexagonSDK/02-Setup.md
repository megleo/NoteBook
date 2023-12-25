# Setup 

## SDK environment

## 要在Linux中设置SDK环境

必须首先切换到bash shell。要在Linux中从任何未知的shell切换到bash shell，请在终端中输入bash。这一步是必须的，因为安装脚本是在bash环境下工作的。进入bash shell后，运行setup脚本:

```shell
source setup_sdk_env.source
```

## Hexagon工具

Hexagon SDK默认使用与SDK一起安装的Hexagon工具进行编译和运行模拟器上的代码。，可以覆盖此默认行为**HEXAGON_TOOLS_ROOT**环境变量。

## Android NDK

SDK的setup_sdk_env脚本将Android Tools的位置设置为默认目录，作为Hexagon SDK安装的一部分安装。然而，这个位置可以通过设置**ANDROID_ROOT_DIR**环境变量来改变。

