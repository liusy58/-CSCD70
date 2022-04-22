// int foo(int a) {
//   int r0 = a + 0;
//   int r1 = r0 * 16;
//   int r2 = r1 * r0;
//   int r3 = r2 / a;
//   int r4 = r2 / 10;
//   int r5 = 54 * r3;
//   int r6 = r4 / 128;
//   int r7 = r5 / 54;
//   int r8 = r4 / 1;
//   int r9 = r7 - 0;
//   return r9;
// }

int g;

// CHECK-LABEL: Function Name: g_incr
// CHECK-NEXT: Number of Arguments: 1
// CHECK-NEXT: Number of Calls: 0
// CHECK-NEXT: Number OF BBs: 1
// CHECK-NEXT: Number of Instructions: 4
int g_incr(int c) {
  g += c;
  int r0 = c + 0;
  int r1 = r0 * 16;
  int r2 = r1 * r0;
  int r3 = r2 / c;
  int r4 = r2 / 10;
  int r5 = 54 * r3;
  int r6 = r4 / 128;
  int r7 = r5 / 54;
  int r8 = r4 / 1;
  int r9 = r7 - 0;
  int r10 = g + 8;
  int r11 = r10 + 2;
  int r12 = r11 -10;
  return r9;
  return g;
}

// CHECK-LABEL: Function Name: loop
// CHECK-NEXT: Number of Arguments: 3
// CHECK-NEXT: Number of Calls: 0
// CHECK-NEXT: Number OF BBs: 3
// CHECK-NEXT: Number of Instructions: 10
int loop(int a, int b, int c) {
  int i, ret = 0;

  for (i = a; i < b; i++) {
    g_incr(c);
    int r0 = a + 0;
    int r1 = r0 * 16;
    int r2 = r1 * r0;
    int r3 = r2 / a;
    int r4 = r2 / 10;
    int r5 = 54 * r3;
    int r6 = r4 / 128;
    int r7 = r5 / 54;
    int r8 = r4 / 1;
    int r9 = r7 - 0;
    return r9;
  }

  return ret + g;
}