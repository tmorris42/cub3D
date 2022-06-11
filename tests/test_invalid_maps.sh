RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

mkdir logs
for f in invalid_maps/*.cub
do
	printf '%-50s' "$f";
	bn="$(basename -- "$f" .cub)"
	./cub3D $f > "logs/$bn.log" 2>&1
	if [[ $(grep "Error" "logs/$bn.log" | wc -l) = 1 ]]; then
		echo -e ${GREEN}Error Printed -- OK${NC}
	else
		echo -e ${RED}Error Missing -- KO${NC}
	fi
done
