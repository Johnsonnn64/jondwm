#!/bin/sh

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/workspace/suckless/dwm/scripts/catppuccin


#cpu() {
	#cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

	#printf "^c$green^  󰞰"
	#printf "^c$green^ $cpu_val"
#}

# pkg_updates() {
# 	# updates=$(sudo pacman -Sy  cl| wc -l) # void
# 	updates=$(checkupdates | wc -l)   # arch , needs pacman contrib
# 	# updates=$(aptitude search '~U' | wc -l)  # apt (ubuntu,debian etc)
#
# 	if [ "$updates" == 0 ]; then
# 		printf "^c$green^  Fully Updated"
# 	else
# 		printf "^c$green^  $updates"" updates"
# 	fi
# }

battery() {
  if [ -f /sys/class/power_supply/BAT0/capacity ]; then
    get_capacity="$(cat /sys/class/power_supply/BAT0/capacity)"
    ac_state="$(cat /sys/class/power_supply/AC/online)"

    if [ "$ac_state" -eq 1 ]; then
      printf "^c$green^  $get_capacity"
    else
      if [ "$get_capacity" -le 20 ]; then
        printf "^c$red^  $get_capacity"
      else
        printf "^c$yellow^  $get_capacity"
      fi
    fi
  fi
}

brightness() {
  if [ -f /sys/class/backlight/*/brightness ]; then
    printf "^c$red^   "
    printf "^c$red^%.0f\n" $(i=$(cat /sys/class/backlight/*/brightness)
    echo $i/25 | bc)0
  fi
}

mem() {
	printf "^c$green^󰆼"
	printf "^c$green^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

wlan() {
  state=$(cat /sys/class/net/*/operstate | grep up)
  if [ -z "$state" ]; then 
    printf "^c$red^  󰤭 ^d^%s" " ^c$red^Disconnected"
  else
    printf "^c$blue^  󰤨 ^d^%s" " ^c$blue^Connected"
  fi
}

clock() {
	printf "^c$blue^󱑆"
  printf "^c$blue^ $(date '+%a, %m-%d ')"
	printf "^c$blue^ $(date '+%H:%M')  "
}

obake() {
  printf "^c$red^ 󰊠 "
  printf "^c$yellow^ 󰊠 "
  printf "^c$blue^ 󰊠 "
  printf "^c$purple^ 󰊠"
}
obake=$(obake)

# weat() {
# 	curl -sf "wttr.in/angeles" > ~/.cache/wttr
# 	weatreport=~/.cache/wttr
# 	wstatus=$(sed '3q;d' $weatreport | sed 's/^.*.0m  *//g' )
# 	degree=$(sed '4q;d' $weatreport | grep -o "m\\([-+]\\)*[0-9]\\+" | tr '\n|m' ' ' | awk '{print $1,"°(",$2,"°)"}' | sed 's/ //g')
# 	printf "^c$blue^ $wstatus $degree"
# }

while true; do

	# [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] # && updates=$(pkg_updates)
	interval=$((interval + 1))

  sleep 1 && xsetroot -name "$(echo "$(wlan)  $(mem)  $(battery) $(brightness)  $(clock)   $obake " | sed 's/   */  /g' | sed 's/   */  /g')"
done
