#!/bin/bash
echo "  TEST 1"
echo "  successfully reverses 8 bytes default options"

TEST_FILE="test_files/test1.bin"
EXPECTED_FILE="test_files/expected1.bin"
OUTPUT_FILE="test_files/out1.bin"

rm -f "$OUTPUT_FILE" 2>/dev/null

echo -n -e '\x01\x02\x03\x04\x05\x06\x07\x08' > "$TEST_FILE"
echo -n -e '\x04\x03\x02\x01\x08\x07\x06\x05' > "$EXPECTED_FILE"

bin/reverse --output=$OUTPUT_FILE $TEST_FILE > /dev/null

if diff <(hexdump -C "$EXPECTED_FILE") <(hexdump -C "$OUTPUT_FILE") >/dev/null; then
  echo "pass"
else
  echo "fail"
fi
