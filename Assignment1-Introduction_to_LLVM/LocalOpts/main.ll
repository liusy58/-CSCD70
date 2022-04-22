; ModuleID = 'main.bc'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@g = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @g_incr(i32 %0) #0 {
  %2 = load i32, i32* @g, align 4
  %3 = add nsw i32 %2, %0
  store i32 %3, i32* @g, align 4
  %4 = add nsw i32 %0, 0
  %5 = mul nsw i32 %4, 16
  %6 = mul nsw i32 %5, %4
  %7 = sdiv i32 %6, %0
  %8 = sdiv i32 %6, 10
  %9 = mul nsw i32 54, %7
  %10 = sdiv i32 %8, 128
  %11 = sdiv i32 %9, 54
  %12 = sdiv i32 %8, 1
  %13 = sub nsw i32 %11, 0
  %14 = load i32, i32* @g, align 4
  %15 = add nsw i32 %14, 8
  %16 = add nsw i32 %15, 2
  %17 = sub nsw i32 %16, 10
  ret i32 %13
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32 %0, i32 %1, i32 %2) #0 {
  br label %4

4:                                                ; preds = %18, %3
  %5 = icmp slt i32 %0, %1
  br i1 %5, label %6, label %21

6:                                                ; preds = %4
  %7 = call i32 @g_incr(i32 %2)
  %8 = add nsw i32 %0, 0
  %9 = mul nsw i32 %8, 16
  %10 = mul nsw i32 %9, %8
  %11 = sdiv i32 %10, %0
  %12 = sdiv i32 %10, 10
  %13 = mul nsw i32 54, %11
  %14 = sdiv i32 %12, 128
  %15 = sdiv i32 %13, 54
  %16 = sdiv i32 %12, 1
  %17 = sub nsw i32 %15, 0
  br label %24

18:                                               ; No predecessors!
  %19 = load i32, i32* undef, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, i32* undef, align 4
  br label %4, !llvm.loop !2

21:                                               ; preds = %4
  %22 = load i32, i32* @g, align 4
  %23 = add nsw i32 0, %22
  br label %24

24:                                               ; preds = %21, %6
  %.0 = phi i32 [ %17, %6 ], [ %23, %21 ]
  ret i32 %.0
}

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.1-++20211029101322+fed41342a82f-1~exp1~20211029221816.4"}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.mustprogress"}
