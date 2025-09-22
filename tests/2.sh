#!/bin/bash
echo "  TEST 2"
echo "  reverses 8 bytes with size=2"

TEST_FILE="test_files/test2.bin"
EXPECTED_FILE="test_files/expected2.bin"
OUTPUT_FILE="test_files/out2.bin"

rm -f "$OUTPUT_FILE" 2>/dev/null

# 8 bytes reversed in groups of 2
echo -n -e '\x01\x02\x03\x04\x05\x06\x07\x08' > "$TEST_FILE"
echo -n -e '\x02\x01\x04\x03\x06\x05\x08\x07' > "$EXPECTED_FILE"

bin/reverse --size=2 --output="$OUTPUT_FILE" "$TEST_FILE" > /dev/null

if diff <(hexdump -C "$EXPECTED_FILE") <(hexdump -C "$OUTPUT_FILE") >/dev/null; then
  echo "pass"
else
  echo "fail"
fi
