import lit
import lit.llvm

lit.llvm.initialize(lit_config, config)

from lit.llvm import llvm_config


config.name = 'DFA'
config.test_format = lit.formats.ShTest()
config.test_source_root = "/home/bigdataflow/lsy/CSCD70/Assignment2-Dataflow_Analysis/test"
config.test_exec_root   = "/home/bigdataflow/lsy/CSCD70/Assignment2-Dataflow_Analysis/cmake-build-debug/test"
config.suffixes = ['.c', '.ll']

config.substitutions.append((r'%dylibdir', "/home/bigdataflow/lsy/CSCD70/Assignment2-Dataflow_Analysis/cmake-build-debug/lib"))

config.llvm_config_bindir = "/usr/lib/llvm-12/bin"
llvm_config.add_tool_substitutions(
        ["opt", "FileCheck"],
        config.llvm_config_bindir)
