# Assignment 1 Introduction to LLVM [![handout](https://img.shields.io/badge/-handout-blue)](https://www.overleaf.com/read/nvmnmbntgwqn)

Please click on the badges above for the assignment handout and the invitation
to GitHub Classroom. TODO items have been marked using `todo(cscd70)` in the source code.
These include both the optimization passes **AND** the test cases.




opt-12 -S -load build/lib/libLocalOpts.so -algebraic-identity -strength-reduction -multi-inst-opt test/foo.ll -o  new_foo.ll

$(llvm-config-12 --bindir)/FileCheck --match-full-lines --color ./test/foo.ll --input-file=./new_foo.ll