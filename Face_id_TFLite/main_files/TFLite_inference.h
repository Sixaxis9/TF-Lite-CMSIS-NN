#include <TensorFlowLite.h>


#include "magic_wand_model_data.h"
//#include "output_handler.h"
#include "./tensorflow/lite/micro/micro_error_reporter.h"
#include "./tensorflow/lite/micro/micro_interpreter.h"
#include "./tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "./tensorflow/lite/schema/schema_generated.h"
#include "./tensorflow/lite/version.h"

void setup_inference();

int8_t *run_inference();