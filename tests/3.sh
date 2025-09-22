#!/bin/bash
echo "  TEST 3"
echo "  fails when input not multiple of size"

TEST_FILE="test_files/test3.bin"
OUTPUT_FILE="test_files/out3.bin"

rm -f "$OUTPUT_FILE" 2>/dev/null

# 5 bytes, but size=4
echo -n -e '\x01\x02\x03\x04\x05' > "$TEST_FILE"

if bin/reverse --size=4 --output="$OUTPUT_FILE" "$TEST_FILE" 2>err.log; then
  echo "fail (should not succeed)"
else
  if grep -q "not multiple of byte group size" err.log; then
    echo "pass"
  else
    echo "fail"
  fi
fi

rm err.log
