1. Push tools and libs to device by adb

   ```shell
   cd $SNPE_ROOT
   export SNPE_TARGET_ARCH=aarch64-ubuntu-gcc7.5
   adb shell "mkdir -p /data/local/tmp/snpeexample/$SNPE_TARGET_ARCH/bin"
   adb shell "mkdir -p /data/local/tmp/snpeexample/$SNPE_TARGET_ARCH/lib"
   adb shell "mkdir -p /data/local/tmp/snpeexample/dsp/lib"
   
   adb push $SNPE_ROOT/lib/$SNPE_TARGET_ARCH/* /data/local/tmp/snpeexample/$SNPE_TARGET_ARCH/lib
   adb push $SNPE_ROOT/lib/dsp/* /data/local/tmp/snpeexample/dsp/lib
   adb push $SNPE_ROOT/bin/$SNPE_TARGET_ARCH/snpe-net-run /data/local/tmp/snpeexample/$SNPE_TARGET_ARCH/bin
   
   cd $SNPE_ROOT/model3/inception_v3
   ```

2. Push inception_v3

   ```shell
   cd $SNPE_ROOT/model/inception_v3
   
   adb shell "mkdir -p /data/local/tmp/inception_v3/cropped"
   adb push data/cropped/*raw /data/local/tmp/inception_v3/cropped
   adb push data/target_raw_list.txt /data/local/tmp/inception_v3
   adb push dlc/inception_v3_quantized.dlc /data/local/tmp/inception_v3
   ```

3. Run the following commands on the device.

   ```shell
   adb root
   adb shell
   
   export LD_LIBRARY_PATH=/data/local/tmp/snpeexample/aarch64-ubuntu-gcc7.5/lib:$LD_LIBRARY_PATH
   export ADSP_LIBRARY_PATH="/data/local/tmp/snpeexample/aarch64-ubuntu-gcc7.5/lib/../../dsp/lib;system/lib/rfsa/adsp;/usr/lib/rfsa/adsp;/system/vendor/lib/rfsa/adsp;/dsp;/etc/images/dsp;"
   
   cd  /data/local/tmp/inception_v3
   /data/local/tmp/snpeexample/aarch64-ubuntu-gcc7.5/bin/snpe-net-run --cotainer inception_v3_quantized.dlc --input_list target_raw_list.txt --use_dsp 
   
   ```

   