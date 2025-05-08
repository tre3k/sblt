# sblt - Simple backlight
just simple daemon and client for control brightness of balcklight for my laptop =)

Просой менджер управления подсветкой дисплея ноутбука для простых оконных менеджеров
типа DWM, awesome, и.т.д.

## Конфигурация
Просто укажите в include/sblt.h в `BRIGHTNESS_DEV` путь к устройству `"/sys/class/backlight/<ваше устройстово>/"`
и пересоберите
`
make && sudo make install
`

## Использование
Запустите через systemd
`
systemctl daemon-reload && systemctl start sbltd
`

Привяжите горячие калвиши вашего оконного менеджера на команды клинета
`sblt +10` для повышения яркости на 10%,
`sblt -10` для уменьшения яркости на 10% и
`sblt t` на клавишу включения/выключения подстветки
