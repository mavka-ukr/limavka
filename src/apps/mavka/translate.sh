set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

LIST_FILES=(
  "$SCRIPT_DIR/src/лімавка/ФайлиЦілі"
  "$SCRIPT_DIR/src/мавка/машина/ФайлиЦілі"
  "$SCRIPT_DIR/src/мавка/пристрій_мавки/ФайлиЦілі"
)

c_to_ucn() {
    python3 -c '
import sys
for path in sys.argv[1:]:
    with open(path, "r", encoding="utf-8") as f:
        content = f.read()
    converted = "".join(f"\\u{ord(c):04x}" if 127 < ord(c) <= 0xFFFF else f"\\U{ord(c):08x}" if ord(c) > 0xFFFF else c for c in content)
    with open(path, "w", encoding="utf-8") as f:
        f.write(converted)
' "$@"
}

rm -rf $SCRIPT_DIR/translated/
mkdir -p $SCRIPT_DIR/translated
mkdir -p $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/cstd

echo "*.c linguist-generated" >$SCRIPT_DIR/translated/.gitattributes

cp $SCRIPT_DIR/src/tsilint.h $SCRIPT_DIR/src/limavka.h $SCRIPT_DIR/src/limavka.cpp $SCRIPT_DIR/translated/
c_to_ucn $SCRIPT_DIR/translated/tsilint.h
c_to_ucn $SCRIPT_DIR/translated/limavka.h
c_to_ucn $SCRIPT_DIR/translated/limavka.cpp

cp -a $SCRIPT_DIR/src/мавка/пристрій_мавки/external/include $SCRIPT_DIR/translated/мавка/пристрій_мавки/external
c_to_ucn $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/include/mavka/numbertypes.h
c_to_ucn $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/include/mavka/prystriy.h
c_to_ucn $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/include/mavka/tsil.h

cp $SCRIPT_DIR/src/мавка/пристрій_мавки/external/prystriy.c $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/prystriy.c
c_to_ucn $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/prystriy.c

cp $SCRIPT_DIR/src/мавка/пристрій_мавки/external/cstd/prystriy_cstd.c $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/cstd/prystriy_cstd.c
c_to_ucn $SCRIPT_DIR/translated/мавка/пристрій_мавки/external/cstd/prystriy_cstd.c

compile_tsil() {
  InputFile="$SCRIPT_DIR/$1"
  OutputFile="$SCRIPT_DIR/translated/$1.c"

  mkdir -p "$(dirname $OutputFile)"

  echo "переклад $1"

  ціль \
    "$OutputFile" -Ф=сіанг -П=есп32с3 \
    перекласти \
    "$InputFile" -В=$SCRIPT_DIR/src/лімавка/визначення -В=$SCRIPT_DIR/src/мавка/машина/визначення -В=$SCRIPT_DIR/src/мавка/машина/КД/визначення -В=$SCRIPT_DIR/src/мавка/машина/РМв1/визначення -В=$SCRIPT_DIR/src/мавка/пристрій_мавки/визначення

  c_to_ucn "$SCRIPT_DIR/translated/$1.c"
}

prefixed_lines() {
  local file="$1"
  local rel_dir prefix
  rel_dir="$(realpath --relative-to="$SCRIPT_DIR" "$(dirname "$file")")"
  if [ "$rel_dir" = "." ]; then
    prefix="."
  else
    prefix="./$rel_dir"
  fi
  while IFS= read -r line || [ -n "$line" ]; do
    line=$(echo "$line" | xargs)
    [[ -z "$line" || "$line" =~ ^# ]] && continue
    echo "$prefix/$line"
  done < "$file"
}

while IFS= read -r line || [ -n "$line" ]; do
  compile_tsil "$line"
done < <(
  for f in "${LIST_FILES[@]}"; do
    prefixed_lines "$f"
  done
)