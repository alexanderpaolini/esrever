#!/bin/bash
echo "  TEST 5"
echo "  auto-generates output filename"

TEST_FILE="test_files/test5.bin"
EXPECTED_FILE="expected5.bin"
OUT_FILE="nib.5tset"

rm -f "$EXPECTED_FILE" "$OUT_FILE" 2>/dev/null

# simple swap
echo -n -e '\x11\x22\x33\x44' > "$TEST_FILE"
echo -n -e '\x22\x11\x44\x33' > "$EXPECTED_FILE"

if echo "y" | bin/reverse --size=2 "$TEST_FILE" > /dev/null; then
  if diff <(hexdump -C "$EXPECTED_FILE") <(hexdump -C "$OUT_FILE") >/dev/null; then
    echo "pass"
  else
    echo "fail (contents mismatch)"
  fi
else
  echo "fail (program crashed)"
fi

rm -f "$OUT_FILE"
