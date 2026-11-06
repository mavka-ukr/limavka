set -x
set -e

bash src/apps/mavka/translate.sh
if [ "$1" = "clean"]; then
  pio run -t clean
fi
pio run -t upload -t monitor -e v2