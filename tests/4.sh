#!/bin/bash
echo "  TEST 4"
echo "  refuses overwrite without confirmation"

TEST_FILE="test_files/test4.bin"
OUTPUT_FILE="test_files/out4.bin"
EXPECTED_FILE="test_files/expected4.bin"

rm -f "$OUTPUT_FILE" "$EXPECTED_FILE" 2>/dev/null

# irrelevant
echo -n -e '\xAA\xBB\xCC\xDD' > "$TEST_FILE"
echo -n -e '\xDD\xCC\xBB\xAA' > "$EXPECTED_FILE"
cp "$EXPECTED_FILE" "$OUTPUT_FILE"

if echo "n" | bin/reverse --size=4 --output="$OUTPUT_FILE" "$TEST_FILE" > /dev/null; then
  if diff <(hexdump -C "$EXPECTED_FILE") <(hexdump -C "$OUTPUT_FILE") >/dev/null; then
    echo "pass"
  else
    echo "fail (file was overwritten)"
  fi
else
  echo "fail (program crashed)"
fi
