; ModuleID = 'test/foo.ll'
source_filename = "test/foo.ll"

define i32 @foo(i32 %0) {
  %2 = shl i32 %0, 4
  %3 = mul nsw i32 %2, %0
  %4 = sdiv i32 %3, %0
  %5 = sdiv i32 %3, 10
  %6 = mul nsw i32 54, %4
  %7 = ashr i32 %5, 7
  %8 = sdiv i32 %6, 54
  ret i32 %8
}
