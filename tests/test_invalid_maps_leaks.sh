RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

mkdir logs
for f in invalid_maps/*.cub
do
	printf '%-45s' "$f";
	bn="$(basename -- "$f" .cub)"
	valgrind ./cub3D $f > "logs/$bn.log" 2> "logs/$bn.valgrind.log"
	if [[ $(grep "Error" "logs/$bn.log" | wc -l) = 1 ]]; then
		echo -n -e ${GREEN}Error Printed${NC}
	else
		echo -n -e ${RED}Error Missing${NC}
	fi
	if [[ $(grep "no leaks are possible" "logs/$bn.valgrind.log" | wc -l) = 1 ]]; then
		echo -e ${GREEN} -- No Leaks Detected${NC}
	else
		echo -e ${RED} -- Leaks Detected   ${NC}
	fi
	if [[ $(grep "no leaks are possible" "logs/$bn.valgrind.log" | wc -l) = 1 ]]; then
		if [[ $(grep "Error" "logs/$bn.log" | wc -l) = 1 ]]; then
			echo -e ${GREEN}OK${NC}
		else
			echo -e ${RED}KO${NC}
		fi
	else
		echo -e ${RED}KO${NC}
	fi


done
