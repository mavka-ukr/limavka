set -x
set -e

bash src/apps/mavka/translate.sh

pio run -t clean
pio run -e v2