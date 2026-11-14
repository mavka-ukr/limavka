set -x
set -e

VERSION="$1"

bash src/apps/mavka/translate.sh

pio run -t clean
pio run -e v2

esptool.py --chip esp32s3 merge_bin \
  -o .pio/build/v2/ЛіМавка-$VERSION.bin \
  0x0 .pio/build/v2/bootloader.bin \
  0x8000 .pio/build/v2/partitions.bin \
  0x10000 .pio/build/v2/firmware.bin