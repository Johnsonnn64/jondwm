#!/bin/sh

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/workspace/suckless/dwm/bar/catppuccin


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
    read=~/.cache/batt
    reset=$(cat "$read")

    if [ "$ac_state" -eq 1 ]; then
      printf "^c$green^ $get_capacity"
      echo "1" > "$read"
    else
      if [ "$get_capacity" -le 15 ]; then
        if [ "$reset" -eq 1 ]; then
          notify-send -u critical "Low Battery!" "Please Charge"
          echo "0" > "$read"
        fi
        printf "^c$red^ $get_capacity"
      else
        printf "^c$yellow^ $get_capacity"
        echo "1" > "$read"
      fi
    fi
  fi
}

brightness() {
  if [ -f /sys/class/backlight/*/brightness ]; then
    printf "^c$red^  "
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
    printf "^c$red^ 󰤭^d^%s" " ^c$red^Disconnected"
  else
    printf "^c$blue^ 󰤨^d^%s" " ^c$blue^Connected"
  fi
}

volume() {
  mute=$(pactl get-sink-mute @DEFAULT_SINK@ | awk '{print $2}')
  if [ yes = "$mute" ]; then
    printf "^$red^  muted" 
  else
    printf "^c$green^ "
    printf "^c$green^ $(pactl get-sink-volume @DEFAULT_SINK@ | awk '{print $5}')%"
  fi
}

clock() {
	printf "^c$blue^"
  printf " $(date '+%a, %m-%d ')"
  printf " 󱑆"
	printf " $(date '+%H:%M')"
}

while true; do

  [ $interval = 0 ] || [ $(($interval % 5)) = 0 ] && wlan=$(wlan) && mem=$(mem) battery=$(battery) && brightness=$(brightness) && volume=$(volume)

  sleep 1 && xsetroot -name "$(echo "$wlan $mem $battery $brightness $volume ")"
done
