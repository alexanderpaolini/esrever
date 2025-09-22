./tests/setup.sh

COUNTER=1
while [ -x "./tests/${COUNTER}.sh" ]; do
  echo ""
  ./tests/"${COUNTER}".sh
  COUNTER=$((COUNTER + 1))
done

echo ""

./tests/cleanup.sh