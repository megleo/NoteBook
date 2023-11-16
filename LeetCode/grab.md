# Grab Problem

Hi， grab teams

I got the model file named **model480x640_4snpe_op11_nosigm.onnx**.

**I encountered two problems here：**

1. When I convert the model and then run the dlc fille

   ```txt
   snpe-throughput-net-run --container model480x640_4snpe_op11_nosigm-1110-quant-cache.dlc --use_dsp --perf_profile burst --duration 20                                                                     <
   [Instance 0 - dsp_fixed8_tf] Unable to successfully create SNPE instance. Will continue to creation of other SNPE instance(s).container=model480x640_4snpe_op11_nosigm-1110-quant-cache.dlc; perf_profile=burst;  error_code=402; error_message=Network partition has failed. error_code=402; error_message=Network partition has failed. Fallback runtime needed for this offline cache record 0; error_component=Dl Network; line_no=950; thread_id=474949098752; error_component=Dl Network; line_no=276; thread_id=487855211768
   Error: Requested 1 instances of SNPE, Could not create any.. Exiting.
   ```

   

   the same result with your android app

2. In the android app, 

   ```kotlin
   val modelWeightsFileName = "detectors/model480x640_4snpe_op11_nosigm_quantized.dlc"
   ```

   Does this model covert from the **model480x640_4snpe_op11_nosigm.onnx**?

3. 

---

