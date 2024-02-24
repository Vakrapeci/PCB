# PCB Design szoftverfejlesztő gyakornok tesztfeladat

## Feladat

Feladatok:
- UART kommunikáció létrehozása
- 2 parancs implementálása (LED fel/le)
- program tesztelése
- flood teszt

Eszközök:
- Board: NUCLEO-H7A3ZI-Q
- Fejlesztői környezet: STM32CubeIDE


## Megvalósítás

A feladatot kétféle módon oldottam meg (PCB_1 és PCB_2 mappa).

A PCB_1 mappában lévő projekt egy interrupt módban futó UART kommunikációt valósít meg.

Két állapota van:
- csak a LED2 kapcsolható UART-on keresztül
- a LED1 villog, és közben a LED2 kapcsolható (interrupt szemléltetésére)
A programot shell scrip-tekkel teszteltem, ezek a *script* mappában találhatók, a kimentett terminál válaszokkal együtt.<br />
Három teszt script-et készítettem el: ```flood_fix.sh``` (500-szor kapcsolja fel és le), ```flood_while.sh``` (megszakításig folyamatosan kapcsolgatja a LED-et) éa ```flood_random.sh``` (random karakterekkel spam-eli a mikrovezérlőt).

A PCB_2 mappában megvalósított projekt protobuf alkalmazásával kommunikál UART-on keresztül.
Az Embedded Proto-t git submodule-ként adtam hozzá a projekthez, ez generálta a kommunikációhoz szükséges fájlok nagy részét. A projekt az [EmbeddedProto Example STM32 UART](https://github.com/Embedded-AMS/EmbeddedProto_Example_STM32_UART/blob/master/README.md) git repo-n alapul. A PC-s python scriptek szabványos protobuf formátumban elküldik a beütött parancsokat a mikrovezérlőnek, a mikrovezérlő ezt dekódolja és a parancsnak megfelelően reagál (LED fel- vagy lekapcsolása).

A projekt a következő mappákból áll:
- desktop: a PC-n futtatandó python scriptek (```main.py``` és ```fload.py```)
- EmbeddedProto: git submodule
- nucleo-h7a3zi-q: STM32CubeIDE projekt (a mikrovezérlő programja és a bináris állományok)
- proto: a kommunikáció során használt proto fájlok struktúráját leíró proto fájl

<p align="right">
Nemes Balázs Boldizsár
</p>
